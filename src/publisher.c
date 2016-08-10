#include "rpc.h"
#include <protorpc/protorpc.h>
#include <os/thread.h>
#include <os/log.h>

#ifdef WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#define closesocket close
#endif

struct subscription {
	int fd;
	str_t in;
};

struct rpc_publisher {
	str_t buffer;
	mtx_t mutex;
	struct subscription *subs;
	size_t subn;
	int id;
	char *wsmsg;
	int wssz;
	char *getmsg;
	int getsz;
};

// the largest we'll let a receive packet get including the header
#define WS_MAX_PACKET_SIZE (64*1024)

#define WS_FIN 0x80
#define WS_OPMASK 0xF
#define WS_CONTINUATION 0x0
#define WS_TEXT 0x1
#define WS_BINARY 0x2
#define WS_CLOSE 0x8
#define WS_PING 0x9
#define WS_PONG 0xA
#define WS_MASK 0x80U

#define WSC_NORMAL 1000
#define WSC_GOAWAY 1001
#define WSC_PROTOCOL_ERROR 1002
#define WSC_INVALID_TYPE 1003
#define WSC_OVERLARGE 1009

#define WS_CONTROL_MAX 125
#define WS_2BYTE 126
#define WS_8BYTE 127

static uint16_t read_big_16(uint8_t const *p) {
	return (((uint16_t) p[0]) << 8) | ((uint16_t) p[1]);
}

static void write_big_16(uint8_t* p, size_t v) {
	p[0] = (uint8_t) (v >> 8);
	p[1] = (uint8_t) (v);
}

static const uint8_t g_null_msg[] = {WS_FIN|WS_TEXT,3,'{','}','\n'};

struct rpc_publisher *rpc_publisher_new() {
	struct rpc_publisher *m = NEW(struct rpc_publisher);
	mtx_init(&m->mutex, mtx_plain);
	str_init(&m->buffer);
	m->wsmsg = (char*) g_null_msg;
	m->wssz = sizeof(g_null_msg);
	m->getmsg = "{}\n";
	m->getsz = 3;
	return m;
}

void rpc_publisher_delete(struct rpc_publisher *m) {
	for (size_t i = 0; i < m->subn; i++) {
		closesocket(m->subs[i].fd);
		str_destroy(&m->subs[i].in);
	}
	free(m->subs);
	mtx_destroy(&m->mutex);
	free(m);
}

void rpc_get_message(struct rpc_publisher *m, str_t *out) {
	mtx_lock(&m->mutex);
	str_add2(out, m->getmsg, m->getsz);
	mtx_unlock(&m->mutex);
}

static int set_non_blocking(int fd) {
#ifdef WIN32
	u_long nonblock = 1;
	if (ioctlsocket(fd, FIONBIO, &nonblock)) {
		return -1;
	}
#else
	if (fcntl(fd, F_SETFD, FD_CLOEXEC) || fcntl(fd, F_SETFL, O_NONBLOCK)) {
		return -1;
	}
#endif
	return 0;
}

int rpc_subscribe(struct rpc_publisher *m, int fd, str_t *in) {
	if (set_non_blocking(fd)) {
		return -1;
	}

	mtx_lock(&m->mutex);
	debug("subscribing %d", fd);
	if (send(fd, (char*) m->wsmsg, m->wssz, 0) != m->wssz) {
		warn("initial send on %d failed - closing", fd);
		mtx_unlock(&m->mutex);
		return -1;
	}

	debug("subscribing %d - success", fd);
	m->subs = (struct subscription*) realloc(m->subs, sizeof(*m->subs) * (m->subn + 1));
	m->subs[m->subn].fd = fd;
	str_init(&m->subs[m->subn].in);
	str_swap(&m->subs[m->subn].in, in);
	m->subn++;
	mtx_unlock(&m->mutex);
	return 0;
}

static void start_text_frame(str_t *s) {
	str_grow(s, 4); // 2 header, 2 length
	str_setlen(s, 4);
}

// returns number of bytes to skip
static int finish_text_frame(str_t *s) {
	int msgsz = s->len - 4;
	uint8_t *h = (uint8_t*) s->buf;
	if (!msgsz) {
		h[2] = WS_FIN | WS_TEXT;
		h[3] = 3;
		str_add(s, "{}\n");
		return 2;

	} else if (msgsz < WS_2BYTE) {
		// 1 byte message
		h[2] = WS_FIN | WS_TEXT;
		h[3] = (uint8_t) msgsz;
		return 2;

	} else if (msgsz < 0x10000) {
		// 2 byte message
		h[0] = WS_FIN | WS_TEXT;
		h[1] = WS_2BYTE;
		write_big_16(h+2, (uint16_t) msgsz);
		return 0;

	} else {
		// we don't support 8 byte lengths
		assert(0);
		h[2] = WS_FIN | WS_TEXT;
		h[3] = 0;
		str_setlen(s, 4);
		return 2;
	}
}

str_t *rpc_publish_start(struct rpc_publisher *m) {
	mtx_lock(&m->mutex);
	start_text_frame(&m->buffer);
	return &m->buffer;
}

static int consume_frames(int fd, str_t *s);

void rpc_publish_finish(struct rpc_publisher *m) {
	int skip = finish_text_frame(&m->buffer);

	m->getmsg = m->buffer.buf + 4;
	m->getsz = m->buffer.len - 4;
	m->wsmsg = m->buffer.buf + skip;
	m->wssz = m->buffer.len - skip;

	bool compact = false;

	for (size_t i = 0; i < m->subn; i++) {
		struct subscription *s = &m->subs[i];
		debug("publishing to %d", s->fd);
		if (send(s->fd, (char*) m->wsmsg, m->wssz, 0) != m->wssz
		 || consume_frames(s->fd, &s->in)) {
			debug("dropping %d", s->fd);
			closesocket(s->fd);
			str_destroy(&s->in);
			s->fd = -1;
			compact = true;
		}
	}

	if (compact) {
		int j = 0;
		for (size_t i = 0; i < m->subn; i++) {
			if (m->subs[i].fd >= 0) {
				m->subs[j++] = m->subs[i];
			}
		}
		m->subn = j;
		m->subs = (struct subscription*) realloc(m->subs, j * sizeof(*m->subs));
	}

	mtx_unlock(&m->mutex);
}


static int send_ping(int fd, uint8_t opcode, const uint8_t *data, int sz) {
	assert(sz < WS_CONTROL_MAX);
	uint8_t buf[WS_CONTROL_MAX + 2];
	buf[0] = WS_FIN | opcode;
	buf[1] = (uint8_t) sz;
	memcpy(buf+2, data, sz);
	return send(fd, (char*) buf, sz+2, 0) == sz+2 ? 0 : -1;
}

// always returns -1, so it can be chained from consume_frame
static int send_close(int fd, uint16_t reason) {
	uint8_t buf[4];
	buf[0] = WS_FIN | WS_CLOSE;
	buf[1] = 2; // size
	write_big_16(buf+2, reason);
	send(fd, (char*) buf, sizeof(buf), 0);
	return -1;
}

// consumes and processes a single frame
// returns
// > 0 - success, returns number of bytes consumed
// 0 - need more data
// < 0 - error
static int read_frame(int fd, uint8_t *p, size_t sz, uint8_t data_opcode, str_t *out) {
	if (sz < 2) {
		return 0;
	}
	if (!(p[1] & WS_MASK)) {
		return send_close(fd, WSC_PROTOCOL_ERROR);
	}
	uint8_t *mask = p+2;
	uint8_t *data = p+6;
	size_t msgsz = p[1] & ~WS_MASK;
	size_t used = msgsz+6;
	if (used > sz) {
		return 0;
	}

	if (msgsz == WS_8BYTE) {
		// 8 byte length
		return send_close(fd, WSC_OVERLARGE);
	} else if (msgsz == WS_2BYTE) {
		// 2 byte length
		msgsz = read_big_16(p+2);
		used = msgsz+8;
		mask = p+4;
		data = p+8;
		if (msgsz < 126) {
			return send_close(fd, WSC_PROTOCOL_ERROR);
		} else if (used > WS_MAX_PACKET_SIZE) {
			return send_close(fd, WSC_OVERLARGE);
		} else if (used > sz) {
			return 0;
		}
	}

	uint8_t opcode = p[0] & ~WS_FIN;
	bool more = !(p[0] & WS_FIN);

	if (opcode >= WS_CLOSE && (more || msgsz > WS_CONTROL_MAX)) {
		return send_close(fd, WSC_OVERLARGE);
	}

	// unmask the data
	for (size_t i = 0; i < msgsz; i++) {
		data[i] = data[i] ^ mask[i % 4];
	}

	switch (opcode) {
	case WS_PING:
		if (send_ping(fd, WS_PONG, data, msgsz)) {
			return send_close(fd, WSC_PROTOCOL_ERROR);
		}
		return used;
	case WS_PONG:
		return used;
	case WS_TEXT:
	case WS_BINARY:
		if (opcode != data_opcode) {
			return send_close(fd, WSC_INVALID_TYPE);
		}
		if (out) {
			str_add2(out, (char*) data, msgsz);
		}
		return used;
	case WS_CONTINUATION:
		return used;
	case WS_CLOSE:
		if (msgsz >= 2) {
			send_close(fd, read_big_16(data));
		} else {
			send_close(fd, WSC_NORMAL);
		}
		return -1;
	default:
		send_close(fd, WSC_PROTOCOL_ERROR);
		return -1;
	}
}

// returns 0 on success, non-zero on error
static int consume_frames(int fd, str_t *s) {
	int off = 0;
	for (;;) {
		if (off < s->len) {
			int used = read_frame(fd, (uint8_t*) s->buf + off, s->len - off, WS_TEXT, NULL);
			if (used < 0) {
				return -1;
			} else if (used > 0) {
				off += used;
				continue;
			}
		}

		// compact the frame buffer
		memmove(s->buf, s->buf + off, s->len - off);
		s->len -= off;
		off = 0;

		// need to read more
		str_grow(s, s->len + 4096);
		int r = recv(fd, s->buf + s->len, s->cap - s->len, 0);
#ifdef WIN32
		if (r < 0 && WSAGetLastError() == WSAEWOULDBLOCK) {
			return 0;
#else
		if (r < 0 && errno == EINTR) {
			continue;
		} else if (r < 0 && errno == EWOULDBLOCK) {
			return 0;
#endif
		} else if (r <= 0) {
			return -1;
		}
		str_setlen(s, s->len + r);
	}
}

struct ws_thread_data {
	int fd;
	str_t in;
	rpc_post_callback cb;
};

static int THREAD_API ws_thread(void *pdata) {
	struct ws_thread_data *d = (struct ws_thread_data*) pdata;
	str_t msg = STR_INIT;
	str_t o = STR_INIT;
	pb_alloc_t obj = PB_ALLOC_INIT;

	int msgoff = 0;
	int inoff = 0;
	for (;;) {
		// process decoded text frames
		if (msgoff < msg.len) {
			char *nl;
			while ((nl = (char*) memchr(msg.buf + msgoff, '\n', msg.len - msgoff)) != NULL) {
				pb_alloc_clear(&obj);
				start_text_frame(&o);
				*nl = '\0';
				str_add(&o, "{\"status\":000,\"data\":");
				int stsoff = o.len - (int) strlen("000,\"data\":");

				int sts = d->cb(&obj, &o, msg.buf + msgoff, (int) (nl - msg.buf + msgoff));

				if (sts < 100 || sts > 999) {
					sts = 500;
				}
				o.buf[stsoff] = (char) (sts / 100 + '0');
				o.buf[stsoff + 1] = (char) ((sts % 100) / 10 + '0');
				o.buf[stsoff + 2] = (char) (sts % 10 + '0');

				if (o.buf[o.len - 1] == '\n' || o.buf[o.len - 1] == ',') {
					o.len--;
				} else if (o.len == stsoff + (int) strlen("000:\"data\":")) {
					str_setlen(&o, stsoff + 3);
				}
				str_add(&o, "}\n");

				int skip = finish_text_frame(&o);
				if (send(d->fd, o.buf + skip, o.len - skip, 0) != (int) o.len - skip) {
					goto err;
				}

				// skip over newline for the next message
				msgoff = (int) (nl + 1 - msg.buf);
			}
		}

		// compact the message buffer
		memmove(msg.buf, msg.buf + msgoff, msg.len - msgoff);
		str_setlen(&msg, msg.len - msgoff);
		msgoff = 0;

		// need to process more frames
		if (inoff < d->in.len) {
			int used = read_frame(d->fd, (uint8_t*) d->in.buf + inoff, d->in.len - inoff, WS_TEXT, &msg);
			if (used < 0) {
				goto err;
			} else if (used > 0) {
				// try and process the updated message buffer
				inoff += used;
				continue;
			}
		}

		// compact the frame buffer
		memmove(d->in.buf, d->in.buf + inoff, d->in.len - inoff);
		str_setlen(&d->in, d->in.len - inoff);
		inoff = 0;

		// need to read more
		str_grow(&d->in, d->in.len + 4096);
		int r = recv(d->fd, d->in.buf + d->in.len, d->in.cap - d->in.len, 0);
		if (r < 0 && errno == EINTR) {
			continue;
		} else if (r <= 0) {
			goto err;
		}
		str_setlen(&d->in, d->in.len + r);
	}

err:
	str_destroy(&msg);
	str_destroy(&o);
	pb_alloc_destroy(&obj);
	closesocket(d->fd);
	str_destroy(&d->in);
	free(d);
	return 0;
}

int rpc_launch_ws_thread(int fd, str_t *in, rpc_post_callback cb) {
	struct ws_thread_data *d = NEW(struct ws_thread_data);
	str_init(&d->in);
	str_swap(&d->in, in);
	d->fd = fd;
	d->cb = cb;

	thrd_t t;
	if (thrd_create(&t, &ws_thread, d)) {
		str_swap(&d->in, in);
		free(d);
		return -1;
	}

	thrd_detach(t);
	return 0;
}
