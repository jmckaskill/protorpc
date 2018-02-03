#include <protorpc/http.h>
#include <protorpc/char-array.h>

#define ERROR -1
#define DONE 0
#define MORE 1
#define MAX_LINE_LENGTH 256

#ifdef _MSC_VER
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
static void *memmem(const void *hay, size_t haysz, const void *needle, size_t needlesz) {
	while (needlesz <= haysz) {
		uint8_t *test = (uint8_t*)memchr(hay, *(uint8_t*)needle, haysz - needlesz + 1);
		if (!test) {
			return NULL;
		}
		if (!memcmp(test, needle, needlesz)) {
			return test;
		}
		test++;
		haysz = (uint8_t*)hay + haysz - test;
		hay = test;
	}
	return NULL;
}
#endif

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

	if (h->have_nextch) {
		line.c_str[0] = h->nextch;
		h->have_nextch = 0;
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
			h->have_content_length = 1;
			char *p = value.c_str;
			while (*p) {
				// include check for overflow
				if (*p < '0' || *p > '9' || h->content_length > (INT64_MAX >> 4)) {
					return ERROR;
				}
				h->content_length *= 10;
				h->content_length += *p - '0';
				p++;
			}
			h->length_remaining = h->content_length;

		} else if (str_itest(key, "transfer-encoding")) {
			while (value.len) {
				slice token = next_token(&value);
				if (str_itest(token, "chunked")) {
					return ERROR;
				}
			}

		} else if (str_itest(key, "connection")) {
			while (value.len) {
				slice token = next_token(&value);
				if (str_itest(token, "close")) {
					h->connection_close = 1;
				}
			}
		
		} else if (str_itest(key, "expect")) {
			while (value.len) {
				slice token = next_token(&value);
				if (str_itest(token, "100-continue")) {
					h->expect_continue = 1;
				}
			}
			
		}
		
		if (h->on_header) {
			h->on_header(h, key.c_str, value.c_str);
		}
	}
}

static void check_request_data(http *h) {
	if (h->have_content_length && h->rxused >= h->length_remaining) {
		h->state = HTTP_DATA_RECEIVED;
		if (!h->have_nextch && h->rxused > h->length_remaining) {
			h->have_nextch = 1;
			h->nextch = h->rxbuf[h->length_remaining];
		}
		if (h->txnext) {
			h->state = HTTP_SENDING_RESPONSE;
		}
	}
}

static int process_request(http *h) {
	slice data = { h->rxused, h->rxbuf };

	switch (h->state) {
	case HTTP_IDLE:
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

		if (!h->have_content_length && !h->connection_close) {
			h->have_content_length = 1;
			h->length_remaining = 0;
			h->content_length = 0;
		}

		h->state = HTTP_HEADERS_RECEIVED;
		break;
	default:
		break;
	}

	if (h->dump_request_data) {
		int todump = data.len;
		if (h->have_content_length && (int64_t)todump > h->length_remaining) {
			todump = (int)h->length_remaining;
		}
		data.c_str += todump;
		data.len -= todump;
	}

	if (data.c_str > h->rxbuf) {
		h->rxused = data.len;
		memmove(h->rxbuf, data.c_str, h->rxused);
	}

	if (h->state == HTTP_RECEIVING_DATA) {
		check_request_data(h);
	}

	return 0;
}

void http_reset(http *h, char *rxbuf, int sz, http_on_header on_header) {
	h->state = HTTP_RESPONSE_SENT;
	h->on_header = on_header;
	h->connection_close = 0;
	h->have_nextch = 0;
	h->rxbuf = rxbuf;
	h->rxbufsz = sz;
	h->rxused = 0;
	http_next_request(h);
}

int http_next_request(http *h) {
	if (h->state != HTTP_RESPONSE_SENT) {
		return -1;
	} else if (h->connection_close) {
		return 1;
	}

	if (h->have_nextch) {
		h->rxbuf[0] = h->nextch;
		h->have_nextch = 0;
	}

	h->expect_continue = 0;
	h->have_content_length = 0;
	h->dump_request_data = 0;

	ca_setlen(&h->method, 0);
	ca_setlen(&h->path, 0);

	h->txnext = NULL;
	h->txleft = 0;

	h->state = HTTP_IDLE;

	return process_request(h);
}

char *http_recv_buffer(const http *h, int *plen) {
	switch (h->state) {
	case HTTP_IDLE:
	case HTTP_RECEIVING_HEADERS:
	case HTTP_RECEIVING_DATA:
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
		*plen = h->txleft;
		return h->txnext;
	default:
		*plen = 0;
		return NULL;
	}
}

int http_sent(http *h, int w) {
	if ((h->state != HTTP_SENDING_CONTINUE && h->state != HTTP_SENDING_RESPONSE) 
		|| w <= 0 || w > h->txleft) {
		return -1;
	}

	h->txleft -= w;
	h->txnext += w;

	if (h->txleft) {
		return 0;
	}

	h->txnext = NULL;

	if (h->state == HTTP_SENDING_CONTINUE) {
		h->state = HTTP_RECEIVING_DATA;
		check_request_data(h);
	} else {
		h->state = HTTP_RESPONSE_SENT;
	}

	return 0;
}

int http_received(http *h, int r) {
	if (r < 0 || h->rxused + r > h->rxbufsz) {
		return -1;
	}

	h->rxused += r;

	if (!r) {
		if (h->state == HTTP_RECEIVING_DATA && h->connection_close) {
			h->state = HTTP_DATA_RECEIVED;
			return 0;
		} else if (h->state == HTTP_IDLE) {
			return 1;
		} else {
			return -1;
		}
	}

	return process_request(h);
}

static const char continue_response[] =
	"HTTP/1.1 100 Continue\r\n\r\n";

void http_send_continue(http *h) {
	if (h->state != HTTP_HEADERS_RECEIVED) {
		// do nothing
	} else if (h->expect_continue) {
		h->txleft = sizeof(continue_response) - 1;
		h->txnext = continue_response;
		h->state = HTTP_SENDING_CONTINUE;
	} else {
		h->state = HTTP_RECEIVING_DATA;
		check_request_data(h);
	}
}

void http_send_response(http *h, const char *p, int len) {
	h->txnext = p;
	h->txleft = len;

	switch (h->state) {
	case HTTP_HEADERS_RECEIVED:
		// we've elected not to send the continue
		if (h->expect_continue) {
			h->expect_continue = 0;
			h->length_remaining = 0;
		} else {
			h->dump_request_data = 1;
		}
		h->state = HTTP_RECEIVING_DATA;
		check_request_data(h);
		break;
	case HTTP_RECEIVING_DATA:
		// we sent the continue, but have an early response
		// check_request_data will set the state to HTTP_SENDING_RESPONSE
		// once the rest of the request data has come in
		h->dump_request_data = 1; // dump data yet to be received
		h->rxused = 0; // dump what we've already got
		break;
	case HTTP_DATA_RECEIVED: {
		// we sent the continue and have received all of the request data
		h->state = HTTP_SENDING_RESPONSE;
		int todump = h->rxused;
		if (h->have_content_length && (int64_t)todump > h->length_remaining) {
			todump = (int)h->length_remaining;
		}
		memmove(h->rxbuf, h->rxbuf + todump, h->rxused - todump);
		h->rxused -= todump;
		break;
	}
	case HTTP_RESPONSE_SENT:
		// user wants to send some more data - used for streaming output
		h->state = HTTP_SENDING_RESPONSE;
		break;
	default:
		// do nothing - invalid usage
		break;
	}
}

char *http_request_data(const http *h, int *plen) {
	if (h->state != HTTP_RECEIVING_DATA && h->state != HTTP_DATA_RECEIVED) {
		*plen = 0;
		return NULL;
	}
	if (h->have_content_length && h->rxused > h->length_remaining) {
		*plen = (int) h->length_remaining;
	} else {
		*plen = h->rxused;
	}
	return h->rxbuf;
}

void http_consume_data(http *h, int used) {
	if ((h->state != HTTP_DATA_RECEIVED && h->state != HTTP_RECEIVING_DATA)
		|| used > h->rxused 
		|| (h->have_content_length && used > h->length_remaining)) {
		return;
	}
	if (h->have_content_length) {
		h->length_remaining -= used;
	}
	memmove(h->rxbuf, h->rxbuf + used, h->rxused - used);
	h->rxused -= used;
}