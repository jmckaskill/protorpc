#define BUILDING_PROTORPC
#include "../protobuf.h"
#include <stdio.h>
#include <math.h>

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
static char *do_print_u64(char *p, uint64_t v) {
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
char *pb_print_u64(char *p, uint64_t v) {
	*p++ = '\"';
	return do_print_u64(p, v);
}
char *pb_print_i64(char *p, int64_t v) {
	*p++ = '\"';
    if (v < 0) {
        *p++ = '-';
        return do_print_u64(p, (uint64_t)(-v));
    } else {
        return do_print_u64(p, (uint64_t)(v));
    }
}
char *pb_print_float(char *p, float v) {
    // printing floats is complex, even though sprintf is slow it's worth it
	if (v != v) {
		memcpy(p, "\"NaN\",", 6);
		return p + 6;
	} else {
		p += sprintf((char*) p, "%.8g", v);
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
		p += sprintf((char*) p, "%.16g", v);
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

int pb_print_string(pb_buf_t *a, pb_string_t v) {
	if (a->next + 1 /*"*/ + v.len + 2 /*",*/ > a->end) {
		return -1;
	}
	char *p = a->next;
	*(p++) = '\"';
    const char *s = v.c_str;
    const char *e = s + v.len;
    while (s < e) {
        uint8_t uch = (uint8_t) *(s++);
		char escape = escapechar[uch];
        if (!escape) {
            *(p++) = (char) uch;
        } else if (escape == 'u') {
			if (p + (e - s) + 2 /*",*/ + 6 /*\u00EE*/ > a->end) {
				return -1;
			}
            *(p++) = '\\';
            *(p++) = 'u';
            *(p++) = '0';
            *(p++) = '0';
            *(p++) = hexchar[uch >> 4];
            *(p++) = hexchar[uch & 0xF];
        } else {
			if (p + (e - s) + 2 /*",*/ + 2 /*\e*/ > a->end) {
				return -1;
			}
            *(p++) = '\\';
            *(p++) = escape;
        }
    }
    *(p++) = '\"';
    *(p++) = ',';
	a->next = p;
	return 0;
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

int pb_print_bytes(pb_buf_t *a, pb_bytes_t v) {
	char *p = a->next;
	if (p + 1 /*"*/ + pb_base64_size(v.len) + 2 /*",*/ > a->end) {
		return -1;
	}
	*(p++) = '\"';
    p = pb_print_base64(p, v.p, v.len);
    *(p++) = '\"';
    *(p++) = ',';
	a->next = p;
	return 0;
}

int pb_print_array_end(pb_buf_t *a) {
	if (a->next[-1] == '\n' || a->next[-1] == ',') {
		a->next--;
	}
	return pb_append(a, "],", 2);
}

int pb_print_map_end(pb_buf_t *a) {
	if (a->next[-1] == '\n' || a->next[-1] == ',') {
		a->next--;
	}
	return pb_append(a, "}\n", 2);
}


