#define BUILDING_PROTORPC
#include "../protobuf.h"

char pb_errret[] = {0}; 

static char *consume_space(char *p) {
    while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') {
        p++;
    }
    return p;
}

static char *consume_string(char *p) {
    for (;;) {
        switch (*p) {
        case '"':
            return p+1;
        case '\\':
            p++;
            if (!*p) {
                return pb_errret;
            }
            p++;
            break;
        default:
            p++;
            break;
        }
    }
}

char *pb_parse_bool(char *p, bool *v) {
    if (!strncmp(p, "true", 4)) {
        *v = 1;
        return p + 4;
    } else if (!strncmp(p, "false", 5)) {
        *v = 0;
        return p + 5;
    } else {
        return pb_errret;
    }
}
static char *parse_number(char *p, uint64_t *v, int *pnegate) {
    // json allows any floating number here, but we want a non-float
    // using strtod is slow, so instead we'll do a basic parse according to the json grammar
    int trailing = 0; // number of digits after the dot
    int exponent = 0; // exponent value
    int quoted = 0;
    uint64_t num = 0;
    if (*p == '"') {
        quoted = 1;
        p++;
    }
    if (*p == '-') {
        if (pnegate) {
            *pnegate = 1;
            p++;
        } else {
            return pb_errret;
        }
    }
    if ('1' <= *p && *p <= '9') {
        num = *p++ - '0';
        while ('0' <= *p && *p <= '9') {
            num = (num * 10) + (*p++ - '0');
        }
    } else if (*p == '0') {
        p++;
    }
    if (*p == '.') {
        p++;
        while ('0' <= *p && *p <= '9') {
            num = (num * 10) + (*p++ - '0');
            trailing++;
        }
    }
    if (*p == 'e' || *p == 'E') {
        int negate = 0;
        p++;
        if (*p == '-') {
            negate = 1;
            p++;
        } else if (*p == '+') {
            p++;
        }
        while ('0' <= *p && *p <= '9') {
            exponent = (exponent * 10) + (*p++ - '0');
        }
        if (negate) {
            exponent = -exponent;
        }
    }
    // for 1.2e4, we would have num 12, exp 4, trailing 1
    // want 12000
    exponent -= trailing;

    if (exponent > 0) {
        do {
            num *= 10;
            exponent--;
        } while (exponent);

    } else if (exponent < 0) {
        do {
            num /= 10;
            exponent++;
        } while (exponent);
    }
    if (quoted) {
        if (*p != '"') {
            return pb_errret;
        }
        p++;
    }

    *v = num;
    return p;
}
char *pb_parse_i32(char *p, int32_t *v) {
    uint64_t num;
    int negate = 0;
    p = parse_number(p, &num, &negate);
    *v = (negate?-1:1) * (int32_t)num;
    return p;
}
char *pb_parse_u32(char *p, uint32_t *v) {
    uint64_t num;
    p = parse_number(p, &num, NULL);
    *v = (uint32_t)num;
    return p;
}
char *pb_parse_i64(char *p, int64_t *v) {
    uint64_t num;
    int negate = 0;
    p = parse_number(p, &num, &negate);
    *v = (negate?-1:1) * (int64_t)num;
    return p;
}
char *pb_parse_u64(char *p, uint64_t *v) {
    p = parse_number(p, v, NULL);
    return p;
}
char *pb_parse_float(char *p, float *v) {
    char *end;
    if (*p == '\"') {
		p++;
	}
    *v = strtof((char*) p, &end);
    if (end == (char*) p) {
		return pb_errret;
	}
    if (*end == '\"') {
		end++;
	}
    return end;
}
char *pb_parse_double(char *p, double *v) {
    char *end;
    if (*p == '\"') {
		p++;
	}
    *v = strtod((char*) p, &end);
    if (end == (char*) p) {
		return pb_errret;
	}
    if (*end == '\"') {
		end++;
	}
    return end;
}

static const signed char base64rev[256] = {
    -1,-1,-1,-1,-1,-1,-1,-1,    -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,    -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,    -1,-1,-1,62,-1,-1,-1,63,
    52,53,54,55,56,57,58,59,    60,61,-1,-1,-1,-1,-1,-1,
    -1, 0, 1, 2, 3, 4, 5, 6,     7, 8, 9,10,11,12,13,14,
    15,16,17,18,19,20,21,22,    23,24,25,-1,-1,-1,-1,-1,
    -1,26,27,28,29,30,31,32,    33,34,35,36,37,38,39,40,
    41,42,43,44,45,46,47,48,    59,50,51,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,    -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,    -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,    -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,    -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,    -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,    -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,    -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,    -1,-1,-1,-1,-1,-1,-1,-1,
};
static const signed char hexrev[256] = {
    -1,-1,-1,-1,-1,-1,-1,-1,    -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,    -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,    -1,-1,-1,-1,-1,-1,-1,-1,
     0, 1, 2, 3, 4, 5, 6, 7,     8, 9,-1,-1,-1,-1,-1,-1,
    -1,10,11,12,13,14,15,-1,    -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,    -1,-1,-1,-1,-1,-1,-1,-1,
    -1,10,11,12,13,14,15,-1,    -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,    -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,    -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,    -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,    -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,    -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,    -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,    -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,    -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,    -1,-1,-1,-1,-1,-1,-1,-1,
};

char *pb_parse_base64(char *p, struct pb_bytes *v) {
    uint8_t *in = (uint8_t*) p;
    uint8_t *out = (uint8_t*) p;
	uint8_t *ret = (uint8_t*) p;
    for (;;) {
        int ch0 = base64rev[in[0]];
        int ch1 = base64rev[in[1]];
        int ch2 = base64rev[in[2]];
        int ch3 = base64rev[in[3]];
        if (ch0 < 0) {
			v->p = ret;
			v->len = (uint32_t) (out - ret);
            return (char*) in;
        } else if (ch1 < 0) {
            // 00000011 11112222 22333333
            out[0] = (uint8_t) ((ch0 << 2));
			v->p = ret;
			v->len = (uint32_t) (out - ret + 1);
            return (char*) in + 1;
        } else if (ch2 < 0) {
            // 00000011 11112222 22333333
            out[0] = (uint8_t) ((ch0 << 2) | (ch1 >> 4));
            out[1] = (uint8_t) ((ch1 << 4));
			v->p = ret;
			v->len = (uint32_t) (out - ret + 2);
            return (char*) in + 2;
        } else if (ch3 < 0) {
            // 00000011 11112222 22333333
            out[0] = (uint8_t) ((ch0 << 2) | (ch1 >> 4));
            out[1] = (uint8_t) ((ch1 << 4) | (ch2 >> 2));
            out[2] = (uint8_t) ((ch2 << 6));
			v->p = ret;
			v->len = (uint32_t) (out - ret + 3);
            return (char*) in + 3;
        } else {
            // 00000011 11112222 22333333
            out[0] = (uint8_t) ((ch0 << 2) | (ch1 >> 4));
            out[1] = (uint8_t) ((ch1 << 4) | (ch2 >> 2));
            out[2] = (uint8_t) ((ch2 << 6) | (ch3));
            out += 3;
            in += 4;
        }
    }
}

char *pb_parse_bytes(char *p, struct pb_bytes *v) {
    if (*p != '\"') {
        return pb_errret;
    }
    p = pb_parse_base64(p, v);
    if (*p != '\"') {
        return pb_errret;
    }
    return p+1;
}

char *pb_parse_string(char *p, struct pb_string *v) {
    if (*p != '\"') {
        return pb_errret;
    }
    p++;
    uint8_t *in = (uint8_t*) p;
    uint8_t *out = (uint8_t*) p;
    uint8_t *ret = (uint8_t*) p;
    for (;;) {
        switch (*in) {
        case '\0':
            return pb_errret;
        case '"':
			v->p = (char*) ret;
			v->len = (int) (out - ret);
            return (char*)in+1;
        case '\\':
            in++;
            switch (*in++) {
            case '"':
                *out++ = '"';
                break;
            case '\\':
                *out++ = '\\';
                break;
            case 'b':
                *out++ = '\b';
                break;
            case 'f':
                *out++ = '\f';
                break;
            case 'n':
                *out++ = '\n';
                break;
            case 'r':
                *out++ = '\r';
                break;
            case 't':
                *out++ = '\t';
                break;
            case 'u': {
                uint32_t u0;
                int i, ch[4];
                for (i = 0; i < 4; i++) {
                    ch[i] = hexrev[in[i]];
                    if (ch[i] < 0) {
                        return pb_errret;
                    }
                }
                in += 4;
                u0 = (ch[0] << 12) | (ch[1] << 8) | (ch[2] << 4) | ch[3];

                if (u0 < 0x80) {
                    // 1 chars utf8 (US-ASCII)
                    // UTF32:  00000000 00000000 00000000 0xxxxxxx
                    // Source: 00000000 0xxxxxxx
                    // Dest:   0xxxxxxx
                    *out++ = u0 & 0x7F;
                } else if (u0 < 0x800) {
                    // 2 chars utf8
                    // Source: 00000yyy xxxxxxxx
                    // Dest:   110yyyxx 10xxxxxx
                    *out++ = 0xC0 | ((u0 >> 6) & 0x1F);
                    *out++ = 0x80 | (u0 & 0x3F);
                } else if (u0 < 0xD800) {
                    // 3 chars utf8, 1 wchar utf16
                    // UTF32:  00000000 00000000 yyyyyyyy xxxxxxxx
                    // Source: yyyyyyyy xxxxxxxx
                    // Dest:   1110yyyy 10yyyyxx 10xxxxxx
                    *out++ = 0xE0 | ((u0 >> 12) & 0x0F);
                    *out++ = 0x80 | ((u0 >> 6) & 0x3F);
                    *out++ = 0x80 | (u0 & 0x3F);
                } else if (u0 < 0xDC00) {
                    // 4 chars utf8, 2 wchars utf16
                    // 0xD8 1101 1000
                    // 0xDB 1101 1011
                    // 0xDC 1101 1100
                    // 0xDF 1101 1111
                    // UTF32:  00000000 000zzzzz yyyyyyyy xxxxxxxx
                    // Source: 110110zz zzyyyyyy 110111yy xxxxxxxx
                    // Dest:   11110zzz 10zzyyyy 10yyyyxx 10xxxxxx
                    // UTF16 data is shifted by 0x10000

                    // look for surrogates
                    if (p[0] != '\\' || p[1] != 'u') {
                        return pb_errret;
                    }
                    in += 2;
                    for (i = 0; i < 4; i++) {
                        ch[i] = hexrev[in[i]];
                        if (ch[i] < 0) {
                            return pb_errret;
                        }
                    }
                    in += 4;
                    uint32_t u1 = (ch[0] << 12) | (ch[1] << 8) | (ch[2] << 4) | ch[3];

                    // Check for a valid surrogate
                    if (u1 < 0xDC00 || u1 > 0xDFFF) {
                        return pb_errret;
                    }

                    uint32_t u32 = ((u0 << 10) & 0x0FFC00) | (u1 & 0x3FF);
                    u32 += 0x10000;
                    *out++ = 0xF0 | ((u32 >> 18) & 0x03);
                    *out++ = 0x80 | ((u32 >> 12) & 0x3F);
                    *out++ = 0x80 | ((u32 >> 6) & 0x3F);
                    *out++ = 0x80 | (u32 & 0x3F);
                } else {
                    // 3 chars utf8, 1 wchar utf16
                    // UTF32:  00000000 00000000 yyyyyyyy xxxxxxxx
                    // Source: yyyyyyyy xxxxxxxx
                    // Dest:   1110yyyy 10yyyyxx 10xxxxxx
                    *out++ = 0xE0 | ((u0 >> 12) & 0x0F);
                    *out++ = 0x80 | ((u0 >> 6) & 0x3F);
                    *out++ = 0x80 | (u0 & 0x3F);
                }
                break;
            }
            default:
                return pb_errret;
            }
            break;
        default:
            *out++ = *in++;
            break;
        }
    }
}

static bool parse_multi(char **p, char start) {
    if (**p == start) {
		*p = consume_space(*p+1);
        return true;
    } else if (!strncmp((char*) *p, "null", 4)) {
        (*p) += 4;
        return false;
    } else {
        *p = pb_errret;
        return false;
    }
}

bool pb_parse_array(char **p) {
	return parse_multi(p, '[');
}

bool pb_parse_map(char **p) {
	return parse_multi(p, '{');
}

bool pb_more_array(char **p) {
    *p = consume_space(*p);
    if (**p == ']') {
        (*p)++;
        return false;
    } else if (**p == ',') {
        (*p)++;
        *p = consume_space(*p);
        return true;
    } else {
        *p = pb_errret;
        return false;
    }
}
uint32_t pb_parse_enum(char **p, struct pb_string *v, uint32_t mul) {
    uint32_t hash = 0;
    char *ret;
    if (**p != '\"') {
        goto err;
    }
    ret = ++(*p);
    for (;;) {
        switch (**p) {
        case '\0':
        case '\\':
            goto err;
        case '\"':
            goto next;
        }
        hash = (hash * mul) + (uint32_t)(**p);
        (*p)++;
    }
next:
	v->p = ret;
	v->len = (int) (*p - ret);
	(*p)++;
    return hash;
err:
    *p = pb_errret;
    return 0;
}


uint32_t pb_parse_field(char **p, struct pb_string *v, uint32_t mul) {
    uint32_t hash = 0;
    *p = consume_space(*p);
    if (**p == '}') {
        (*p)++;
        return 0;
    } else if (**p == ',') {
        *p = consume_space(*p+1);
    }

    char *ret;
    if (**p != '\"') {
        goto err;
    }
    ret = ++(*p);
    for (;;) {
        switch (**p) {
        case '\0':
        case '\\':
            goto err;
        case '\"':
            goto next;
        }
        hash = (hash * mul) + (uint32_t)(**p);
        (*p)++;
    }
next:
	v->p = ret;
	v->len = (int) (*p - ret);
    *p = consume_space(*p+1);
    if (**p != ':') {
        goto err;
    }
    *p = consume_space(*p+1);
    return hash;
err:
    *p = pb_errret;
    return 0;
}

char *pb_parse_skip(char *p) {
    switch (*p) {
    case '[':
        p = consume_space(p+1);
        if (*p == ']') {
            return p+1;
        } else {
            for (;;) {
                p = pb_parse_skip(p);
                p = consume_space(p);
                if (*p == ']') {
                    break;
                } else if (*p != ',') {
                    goto err;
                }
                p = consume_space(p);
            }
        }
    case '{':
        p = consume_space(p+1);
        if (*p == '}') {
            return p+1;
        } else {
            for (;;) {
                if (*p != '"') {
                    goto err;
                }
                p = consume_string(p+1);
                p = consume_space(p);
                if (*p != ':') {
                    goto err;
                }
                p = consume_space(p+1);
                p = pb_parse_skip(p);
                p = consume_space(p);
                if (*p == '}') {
                    return p+1;
                } else if (*p != ',') {
                    goto err;
                }
                p = consume_space(p+1);
            }
        }
    case '"':
        return consume_string(p+1);
    case 'n':
        if (!strncmp((char*)p, "null", 4)) {
            return p+4;
        } else {
            goto err;
        }
    case 'f':
        if (!strncmp((char*)p, "false", 5)) {
            return p+5;
        } else {
            goto err;
        }
    case 't':
        if (!strncmp((char*)p, "true", 4)) {
            return p+4;
        } else {
            goto err;
        }
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        while ('0' <= *p && *p <= '9') {
            p++;
        }
        if (*p == '.') {
            p++;
            while ('0' <= *p && *p <= '9') {
                p++;
            }   
        }
        if (*p == 'e' || *p == 'E') {
            p++;
            if (*p == '+' || *p == '-') {
                p++;
            }
            while ('0' <= *p && *p <= '9') {
                p++;
            }   
        }
        return p;
    default:
        goto err;
    }
err:
    return pb_errret;
}

#define PARSE_ARRAY(type, suffix)										\
	if (pb_parse_array(&p)) {											\
		*pv = (type*) a->next;											\
		do {															\
			if (a->next + ((*plen)*sizeof(type)) > a->end) {			\
				return pb_errret;										\
			}															\
			p = pb_parse_ ## suffix(p, (type*) *pv + *plen);			\
			(*plen)++;													\
		} while (pb_more_array(&p));									\
		a->next += (*plen) * sizeof(type);								\
	}																	\
	return p

char *pb_parse_array_bool(char *p, pb_buf_t *a, bool const **pv, int *plen) {
	PARSE_ARRAY(bool, bool);
}

char *pb_parse_array_i32(char *p, pb_buf_t *a, int32_t const **pv, int *plen) {
	PARSE_ARRAY(int32_t, i32);
}

char *pb_parse_array_u32(char *p, pb_buf_t *a, uint32_t const **pv, int *plen) {
	PARSE_ARRAY(uint32_t, u32);
}

char *pb_parse_array_i64(char *p, pb_buf_t *a, int64_t const **pv, int *plen) {
	PARSE_ARRAY(int64_t, i64);
}

char *pb_parse_array_u64(char *p, pb_buf_t *a, uint64_t const **pv, int *plen) {
	PARSE_ARRAY(uint64_t, u64);
}

char *pb_parse_array_float(char *p, pb_buf_t *a, float const **pv, int *plen) {
	PARSE_ARRAY(float, float);
}

char *pb_parse_array_double(char *p, pb_buf_t *a, double const **pv, int *plen) {
	PARSE_ARRAY(double, double);
}

char *pb_parse_array_bytes(char *p, pb_buf_t *a, struct pb_bytes const **pv, int *plen) {
	PARSE_ARRAY(struct pb_bytes, bytes);
}

char *pb_parse_array_string(char *p, pb_buf_t *a, struct pb_string const **pv, int *plen) {
	PARSE_ARRAY(struct pb_string, string);
}

