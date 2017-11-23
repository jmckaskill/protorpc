#include "../protorpc.h"
#include "../char-array.h"
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#ifdef _MSC_VER
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
static void *memmem(const void *hay, size_t haysz, const void *needle, size_t needlesz) {
	while (needlesz <= haysz) {
		uint8_t *test = (uint8_t*) memchr(hay, *(uint8_t*) needle, haysz - needlesz + 1);
		if (!test) {
			return NULL;
		}
		if (!memcmp(test, needle, needlesz)) {
			return test;
		}
		test++;
		haysz = (uint8_t*) hay + haysz - test;
		hay = test;
	}
	return NULL;
}
#endif

#define MAX_LINE_LENGTH 256

static int get_line(char **p, char *e) {
	char *nl = (char*) memchr(*p, '\n', e - *p);
	if (!nl && (e - *p) > MAX_LINE_LENGTH) {
		return PR_ERROR;
	} else if (!nl) {
		return PR_CONTINUE;
	} else if (nl == *p || nl[-1] != '\r') {
		return PR_ERROR;
	}

	*nl = '\0';
	*p = nl + 1;
	return PR_FINISHED;
}

static int parse_request_header(struct pr_http *h, char **p, char *e) {
    assert(h->path.len == 0);
    assert(!h->method);
    assert(!h->version);

	char *n = *p;
	int err = get_line(p, e);
	if (err) {
		h->error_string = "414 URI Too Long";
		return err;
	}

	char *nl = *p - 2;
    
	slice_t method;
    method.buf = n;
    n = (char*) memchr(method.buf, ' ', nl - method.buf);
    if (!n) {
        h->error_string = "400 Malformed Path";
        return PR_ERROR;
    }
    method.len = (int) (n - method.buf);

	slice_t path;
    path.buf = n + 1;
    n = (char*) memchr(path.buf, ' ', nl - path.buf);
    if (!n) {
        h->error_string = "400 Malformed Path";
        return PR_ERROR;
    }
    path.len = (int) (n - path.buf);

	slice_t version;
    version.buf = n + 1;
    version.len = (int) (nl - version.buf);

    if (str_itest(method, "GET")) {
        h->method = PR_HTTP_GET;
    } else if (str_itest(method, "POST")) {
        h->method = PR_HTTP_POST;
    } else {
        h->error_string = "501 Method Not Implemented";
        return PR_ERROR;
    }

    if (str_itest(version, "HTTP/1.1")) {
		h->version_1_0 = 0;
    } else if (str_itest(version, "HTTP/1.0")) {
		h->version_1_0 = 1;
    } else {
        h->error_string = "505 Version Not Supported";
        return PR_ERROR;
    }

    // check for an absolute form address e.g. GET http://www.example.com:80/foo HTTP/1.1
    char *scheme = (char*) memmem(path.buf, path.len, "://", 3);
    if (scheme) {
        char *slash = (char*) memchr(scheme + 3, '/', path.buf + path.len - (scheme+3));
        if (slash) {
            path.len += (int) (slash - path.buf);
            path.buf = slash;
        } else {
            path.len = 1;
            path.buf = (char*) "/";
        }
    }

    // remove the query string
    char *query = memchr(path.buf, '?', path.len);
    if (query) {
        path.len = (int) (query - path.buf);
    }

	// path checks - generally refuse anything that may cause a root path escape
	// or we don't want to process
    if (memchr(path.buf, '%', path.len)
	 || memmem(path.buf, path.len, "/..", 3)
	 || memmem(path.buf, path.len, "//", 2)
	 || memchr(path.buf, '\\', path.len)
	 || path.len == 0
	 || path.buf[0] != '/') {
        h->error_string = "400 Malformed Path";
        return PR_ERROR;
    }

    // take a local copy of the path string

    if (ca_addstr(&h->path, path)) {
        h->error_string = "414 URI Too Long";
        return PR_ERROR;
    }

    return PR_FINISHED;
}

static char *consume_whitespace(char *p) {
    while (*p == ' ' || *p == '\t') {
        p++;
    }
    return p;
}

// the newline has already been verified, so we don't need to check against char *e
static char *next_element(char **p, bool *more) {
    char *ret = *p = consume_whitespace(*p);

    while (**p != ' ' && **p != '\t' && **p != ';' && **p != ',' && **p != '\r') {
        assert(**p != '\n');
        (*p)++;
    }

    char *end = *p;
    *p = consume_whitespace(end);

    if (more && (*end == ',' || *end == ';')) {
        // another element after this one
		(*p)++;
		*more = true;

    } else if (*end == '\r') {
        // reached end of line
		if (more) {
			*more = false;
		}

    } else {
        // invalid line
        return NULL;
    }

    *end = '\0';
    return ret;
}

static int parse_http_header(char **p, char *e, slice_t *key, char **val) {
	char *n = *p;
	int err = get_line(p, e);
	if (err) {
		return err;
	}

	if (*p - 2 == n) {
		key->len = 0;
		return PR_FINISHED;
	}

	char *colon = (char*) memchr(n, ':', *p - n);
	if (!colon || colon == n) {
		return PR_ERROR;
	}

	key->buf = n;
	key->len = (int) (colon - n);
	*val = colon + 1;

	return PR_FINISHED;
}

#define HTTP_ETAG_LENGTH 16 // 8 hex characters

static int parse_headers(struct pr_http *h, char **p, char *e) {
    for (;;) {
		slice_t key;
		char *val;
		int err = parse_http_header(p, e, &key, &val);
		if (err) {
			h->error_string = "400 Bad Request";
			return err;
		}

        bool more;

		if (!key.len) {
			if (h->length_type == HTTP_LENGTH_UNSET) {
				h->length_type = HTTP_LENGTH_FIXED;
				h->content_length = 0;
			}
            return PR_FINISHED;

		} else if (str_itest(key, "transfer-encoding")) {
			do {
				char *n = next_element(&val, &more);
				if (!n) {
					h->error_string = "400 Malformed Header";
					return PR_ERROR;
				}
				if (!strcasecmp(n, "chunked")) {
					if (h->length_type != HTTP_LENGTH_UNSET) {
						h->error_string = "400 Bad Request";
						return PR_ERROR;
					}
					h->length_type = HTTP_LENGTH_CHUNKED;
				}
			} while (more);

		} else if (str_itest(key, "content-type")) {
			char *n = next_element(&val, &more);
			if (!strcasecmp(n, "multipart/form-data")) {
				while (more) {
					char *n = next_element(&val, &more);
					if (!strncasecmp(n, "boundary=", strlen("boundary="))) {
						char *boundary = n + strlen("boundary=");
						ca_set(&h->boundary, boundary);
					}
				}

				if (!h->boundary.len) {
					h->error_string = "400 Bad Request";
					return PR_ERROR;
				}

				h->multipart_form = 1;
			}

        } else if (str_itest(key, "content-length")) {
            if (h->length_type != HTTP_LENGTH_UNSET) {
                h->error_string = "400 Bad Request";
                return PR_ERROR;
            }
            char *n = next_element(&val, NULL);
            if (!n) {
				h->error_string = "400 Malformed Header";
                return PR_ERROR;
            }
            while (*n) {
                if (*n < '0' || *n > '9') {
                    h->error_string = "400 Malformed Content-Length";
                    return PR_ERROR;
                }
                // check for overflow
                if (h->content_length > (INT64_MAX >> 4)) {
                    h->error_string = "413 Request Too Large";
                    return PR_ERROR;
                }
                h->content_length = (h->content_length * 10) + (*n - '0');
                n++;
            }
			h->length_type = HTTP_LENGTH_FIXED;

        } else if (str_itest(key, "connection")) {
            do {
                char *n = next_element(&val, &more);
                if (!n) {
					h->error_string = "400 Malformed Header";
                    return PR_ERROR;
                }

                if (!strcasecmp(n, "close")) {
                    if (h->length_type != HTTP_LENGTH_UNSET) {
                        h->error_string = "400 Bad Request";
                        return PR_ERROR;
                    }
                    h->length_type = HTTP_LENGTH_CLOSE;
                }
            } while (more);

        } else if (str_itest(key, "cookie")) {
            do {
                char *n = next_element(&val, &more);
                if (!n) {
					h->error_string = "400 Malformed Header";
                    return PR_ERROR;
                }
                char *equals = strchr(n, '=');
                if (!equals) {
                    h->error_string = "400 Malformed Cookie";
                    return PR_ERROR;
                }
                *equals = '\0';
                char *cookie = equals+1;
                if (!strcmp(n, "login")) {
                    ca_setlen(&h->login, 0);
                    ca_add(&h->login, cookie);
                }
            } while (more);

        } else if (str_itest(key, "expect")) {
            do {
                char *n = next_element(&val, &more);
                if (!n) {
					h->error_string = "400 Malformed Header";
                    return PR_ERROR;
                }
                if (!strcasecmp(n, "100-continue")) {
                    h->expect_continue = 1;
                } else {
                    h->error_string = "417 Bad Expectation";
                    return PR_ERROR;
                }
            } while (more);

        } else if (str_itest(key, "if-none-match")) {
            char *n = next_element(&val, NULL);
            // ignore invalid etag
            if (n && strlen(n) == HTTP_ETAG_LENGTH) {
                for (int i = 0; i < HTTP_ETAG_LENGTH; i++) {
                    h->etag = (h->etag << 4) | (uint64_t) (n[i] - 'A');
                }
                h->have_etag = 1;
            }
        }
    }
}

int pr_parse_request(struct pr_http *h, char *buf, int *avail) {
	char *next = buf;
    char *end = buf + *avail;
    int err = 0;

    if (!h->method) {
        err = parse_request_header(h, &next, end);
    }
    if (!err) {
        err = parse_headers(h, &next, end);
    }
	assert(err != PR_ERROR || h->error_string);

	memmove(buf, next, end - next);
    *avail = (int) (end - next);
    return err;
}

int pr_parse_body(struct pr_http *h, char **data, int *sz) {
}

int pr_parse_multipart(struct pr_multipart *m, char **data, int *sz) {
	if (!m->have_boundary) {
		char *b = *data;
		int err = get_line(b, b + *sz);
		if (err) {
			return err;
		}
		ca_add(&m->boundary, "\r\n");
		ca_add2(&m->boundary, b, (int) (*p - b - 2));
		m->have_boundary = 1;
	}

	if (!m->have_header) {
		for (;;) {
			slice_t key;
			char *val;
			int err = parse_http_header(p, e, &key, &val);
			if (err) {
				return err;
			}

			if (!key.len) {
				m->have_header = true;
				break;
			} else if (str_iequals(&key, "content-disposition")) {
				bool more;
				do {
					char *n = next_element(&val, &more);
					if (!n) {
						return PR_ERROR;
					} else if (!strncmp(n, "name=\"", 6)) {
						char *name = n + 6;
						char *quote = strchr(name, '\"');
						if (!quote) {
							return PR_ERROR;
						}
						ca_set2(&m->name, name, (int) (quote - name));
						break;
					}
				} while (more);
			}
		}
	}

	// look for the boundary at the head of the buffer
	// the last boundary is followed by --\r\n
	// a not-last boundary is followed by \r\n

	m->next_out = *p;

	for (;;) {
		if (e - *p < m->boundary.len && !memcmp(*p, m->boundary.buf, e - *p)) {
			// we have an incomplete boundary at the head of the incoming buffer
			return PR_CONTINUE;
		}

		if (e - *p >= m->boundary.len + 2 && !memcmp(*p, m->boundary.buf, m->boundary.len)) {
			// we have a complete boundary at the head of the incoming buffer
			if (e - *p < m->boundary.len + 2) {
				return PR_CONTINUE;
			}
			// check whether it's the last boundary or not
			if (!memcmp(*p + m->boundary.len, "--", 2)) {
				// last boundary
				*p += m->boundary.len;
				return PR_FINISHED;
			} else if (!memcmp(*p + m->boundary.len, "\r\n", 2)) {
				// not last
				m->have_boundary = false;
				m->have_header = false;
				if (m->avail_out) {
					return PR_CONTINUE;
				}
				p += m->boundary.len + 2;
				break;
			} else {
				return PR_ERROR;
			}
		}

		// read until the next boundary candidate
		char *start = *p + 1;
		char *cr = memchr(start, '\r', e - start);
		if (!cr) {
			*p = e;
			m->avail_out = (int) (e - m->next_out);
			return PR_CONTINUE;
		}

		// loop around to test the next boundary candidate
		m->avail_out = (int) (cr - m->next_out);
		*p = cr;
	}
}

int pr_parse_multipart(struct pr_multipart *m, char **data, int *sz) {
	char *end = *data *sz;
	int err = do_parse_multipart(m, next, end);
	*avail = (int) (end - *next);
	return err;
}
