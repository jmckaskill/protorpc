#include "compact.h"
#include <inttypes.h>

#define MAX_DEPTH 8

struct print_stack {
	const struct proto_field *next_field;
	const struct proto_field *field_end;
	char *msg;
	int next_index;
};

struct out {
	char *next;
	char *end;
};

static void print_key(struct out *out, const char *key) {
	size_t keysz = strlen(key);
	char *keyend = out->next + 1 + keysz + 2;
	if (keyend > out->end) {
		out->next = out->end = NULL;
	} else {
		*(out->next++) = '"';
		memcpy(out->next, key, keysz);
		out->next += keysz;
		*(out->next++) = '"';
		*(out->next++) = ':';
	}
}

static void print_text(struct out *out, const char *text, int sz) {
	char *end = out->next + sz;
	if (end > out->end) {
		out->next = out->end = NULL;
	} else {
		memcpy(out->next, text, sz);
		out->next = end;
	}
}

static void print_u32(struct out *out, unsigned val) {
	char buf[32];
	int len = sprintf(buf, "%u,", val);
	print_text(out, buf, len);
}

static void print_i32(struct out *out, int val) {
	char buf[32];
	int len = sprintf(buf, "%d,", val);
	print_text(out, buf, len);
}

static void print_u64(struct out *out, uint64_t val) {
	char buf[32];
	int len = sprintf(buf, "\"%" PRIu64 "\",", val);
	print_text(out, buf, len);
}

static void print_i64(struct out *out, int64_t val) {
	char buf[32];
	int len = sprintf(buf, "\"%" PRId64 "\",", val);
	print_text(out, buf, len);
}

static void print_float(struct out *out, float val) {
	if (val != val) {
		print_text(out, "\"NaN\",", 6);
	} else {
		char buf[32];
		int len = sprintf(buf, "%.8g,", val);
		print_text(out, buf, len);
	}
}

static void print_double(struct out *out, double val) {
	if (val != val) {
		print_text(out, "\"NaN\",", 6);
	} else {
		char buf[32];
		int len = sprintf(buf, "%.16g,", val);
		print_text(out, buf, len);
	}
}

static void print_enum(struct out *o, int value, const struct proto_enum *en) {
	for (size_t i = 0; i < en->num_values; i++) {
		const struct proto_enum_value *v = &en->values[i];
		if (v->number == value) {
			size_t vsz = strlen(v->name);
			if (o->next + 1 + vsz + 2 > o->end) {
				o->next = o->end = NULL;
				return;
			}
			*(o->next++) = '"';
			*(o->next++) = '"';
			memcpy(o->next, v->name, vsz);
			o->next += vsz;
			*(o->next++) = ',';
			return;
		}
	}
	print_text(o, "\"\",", 3);
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

static void print_string(struct out *o, pb_string_t v) {
	if (o->next + 1 /*"*/ + v.len + 2 /*",*/ > o->end) {
		goto err;
	}
	char *p = o->next;
	*(p++) = '\"';
	const char *s = v.c_str;
	const char *e = s + v.len;
	while (s < e) {
		uint8_t uch = (uint8_t) *(s++);
		char escape = escapechar[uch];
		if (!escape) {
			*(p++) = (char)uch;
		} else if (escape == 'u') {
			if (p + (e - s) + 2 /*",*/ + 6 /*\u00EE*/ > o->end) {
				goto err;
			}
			*(p++) = '\\';
			*(p++) = 'u';
			*(p++) = '0';
			*(p++) = '0';
			*(p++) = hexchar[uch >> 4];
			*(p++) = hexchar[uch & 0xF];
		} else {
			if (p + (e - s) + 2 /*",*/ + 2 /*\e*/ > o->end) {
				goto err;
			}
			*(p++) = '\\';
			*(p++) = escape;
		}
	}
	*(p++) = '\"';
	*(p++) = ',';
	o->next = p;
	return;
err:
	o->next = o->end = NULL;
}

static inline int base64_size(int sz) {
	return (sz * 4 + 3) / 3;
}

static char *print_base64(char *p, const uint8_t *v, int n) {
	int i;
	for (i = 0; i < n - 2; i += 3) {
		*p++ = base64char[(v[i] >> 2)];
		*p++ = base64char[((v[i] & 3) << 4) | (v[i + 1] >> 4)];
		*p++ = base64char[((v[i + 1] & 0xF) << 2) | (v[i + 2] >> 6)];
		*p++ = base64char[(v[i + 2] & 0x3F)];
	}
	if (i == n - 1) {
		*p++ = base64char[(v[i] >> 2)];
		*p++ = base64char[((v[i] & 3) << 4)];
	} else if (i == n - 2) {
		*p++ = base64char[(v[i] >> 2)];
		*p++ = base64char[((v[i] & 3) << 4) | (v[i + 1] >> 4)];
		*p++ = base64char[((v[i + 1] & 0xF) << 2)];
	}
	return p;
}

static void print_bytes(struct out *o, pb_bytes_t v) {
	char *p = o->next;
	if (p + 1 /*"*/ + pb_base64_size(v.len) + 2 /*",*/ > o->end) {
		o->next = o->end = NULL;
		return;
	}
	*(p++) = '\"';
	p = pb_print_base64(p, v.p, v.len);
	*(p++) = '\"';
	*(p++) = ',';
	o->next = p;
}

static void start_array(struct out *o, const char *json_name) {
	print_key(o, json_name);
	print_text(o, "[", 1);
}

static void finish_array(struct out *o) {
	o->next--; // remove trailing ,
	print_text(o, "],", 2);
}

int pb_print(void *obj, const struct proto_message *type, char *buf, int sz) {
	int depth = 0;
	struct print_stack stack[MAX_DEPTH];
	const struct proto_field *f = type->fields;
	const struct proto_field *end = f + type->num_fields;
	char *msg = (char*)obj;
	int list_index = 0;
	struct out out;
	out.next = buf;
	out.end = buf + sz;

	print_text(&out, "{", 1);

	for (;;) {
		while (f < end) {
			switch (f->type) {
			case PROTO_BOOL:
				if (*(bool*)(msg + f->offset)) {
					print_key(&out, f->json_name);
					print_text(&out, "true,", 5);
				}
				break;
			case PROTO_F32:
			case PROTO_U32: {
				unsigned val = *(unsigned*)(msg + f->offset);
				if (val) {
					print_key(&out, f->json_name);
					print_u32(&out, val);
				}
				break;
			}
			case PROTO_OPTIONAL_U32: {
				pb_opt_uint *opt = (pb_opt_uint*)(msg + f->offset);
				if (opt->set) {
					print_key(&out, f->json_name);
					print_u32(&out, opt->val);
				}
				break;
			}
			case PROTO_S32:
			case PROTO_SF32:
			case PROTO_I32: {
				int val = *(int*)(msg + f->offset);
				if (val) {
					print_key(&out, f->json_name);
					print_i32(&out, val);
				}
				break;
			}
			case PROTO_F64:
			case PROTO_U64: {
				uint64_t val = *(uint64_t*)(msg + f->offset);
				if (val) {
					print_key(&out, f->json_name);
					print_u64(&out, val);
				}
				break;
			}
			case PROTO_S64:
			case PROTO_SF64:
			case PROTO_I64: {
				int64_t val = *(int64_t*)(msg + f->offset);
				if (val) {
					print_key(&out, f->json_name);
					print_i64(&out, val);
				}
				break;
			}
			case PROTO_FLOAT: {
				float val = *(float*)(msg + f->offset);
				if (val != 0) {
					print_key(&out, f->json_name);
					print_float(&out, val);
				}
				break;
			}
			case PROTO_DOUBLE: {
				double val = *(double*)(msg + f->offset);
				if (val != 0) {
					print_key(&out, f->json_name);
					print_double(&out, val);
				}
				break;
			}
			case PROTO_STRING: {
				pb_string_t *s = (pb_string_t*)(msg + f->offset);
				if (s->len) {
					print_key(&out, f->json_name);
					print_string(&out, *s);
				}
				break;
			}
			case PROTO_BYTES: {
				pb_bytes_t *b = (pb_bytes_t*)(msg + f->offset);
				if (b->len) {
					print_key(&out, f->json_name);
					print_bytes(&out, *b);
				}
				break;
			}
			case PROTO_ENUM: {
				int val = *(int*)(msg + f->offset);
				if (val) {
					print_key(&out, f->json_name);
					print_enum(&out, val, (struct proto_enum*) f->type);
				}
				break;
			}
			case PROTO_LIST_BOOL: {
				pb_bool_list *list = (pb_bool_list*)(msg + f->offset);
				if (list->len) {
					start_array(&out, f->json_name);
					for (int i = 0; i < list->len; i++) {
						if (list->v[i]) {
							print_text(&out, "true,", 5);
						} else {
							print_text(&out, "false,", 6);
						}
					}
					finish_array(&out);
				}
				break;
			}
			case PROTO_LIST_F32:
			case PROTO_LIST_U32: {
				pb_uint_list *list = (pb_uint_list*)(msg + f->offset);
				if (list->len) {
					start_array(&out, f->json_name);
					for (int i = 0; i < list->len; i++) {
						print_u32(&out, list->v[i]);
					}
					finish_array(&out);
				}
				break;
			}
			case PROTO_LIST_S32:
			case PROTO_LIST_SF32:
			case PROTO_LIST_I32: {
				pb_int_list *list = (pb_int_list*)(msg + f->offset);
				if (list->len) {
					start_array(&out, f->json_name);
					for (int i = 0; i < list->len; i++) {
						print_i32(&out, list->v[i]);
					}
					finish_array(&out);
				}
				break;
			}
			case PROTO_LIST_F64:
			case PROTO_LIST_U64: {
				pb_u64_list *list = (pb_u64_list*)(msg + f->offset);
				if (list->len) {
					start_array(&out, f->json_name);
					for (int i = 0; i < list->len; i++) {
						print_u64(&out, list->v[i]);
					}
					finish_array(&out);
				}
				break;
			}
			case PROTO_LIST_S64:
			case PROTO_LIST_SF64:
			case PROTO_LIST_I64: {
				pb_i64_list *list = (pb_i64_list*)(msg + f->offset);
				if (list->len) {
					start_array(&out, f->json_name);
					for (int i = 0; i < list->len; i++) {
						print_i64(&out, list->v[i]);
					}
					finish_array(&out);
				}
				break;
			}
			case PROTO_LIST_FLOAT: {
				pb_float_list *list = (pb_float_list*)(msg + f->offset);
				if (list->len) {
					start_array(&out, f->json_name);
					for (int i = 0; i < list->len; i++) {
						print_float(&out, list->v[i]);
					}
					finish_array(&out);
				}
				break;
			}
			case PROTO_LIST_DOUBLE: {
				pb_double_list *list = (pb_double_list*)(msg + f->offset);
				if (list->len) {
					start_array(&out, f->json_name);
					for (int i = 0; i < list->len; i++) {
						print_double(&out, list->v[i]);
					}
					finish_array(&out);
				}
				break;
			}
			case PROTO_LIST_STRING: {
				pb_string_list *list = (pb_string_list*)(msg + f->offset);
				if (list->len) {
					start_array(&out, f->json_name);
					for (int i = 0; i < list->len; i++) {
						print_string(&out, list->v[i]);
					}
					finish_array(&out);
				}
				break;
			}
			case PROTO_LIST_BYTES: {
				pb_bytes_list *list = (pb_bytes_list*)(msg + f->offset);
				if (list->len) {
					start_array(&out, f->json_name);
					for (int i = 0; i < list->len; i++) {
						print_bytes(&out, list->v[i]);
					}
					finish_array(&out);
				}
				break;
			}
			case PROTO_LIST_ENUM: {
				pb_int_list *list = (pb_int_list*)(msg + f->offset);
				if (list->len) {
					start_array(&out, f->json_name);
					for (int i = 0; i < list->len; i++) {
						print_enum(&out, list->v[i], (struct proto_enum*) f->type);
					}
					finish_array(&out);
				}
				break;
			}
			case PROTO_MESSAGE:
				if (*(char**)(msg + f->offset) == NULL) {
					break;
				}
				// fallthrough
			case PROTO_POD: {
				stack[depth].next_field = f;
				stack[depth].field_end = end;
				stack[depth].msg = msg;

				print_key(&out, f->json_name);
				print_text(&out, "{", 1);

				if (++depth == MAX_DEPTH) {
					return -1;
				}

				if (f->type == PROTO_POD) {
					msg = msg + f->offset;
				} else {
					msg = *(char**)(msg + f->offset);
				}

				const struct proto_message *ct = (const struct proto_message*) f->proto_type;
				f = ct->fields;
				end = f + ct->num_fields;
				continue;
			}
			case PROTO_LIST_POD:
			case PROTO_LIST_MESSAGE:
				list_index = 0;
				// fallthrough
			next_message_in_list: 
				{
					struct pb_message_list *msgs = (struct pb_message_list*) (msg + f->offset);
					if (list_index >= msgs->len) {
						break;
					}

					if (!list_index) {
						start_array(&out, f->json_name);
					}

					stack[depth].next_field = f;
					stack[depth].field_end = end;
					stack[depth].msg = msg;
					stack[depth].next_index = list_index + 1;

					if (++depth == MAX_DEPTH) {
						return -1;
					}

					const struct proto_message *ct = (const struct proto_message*) f->proto_type;

					if (f->type == PROTO_LIST_POD) {
						struct pb_pod_list *pods = (struct pb_pod_list*) msgs;
						msg = pods->data + (list_index * ct->datasz);
					} else {
						msg = (char*)msgs->u.v[list_index];
					}

					f = ct->fields;
					end = f + ct->num_fields;
					continue;
				}
			}

			f++;
		}

		if (!depth) {
			if (out.next[-1] == ',') {
				// non-empty message
				out.next[-1] = '}';
			} else {
				// empty message "{}"
				print_text(&out, "}", 1);
			}
			if (!out.next) {
				return -1;
			}
			return out.next - buf;
		}

		depth--;

		f = stack[depth].next_field;
		end = stack[depth].field_end;
		msg = stack[depth].msg;

		switch (f->type) {
		case PROTO_LIST_POD:
		case PROTO_LIST_MESSAGE:
			// remove the trailing comma
			// only present if it's a non-empty message
			if (out.next[-1] == ',') {
				out.next--;
			}
			print_text(&out, "},", 2);
			list_index = stack[depth].next_index;
			goto next_message_in_list;

		case PROTO_POD:
		case PROTO_MESSAGE:
		default:
			// remove empty messages
			if (out.next[-1] == '{') {
				out.next -= 1 /*"*/ + strlen(f->json_name) + 3 /*":{*/;
			}
			// remove the trailing comma
			out.next--;
			print_text(&out, "},", 2);
			break;
		}

		f++;
	}
}
