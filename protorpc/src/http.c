#include <protorpc/http.h>
#include <protorpc/char-array.h>
#include <protorpc/sha1.h>
#include <protorpc/protorpc.h>

#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif

#define ERROR -1
#define DONE 0
#define MORE 1
#define MAX_LINE_LENGTH 256

#define HAVE_WS_KEY 1
#define HAVE_WS_UPGRADE 2
#define HAVE_UPGRADE 4
#define HAVE_WS_VERSION 8
#define HAVE_WS_ALL (HAVE_WS_KEY | HAVE_WS_UPGRADE | HAVE_UPGRADE | HAVE_WS_VERSION)

#define WS_FIN 0x80
#define WS_CONTINUATION 0x0
#define WS_TEXT 0x1
#define WS_BINARY 0x2
#define WS_CLOSE 0x8
#define WS_PING 0x9
#define WS_PONG 0xA
#define WS_MASK 0x80

#define WSC_NORMAL 1000
#define WSC_GOAWAY 1001
#define WSC_PROTOCOL_ERROR 1002
#define WSC_INVALID_TYPE 1003
#define WSC_OVERLARGE 1009

#define WS_CONTROL_MAX 125
#define WS_2BYTE 126
#define WS_8BYTE 127

typedef struct {
	int len;
	char *c_str;
} slice;

static int get_line(slice *d, slice *line) {
	char *nl = (char*)memchr(d->c_str, '\n', d->len);
	if (!nl && d->len > MAX_LINE_LENGTH) {
		return ERROR;
	} else if (!nl) {
		return MORE;
	} else if (nl == d->c_str || nl[-1] != '\r') {
		return ERROR;
	}

	char *e = d->c_str + d->len;
	line->c_str = d->c_str;
	line->len = nl - 1 - d->c_str; // don't include the \r
	d->c_str = nl + 1;
	d->len = e - d->c_str;

	return DONE;
}

static int split(slice *d, slice *left, char ch) {
	char *p = memchr(d->c_str, ch, d->len);
	if (!p) {
		return -1;
	}
	char *e = d->c_str + d->len;
	left->c_str = d->c_str;
	left->len = p - d->c_str;
	d->c_str = p + 1;
	d->len = e - d->c_str;
	return 0;
}

static int process_path(http *h, slice *data) {
	slice line;
	int err = get_line(data, &line);
	if (err) {
		return err;
	}

	slice method;
	if (split(&line, &method, ' ')) {
		return ERROR;
	}

	slice path;
	if (split(&line, &path, ' ')) {
		return ERROR;
	}

	h->connection_close = str_itest(line, "HTTP/1.0") ? 1 : 0;

	ca_setstr(&h->method, method);
	ca_setstr(&h->path, path);

	// check for an absolute form address e.g. GET http://www.example.com:80/foo HTTP/1.1
	char *scheme = (char*)memmem(h->path.c_str, h->path.len, "://", 3);
	if (scheme) {
		char *slash = strchr(scheme + 3, '/');
		if (slash) {
			ca_set(&h->path, slash);
		} else {
			ca_set(&h->path, "/");
		}
	}

	// remove the query string
	char *query = str_find_char(h->path, '?');
	if (query) {
		ca_setlen(&h->path, query - h->path.c_str);
	}

	// path checks - generally refuse anything that may cause a root path escape
	// or we don't want to process
	if (str_find_char(h->path, '%')
		|| memmem(h->path.c_str, h->path.len, "/..", 3)
		|| memmem(h->path.c_str, h->path.len, "//", 2)
		|| str_find_char(h->path, '\\')
		|| h->path.len == 0
		|| h->path.c_str[0] != '/') {
		return ERROR;
	}

	return DONE;
}

static bool is_space(char ch) {
	return ch == ' ' || ch == '\t';
}

static void trim_left(slice *p) {
	while (p->len && is_space(p->c_str[0])) {
		p->c_str++;
		p->len--;
	}
}

static void trim_right(slice *p) {
	while (p->len && is_space(p->c_str[p->len-1])) {
		p->len--;
	}
}

static slice next_token(slice *line) {
	slice token;
	if (split(line, &token, ',')) {
		token = *line;
		line->c_str += line->len;
		line->len = 0;
	} else {
		trim_right(&token);
		trim_left(line);
	}
	return token;
}

static int process_headers(http *h, slice *data) {
	for (;;) {
		slice line;
		int err = get_line(data, &line);
		if (err) {
			return err;
		}

		if (!line.len) {
			return DONE;
		}

		slice key;
		if (split(&line, &key, ':')) {
			return ERROR;
		}

		slice value = line;
		trim_right(&key);
		trim_left(&value);
		trim_right(&value);
		key.c_str[key.len] = 0;
		value.c_str[value.len] = 0;

		if (str_itest(key, "content-length")) {
			h->content_length = 0;
			char *p = value.c_str;
			while (*p) {
				// include check for overflow
				if (*p < '0' || *p > '9' || h->content_length > (INT64_MAX >> 4)) {
					h->internal_error = "HTTP/1.1 400 Bad Request\r\nContent-Length\r\n\r\n";
					break;
				}
				h->content_length *= 10;
				h->content_length += *p - '0';
				p++;
			}

		} else if (str_itest(key, "transfer-encoding")) {
			while (value.len) {
				slice token = next_token(&value);
				if (str_itest(token, "chunked")) {
					h->internal_error = "HTTP/1.1 411 Length Required\r\nContent-Length:0\r\n\r\n";
				}
			}

		} else if (str_itest(key, "connection")) {
			while (value.len) {
				slice token = next_token(&value);
				if (str_itest(token, "close")) {
					h->connection_close = 1;
				} else if (str_itest(token, "upgrade")) {
					h->websocket_flags |= HAVE_UPGRADE;
				}
			}

		} else if (str_itest(key, "upgrade") && str_itest(value, "websocket")) {
			h->websocket_flags |= HAVE_WS_UPGRADE;

		} else if (str_itest(key, "expect")) {
			while (value.len) {
				slice token = next_token(&value);
				if (str_itest(token, "100-continue")) {
					h->expect_continue = 1;
				} else {
					h->internal_error = "HTTP/1.1 417 Expectation Failed\r\nContent-Length:0\r\n\r\n";
				}
			}

		} else if (str_itest(key, "sec-websocket-key")) {
			static const char resp[] = "HTTP/1.1 101 \r\nUpgrade:websocket\r\nConnection:upgrade\r\nSec-WebSocket-Accept:";
			static const char guid[] = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
			unsigned char sha1[20];
			SHA1_CTX ctx;
			SHA1Init(&ctx);
			SHA1Update(&ctx, (uint8_t*)value.c_str, value.len);
			SHA1Update(&ctx, (uint8_t*)guid, strlen(guid));
			SHA1Final(sha1, &ctx);
			ca_set(&h->ws_response, resp);
			pb_encode_base64(h->ws_response.c_str + strlen(resp), sha1, sizeof(sha1));
			ca_setlen(&h->ws_response, strlen(resp) + 28);
			ca_add(&h->ws_response, "=\r\n\r\n");
			h->websocket_flags |= HAVE_WS_KEY;

		} else if (str_itest(key, "sec-websocket-version") && str_test(value, "13")) {
			h->websocket_flags |= HAVE_WS_VERSION;
		}

		if (h->on_header) {
			h->on_header(h, key.c_str, value.c_str);
		}
	}
}

static int process_websocket(http *h) {
	if (!h->content_length && h->have_nextch) {
		h->rxbuf[0] = h->nextch;
		h->have_nextch = 0;
	}

	for (;;) {
		int hlen = 6;
		if (h->rxused < h->content_length + hlen) {
			return 0;
		}

		uint8_t *p = (uint8_t*) h->rxbuf + h->content_length;

		if (!(p[1] & WS_MASK)) {
			// all client->server traffic should be masked
			return -1;
		}

		int len = p[1] &~ WS_MASK;

		// decode multibyte lengths
		switch (len) {
		case WS_2BYTE:
			hlen = 8;
			if (h->rxused < h->content_length + hlen) {
				return 0;
			}
			len = ((int)p[2] << 8) | (int)p[3];
			if (len < WS_2BYTE || (p[0]&~WS_FIN) > WS_BINARY) {
				// length must be shortest form
				// multibyte lengths aren't supported on control frames
				return -1;
			}
			break;
		case WS_8BYTE:
			// overlarge message
			return -1;
		}

		// undo the masking
		uint8_t *pmask = p + hlen - 4;
		uint8_t *pdata = p + hlen;
		for (int i = 0; i < len; i++) {
			pdata[i] ^= pmask[i&3];
		}

		// decide what to do with the message
		switch (p[0]) {
		case WS_TEXT:
		case WS_BINARY:
		case WS_CONTINUATION:
		case WS_CONTINUATION | WS_FIN:
		case WS_TEXT | WS_FIN:
		case WS_BINARY | WS_FIN:
			memmove(h->rxbuf + h->content_length, p, h->rxused - h->content_length - hlen);
			h->rxused -= hlen;
			h->content_length += len;
			if (!(p[0] & WS_FIN)) {
				continue;
			}
			h->length_remaining = h->content_length;
			h->state = HTTP_WEBSOCKET_RECEIVED;
			return 0;

		case WS_PING | WS_FIN:
			if (!h->txnext) {
				ca_clear(&h->ws_response);
				ca_addch(&h->ws_response, (char)(uint8_t)(WS_FIN|WS_PONG));
				ca_addch(&h->ws_response, (char)(uint8_t)len);
				ca_add2(&h->ws_response, (char*)pdata, len);
				http_send_response(h, h->ws_response.c_str, h->ws_response.len);
			}
			continue;

		case WS_PONG | WS_FIN:
			if (h->content_length) {
				// can't be after fragments
				return -1;
			}
			// ignore
			memmove(h->rxbuf, pdata + len, h->rxused - len - hlen);
			continue;

		default:
			// everything else should close the connection
			// this includes WS_CLOSE
			return -1;
		}
	}
}

static void dump_request_data(http *h) {
	assert(h->state == HTTP_DUMPING_DATA && h->txnext != NULL);
	int todump = h->rxused;
	if ((int64_t) todump > h->length_remaining) {
		// we have more than just this request remaining
		todump = (int) h->length_remaining;
		memmove(h->rxbuf, h->rxbuf + todump, h->rxused - todump);
	}
	h->rxused -= todump;
	if (h->content_length >= 0) {
		h->length_remaining -= todump;
		if (!h->length_remaining) {
			h->state = HTTP_SENDING_RESPONSE;
		}
	}
}

static void check_data_received(http *h) {
	assert(h->state == HTTP_RECEIVING_DATA);
	if ((int64_t) h->rxused >= h->length_remaining) {
		h->state = HTTP_DATA_RECEIVED;
	}
}

static int process_request(http *h) {
	slice data = { h->rxused, h->rxbuf };

	switch (h->state) {
	case HTTP_RECEIVING_HEADERS:
		if (!h->method.len) {
			if (h->rxused) {
				h->state = HTTP_RECEIVING_HEADERS;
			}

			switch (process_path(h, &data)) {
			case ERROR:
				return -1;
			case MORE:
				return 0;
			}
		}

		switch (process_headers(h, &data)) {
		case ERROR:
			return -1;
		case MORE:
			return 0;
		}

		h->state = HTTP_HEADERS_RECEIVED;

		// remove the headers from the rxbuffer
		h->rxused = data.len;
		memmove(h->rxbuf, data.c_str, h->rxused);

		if (h->internal_error) {
			// malformed request
			h->connection_close = 1;
			h->content_length = 0;
			h->length_remaining = 0;
			http_send_response(h, h->internal_error, strlen(h->internal_error));
			h->internal_error = NULL;

		} else if (h->content_length >= 0) {
			// normal post with data
			h->length_remaining = h->content_length;

		} else {
			if (h->connection_close) {
				// http/1.0 style post
				h->length_remaining = INT64_MAX;
			} else {
				// get or post with unspecified length
				h->length_remaining = 0;
				h->content_length = 0;
			}
			if (str_itest(h->method, "GET") && h->websocket_flags == HAVE_WS_ALL) {
				// candidate for websocket - still needs to be accepted
				// in http_send_continue
				h->expect_websocket = 1;
			}
		}

		return 0;
	case HTTP_RECEIVING_WEBSOCKET:
		return process_websocket(h);
	case HTTP_DUMPING_DATA:
		dump_request_data(h);
		return 0;
	case HTTP_RECEIVING_DATA:
		check_data_received(h);
		return 0;
	default:
		// just buffer up the data for later processing
		return 0;
	}
}

void http_reset(http *h, char *rxbuf, int sz, http_on_header on_header) {
	h->state = HTTP_RESPONSE_SENT;
	h->on_header = on_header;
	h->expect_continue = 0;
	h->expect_websocket = 0;
	h->connection_close = 0;
	h->have_nextch = 0;
	h->websocket_flags = 0;
	h->rxbuf = rxbuf;
	h->rxbufsz = sz;
	h->rxused = 0;
	h->internal_error = NULL;
	http_next_request(h);
}

int http_next_request(http *h) {
	// these should've already been cleared
	assert(!h->internal_error && !h->expect_continue && !h->expect_websocket);

	if (h->state != HTTP_RESPONSE_SENT) {
		return -1;
	} else if (h->connection_close) {
		return 1;
	}

	if (h->have_nextch) {
		h->rxbuf[0] = h->nextch;
		h->have_nextch = 0;
	}

	h->content_length = 0;
	h->websocket_flags = 0;

	ca_setlen(&h->method, 0);
	ca_setlen(&h->path, 0);
	ca_setlen(&h->ws_response, 0);

	h->txnext = NULL;
	h->txleft = 0;

	h->state = HTTP_RECEIVING_HEADERS;

	return process_request(h);
}

char *http_recv_buffer(const http *h, int *plen) {
	switch (h->state) {
	case HTTP_RECEIVING_HEADERS:
	case HTTP_RECEIVING_DATA:
	case HTTP_RECEIVING_WEBSOCKET:
	case HTTP_DUMPING_DATA:
		*plen = h->rxbufsz - h->rxused;
		return h->rxbuf + h->rxused;
	default:
		*plen = 0;
		return NULL;
	}
}

const char *http_send_buffer(const http *h, int *plen) {
	switch (h->state) {
	case HTTP_SENDING_CONTINUE:
	case HTTP_SENDING_RESPONSE:
	case HTTP_RECEIVING_WEBSOCKET:
	case HTTP_WEBSOCKET_RECEIVED:
		*plen = h->txleft;
		return h->txnext;
	default:
		*plen = 0;
		return NULL;
	}
}

int http_sent(http *h, int w) {
	if (w <= 0 || w > h->txleft) {
		return -1;
	}

	switch (h->state) {
	case HTTP_SENDING_CONTINUE:
	case HTTP_SENDING_RESPONSE:
	case HTTP_RECEIVING_WEBSOCKET:
	case HTTP_WEBSOCKET_RECEIVED:
		break;
	default:
		return -1;
	}

	h->txleft -= w;
	h->txnext += w;

	if (h->txleft) {
		return 0;
	}

	// we've finished the current send, where to next?
	h->txnext = NULL;

	switch (h->state) {
	case HTTP_SENDING_CONTINUE:
		h->state = HTTP_RECEIVING_DATA;
		check_data_received(h);
		return 0;
	case HTTP_SENDING_RESPONSE:
		h->state = HTTP_RESPONSE_SENT;
		return 0;
	default:
		// websocket states
		return 0;
	}
}

int http_received(http *h, int r) {
	if (r < 0 || h->rxused + r > h->rxbufsz) {
		return -1;
	}

	h->rxused += r;

	if (!r) {
		if (h->content_length < 0) {
			// http/1.0 style post
			// process_request will then take it from here
			h->content_length = h->rxused;
			h->length_remaining = h->rxused;
		} else {
			return -1;
		}
	}

	return process_request(h);
}

int http_send_continue(http *h) {
	if (h->state != HTTP_HEADERS_RECEIVED) {
		// invalid usage
		return -1;
	}

	if (h->expect_websocket) {
		// accept the websocket
		assert(h->ws_response.len);
		h->txleft = h->ws_response.len;
		h->txnext = h->ws_response.c_str;
		h->state = HTTP_RECEIVING_WEBSOCKET;
		h->content_length = -1;
		h->expect_websocket = 0;
		return 0;
	} else if (h->expect_continue) {
		// accept the post
		static const char resp[] = "HTTP/1.1 100 Continue\r\n\r\n";
		h->txleft = strlen(resp);
		h->txnext = resp;
		h->state = HTTP_SENDING_CONTINUE;
		h->expect_continue = 0;
		return 0;
	} else {
		// just carry on receiving data
		h->state = HTTP_RECEIVING_DATA;
		check_data_received(h);
		return 0;
	}
}

int http_send_response(http *h, const char *p, int len) {
	if (h->txnext || h->txleft) {
		// invalid usage - can't send when a response is still active
		return -1;
	}

	h->txnext = p;
	h->txleft = len;

	switch (h->state) {
	case HTTP_HEADERS_RECEIVED:
		// we've elected not to send the continue
		if (h->expect_websocket) {
			h->expect_websocket = 0;
		} else if (h->expect_continue) {
			// The client's specified content-length is only applicable
			// if we accept and send a 100-continue. If we don't it's as
			// if the client had send a Content-Length:0.
			h->expect_continue = 0;
			h->length_remaining = 0;
		}
		h->state = HTTP_DUMPING_DATA;
		dump_request_data(h);
		return 0;
	case HTTP_RECEIVING_DATA:
		// we sent the continue, but have an early response
		// dump_request_data will set the state to HTTP_SENDING_RESPONSE
		// once the rest of the request data has come in
		h->state = HTTP_DUMPING_DATA;
		dump_request_data(h);
		return 0;
	case HTTP_DATA_RECEIVED:
		// we sent the continue and have received all of the request data
		// dump_request_data should dump the rest and then switch on to sending the response
		h->state = HTTP_DUMPING_DATA;
		dump_request_data(h);
		assert(h->state == HTTP_SENDING_RESPONSE);
		return 0;
	case HTTP_DATA_CONSUMED:
	case HTTP_RESPONSE_SENT:
		// CONSUMED - user has processed all payload bytes
		// RESPONSE_SENT - user wants to send some more data - used for streaming output
		h->state = HTTP_SENDING_RESPONSE;
		return 0;
	case HTTP_RECEIVING_WEBSOCKET:
	case HTTP_WEBSOCKET_RECEIVED:
		// nothing to do beyond setting up txnext & txleft above
		return 0;
	default:
		// do nothing - invalid usage
		return -1;
	}
}

char *http_request_data(http *h, int *plen) {
	switch (h->state) {
	case HTTP_RECEIVING_DATA:
	case HTTP_DATA_RECEIVED:
	case HTTP_WEBSOCKET_RECEIVED:
		// save off the first byte of the next request so we can
		// overwrite it with a null
		if ((int64_t) h->rxused > h->length_remaining) {
			h->have_nextch = 1;
			h->nextch = h->rxbuf[h->length_remaining];
			*plen = (int) h->length_remaining;
		} else {
			*plen = h->rxused;
		}
		h->rxbuf[*plen] = '\0';
		return h->rxbuf;
	default:
		*plen = 0;
		return NULL;
	}
}

int http_consume_data(http *h, int used) {
	if ((int64_t)used > min((int64_t)h->rxused, h->length_remaining)) {
		// invalid usage
		return -1;
	}
	if (h->content_length >= 0) {
		h->length_remaining -= used;
	}

	switch (h->state) {
	case HTTP_RECEIVING_DATA:
		assert(h->length_remaining > 0);
		break;
	case HTTP_DATA_RECEIVED:
		if (h->length_remaining == 0) {
			h->state = HTTP_DATA_CONSUMED;
		}
		break;
	case HTTP_WEBSOCKET_RECEIVED:
		if (h->length_remaining == 0) {
			h->state = HTTP_RECEIVING_WEBSOCKET;
			h->content_length = 0;
		}
		break;
	default:
		return -1;
	}

	memmove(h->rxbuf, h->rxbuf + used, h->rxused - used);
	h->rxused -= used;
	return 0;
}
