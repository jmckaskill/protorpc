#define BUILDING_PROTORPC
#include <protorpc/protorpc.h>

char *pb_print_bool(char *p, bool v) {
    if (v) {
        memcpy(p, "true,", 5);
        return p + 5;
    } else {
        memcpy(p, "false,", 6);
        return p + 6;
    }
}
char *pb_print_u32(char *p, uint32_t v) {
    // use a manual loop as this is much faster then calling sprintf and for fixed integers it's pretty simple
    // print out in reverse order
    char *b = p;
    do {
        *p++ = (v % 10) + '0';
        v = v / 10;
    } while (v);

    // now flip it around for v=1234, have "4321", want "1234"
    char *e = p-1;
    while (b < e) {
        char ch = *b;
        *b++ = *e;
        *e-- = ch;
    }
    *p++ = ',';
    return p;
}
char *pb_print_u64(char *p, uint64_t v) {
    // use a manual loop as this is much faster then calling sprintf and for fixed integers it's pretty simple
    // print out in reverse order
    char *b = p;
	*p++ = '\"';
    do {
        *p++ = (v % 10) + '0';
        v = v / 10;
    } while (v);

    // now flip it around for v=1234, have "4321", want "1234"
    char *e = p-1;
    while (b < e) {
        char ch = *b;
        *b++ = *e;
        *e-- = ch;
    }
    *p++ = '\"';
    *p++ = ',';
    return p;
}
char *pb_print_i32(char *p, int32_t v) {
    if (v < 0) {
        *p++ = '-';
        return pb_print_u32(p, (uint32_t)(-v));
    } else {
        return pb_print_u32(p, (uint32_t)(v));
    }
}
char *pb_print_i64(char *p, int64_t v) {
    if (v < 0) {
        *p++ = '-';
        return pb_print_u64(p, (uint64_t)(-v));
    } else {
        return pb_print_u64(p, (uint64_t)(v));
    }
}
char *pb_print_float(char *p, float v) {
    // printing floats is complex, even though sprintf is slow it's worth it
	if (v != v) {
		memcpy(p, "\"NaN\",", 6);
		return p + 6;
	} else {
		p += sprintf((char*) p, "%.9g", v);
		*p++ = ',';
		return p;
	}
}
char *pb_print_double(char *p, double v) {
    // printing floats is complex, even though sprintf is slow it's worth it
	if (v != v) {
		memcpy(p, "\"NaN\",", 6);
		return p + 6;
	} else {
		p += sprintf((char*) p, "%.17g", v);
		*p++ = ',';
		return p;
	}
}

static const char base64char[] =
    "ABCDEFGH" "IJKLMNOP"
    "QRSTUVWX" "YZabcdef"
    "ghijklmn" "opqrstuv"
    "wxyz0123" "456789+/";
static const char hexchar[] = "0123456789abcdef";
static const char escapechar[] =
    "uuuuuuuu"          "btnufruu"
    "uuuuuuuu"          "uuuuuuuu"
    "\0\0\"\0\0\0\0\0"  "\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0"  "\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0"  "\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0"  "\0\0\0\0\\\0\0\0"
    "\0\0\0\0\0\0\0\0"  "\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0"  "\0\0\0\0\0\0\0u"
    "\0\0\0\0\0\0\0\0"  "\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0"  "\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0"  "\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0"  "\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0"  "\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0"  "\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0"  "\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0"  "\0\0\0\0\0\0\0\0";

void pb_print_string(str_t *a, struct pb_string v) {
	int i = a->len;
	str_grow(a, i + 1 /*"*/ + v.len + 2 /*",*/);
	a->buf[i++] = '\"';
    const char *s = v.p;
    const char *e = s + v.len;
    while (s < e) {
        char ch = *s++;
        char escape = escapechar[(uint8_t)ch];
        if (!escape) {
            a->buf[i++] = ch;
        } else if (escape == 'u') {
			str_grow(a, i + (int) (e-s) + 2 /*",*/ + 6 /*\u00EE*/);
            a->buf[i++] = '\\';
            a->buf[i++] = 'u';
            a->buf[i++] = '0';
            a->buf[i++] = '0';
            a->buf[i++] = hexchar[ch >> 4];
            a->buf[i++] = hexchar[ch & 0xF];
        } else {
            str_grow(a, i + (int) (e-s) + 2 /*",*/ + 2 /*\e*/);
            a->buf[i++] = '\\';
            a->buf[i++] = escape;
        }
    }
    a->buf[i++] = '\"';
    a->buf[i++] = ',';
	a->len = i;
}

char *pb_print_base64(char *p, const uint8_t *v, int n) {
    int i;
    for (i = 0; i < n-2; i += 3) {
        *p++ = base64char[(v[i]>>2)];
        *p++ = base64char[((v[i]&3) << 4) | (v[i+1]>>4)];
        *p++ = base64char[((v[i+1]&0xF) << 2) | (v[i+2]>>6)];
        *p++ = base64char[(v[i+2]&0x3F)];
    }
    if (i == n-1) {
        *p++ = base64char[(v[i]>>2)];
        *p++ = base64char[((v[i]&3) << 4)];
    } else if (i == n-2) {
        *p++ = base64char[(v[i]>>2)];
        *p++ = base64char[((v[i]&3) << 4) | (v[i+1]>>4)];
        *p++ = base64char[((v[i+1]&0xF) << 2)];
    }
    return p;
}

void pb_print_bytes(str_t *a, struct pb_bytes v) {
	str_grow(a, a->len + 1 /*"*/ + pb_base64_size(v.len) + 2 /*",*/);
	char *p = a->buf + a->len;
	*p++ = '\"';
    p = pb_print_base64(p, v.p, v.len);
    *p++ = '\"';
    *p++ = ',';
	a->len = (int) (p - a->buf);
}

void pb_print_array_end(str_t *a) {
	if (a->buf[a->len - 1] == '\n' || a->buf[a->len-1] == ',') {
		a->len--;
	}

	str_grow(a, a->len + 2);
	a->buf[a->len++] = ']';
	a->buf[a->len++] = ',';
}

void pb_print_map_end(str_t *a) {
	if (a->buf[a->len - 1] == '\n' || a->buf[a->len-1] == ',') {
		a->len--;
	}

	str_grow(a, a->len + 2);
	a->buf[a->len++] = '}';
	a->buf[a->len++] = '\n';
}


