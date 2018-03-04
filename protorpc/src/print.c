#include "common.h"
#include <protorpc/char-array.h>
#include <inttypes.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

struct print_stack {
	const struct proto_field *next_field;
	const struct proto_field *field_end;
	char *msg;
	char *key_start;
	char *next_msg;
};

static void print_key(pb_allocator *out, pb_string key, int indent) {
	char *keyend = out->next + /*\n*/ 1 + (indent < 0 ? -1 : indent) + 1 /*"*/ + key.len + 3 /*": */;
	if (keyend >= out->end) {
		out->end = out->next;
	} else {
		if (indent >= 0) {
			*(out->next++) = '\n';
			while (indent) {
				*(out->next++) = '\t';
				indent--;
			}
		}
		*(out->next++) = '"';
		memcpy(out->next, key.c_str, key.len);
		out->next += key.len;
		*(out->next++) = '"';
		*(out->next++) = ':';
		if (indent >= 0) {
			*(out->next++) = ' ';
		}
	}
}

static void print_text(pb_allocator *out, const char *text, int sz) {
	char *end = out->next + sz;
	if (end >= out->end) {
		out->end = out->next;
	} else {
		memcpy(out->next, text, sz);
		out->next = end;
	}
}

static void print_bool(pb_allocator *out, bool val) {
	if (val) {
		print_text(out, "true,", 5);
	} else {
		print_text(out, "false,", 6);
	}
}

static void print_u32(pb_allocator *out, unsigned val) {
	char buf[32];
	int len = sprintf(buf, "%u,", val);
	print_text(out, buf, len);
}

static void print_i32(pb_allocator *out, int val) {
	char buf[32];
	int len = sprintf(buf, "%d,", val);
	print_text(out, buf, len);
}

static void print_u64(pb_allocator *out, uint64_t val) {
	char buf[32];
	int len = sprintf(buf, "\"%" PRIu64 "\",", val);
	print_text(out, buf, len);
}

static void print_i64(pb_allocator *out, int64_t val) {
	char buf[32];
	int len = sprintf(buf, "\"%" PRId64 "\",", val);
	print_text(out, buf, len);
}

static void print_float(pb_allocator *out, float val) {
	if (val != val) {
		print_text(out, "\"NaN\",", 6);
	} else {
		char buf[32];
		int len = sprintf(buf, "%.8g,", val);
		print_text(out, buf, len);
	}
}

static void print_double(pb_allocator *out, double val) {
	if (val != val) {
		print_text(out, "\"NaN\",", 6);
	} else {
		char buf[32];
		int len = sprintf(buf, "%.16g,", val);
		print_text(out, buf, len);
	}
}

static void print_enum(pb_allocator *o, int value, const struct proto_enum *en) {
	for (size_t i = 0; i < en->num_values; i++) {
		const struct proto_enum_value *v = &en->values[i];
		if (v->number == value) {
			if (o->next + 1 + v->name.len + 2 >= o->end) {
				o->end = o->next;
				return;
			}
			*(o->next++) = '"';
			memcpy(o->next, v->name.c_str, v->name.len);
			o->next += v->name.len;
			*(o->next++) = '"';
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

static void print_string(pb_allocator *o, pb_string v) {
	if (o->next + 1 /*"*/ + v.len + 2 /*",*/ >= o->end) {
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
	o->end = o->next;
}

char *pb_encode_base64(char *p, const uint8_t *v, int n) {
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
	*p = '\0';
	return p;
}

static void print_bytes(pb_allocator *o, pb_bytes v) {
	char *p = o->next;
	if (p + 1 /*"*/ + pb_base64_size(v.len) + 2 /*",*/ >= o->end) {
		o->end = o->next;
		return;
	}
	*(p++) = '\"';
	p = pb_encode_base64(p, v.p, v.len);
	*(p++) = '\"';
	*(p++) = ',';
	o->next = p;
}

static void start_array(pb_allocator *o, pb_string json_name, int indent) {
	print_key(o, json_name, indent);
	print_text(o, "[", 1);
}

static void print_indent(pb_allocator *o, int indent) {
	if (indent >= 0) {
		if (o->next + 1 /*\n*/ + indent >= o->end) {
			o->end = o->next;
			return;
		}
		*(o->next++) = '\n';
		while (indent) {
			*(o->next++) = '\t';
			indent--;
		}
	}
}

static void finish_array(pb_allocator *o, int indent) {
	// now add \n<indent>],
	if (o->next - 1 /*,*/ + 1 /*\n*/ + indent + 2 /*],*/ >= o->end) {
		o->end = o->next;
		return;
	}
	o->next--; // remove trailing ,
	*(o->next++) = '\n';
	while (indent) {
		*(o->next++) = '\t';
		indent--;
	}
	*(o->next++) = ']';
	*(o->next++) = ',';
}

int pb_print(char *buf, int sz, const void *obj, const struct proto_message *type, int indent) {
	int depth = 0;
	struct print_stack stack[MAX_DEPTH];
	const struct proto_field *f = type->fields;
	const struct proto_field *end = f + type->num_fields;
	char *msg = (char*)obj;
	char *next_msg = NULL;
	pb_allocator out;
	out.next = buf;
	out.end = buf + sz;

	print_text(&out, "{", 1);
	if (indent >= 0) {
		indent++;
	} else {
		indent = INT_MIN;
	}

	for (;;) {
		while (f < end) {
			if (f->oneof >= 0) {
				unsigned oneof = *(unsigned*)(msg + f->oneof);

				// bottom three bits of the tag is the wire type
				// type enum just have the field number
				if (oneof != (f->tag >> 3)) {
					f++;
					continue;
				}
			}

			switch (f->type) {
			case PROTO_BOOL:
				if (*(bool*)(msg + f->offset)) {
					print_key(&out, f->json_name, indent);
					print_text(&out, "true,", 5);
				}
				break;
			case PROTO_F32:
			case PROTO_U32: {
				unsigned val = *(unsigned*)(msg + f->offset);
				if (val) {
					print_key(&out, f->json_name, indent);
					print_u32(&out, val);
				}
				break;
			}
			case PROTO_OPTIONAL_U32: {
				pb_opt_uint *opt = (pb_opt_uint*)(msg + f->offset);
				if (opt->set) {
					print_key(&out, f->json_name, indent);
					print_u32(&out, opt->val);
				}
				break;
			}
			case PROTO_S32:
			case PROTO_SF32:
			case PROTO_I32: {
				int val = *(int*)(msg + f->offset);
				if (val) {
					print_key(&out, f->json_name, indent);
					print_i32(&out, val);
				}
				break;
			}
			case PROTO_F64:
			case PROTO_U64: {
				uint64_t val = *(uint64_t*)(msg + f->offset);
				if (val) {
					print_key(&out, f->json_name, indent);
					print_u64(&out, val);
				}
				break;
			}
			case PROTO_S64:
			case PROTO_SF64:
			case PROTO_I64: {
				int64_t val = *(int64_t*)(msg + f->offset);
				if (val) {
					print_key(&out, f->json_name, indent);
					print_i64(&out, val);
				}
				break;
			}
			case PROTO_FLOAT: {
				float val = *(float*)(msg + f->offset);
				if (val != 0) {
					print_key(&out, f->json_name, indent);
					print_float(&out, val);
				}
				break;
			}
			case PROTO_DOUBLE: {
				double val = *(double*)(msg + f->offset);
				if (val != 0) {
					print_key(&out, f->json_name, indent);
					print_double(&out, val);
				}
				break;
			}
			case PROTO_STRING: {
				pb_string *s = (pb_string*)(msg + f->offset);
				if (s->len) {
					print_key(&out, f->json_name, indent);
					print_string(&out, *s);
				}
				break;
			}
			case PROTO_BYTES: {
				pb_bytes *b = (pb_bytes*)(msg + f->offset);
				if (b->len) {
					print_key(&out, f->json_name, indent);
					print_bytes(&out, *b);
				}
				break;
			}
			case PROTO_ENUM: {
				int val = *(int*)(msg + f->offset);
				if (val) {
					print_key(&out, f->json_name, indent);
					print_enum(&out, val, (struct proto_enum*) f->proto_type);
				}
				break;
			}
			case PROTO_LIST_BOOL: {
				pb_bool_list *list = (pb_bool_list*)(msg + f->offset);
				if (list->len) {
					start_array(&out, f->json_name, indent++);
					for (int i = 0; i < list->len; i++) {
						print_indent(&out, indent);
						print_bool(&out, list->v[i]);
					}
					finish_array(&out, --indent);
				}
				break;
			}
			case PROTO_LIST_F32:
			case PROTO_LIST_U32: {
				pb_u32_list *list = (pb_u32_list*)(msg + f->offset);
				if (list->len) {
					start_array(&out, f->json_name, indent++);
					for (int i = 0; i < list->len; i++) {
						print_indent(&out, indent);
						print_u32(&out, list->v[i]);
					}
					finish_array(&out, --indent);
				}
				break;
			}
			case PROTO_LIST_S32:
			case PROTO_LIST_SF32:
			case PROTO_LIST_I32: {
				pb_i32_list *list = (pb_i32_list*)(msg + f->offset);
				if (list->len) {
					start_array(&out, f->json_name, indent++);
					for (int i = 0; i < list->len; i++) {
						print_indent(&out, indent);
						print_i32(&out, list->v[i]);
					}
					finish_array(&out, --indent);
				}
				break;
			}
			case PROTO_LIST_F64:
			case PROTO_LIST_U64: {
				pb_u64_list *list = (pb_u64_list*)(msg + f->offset);
				if (list->len) {
					start_array(&out, f->json_name, indent++);
					for (int i = 0; i < list->len; i++) {
						print_indent(&out, indent);
						print_u64(&out, list->v[i]);
					}
					finish_array(&out, --indent);
				}
				break;
			}
			case PROTO_LIST_S64:
			case PROTO_LIST_SF64:
			case PROTO_LIST_I64: {
				pb_i64_list *list = (pb_i64_list*)(msg + f->offset);
				if (list->len) {
					start_array(&out, f->json_name, indent++);
					for (int i = 0; i < list->len; i++) {
						print_indent(&out, indent);
						print_i64(&out, list->v[i]);
					}
					finish_array(&out, --indent);
				}
				break;
			}
			case PROTO_LIST_FLOAT: {
				pb_float_list *list = (pb_float_list*)(msg + f->offset);
				if (list->len) {
					start_array(&out, f->json_name, indent++);
					for (int i = 0; i < list->len; i++) {
						print_indent(&out, indent);
						print_float(&out, list->v[i]);
					}
					finish_array(&out, --indent);
				}
				break;
			}
			case PROTO_LIST_DOUBLE: {
				pb_double_list *list = (pb_double_list*)(msg + f->offset);
				if (list->len) {
					start_array(&out, f->json_name, indent++);
					for (int i = 0; i < list->len; i++) {
						print_indent(&out, indent);
						print_double(&out, list->v[i]);
					}
					finish_array(&out, --indent);
				}
				break;
			}
			case PROTO_LIST_STRING: {
				pb_string_list *list = (pb_string_list*)(msg + f->offset);
				if (list->len) {
					start_array(&out, f->json_name, indent++);
					for (int i = 0; i < list->len; i++) {
						print_indent(&out, indent);
						print_string(&out, list->v[i]);
					}
					finish_array(&out, --indent);
				}
				break;
			}
			case PROTO_LIST_BYTES: {
				pb_bytes_list *list = (pb_bytes_list*)(msg + f->offset);
				if (list->len) {
					start_array(&out, f->json_name, indent++);
					for (int i = 0; i < list->len; i++) {
						print_indent(&out, indent);
						print_bytes(&out, list->v[i]);
					}
					finish_array(&out, --indent);
				}
				break;
			}
			case PROTO_LIST_ENUM: {
				pb_i32_list *list = (pb_i32_list*)(msg + f->offset);
				if (list->len) {
					start_array(&out, f->json_name, indent++);
					for (int i = 0; i < list->len; i++) {
						print_indent(&out, indent);
						print_enum(&out, list->v[i], (struct proto_enum*) f->proto_type);
					}
					finish_array(&out, --indent);
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
				stack[depth].key_start = out.next;

				print_key(&out, f->json_name, indent++);
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
			{
				pb_pod_list *list = (pb_pod_list*)(msg + f->offset);
				if (list->len) {
					next_msg = list->data;
					start_array(&out, f->json_name, indent++);
					goto next_message_in_list;
				}
				break;
			}
			case PROTO_LIST_MESSAGE:
			{
				pb_msg_list *list = (pb_msg_list*)(msg + f->offset);
				next_msg = (char*)list->first;
				if (next_msg) {
					start_array(&out, f->json_name, indent++);
					goto next_message_in_list;
				}
				break;
			}
			next_message_in_list:
				{
					print_indent(&out, indent++);
					print_text(&out, "{", 1);

					stack[depth].next_field = f;
					stack[depth].field_end = end;
					stack[depth].msg = msg;

					const struct proto_message *ct = (const struct proto_message*) f->proto_type;

					if (f->type == PROTO_LIST_POD) {
						pb_pod_list *pods = (pb_pod_list*) (msg + f->offset);
						msg = next_msg;
						next_msg = msg + ct->datasz;
						if (next_msg == pods->data + (pods->len * ct->datasz)) {
							next_msg = NULL;
						}
					} else {
						msg = next_msg;
						next_msg = (char*) ((pb_message*)msg)->next;
					}

					stack[depth].next_msg = next_msg;

					if (++depth == MAX_DEPTH) {
						return -1;
					}

					f = ct->fields;
					end = f + ct->num_fields;
					continue;
				}
			}

			f++;
		}

		if (!depth) {
			if (out.next == out.end) {
				return -1;
			}
			if (out.next[-1] == ',') {
				// non-empty message
				out.next--;
				print_indent(&out, --indent);
				// otherwise empty "{}\n"
			}
			print_text(&out, "}\n", 2);
			*out.next = '\0';
			return out.next - buf;
		}

		depth--;
		indent--;

		f = stack[depth].next_field;
		end = stack[depth].field_end;
		msg = stack[depth].msg;

		switch (f->type) {
		case PROTO_LIST_POD:
		case PROTO_LIST_MESSAGE:
			if (out.next[-1] == ',') {
				// non-empty message
				// remove the trailing comma
				out.next--;
				print_indent(&out, indent);
			}
			print_text(&out, "},", 2);
			next_msg = stack[depth].next_msg;
			if (next_msg) {
				goto next_message_in_list;
			} else {
				finish_array(&out, --indent);
			}
			break;

		case PROTO_POD:
		case PROTO_MESSAGE:
		default:
			if (out.next[-1] == '{') {
				// remove empty messages
				out.next = stack[depth].key_start;
			} else {
				// remove the trailing comma
				out.next--;
				print_indent(&out, indent);
				print_text(&out, "},", 2);
			}
			break;
		}

		f++;
	}
}

int pb_vprint(char *buf, int sz, const char *fmt, va_list ap, int indent) {
	pb_allocator o;
	o.next = buf;
	o.end = buf + sz;

	while (*fmt) {
		const char *bar = strchr(fmt, '|');
		const char *next = bar ? (bar + 1) : (fmt + strlen(fmt));
		if (!bar) {
			bar = next;
		}
		const char *colon = (const char*) memchr(fmt, ':', bar - fmt);
		if (!colon) {
			return -1;
		}

		pb_string key = {colon - fmt, fmt};
		pb_string type = {bar - colon - 1, colon + 1};
		fmt = next;

		print_key(&o, key, indent);

		if (str_test(type, "%p%p")) {
			// protobuf type
			const proto_message *ct = va_arg(ap, const proto_message*);
			const void *obj = va_arg(ap, const void*);
			int r = pb_print(o.next, o.end - o.next, obj, ct, indent);
			if (r < 0) {
				return -1;
			}
			o.next += r;
			o.next[-1] = ','; // replace trailing \n with a comma

		} else if (str_test(type, "%.*s")) {
			int len = va_arg(ap, int);
			const char *cstr = va_arg(ap, const char*);
			pb_string str = {len, cstr};
			print_string(&o, str);

		} else if (str_test(type, "%s")) {
			const char *cstr = va_arg(ap, const char*);
			int len = (int) strlen(cstr);
			pb_string str = {len, cstr};
			print_string(&o, str);

		} else if (str_test(type, "%d")) {
			int val = va_arg(ap, int);
			print_i32(&o, val);

		} else if (str_test(type, "%u")) {
			unsigned val = va_arg(ap, unsigned);
			print_u32(&o, val);

		} else if (str_test(type, "%"PRIu64)) {
			uint64_t val = va_arg(ap, uint64_t);
			print_u64(&o, val);

		} else if (str_test(type, "%"PRId64)) {
			int64_t val = va_arg(ap, int64_t);
			print_i64(&o, val);

		} else if (str_test(type, "%g")) {
			double val = va_arg(ap, double);
			print_double(&o, val);

		} else if (str_test(type, "%c")) {
			int val = va_arg(ap, int);
			print_bool(&o, val != 0);

		} else {
			print_string(&o, type);
		}
	}

	if (o.next == o.end) {
		return -1;
	}
	*o.next = 0;
	return o.next - buf;
}



