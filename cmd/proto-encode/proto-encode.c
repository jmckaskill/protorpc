#include <protorpc/protorpc.h>
#include <stdio.h>
#include <stdint.h>

// number entries are of the form
// [TAG] [TYPE] [NUM]
// TYPE can be:
// <nothing> -- variable length number
// I -- variable length zigzag number
// I32 -- signed 32 bit fixed number
// U32 -- unsigned 32 bit fixed number
// I64 -- signed 64 bit fixed number
// U64 -- unsigned 64 bit fixed number
// F32 -- 32 bit float
// F64 -- 64 bit float

// string entries are of the form
// [TAG] "STRING"
// and uses C style hex escapes \x00

// byte entries are of the form
// [TAG] B [HEX BYTES]
// hex bytes can include any number of whitespace between bytes

// records are of the form
// [TAG] {
// and continue until an end line
// }

// each entry must be on a different line
// whitespace can occure between items

// comments start with # and continue until the end of the line

static int invalid_line(char *line) {
    fprintf(stderr, "invalid line: %s", line);
    return 3;
}

static char *consume_whitespace(char *p) {
    while (*p == ' ' || *p == '\t') {
        p++;
    }
    return p;
}

static void add_varint(str_t *s, uint64_t v) {
    while (v >= 0x80) {
        str_addch(s, (char) (0x80 | (uint8_t) v));
        v >>= 7;
    }
    str_addch(s, (char) (uint8_t) v);
}

static void add_tag(str_t *s, enum pb_wiretype type, unsigned int tag) {
    add_varint(s, (tag << 3) | type);
}

static int decode_hex(char ch) {
    if ('0' <= ch && ch <= '9') {
        return ch - '0';
    } else if ('a' <= ch && ch <= 'f') {
        return ch - 'a' + 10;
    } else if ('A' <= ch && ch <= 'F') {
        return ch - 'A' + 10;
    } else {
        return -1;
    }
}

static char *parse_bytes(char *p, unsigned *plen) {
    char *start = p;
    char *d = p;
    fprintf(stderr, "parse_bytes %s", p);
    while (*p) {
        if (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') {
            p++;
            continue;
        }

        int ch1 = decode_hex(p[0]);
        int ch2 = decode_hex(p[1]);
        fprintf(stderr, "parse_bytes %d %d\n", ch1, ch2);
        if (ch1 < 0 || ch2 < 0) {
            return NULL;
        }

        *d++ = (char) ((uint8_t)(ch1 << 4) | (uint8_t) ch2);
        p += 2;
    }
    *plen = (unsigned) (d - start);
    return p;
}

static char *parse_string(char *p, unsigned *plen) {
    char *start = p;
    char *d = p;
    for (;;) {
        //fprintf(stderr, "parse_string %x %c\n", (uint8_t) *p, (0x20 <= *p && *p < 0x7F) ? *p : '?');
        switch (*p) {
        case '\"':
            *plen = (unsigned) (d - start);
            return p+1;
        case '\n':
        case '\r':
        case '\0':
            return NULL;
        case '\\':
            switch (p[1]) {
            case 'n':
                *d++ = '\n';
                p += 2;
                break;
            case 'r':
                *d++ = '\r';
                p += 2;
                break;
            case 't':
                *d++ = '\t';
                p += 2;
                break;
            case '"':
                *d++ = '"';
                p += 2;
                break;
            case '\\':
                *d++ = '\\';
                p += 2;
                break;
            case '\0':
                *d++ = '\0';
                p += 2;
                break;
            case 'x':
                {
                    int ch1 = decode_hex(p[2]);
                    int ch2 = decode_hex(p[3]);
                    if (ch1 < 0 || ch2 < 0) {
                        return NULL;
                    }
                    *d++ = (char) ((uint8_t)(ch1 << 4) | (uint8_t)(ch2));
                    p += 4;
                }
                break;
            default:
                return NULL;
            }
            break;
        default:
            *d++ = *p++;
            break;
        }
    }
}

#define MAX_DEPTH 16

int main(int argc, char *argv[]) {
    FILE *in = stdin;
    FILE *out = stdout;
    int argi = 1;

    while (argi < argc) {
        if (!strcmp(argv[argi], "-o") && argi+1 < argc) {
            out = fopen(argv[argi+1], "wb");
            if (!out) {
                fprintf(stderr, "failed to open output %s\n", argv[argi+1]);
                return 2;
            }
            argi += 2;
        } else if (argv[argi][0] != '-') {
            in = fopen(argv[argi], "r");
            if (!in) {
                fprintf(stderr, "failed to open %s\n", argv[argi]);
                return 2;
            }
            argi++;
        } else {
            fprintf(stderr, "unexpected argument %s\n", argv[argi]);
            fprintf(stderr, "usage: ./proto-encode.exe [input] [-o output]\n");
            return 2;
        }
    }

    str_t s[MAX_DEPTH];
    int depth = 0;
    for (int i = 0; i < MAX_DEPTH; i++) {
        str_init(&s[i]);
    }

    char line[4096];
    while (fgets(line, sizeof(line), in)) {
        char *p = line;
        p = consume_whitespace(p);
        //fprintf(stderr, "line: %s", p);

        if (*p == '}') {
            if (depth == 0) {
                fprintf(stderr, "extra }\n");
                return 3;
            }
            add_varint(&s[depth-1], s[depth].len);
            str_addstr(&s[depth-1], &s[depth]);
            str_clear(&s[depth]);
            depth--;
            continue;
        } else if (*p == '#' || *p == '\n' || *p == '\r') {
            // comment only line
            continue;
        }

        if (!('0' <= *p && *p <= '9')) {
            return invalid_line(line);
        }

        unsigned int tag = strtoul(p, &p, 0);
        p = consume_whitespace(p);
        //fprintf(stderr, "tag %u, remaining %s", tag, p);
        if (*p == '{') {
            if (depth+1 == MAX_DEPTH) {
                fprintf(stderr, "too deep\n");
                return 3;
            }
            add_tag(&s[depth], PB_WIRE_VARIABLE, tag);
            depth++;

        } else if (*p == '\"') {
            add_tag(&s[depth], PB_WIRE_VARIABLE, tag);
            unsigned len;
            char *begin = p+1;
            p = parse_string(begin, &len);
            if (!p) {
                return invalid_line(line);
            }
            add_varint(&s[depth], len);
            str_add2(&s[depth], begin, len);

        } else if (!strncmp(p, "B ", 2)) {
            add_tag(&s[depth], PB_WIRE_VARIABLE, tag);
            unsigned len;
            char *begin = p+2;
            p = parse_bytes(begin, &len);
            if (!p) {
                return invalid_line(line);
            }
            add_varint(&s[depth], len);
            str_add2(&s[depth], begin, len);

        } else if (!strncmp(p, "F32 ", 4)) {
            union {float f; char c[4];} u;
            u.f = strtof(p+4, &p);
            add_tag(&s[depth], PB_WIRE_F32, tag);
            str_add2(&s[depth], u.c, 4);

        } else if (!strncmp(p, "F64 ", 4)) {
            union {double f; char c[8];} u;
            u.f = strtod(p+4, &p);
            add_tag(&s[depth], PB_WIRE_F64, tag);
            str_add2(&s[depth], u.c, 8);

        } else if (!strncmp(p, "I32 ", 4)) {
            union {int32_t i; char c[4];} u;
            u.i = strtol(p+4, &p, 0);
            add_tag(&s[depth], PB_WIRE_F32, tag);
            str_add2(&s[depth], u.c, 4);

        } else if (!strncmp(p, "U32 ", 4)) {
            union {uint32_t i; char c[4];} u;
            u.i = strtoul(p+4, &p, 0);
            add_tag(&s[depth], PB_WIRE_F32, tag);
            str_add2(&s[depth], u.c, 4);

        } else if (!strncmp(p, "I64 ", 4)) {
            union {int64_t i; char c[8];} u;
            u.i = strtoll(p+4, &p, 0);
            add_tag(&s[depth], PB_WIRE_F64, tag);
            str_add2(&s[depth], u.c, 8);

        } else if (!strncmp(p, "U64 ", 4)) {
            union {uint64_t i; char c[8];} u;
            u.i = strtoull(p+4, &p, 0);
            add_tag(&s[depth], PB_WIRE_F64, tag);
            str_add2(&s[depth], u.c, 8);

        } else if (!strncmp(p, "I ", 2)) {
            int64_t v = strtoll(p+2, &p, 0);
            add_tag(&s[depth], PB_WIRE_VARINT, tag);
            add_varint(&s[depth], ((uint64_t)(v) << 1) ^ (uint64_t)(v >> 63));

        } else if (*p == '-') {
            int64_t val = strtoll(p, &p, 0);
            add_tag(&s[depth], PB_WIRE_VARINT, tag);
            add_varint(&s[depth], (uint64_t) val);

        } else if ('0' <= *p && *p <= '9') {
            uint64_t val = strtoull(p, &p, 0);
            add_tag(&s[depth], PB_WIRE_VARINT, tag);
            add_varint(&s[depth], val);

        } else {
            return invalid_line(line);
        }
    }

    fwrite(s[0].c_str, 1, s[0].len, out);
    fflush(out);
    return 0;
}