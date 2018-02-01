#include "common.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>


struct parse_stack {
	const struct proto_message *type;
	const struct proto_field *field;
	char *msg;
};

static char errret[] = { 0 };

static char *consume_space(char *p) {
	while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') {
		p++;
	}
	return p;
}

static char *parse_bool(char *p, bool *v) {
	if (!strncmp(p, "true", 4)) {
		*v = 1;
		return p + 4;
	} else if (!strncmp(p, "false", 5)) {
		*v = 0;
		return p + 5;
	} else {
		return errret;
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
			return errret;
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
			return errret;
		}
		p++;
	}

	*v = num;
	return p;
}

static char *parse_i32(char *p, int32_t *v) {
	uint64_t num;
	int negate = 0;
	p = parse_number(p, &num, &negate);
	if (num > INT32_MAX) {
		return errret;
	}
	*v = (negate ? -1 : 1) * (int32_t)num;
	return p;
}

static char *parse_u32(char *p, uint32_t *v) {
	uint64_t num;
	p = parse_number(p, &num, NULL);
	if (num > UINT32_MAX) {
		return errret;
	}
	*v = (uint32_t)num;
	return p;
}

static char *parse_i64(char *p, int64_t *v) {
	uint64_t num;
	int negate = 0;
	p = parse_number(p, &num, &negate);
	if (num > INT64_MAX) {
		return errret;
	}
	*v = (negate ? -1 : 1) * (int64_t)num;
	return p;
}

static char *parse_u64(char *p, uint64_t *v) {
	p = parse_number(p, v, NULL);
	return p;
}

static char *parse_float(char *p, float *v) {
	char *end;
	if (*p == '\"') {
		p++;
	}
	*v = strtof((char*)p, &end);
	if (end == (char*)p) {
		return errret;
	}
	if (*end == '\"') {
		end++;
	}
	return end;
}

static char *parse_double(char *p, double *v) {
	char *end;
	if (*p == '\"') {
		p++;
	}
	*v = strtod((char*)p, &end);
	if (end == (char*)p) {
		return errret;
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

static char *decode_unpadded(char *p, pb_bytes *v) {
	uint8_t *in = (uint8_t*)p;
	uint8_t *out = (uint8_t*)p;
	uint8_t *ret = (uint8_t*)p;
	for (;;) {
		int ch0 = base64rev[in[0]];
		int ch1 = base64rev[in[1]];
		int ch2 = base64rev[in[2]];
		int ch3 = base64rev[in[3]];
		if (ch0 < 0 || ch1 < 0) {
			v->p = ret;
			v->len = (uint32_t)(out - ret);
			return (char*)in;
		} else if (ch1 < 0) {
			// 00000011 11112222 22333333
			v->p = ret;
			v->len = (uint32_t)(out - ret);
			return (char*)in + 1;
		} else if (ch2 < 0) {
			// 00000011 11112222 22333333
			out[0] = (uint8_t)((ch0 << 2) | (ch1 >> 4));
			v->p = ret;
			v->len = (uint32_t)(out - ret + 1);
			return (char*)in + 2;
		} else if (ch3 < 0) {
			// 00000011 11112222 22333333
			out[0] = (uint8_t)((ch0 << 2) | (ch1 >> 4));
			out[1] = (uint8_t)((ch1 << 4) | (ch2 >> 2));
			v->p = ret;
			v->len = (uint32_t)(out - ret + 2);
			return (char*)in + 3;
		} else {
			// 00000011 11112222 22333333
			out[0] = (uint8_t)((ch0 << 2) | (ch1 >> 4));
			out[1] = (uint8_t)((ch1 << 4) | (ch2 >> 2));
			out[2] = (uint8_t)((ch2 << 6) | (ch3));
			out += 3;
			in += 4;
		}
	}
}

char *pb_decode_base64(char *p, pb_bytes *v) {
	p = decode_unpadded(p, v);
	if (*p == '=') {
		p++;
	}
	if (*p == '=') {
		p++;
	}
	return p;
}

static char *parse_bytes(char *p, pb_bytes *v) {
	if (*p != '\"') {
		return errret;
	}
	p = pb_decode_base64(p + 1, v);
	if (*p != '\"') {
		return errret;
	}
	((uint8_t*)v->p)[v->len] = 0;
	return p + 1;
}

static char *parse_string(char *p, pb_string *v) {
	if (*p != '\"') {
		return errret;
	}
	p++;
	uint8_t *in = (uint8_t*)p;
	uint8_t *out = (uint8_t*)p;
	uint8_t *ret = (uint8_t*)p;
	for (;;) {
		switch (*in) {
		case '\0':
			return errret;
		case '"':
			v->c_str = (char*)ret;
			v->len = (int)(out - ret);
			*out = '\0';
			return (char*)in + 1;
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
			case 'u':
				{
					uint32_t u0;
					int i, ch[4];
					for (i = 0; i < 4; i++) {
						ch[i] = hexrev[in[i]];
						if (ch[i] < 0) {
							return errret;
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
							return errret;
						}
						in += 2;
						for (i = 0; i < 4; i++) {
							ch[i] = hexrev[in[i]];
							if (ch[i] < 0) {
								return errret;
							}
						}
						in += 4;
						uint32_t u1 = (ch[0] << 12) | (ch[1] << 8) | (ch[2] << 4) | ch[3];

						// Check for a valid surrogate
						if (u1 < 0xDC00 || u1 > 0xDFFF) {
							return errret;
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
				return errret;
			}
			break;
		default:
			*out++ = *in++;
			break;
		}
	}
}

static char *consume_key(char *p, pb_string *s) {
	if (*p != '"') {
		goto err;
	}

	s->c_str = ++p;

	for (;;) {
		if (*p == '\"') {
			break;
		} else if (*p == '\\') {
			goto err;
		}
		p++;
	}

	s->len = p - s->c_str;
	*p = '\0';

	p = consume_space(p+1);
	if (*p != ':') {
		goto err;
	}
	return consume_space(p+1);

err:
	s->len = 0;
	return errret;
}

static bool start_array(char **pp) {
	char *p = *pp;
	if (*p != '[') {
		*pp = errret;
		return false;
	}
	p = consume_space(p+1);
	if (*p == ']') {
		// empty list []
		*pp = p + 1;
		return false;
	}
	*pp = p;
	return true;
}

static bool more_in_array(char **pp) {
	char *p = consume_space(*pp);
	char ch = *(p++);
	if (ch == ']') {
		*pp = p;
		return false;
	} else if (ch == ',') {
		*pp = consume_space(p);
		return true;
	} else {
		*pp = errret;
		return false;
	}
}

static void *start_list(pb_allocator *obj, char **pp, pb_pod_list *list, size_t datasz) {
	if (!start_array(pp)) {
		return NULL;
	}
	char *ret = align(obj->next, OBJ_ALIGN);
	obj->next = ret + datasz;
	if (obj->next > obj->end) {
		*pp = errret;
		return NULL;
	}
	list->data = ret;
	list->len = 1;
	return ret;
}

static void *append_to_list(pb_allocator *obj, char **pp, pb_pod_list *list, size_t datasz) {
	if (!more_in_array(pp)) {
		return NULL;
	}
	char *ret = obj->next;
	obj->next += datasz;
	if (obj->next > obj->end) {
		*pp = errret;
		return NULL;
	}
	list->len++;
	return ret;
}

static char *skip_value(char *p) {
	int depth = 0;
	for (;;) {
		char ch = *(p++);
		if (ch == '[' || ch == '{') {
			depth++;
		} else if (ch == '}' || ch == ']') {
			depth--;
		} else if (ch == ':') {
			// field separator
		} else if (ch == '"') {
			// strings
			for (;;) {
				char ch2 = *(p++);
				if (ch2 == '"') {
					break;
				} else if (ch2 == '\\') {
					ch2 = *(p++);
				}

				if (!ch2) {
					return errret;
				}

				p++;
			}
		} else if ('a' <= ch && ch <= 'z') {
			// atoms
			// null, false, true
			while ('a' <= *p && *p <= 'z') {
				p++;
			}
		} else if (('0' <= ch && ch <= '9') || ch == '-' || ch == '+') {
			// numbers
			// -0.3e+9
			while (('0' <= *p && *p <= '9') || *p == '.' || *p == 'e' || *p == 'E' || *p == '-' || *p == '+') {
				p++;
			}
		} else {
			return errret;
		}

		if (!depth) {
			return p;
		}
	}
}

char *parse_enum(char *p, const struct proto_enum *en, int *pv) {
	pb_string str;
	p = parse_string(p, &str);
	const pb_string *vname = binary_search(en->by_name, en->num_values, str);
	if (vname) {
		const struct proto_enum_value *v = (const struct proto_enum_value*) vname;
		*pv = v->number;
	}
	return p;
}

void *pb_parse(pb_allocator *obj, const struct proto_message *type, char *p) {
	int depth = 0;
	struct parse_stack stack[MAX_DEPTH];
	char *objstart = obj->next;

	p = consume_space(p);
	if (*p != '{') {
		goto err;
	}

	char *msg = (char*) pb_calloc(obj, 1, type->datasz);
	if (!msg) {
		goto err;
	}

	pb_string key;
	const pb_string *fname;
	const struct proto_field *f;
	pb_pod_list *list;

	char field_seperator = '{';

	for (;;) {
		p = consume_space(p);
		if (*p == field_seperator) {
			// beginning of a key
			field_seperator = ',';

			p = consume_space(p + 1);
			if (*p == '}') {
				// close of the structure when a key was expected
				// this happens with an empty struct {} or with a trailing
				// comma
				continue;
			}

			p = consume_key(p, &key);
			fname = binary_search(type->by_name, type->num_fields, key);
			if (!fname) {
				p = skip_value(p);
				continue;
			}

			f = (const struct proto_field*) fname;
			list = (pb_pod_list*)(msg + f->offset);

			if (f->oneof >= 0) {
				unsigned *oneof = (unsigned*)(msg + f->oneof);
				if (*oneof) {
					// multiple field from the same union in the message
					goto err;
				}
				*oneof = (f->tag >> 3);
			}

			switch (f->type) {
			case PROTO_BOOL:
				p = parse_bool(p, (bool*)(msg + f->offset));
				break;
			case PROTO_F32:
			case PROTO_U32:
				p = parse_u32(p, (unsigned*)(msg + f->offset));
				break;
			case PROTO_SF32:
			case PROTO_I32:
			case PROTO_S32:
				p = parse_i32(p, (int*)(msg + f->offset));
				break;
			case PROTO_U64:
			case PROTO_F64:
				p = parse_u64(p, (uint64_t*)(msg + f->offset));
				break;
			case PROTO_S64:
			case PROTO_SF64:
			case PROTO_I64:
				p = parse_i64(p, (int64_t*)(msg + f->offset));
				break;
			case PROTO_FLOAT:
				p = parse_float(p, (float*)(msg + f->offset));
				break;
			case PROTO_DOUBLE:
				p = parse_double(p, (double*)(msg + f->offset));
				break;
			case PROTO_STRING:
				p = parse_string(p, (pb_string*)(msg + f->offset));
				break;
			case PROTO_BYTES:
				p = parse_bytes(p, (pb_bytes*)(msg + f->offset));
				break;
			case PROTO_ENUM:
				p = parse_enum(p, (struct proto_enum*) f->proto_type, (int*)(msg + f->offset));
				break;
			case PROTO_LIST_BOOL: 
				for (void *v = start_list(obj, &p, list, 1); v != NULL; v = append_to_list(obj, &p, list, 1)) {
					p = parse_bool(p, (bool*) v);
				}
				break;
			case PROTO_LIST_F32:
			case PROTO_LIST_U32: 
				for (void *v = start_list(obj, &p, list, 4); v != NULL; v = append_to_list(obj, &p, list, 4)) {
					p = parse_u32(p, (uint32_t*) v);
				}
				break;
			case PROTO_LIST_S32:
			case PROTO_LIST_SF32:
			case PROTO_LIST_I32:
				for (void *v = start_list(obj, &p, list, 4); v != NULL; v = append_to_list(obj, &p, list, 4)) {
					p = parse_i32(p, (int32_t*)v);
				}
				break;
			case PROTO_LIST_F64:
			case PROTO_LIST_U64:
				for (void *v = start_list(obj, &p, list, 8); v != NULL; v = append_to_list(obj, &p, list, 8)) {
					p = parse_u64(p, (uint64_t*)v);
				}
				break;
			case PROTO_LIST_S64:
			case PROTO_LIST_SF64:
			case PROTO_LIST_I64:
				for (void *v = start_list(obj, &p, list, 8); v != NULL; v = append_to_list(obj, &p, list, 8)) {
					p = parse_i64(p, (int64_t*)v);
				}
				break;
			case PROTO_LIST_FLOAT:
				for (void *v = start_list(obj, &p, list, 4); v != NULL; v = append_to_list(obj, &p, list, 4)) {
					p = parse_float(p, (float*)v);
				}
				break;
			case PROTO_LIST_DOUBLE:
				for (void *v = start_list(obj, &p, list, 8); v != NULL; v = append_to_list(obj, &p, list, 8)) {
					p = parse_double(p, (double*)v);
				}
				break;
			case PROTO_LIST_STRING:
				for (void *v = start_list(obj, &p, list, sizeof(pb_string)); v != NULL; v = append_to_list(obj, &p, list, sizeof(pb_string))) {
					p = parse_string(p, (pb_string*)v);
				}
				break;
			case PROTO_LIST_BYTES:
				for (void *v = start_list(obj, &p, list, sizeof(pb_bytes)); v != NULL; v = append_to_list(obj, &p, list, sizeof(pb_bytes))) {
					p = parse_bytes(p, (pb_bytes*)v);
				}
				break;
			case PROTO_LIST_ENUM:
				for (void *v = start_list(obj, &p, list, sizeof(int)); v != NULL; v = append_to_list(obj, &p, list, sizeof(int))) {
					p = parse_enum(p, (struct proto_enum*) f->proto_type, (int*)v);
				}
				break;
			case PROTO_POD:
			case PROTO_MESSAGE:
				stack[depth].type = type;
				stack[depth].msg = msg;
				stack[depth].field = f;
				field_seperator = '{';

				if (++depth == MAX_DEPTH) {
					goto err;
				}

				type = (const struct proto_message*) f->proto_type;
				
				if (f->type == PROTO_POD) {
					msg += f->offset;
				} else {
					msg = create_child_message(obj, msg + f->offset, type->datasz);
				}

				if (!msg) {
					goto err;
				}
				break;
			case PROTO_LIST_POD:
			case PROTO_LIST_MESSAGE:
				if (!start_array(&p)) {
					break;
				}
			next_message_in_list:
				stack[depth].type = type;
				stack[depth].msg = msg;
				stack[depth].field = f;
				field_seperator = '{';

				if (++depth == MAX_DEPTH) {
					goto err;
				}

				type = (const struct proto_message*) f->proto_type;

				if (f->type == PROTO_LIST_POD) {
					msg = append_pod_list(obj, msg + f->offset, type->datasz);
				} else {
					msg = append_message_list(obj, msg + f->offset, type->datasz);
				}

				if (!msg) {
					goto err;
				}
				break;
			}

		} else if (*p == '}') {
			// end of the current structure
			if (!depth) {
				return msg;
			}

			p++;
			depth--;

			f = stack[depth].field;
			msg = stack[depth].msg;
			type = stack[depth].type;
			list = (pb_pod_list*)(msg + f->offset);
			field_seperator = ',';

			if (f->type == PROTO_LIST_POD || f->type == PROTO_LIST_MESSAGE) {
				if (more_in_array(&p)) {
					goto next_message_in_list;
				}

				// commit the list
				if (f->type == PROTO_LIST_MESSAGE && create_message_list(obj, msg + f->offset)) {
					goto err;
				}
			}
		} else {
			goto err;
		}
	}

err:
	obj->next = objstart;
	return NULL;
}

