#include "str/str.h"
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

// see proto-encode.c for the line format
// protobuf does not distinguish between child messages, strings, bytes and packed arrays
// on decode, we will first assume a variable length entry is a message
// and then backtrack to a string if it fails to decode

enum pb_wiretype {
    PB_WIRE_VARINT = 0,
    PB_WIRE_F64 = 1,
    PB_WIRE_VARIABLE = 2,
    PB_WIRE_F32 = 5,
};

uint8_t *decode_varint(uint8_t *p, uint8_t *e, uint64_t *pv) {
    size_t shift = 0;
    uint64_t v = 0;
    while (p < e) {
        uint64_t u = (uint64_t) *p++;
        v |= (u & 0x7F) << shift;
        if (!(u & 0x80)) {
            *pv = v;
            return p;
        }
        shift += 7;
    }
    // invalid varint
    return NULL;
}

static void append_bytes(str_t *s, uint8_t *p, uint8_t *e) {
    str_addch(s, 'B');
    size_t n = 0;
    while (p < e) {
        if ((n++ % 4) == 0) {
            str_addch(s, ' ');
        }
        const char *hex = "0123456789abcdef";
        str_addch(s, hex[*p >> 4]);
        str_addch(s, hex[*p & 0xF]);
        p++;
    }
    str_addch(s, '\n');
}

static bool is_string(uint8_t *p, uint8_t *e) {
    while (p < e) {
        if (!(*p == '\n' || *p == '\r' || *p == '\t' || (0x20 <= *p && *p < 0x7F))) {
            return false;
        }
        p++;
    }
    return true;
}

static void append_string(str_t *s, uint8_t *p, uint8_t *e) {
    str_addch(s, '"');
    while (p < e) {
        if (*p == '\n') {
            str_addch(s, '\\');
            str_addch(s, 'n');

        } else if (*p == '\r') {
            str_addch(s, '\\');
            str_addch(s, 'r');

        } else if (*p == '\t') {
            str_addch(s, '\\');
            str_addch(s, 't');

        } else if (*p == '\\') {
            str_addch(s, '\\');
            str_addch(s, '\\');

        } else if (*p == '"') {
            str_addch(s, '\\');
            str_addch(s, '"');

        } else {
            str_addch(s, (char) *p);
        }
        p++;
    }
    str_addch(s, '"');
    str_addch(s, '\n');
}

static int invalid_file() {
    fprintf(stderr, "invalid input\n");
    return 3;
}

#define MAX_DEPTH 16
static const char indent[] = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";

int main(int argc, char *argv[]) {
    FILE *in = stdin;
    if (argc > 1) {
        in = fopen(argv[1], "rb");
        if (!in) {
            fprintf(stderr, "failed to open %s\n", argv[1]);
            return 2;
        }
    }

    str_t inbuf = STR_INIT;
    str_fread_all(&inbuf, in);

    uint8_t *begin[MAX_DEPTH];
    uint8_t *end[MAX_DEPTH];
    str_t out[MAX_DEPTH];
    int depth = 0;

    for (int i = 0; i < MAX_DEPTH; i++) {
        str_init(&out[i]);
    }

    begin[0] = (uint8_t*) inbuf.c_str;
    end[0] = begin[0] + inbuf.len;
    uint8_t *p = begin[0];

    while (depth || p < end[depth]) {
        uint8_t *e = end[depth];
        str_t *s = &out[depth];

        if (p == e) {
            // we've finished the current scope
            str_add2(&out[depth], indent, depth-1);
            str_add(&out[depth], "}\n");
            str_addstr(&out[depth-1], &out[depth]);
            str_clear(&out[depth]);
            depth--;
            continue;
        }

        uint64_t tag;
        p = decode_varint(p, e, &tag);
        if (!p || (tag >> 3) == 0) {
            goto backtrack;
        }

        str_add2(s, indent, depth);
        str_addf(s, "%u ", (unsigned int)(tag >> 3));

        switch (tag & 7) {
        case PB_WIRE_F32:
            {
                if (p + 4 > e) {
                    goto backtrack;
                }
                uint32_t val = *(uint32_t*) p;
                p += 4;
                str_addf(s, "U32 0x%08X\n", val);
            }
            continue;
        case PB_WIRE_F64:
            {
                if (p + 8 > e) {
                    goto backtrack;
                }
                uint64_t val = *(uint64_t*) p;
                p += 8;
                str_addf(s, "U64 0x%08" PRIX64 "\n", val);
            }
            continue;
        case PB_WIRE_VARINT:
            {
                uint64_t val;
                p = decode_varint(p, e, &val);
                if (!p) {
                    goto backtrack;
                }
                str_addf(s, "%" PRIu64 "\n", val);
            }
            continue;
        case PB_WIRE_VARIABLE:
            {
                uint64_t len;
                p = decode_varint(p, e, &len);
                if (!p || len > (e-p)) {
                    goto backtrack;
                }
                begin[depth+1] = p;
                end[depth+1] = p + len;
                str_add(&out[depth+1], "{\n");
                depth++;
            }
            continue;
        default:
            goto backtrack;
        }

backtrack:
        if (depth == 0) {
            return invalid_file();
        }
        // the current scope is invalid, we need to bracktrack a single stage
        p = begin[depth];

        if (is_string(p, e)) {
            append_string(&out[depth-1], p, e);
        } else {
            append_bytes(&out[depth-1], p, e);
        }

        p = end[depth];
        str_clear(&out[depth]);
        depth--;
    }

    fwrite(out[0].c_str, 1, out[0].len, stdout);
    return 0;
}