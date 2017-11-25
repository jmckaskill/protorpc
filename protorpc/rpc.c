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

static int get_line(pb_string_t *p, pb_string_t *line) {
	const char *nl = str_find_char(*p, '\n');
	if (!nl && p->len > MAX_LINE_LENGTH) {
		return PR_ERROR;
	} else if (!nl) {
		return PR_CONTINUE;
	} else if (nl == p->c_str || nl[-1] != '\r') {
		return PR_ERROR;
	}

	line->c_str = p->c_str;
	line->len = nl - 1 - p->c_str; // don't include the \r
	p->len -= nl+1 - p->c_str;
	p->c_str = nl + 1;
	return PR_FINISHED;
}

static int split(pb_string_t in, pb_string_t *left, pb_string_t *right, char ch) {
	const char *p = str_find_char(in, ch);
	if (!p) {
		return -1;
	}
	const char *b = in.c_str;
	const char *e = in.c_str + in.len;
	left->c_str = b;
	left->len = (int) (p - b);
	right->c_str = p+1;
	right->len = (int) (e - (p+1));
	return 0;
}

static int parse_request_header(struct pr_http *h, pb_string_t *data) {
    assert(h->name.len == 0);
    assert(!h->method);

	pb_string_t line;
	int err = get_line(data, &line);
	if (err) {
		h->error_string = "414 URI Too Long";
		return err;
	}
    
	pb_string_t method;
	if (split(line, &method, &line, ' ')) {
        h->error_string = "400 Malformed Path";
        return PR_ERROR;
    }

	pb_string_t path, version;
	if (split(line, &path, &version, ' ')) {
        h->error_string = "400 Malformed Path";
        return PR_ERROR;
    }

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
    const char *scheme = (const char*) memmem(path.c_str, path.len, "://", 3);
    if (scheme) {
		const char *pathe = path.c_str + path.len;
		pb_string_t host = {pathe - (scheme+3), scheme+3};
        const char *slash = str_find_char(host, '/');
        if (slash) {
            path.len = pathe - slash;
            path.c_str = slash;
        } else {
            path.len = 1;
            path.c_str = "/";
        }
    }

    // remove the query string
    const char *query = str_find_char(path, '?');
    if (query) {
        path.len = (int) (query - path.c_str);
    }

	// path checks - generally refuse anything that may cause a root path escape
	// or we don't want to process
    if (str_find_char(path, '%')
	 || memmem(path.c_str, path.len, "/..", 3)
	 || memmem(path.c_str, path.len, "//", 2)
	 || str_find_char(path, '\\')
	 || path.len == 0
	 || path.c_str[0] != '/') {
        h->error_string = "400 Malformed Path";
        return PR_ERROR;
    }

    // take a local copy of the path string

    if (ca_addstr(&h->name, path)) {
        h->error_string = "414 URI Too Long";
        return PR_ERROR;
    }

    return PR_FINISHED;
}

static void consume_whitespace(pb_string_t *p) {
	while (p->len && (p->c_str[0] == ' ' || p->c_str[0] == '\t')) {
		p->c_str++;
		p->len--;
	}
}

// the newline has already been verified, so we don't need to check against char *e
static pb_string_t next_element(pb_string_t *p, bool *more) {
	consume_whitespace(p);
	pb_string_t ret = *p;

	while (p->len && (p->c_str[0] != ' ' && p->c_str[0] != '\t' && p->c_str[0] != ';' && p->c_str[0] != ',')) {
		p->c_str++;
		p->len--;
    }

	ret.len = (int) (p->c_str - ret.c_str);
	consume_whitespace(p);

    if (more && p->len && (p->c_str[0] == ',' || p->c_str[0] == ';')) {
		// another element after this one
		p->c_str++;
		p->len--;
		*more = true;
    } else if (!p->len) {
        // reached end of line
		if (more) {
			*more = false;
		}
    } else {
		// invalid line
		ret.c_str = NULL;
		ret.len = 0;
    }

    return ret;
}

static int parse_http_header(pb_string_t *p, pb_string_t *key, pb_string_t *val) {
	pb_string_t line;
	int err = get_line(p, &line);
	if (err) {
		return err;
	}

	if (!line.len) {
		key->len = 0;
		return PR_FINISHED;
	}

	return split(line, key, val, ':') ? PR_ERROR : PR_FINISHED;
}

#define HTTP_ETAG_LENGTH 16 // 8 hex characters

static int parse_headers(struct pr_http *h, pb_string_t *data) {
    for (;;) {
		pb_string_t key, val;
		int err = parse_http_header(data, &key, &val);
		if (err) {
			h->error_string = "400 Bad Request";
			return err;
		}

        bool more;

		if (!key.len) {
			if (h->length_type == PR_HTTP_LENGTH_UNSET) {
				h->length_type = PR_HTTP_LENGTH_FIXED;
				h->content_length = 0;
				h->have_body = 0;
			}
            return PR_FINISHED;

		} else if (str_itest(key, "transfer-encoding")) {
			do {
				pb_string_t n = next_element(&val, &more);
				if (n.len) {
					h->error_string = "400 Malformed Header";
					return PR_ERROR;
				}
				if (str_itest(n, "chunked")) {
					if (h->length_type != PR_HTTP_LENGTH_UNSET) {
						h->error_string = "400 Bad Request";
						return PR_ERROR;
					}
					h->length_type = PR_HTTP_LENGTH_CHUNKED;
					h->have_body = 1;
				}
			} while (more);

		} else if (str_itest(key, "content-type")) {
			pb_string_t n = next_element(&val, &more);
			if (str_itest(n, "multipart/form-data")) {
				while (more) {
					n = next_element(&val, &more);
					if (str_ibegins_with(n, "boundary=")) {
						size_t off = strlen("boundary=");
						ca_set2(&h->boundary, n.c_str + off, n.len - off);
					}
				}

				if (!h->boundary.len) {
					h->error_string = "400 Bad Request";
					return PR_ERROR;
				}

				h->multipart_form = 1;
			}

        } else if (str_itest(key, "content-length")) {
            if (h->length_type != PR_HTTP_LENGTH_UNSET) {
                h->error_string = "400 Bad Request";
                return PR_ERROR;
            }
            pb_string_t n = next_element(&val, NULL);
            if (!n.len) {
				h->error_string = "400 Malformed Header";
                return PR_ERROR;
            }
            while (n.len) {
                if (n.c_str[0] < '0' || n.c_str[0] > '9') {
                    h->error_string = "400 Malformed Content-Length";
                    return PR_ERROR;
                }
                // check for overflow
                if (h->content_length > (INT64_MAX >> 4)) {
                    h->error_string = "413 Request Too Large";
                    return PR_ERROR;
                }
				h->content_length = (h->content_length * 10) + (n.c_str[0] - '0');
				n.c_str++;
				n.len--;
            }
			h->length_type = PR_HTTP_LENGTH_FIXED;
			h->left_in_chunk = h->content_length;
			h->have_body = h->content_length?1:0;

        } else if (str_itest(key, "connection")) {
            do {
                pb_string_t n = next_element(&val, &more);
                if (!n.len) {
					h->error_string = "400 Malformed Header";
                    return PR_ERROR;
                }

                if (str_itest(n, "close")) {
                    if (h->length_type != PR_HTTP_LENGTH_UNSET) {
                        h->error_string = "400 Bad Request";
                        return PR_ERROR;
                    }
					h->length_type = PR_HTTP_LENGTH_CLOSE;
					h->have_body = 1;
                }
            } while (more);

        } else if (str_itest(key, "cookie")) {
            do {
                pb_string_t n = next_element(&val, &more);
                if (!n.len) {
					h->error_string = "400 Malformed Header";
                    return PR_ERROR;
				}
				pb_string_t cookie, token;
				if (split(n, &cookie, &token, '=')) {
                    h->error_string = "400 Malformed Cookie";
                    return PR_ERROR;
				}
				if (str_test(cookie, "login")) {
                    ca_setstr(&h->login, token);
                }
            } while (more);

        } else if (str_itest(key, "expect")) {
            do {
                pb_string_t n = next_element(&val, &more);
                if (!n.len) {
					h->error_string = "400 Malformed Header";
                    return PR_ERROR;
                }
                if (str_itest(n, "100-continue")) {
                    h->expect_continue = 1;
                } else {
                    h->error_string = "417 Bad Expectation";
                    return PR_ERROR;
                }
            } while (more);

        } else if (str_itest(key, "if-none-match")) {
            pb_string_t n = next_element(&val, NULL);
            // ignore invalid/different etag
            if (n.len == HTTP_ETAG_LENGTH) {
				h->etag = 0;
                for (int i = 0; i < HTTP_ETAG_LENGTH; i++) {
                    h->etag = (h->etag << 4) | (uint64_t) (n.c_str[i] - 'A');
                }
                h->have_etag = 1;
            }
        }
    }
}

int pr_parse_request(struct pr_http *h, const char **data, int *sz) {
	pb_string_t p = {*sz, *data};
    int err = 0;

    if (!h->method) {
        err = parse_request_header(h, &p);
    }
    if (!err) {
        err = parse_headers(h, &p);
    }
	assert(err != PR_ERROR || h->error_string);

	*data = p.c_str;
	*sz = p.len;
    return err;
}

int pr_parse_body(struct pr_http *h, const char **data, int *sz) {
	switch (h->length_type) {
	case PR_HTTP_LENGTH_CHUNKED:
		return PR_ERROR;
	case PR_HTTP_LENGTH_FIXED:
		h->body_chunk = *data;
		h->chunk_size = *sz;
		if (!*sz) {
			return PR_ERROR;
		} else if (*sz >= h->left_in_chunk) {
			*data += h->left_in_chunk;
			*sz -= (int) h->left_in_chunk;
			h->left_in_chunk = 0;
			return PR_FINISHED;
		}
		*data += *sz;
		*sz = 0;
		h->left_in_chunk -= *sz;
		return PR_CONTINUE;
	case PR_HTTP_LENGTH_CLOSE:
		if (!*sz) {
			return PR_FINISHED;
		}
		*data += *sz;
		*sz = 0;
		h->body_chunk = *data;
		h->chunk_size = *sz;
		return PR_CONTINUE;
	default:
		return PR_ERROR;
	}
}

uint32_t pr_hash_path(pb_string_t p, uint32_t mul) {
	uint32_t hash = 0;
	for (int i = 0; i < p.len; i++) {
		hash = (hash * mul) + (uint32_t)p.c_str[i];
	}
	return hash;
}

const char pr_not_found[] = "HTTP/1.1 404 Not Found\r\n";
const char pr_parse_error[] = "HTTP/1.1 400 Malformed JSON\r\n";
const char pr_print_error[] = "HTTP/1.1 500 Print Error\r\n";

#if 0
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
			pb_string_t key;
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
		if (e - *p < m->boundary.len && !memcmp(*p, m->boundary.c_str, e - *p)) {
			// we have an incomplete boundary at the head of the incoming buffer
			return PR_CONTINUE;
		}

		if (e - *p >= m->boundary.len + 2 && !memcmp(*p, m->boundary.c_str, m->boundary.len)) {
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
#endif

