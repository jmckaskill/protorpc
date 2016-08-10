#define BUILDING_PROTORPC
#include "rpc.h"
#include <protorpc/protorpc.h>
#include <os/http-parser.h>
#include <os/thread.h>
#include <os/log.h>
#include <os/hash.h>
#include <os/char-array.h>

#include <ext/bearssl-0.5/inc/bearssl.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#ifdef WIN32
#define NOMINMAX
#include <WS2tcpip.h>
#define fcntl(fd,opt,optlen) (0)
#define SHUT_RDWR SD_BOTH
#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "mswsock.lib")
typedef SOCKET socket_t;
#define ftello _ftelli64
#else
#include <errno.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <netdb.h>
#include <strings.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#define closesocket(fd) close(fd)
typedef int socket_t;
#endif

#ifdef _MSC_VER
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#endif

#ifndef __linux__
#define accept4(fd,sa,salen,opts) (int) accept(fd,sa,salen)
#endif

#ifndef SOCK_CLOEXEC
#define SOCK_CLOEXEC 0
#endif

#define MAXREQSZ 65536
#define REQTIMEOUTMS 5000

// last header and includes 6 spaces
#define CONTENT_LENGTH_TAIL "Content-Length:      \r\n\r\n"

static bool set_content_length(str_t *output, size_t hsz) {
    // Content-Length field must be the last header
    size_t wlen = output->len - hsz;
    if (wlen > 999999) {
        return false;
    }

    // start at the last digit _\r\n\r\n
    char *p = output->buf + hsz - 5;
    do {
        *p-- = (wlen % 10) + '0';
        wlen /= 10;
    } while (wlen);

    return true;
}

static bool set_response_status(int ret, char *hdr) {
    size_t stsoff = 9; // HTTP/1.1 100

    if (ret < 100 || ret > 999) {
        return false;
    }

    hdr[stsoff] = (char) (ret / 100) + '0';
    hdr[stsoff+1] = (char) ((ret % 100) / 10) + '0';
    hdr[stsoff+2] = (char) (ret % 10) + '0';
    return true;
}

struct rpc_server {
	struct {map_t h; slice_t *keys; rpc_post_callback *values;} methods;
	struct {map_t h; slice_t *keys; struct rpc_publisher **values;} streams;
	int listenfd;
	pb_alloc_t alloc;
	str_t input;
	str_t output;
};

struct rpc_server *rpc_server_new() {
#ifdef WIN32
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
	struct rpc_server *m = NEW(struct rpc_server);
	pb_alloc_init(&m->alloc);
	str_init(&m->input);
	str_init(&m->output);
	m->listenfd = -1;
	return m;
}

void rpc_server_delete(struct rpc_server *m) {
	if (m->listenfd >= 0) {
		closesocket(m->listenfd);
	}
	map_free(&m->methods);
	map_free(&m->streams);
	pb_alloc_destroy(&m->alloc);
	str_destroy(&m->input);
	str_destroy(&m->output);
	free(m);
}

struct thread_data {
	const struct rpc_server *server;
	int fd;
};

static int send_string(int fd, const char *str) {
	return send(fd, str, (int) strlen(str), 0) == (int) strlen(str) ? 0 : -1;
}

static const char s_bad_request[] = "HTTP/1.1 400 Bad Request\r\nContent-Length:0\r\n\r\n";
static const char s_length_required[] = "HTTP/1.1 411 Length Required\r\nContent-Length:0\r\n\r\n";
static const char s_request_too_large[] = "HTTP/1.1 413 Request Too Large\r\nContent-Length:0\r\n\r\n";
static const char s_not_found[] = "HTTP/1.1 404 Not Found\r\nContent-Length:0\r\n\r\n";
static const char s_closesocket[] = ""; // no error, just close it

// returns the error string or NULL on success
static const char *parse_request(int fd, struct http_parser *p, char *buf, int bufsz, int *pleft) {
	char *next = buf;
	int have = 0;

	for (;;) {
		if (have == bufsz) {
			return s_bad_request;
		}

		memmove(buf, next, have);
		next = buf;

		int r;
		do {
			r = recv(fd, buf + have, bufsz - have, 0);
		} while (r < 0 && errno == EINTR);

		if (r <= 0) {
			return s_bad_request;
		}

		debug_data(buf + have, r, "parse_request");
		have += r;

		switch (http_parse_request(p, &next, &have)) {
		case HTTP_FINISHED:
			if (p->length_type == HTTP_LENGTH_CHUNKED) {
				return s_length_required;
			}
			memmove(buf, next, have);
			*pleft = have;
			return NULL;
		case HTTP_ERROR:
			return p->error_string;
		case HTTP_CONTINUE:
			continue;
		}
	}
}

static const char *read_body(int fd, struct http_parser *p, str_t *s) {
	if (p->length_type == HTTP_LENGTH_CLOSE) {
		for (;;) {
			str_grow(s, s->len + 4096);
			int r = recv(fd, s->buf + s->len, s->cap - s->len, 0);
			if (r < 0 && errno == EINTR) {
				continue;
			} else if (r < 0) {
				return s_bad_request;
			} else if (r == 0) {
				return NULL;
			}
			str_setlen(s, s->len + r);
		}

	} else {
		// fixed content length
		if (p->content_length > MAXREQSZ) {
			return s_request_too_large;
		}
		str_grow(s, (int) p->content_length);

		while (s->len < p->content_length) {
			int r = recv(fd, s->buf + s->len, (int) p->content_length - s->len, 0);
			if (r < 0 && errno == EINTR) {
				continue;
			} else if (r <= 0) {
				return s_bad_request;
			}
			str_setlen(s, s->len + r);
		}

		return NULL;
	}
}

static void append_websocket_accept(str_t *to, const char* key) {
	unsigned char hash[br_sha1_SIZE];
	br_sha1_context ctx;
	br_sha1_init(&ctx);
	br_sha1_update(&ctx, key, strlen(key));
	br_sha1_update(&ctx, "258EAFA5-E914-47DA-95CA-C5AB0DC85B11", 36);
	br_sha1_out(&ctx, hash);

	str_grow(to, to->len + pb_base64_size(br_sha1_SIZE));
	pb_print_base64(to->buf + to->len, hash, sizeof(hash));
	str_setlen(to, to->len + pb_base64_size(br_sha1_SIZE));
	str_addch(to, '=');
}

// returns error string or NULL on success
static const char *serve_rpc(struct rpc_server *m, int fd) {
	struct http_parser p = {0};

	pb_alloc_clear(&m->alloc);
	str_clear(&m->input);
	str_clear(&m->output);
	str_grow(&m->input, 4096);

	int left;
	const char *err = parse_request(fd, &p, m->input.buf, m->input.cap, &left);
	if (err) {
		return err;
	}

	str_setlen(&m->input, left);

	int idx;
	if (p.method == HTTP_METHOD_POST && smap_find(&m->methods, p.path.buf, p.path.len, &idx)) {
		// API request/response
		const char *str = "HTTP/1.1 000 \r\n" CONTENT_LENGTH_TAIL;
		int hdrsz = (int) strlen(str);
		str_set2(&m->output, str, hdrsz);

		if ((err = read_body(fd, &p, &m->input)) != NULL) {
			return err;
		}

		int ret = m->methods.values[idx](&m->alloc, &m->output, m->input.buf, m->input.len);

		if (!set_response_status(ret, m->output.buf) || !set_content_length(&m->output, hdrsz)) {
			return s_closesocket;
		}

		if (send(fd, m->output.buf, m->output.len, 0) != (int) m->output.len) {
			return s_closesocket;
		}

	} else if (p.method == HTTP_METHOD_GET && p.have_ws_key && p.have_ws_upgrade && p.have_connection_upgrade) {
		if (smap_find(&m->streams, p.path.buf, p.path.len, &idx)) {
			// API streams
			str_set(&m->output, "HTTP/1.1 101 Upgrade\r\n"
				"Connection:Upgrade\r\n"
				"Upgrade:websocket\r\n"
				"Sec-WebSocket-Accept: ");

			append_websocket_accept(&m->output, p.ws_key.buf);
			str_add(&m->output, "\r\n\r\n");

			if (send(fd, m->output.buf, (int) m->output.len, 0) != (int) m->output.len) {
				return s_closesocket;
			}

			if (rpc_subscribe(m->streams.values[idx], fd, &m->input)) {
				return s_closesocket;
			}

			// publisher will close the connection
			return NULL;

		} else if (smap_find(&m->methods, p.path.buf, p.path.len, &idx)) {
			// websocket for interacting with a method
			// we need to launch a thread that just processes request/replies to that method
			str_set(&m->output, "HTTP/1.1 101 Upgrade\r\n"
				"Connection:Upgrade\r\n"
				"Upgrade:websocket\r\n"
				"Sec-WebSocket-Accept: ");

			append_websocket_accept(&m->output, p.ws_key.buf);
			str_add(&m->output, "\r\n\r\n");

			if (send(fd, m->output.buf, (int) m->output.len, 0) != (int) m->output.len) {
				return s_closesocket;
			}

			if (rpc_launch_ws_thread(fd, &m->input, m->methods.values[idx])) {
				return s_closesocket;
			}

			// websocket thread will close the connection
			return NULL;

		} else {
			return s_not_found;
		}

	} else if (p.method == HTTP_METHOD_GET && smap_find(&m->streams, p.path.buf, p.path.len, &idx)) {
		// single poll of an API stream
		const char *str = "HTTP/1.1 200 \r\n" CONTENT_LENGTH_TAIL;
		int hdrsz = (int) strlen(str);
		str_set2(&m->output, str, hdrsz);

		rpc_get_message(m->streams.values[idx], &m->output);

		if (!set_content_length(&m->output, hdrsz)) {
			return s_closesocket;
		}

		if (send(fd, m->output.buf, m->output.len, 0) != (int) m->output.len) {
			return s_closesocket;
		}

	} else if (p.method == HTTP_METHOD_GET && str_equals(&p.path, "/")) {
		send_string(fd, "HTTP/1.1 307 Temporary Redirect\r\nLocation:/en/index.html\r\nContent-Length\r\n\r\n");

	} else if (p.method == HTTP_METHOD_GET) {
		str_set(&m->output, "www");
		str_addstr(&m->output, &p.path);
		
		FILE *f = fopen(m->output.buf, "rb");

		if (!f) {
			return s_not_found;
		}

		const char *type = "";
		if (str_ends_with(&m->output, ".js")) {
			type = "Content-Type:application/json\r\n";
		} else if (str_ends_with(&m->output, ".html")) {
			type = "Content-Type:text/html\r\n";
		} else if (str_ends_with(&m->output, ".css")) {
			type = "Content-Type:text/css\r\n";
		} else if (str_ends_with(&m->output, ".svg")) {
			type = "Content-Type:image/svg+xml\r\n";
		} else if (str_ends_with(&m->output, ".ttf")) {
			type = "Content-Type:font/opentype\r\n";
		}

		fseek(f, 0, SEEK_END);
		int64_t sz = ftello(f);
		fseek(f, 0, SEEK_SET);
		str_clear(&m->output);
		str_addf(&m->output, "HTTP/1.1 200 OK\r\nContent-Length:%" PRIuPTR "\r\n%s\r\n", (uintptr_t) sz, type);

		if (send(fd, m->output.buf, m->output.len, 0) != (int) m->output.len) {
			fclose(f);
			return s_closesocket;
		}

		while (!feof(f)) {
			char buf[4096];
			int n = (int) fread(buf, 1, sizeof(buf), f);
			if (send(fd, buf, n, 0) != n) {
				fclose(f);
				return s_closesocket;
			}
		}
		fclose(f);

	} else {
		return s_not_found;
	}

	// clean closure
    debug("closing %d in serve", fd);
	shutdown(fd, SHUT_RDWR);
	closesocket(fd);
	return NULL;
}

void rpc_handle_post(struct rpc_server *m, const char *path, rpc_post_callback cb) {
	smap_set(&m->methods, path, (int) strlen(path), cb);
}

void rpc_handle_stream(struct rpc_server *m, const char *path, struct rpc_publisher *pub) {
	smap_set(&m->streams, path, (int) strlen(path), pub);
}

int rpc_listen(struct rpc_server *m, int port) {
	struct sockaddr_in6 sa6 = {0};
	sa6.sin6_family = AF_INET6;
	sa6.sin6_port = ntohs((uint16_t) port);
	memcpy(sa6.sin6_addr.s6_addr, &in6addr_loopback, sizeof(in6addr_loopback));

	struct sockaddr_in sa4 = {0};
	sa4.sin_family = AF_INET;
	sa4.sin_port = ntohs((uint16_t) port);
	//sa4.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);
  sa4.sin_addr.s_addr = ntohl(INADDR_ANY);

	struct sockaddr *sa = (struct sockaddr*) &sa6;
	socklen_t sasz = sizeof(sa6);

	int fd = (int) socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
	if (fd < 0) {
		sa = (struct sockaddr*) &sa4;
		sasz = sizeof(sa4);
		fd = (int) socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	}
	if (fd < 0) {
		syserror("socket");
		return -1;
	}

	int on = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char*) &on, sizeof(on))) {
		syserror("reuse address");
		closesocket(fd);
		return -1;
	}

	if (bind(fd, sa, sasz) || listen(fd, SOMAXCONN)) {
		syserror("bind/listen");
		closesocket(fd);
		return -1;
	}

	m->listenfd = fd;
	return 0;
}

static int THREAD_API do_server(void *pdata) {
	thrd_set_name("rpc server");
#ifndef WIN32
	signal(SIGPIPE, SIG_IGN);
#endif
	struct rpc_server *m = (struct rpc_server*) pdata;
    for (;;) {
        int clientfd = accept4(m->listenfd, NULL, NULL, SOCK_CLOEXEC);
        debug("accept %d", clientfd);
        if (clientfd < 0 && errno == EINTR) {
            continue;
        } else if (clientfd < 0) {
            syserror("accept failed");
            closesocket(m->listenfd);
			m->listenfd = -1;
			return 0;
        }

		const char *err = serve_rpc(m, clientfd);
		if (err) {
			if (*err) {
				send_string(clientfd, err);
			}
			closesocket(clientfd);
		}
    }
}

void rpc_detached_serve(struct rpc_server *m) {
	thrd_t thrd;
	if (!thrd_create(&thrd, &do_server, m)) {
		thrd_detach(thrd);
	}
}

void rpc_serve(struct rpc_server *m) {
	do_server(m);
}
