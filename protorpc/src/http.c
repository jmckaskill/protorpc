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

	slice version;
	if (split(&line, &version, ' ')) {
		return ERROR;
	}

	h->version_1_0 = str_itest(version, "HTTP/1.0") ? 1 : 0;

	ca_setstr(&h->method, method);
	ca_setstr(&h->path, line);

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
		line->len = 0;
		return *line;
	}
	trim_right(&token);
	trim_left(line);
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

		slice value;
		if (split(&line, &value, ':')) {
			return ERROR;
		}

		slice key = line;
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
				if (*p < '0' || *p > '0' || h->content_length > (INT64_MAX >> 4)) {
					return ERROR;
				}
				h->content_length *= 10;
				h->content_length += *p - '0';
			}

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
			
		} else if (h->on_header) {
			h->on_header(h, key.c_str, value.c_str);
		}

	}
}

void http_reset(http *h, char *rxbuf, int rxbufsz, char *txbuf, int txbufsz, http_on_header on_header) {
	assert(rxbufsz >= 256 && txbufsz >= 256);
	memset(h, 0, sizeof(*h));
	h->on_header = on_header;
	h->rxbuf = rxbuf;
	h->rxbufsz = rxbufsz;
	h->txbuf = txbuf;
	h->txbufsz = txbufsz;
	http_next_request(h);
}

void http_next_request(http *h) {
	assert(h->txleft == 0);
	assert(!h->connection_close);
	assert(!h->expect_continue);

	if (h->length_remaining) {
		http_consume_data(h, h->length_remaining);
	}

	if (h->have_nextch) {
		h->rxbuf[0] = h->nextch;
		h->have_nextch = 0;
	}

	h->content_length = 0;
	h->length_remaining = 0;

	h->path_complete = 0;
	h->headers_complete = 0;
	h->request_complete = 0;
	h->response_complete = 0;

	h->method.len = 0;
	h->path.len = 0;
}

char *http_recv_buffer(http *h, int *plen) {
	if (h->txleft || (h->request_complete && h->connection_close)) {
		*plen = 0;
		return NULL;
	} else {
		*plen = h->rxbufsz - h->rxused;
		return h->rxbuf + h->rxused;
	}
}

char *http_send_buffer(http *h, int *plen) {
	*plen = h->txleft;
	return h->txnext;
}

int http_sent(http *h, int w) {
	if (w <= 0 || w > h->txleft) {
		return -1;
	}

	h->txleft -= w;
	h->txnext += w;

	if (h->txleft) {
		// more data to be sent
		return 0;
	} else if (h->expect_continue) {
		// finished sending the 101 continue
		h->expect_continue = 0;
		return 0;
	} else if (h->connection_close) {
		// connection:close finish
		return 1;
	} else {
		// finished sending the full response
		h->response_complete = 1;
		return 0;
	}
}

int http_received(http *h, int r) {
	if (r < 0 || h->rxused + r > h->rxbufsz) {
		return -1;
	}

	h->rxused += r;

	if (!r) {
		if (h->headers_complete && h->connection_close) {
			h->request_complete = 1;
			return 0;
		} else {
			return -1;
		}
	}

	slice data = { h->rxused, h->rxbuf };

	if (!h->path_complete) {
		switch (process_path(h, &data)) {
		case ERROR:
			return -1;
		case MORE:
			return 0;
		}
		h->path_complete = true;
	}

	if (!h->headers_complete) {
		switch (process_headers(h, &data)) {
		case ERROR:
			return -1;
		case MORE:
			return 0;
		}
		h->headers_complete = true;
	}

	if (h->expect_continue) {
		h->txleft = sprintf(h->txbuf, "100 Continue\r\n\r\n");
		h->txnext = h->txbuf;
	}

	if (data.c_str > h->rxbuf) {
		h->rxused = data.len;
		memmove(h->rxbuf, data.c_str, h->rxused);
	}

	if (!h->connection_close && h->rxused >= h->length_remaining) {
		h->request_complete = 1;
		if (h->rxused > h->length_remaining) {
			h->nextch = h->rxbuf[h->rxused];
			h->have_nextch = 1;
		}
	}

	return 0;
}

char *http_request_data(http *h, int *plen) {
	assert(h->headers_complete);
	if (!h->connection_close && h->rxused > h->length_remaining) {
		*plen = h->length_remaining;
	} else {
		*plen = h->rxused;
	}
	return h->rxbuf;
}

void http_consume_data(http *h, int used) {
	assert(used <= h->rxused);
	if (!h->connection_close) {
		assert(used <= h->length_remaining);
		h->length_remaining -= used;
	}
	memmove(h->rxbuf, h->rxbuf + used, h->rxused - used);
	h->rxused -= used;
}

static const char response_template[] =
	"HTTP/1.1 123 \r\nContent-Length:      \r\n\r\n";

char *http_start_response(http *h, int *plen) {
	assert(h->headers_complete);
	size_t tsz = sizeof(response_template) - 1;
	memcpy(h->txbuf, response_template, tsz);
	if (plen) {
		*plen = h->txbufsz - tsz;
	}
	return h->txbuf;
}

void http_finish_response(http *h, int sts, int len) {
	char *p = h->txbuf + strlen("HTTP/1.1 ");
	*(p++) = (char)(sts / 100) + '0';
	*(p++) = (char)((sts % 100) / 10) + '0';
	*(p++) = (char)(sts % 10) + '0';

	// start at last digit and work backwards
	p += strlen(" \r\nContent-Length:") + 5;
	do {
		*p-- = (len % 10) + '0';
		len /= 10;
	} while (len);

	h->txnext = h->txbuf;
	h->txleft = sizeof(response_template) - 1 + len;
}

void http_send_data(http *h, char *p, int len) {
	h->txnext = p;
	h->txleft = len;
}

