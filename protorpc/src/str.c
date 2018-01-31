#include <protorpc/str.h>

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

char str_initbuf[] = {0};

void str_destroy(str_t *s) {
    if (s->cap) {
        free(s->c_str);
    }
}

void str_grow(str_t *s, int cap) {
    if (cap < s->cap) {
        return;
    }

    char *buf = s->cap ? s->c_str : NULL;
    int newcap = (s->cap + 16) * 3 / 2;
    if (newcap < cap) {
        newcap = cap;
    }

    s->c_str = (char*) realloc(buf, newcap+1);
    s->cap = newcap;
}

void str_add2(str_t *s, const char *a, int len) {
    str_grow(s, s->len + len);
    memcpy(s->c_str + s->len, a, len);
    s->len += len;
    s->c_str[s->len] = 0;
}

int str_addf(str_t *s, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    return str_vaddf(s, fmt, ap);
}

int str_vaddf(str_t *s, const char *fmt, va_list ap) {
    str_grow(s, s->len + 1);
    for (;;) {
        va_list aq;
        va_copy(aq, ap);

        // detect when snprintf runs out of buffer by seeing whether
        // it overwrites the null terminator at the end of the buffer
        char* buf = s->c_str + s->len;
        int bufsz = s->cap - s->len;
        buf[bufsz] = '\0';
        int ret = vsnprintf(buf, bufsz + 1, fmt, aq);

        if (ret > bufsz) {
            // snprintf has told us the size of buffer required (ISO C99
            // behavior)
            str_grow(s, s->len + ret);
            // loop around to try again

        } else if (ret >= 0) {
            /* success */
            s->len += ret;
            return ret;

        } else if (buf[bufsz] != '\0') {
            /* snprintf has returned an error but has written to the end of the
             * buffer (MSVC behavior). The buffer is not large enough so grow
             * and retry. This can also occur with a format error if it occurs
             * right on the boundary, but then we grow the buffer and can
             * figure out its an error next time around.
             */
            str_grow(s, s->cap + 1);

        } else {
            /* snprintf has returned an error but has not written to the last
             * character in the buffer. We have a format error.
             */
            return -1;
        }
    }
}

void str_fread_all(str_t *s, FILE *f) {
	for (;;) {
        str_grow(s, s->len + 4096);
        int r = (int) fread(s->c_str + s->len, 1, s->cap - s->len, f);
		if (r == 0) {
			break;
		}
        s->len += r;
        s->c_str[s->len] = 0;
    }
}

void str_read_file(str_t *s, const char *fn) {
	FILE *f;
	if (!strcmp(fn, "-")) {
		str_fread_all(s, stdin);
	} else if ((f = fopen(fn, "rb")) != NULL) {
		str_fread_all(s, f);
		fclose(f);
	}
}
