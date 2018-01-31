#include "compact.h"

#define WIRE_VARINT 0
#define WIRE_FIXED_64 1
#define WIRE_VARIABLE 2
#define WIRE_FIXED_32 5
#define MAX_DEPTH 8

struct encode_stack {
	const struct proto_field *next_field;
	const struct proto_field *field_end;
	char *msg;
	int next_index;
	union {
		int val;
		uint8_t *ptr;
	} encoded_size;
};

static int varint_size(unsigned val) {
	if (val < 0x80) {
		return 1;
	} else if (val < 0x4000) {
		return 2;
	} else if (val < 0x200000) {
		return 3;
	} else if (val < 0x10000000) {
		return 4;
	} else {
		return 5;
	}
}

static int varint_size_64(uint64_t val) {
	int ret = 0;
	do {
		ret += 1;
		val >>= 7;
	} while (val);
	return ret;
}

static uint32_t encode_zigzag(int32_t v) {
	return ((uint32_t)(v) << 1) ^ (uint32_t)(v >> 31);
}

static uint64_t encode_zigzag_64(int64_t v) {
	return ((uint64_t)(v) << 1) ^ (uint64_t)(v >> 63);
}

int pb_encoded_size(void *obj, const struct proto_message *type) {
	struct encode_stack stack[MAX_DEPTH];
	int depth = 0;
	const struct proto_field *f = type->fields;
	const struct proto_field *end = f + type->num_fields;
	int ret = 0;
	int list_index = 0;
	char *msg = (char*) obj;

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
				if (*(bool*) (msg + f->offset)) {
					ret += varint_size(f->tag);
					ret += 1;
				}
				break;
			case PROTO_ENUM:
			case PROTO_I32:
			case PROTO_U32: {
				uint32_t val = *(uint32_t*) (msg + f->offset);
				if (val) {
					ret += varint_size(f->tag);
					ret += varint_size(val);
				}
				break;
			}
			case PROTO_S32: {
				int32_t val = *(int32_t*)(msg + f->offset);
				if (val) {
					ret += varint_size(f->tag);
					ret += varint_size(encode_zigzag(val));
				}
				break;
			}
			case PROTO_FLOAT:
			case PROTO_SF32:
			case PROTO_F32: {
				uint32_t val = *(uint32_t*) (msg + f->offset);
				if (val) {
					ret += varint_size(f->tag);
					ret += 4;
				}
				break;
			}
			case PROTO_S64: {
				int64_t val = *(int64_t*)(msg + f->offset);
				if (val) {
					ret += varint_size(f->tag);
					ret += varint_size_64(encode_zigzag_64(val));
				}
				break;
			}
			case PROTO_I64:
			case PROTO_U64: {
				uint64_t val = *(uint64_t*) (msg + f->offset);
				if (val) {
					ret += varint_size(f->tag);
					ret += varint_size_64(val);
				}
				break;
			}
			case PROTO_DOUBLE:
			case PROTO_SF64:
			case PROTO_F64: {
				uint64_t val = *(uint64_t*) (msg + f->offset);
				if (val) {
					ret += varint_size(f->tag);
					ret += 8;
				}
				break;
			}
			case PROTO_BYTES:
			case PROTO_LIST_BOOL:
			case PROTO_STRING: {
				pb_bytes_t *bytes = (pb_bytes_t*) (msg + f->offset);
				if (bytes->len) {
					ret += varint_size(f->tag);
					ret += varint_size(bytes->len);
					ret += bytes->len;
				}
				break;
			}
			case PROTO_LIST_ENUM:
			case PROTO_LIST_U32:
			case PROTO_LIST_I32: {
				pb_uint_list *list = (pb_uint_list*) (msg + f->offset);
				if (list->len) {
					ret += varint_size(f->tag);
					int enc = 0;
					for (int i = 0; i < list->len; i++) {
						enc += varint_size(list->v[i]);
					}
					list->_encoded = enc;
					ret += varint_size(enc);
					ret += enc;
				}
				break;
			}
			case PROTO_LIST_S32: {
				pb_int_list *list = (pb_int_list*)(msg + f->offset);
				if (list->len) {
					ret += varint_size(f->tag);
					int enc = 0;
					for (int i = 0; i < list->len; i++) {
						enc += varint_size(encode_zigzag(list->v[i]));
					}
					list->_encoded = enc;
					ret += varint_size(enc);
					ret += enc;
				}
				break;
			}
			case PROTO_LIST_U64:
			case PROTO_LIST_I64: {
				pb_u64_list *list = (pb_u64_list*)(msg + f->offset);
				if (list->len) {
					ret += varint_size(f->tag);
					int enc = 0;
					for (int i = 0; i < list->len; i++) {
						enc += varint_size_64(list->v[i]);
					}
					list->_encoded = enc;
					ret += varint_size(enc);
					ret += enc;
				}
				break;
			}
			case PROTO_LIST_S64: {
				pb_i64_list *list = (pb_i64_list*) (msg + f->offset);
				if (list->len) {
					ret += varint_size(f->tag);
					int enc = 0;
					for (int i = 0; i < list->len; i++) {
						enc += varint_size_64(encode_zigzag_64(list->v[i]));
					}
					list->_encoded = enc;
					ret += varint_size(enc);
					ret += enc;
				}
				break;
			}
			case PROTO_LIST_FLOAT:
			case PROTO_LIST_SF32:
			case PROTO_LIST_F32: {
				pb_float_list *list = (pb_float_list*) (msg + f->offset);
				if (list->len) {
					ret += varint_size(f->tag);
					ret += varint_size(list->len * 4);
					ret += list->len * 4;
				}
				break;
			}
			case PROTO_LIST_F64:
			case PROTO_LIST_SF64:
			case PROTO_LIST_DOUBLE: {
				pb_double_list *list = (pb_double_list*) (msg + f->offset);
				if (list->len) {
					ret += varint_size(f->tag);
					ret += varint_size(list->len * 8);
					ret += list->len * 8;
				}
				break;
			}
			case PROTO_LIST_BYTES:
			case PROTO_LIST_STRING: {
				pb_string_list *list = (pb_string_list*) (msg + f->offset);
				for (int i = 0; i < list->len; i++) {
					ret += varint_size(f->tag);
					ret += varint_size(list->v[i].len);
					ret += list->v[i].len;
				}
				break;
			}
			case PROTO_MESSAGE:
				if (*(char**) (msg + f->offset) == NULL) {
					break;
				}
				// fallthrough
			case PROTO_POD: {
				stack[depth].next_field = f;
				stack[depth].field_end = end;
				stack[depth].msg = msg;
				stack[depth].encoded_size.val = ret;

				if (++depth == MAX_DEPTH) {
					return -1;
				}

				if (f->type == PROTO_POD) {
					msg = msg + f->offset;
				} else {
					msg = *(char**) (msg + f->offset);
				}

				const struct proto_message *ct = (struct proto_message*) f->proto_type;
				f = ct->fields;
				end = f + ct->num_fields;
				ret = 0;
				continue;
			}
			case PROTO_LIST_POD:
			case PROTO_LIST_MESSAGE:
				list_index = 0;
				// fallthrough
			next_message_in_list: {
				struct pb_pod_list *pod = (struct pb_pod_list*) (msg + f->offset);
				if (list_index >= pod->len) {
					break;
				}

				stack[depth].next_field = f;
				stack[depth].field_end = end;
				stack[depth].msg = msg;
				stack[depth].encoded_size.val = ret;
				stack[depth].next_index = list_index + 1;

				if (++depth == MAX_DEPTH) {
					return -1;
				}

				const struct proto_message *ct = (struct proto_message*) f->proto_type;

				if (f->type == PROTO_LIST_POD) {
					msg = pod->data + (list_index * ct->datasz);
				} else {
					struct pb_message_list *list = (struct pb_message_list*) pod;
					msg = (char*) list->u.v[list_index];
				}

				f = ct->fields;
				end = f + ct->num_fields;
				ret = 0;
				continue;
			}
			}
			f++;
		}

		if (!depth) {
			return ret;
		}

		depth--;

		union pb_msg *child = (union pb_msg*) msg;
		int childsz = ret;

		f = stack[depth].next_field;
		end = stack[depth].field_end;
		msg = stack[depth].msg;
		ret = stack[depth].encoded_size.val;

		if (childsz || (f->type == PROTO_LIST_POD || f->type == PROTO_LIST_MESSAGE)) {
			// remove empty non-repeated messages
			ret += varint_size(f->tag);
			ret += varint_size(childsz);
			ret += childsz;
		}

		switch (f->type) {
		case PROTO_LIST_MESSAGE:
			child->encoded_size = childsz;
			// fallthrough
		case PROTO_LIST_POD:
			list_index = stack[depth].next_index;
			goto next_message_in_list;

		case PROTO_MESSAGE:
			child->encoded_size = childsz;
			// fallthrough
		case PROTO_POD:
		default:
			break;
		}

		f++;
	}
}

static uint8_t *put_varint(uint8_t *p, unsigned v) {
	while (v >= 0x80) {
		*(p++) = 0x80 | (uint8_t)v;
		v >>= 7;
	}
	*(p++) = (uint8_t)v;
	return p;
}

static uint8_t *put_varint_64(uint8_t *p, uint64_t v) {
	while (v >= 0x80) {
		*(p++) = 0x80 | (uint8_t)v;
		v >>= 7;
	}
	*(p++) = (uint8_t)v;
	return p;
}

static uint8_t *put_fixed_32(uint8_t *p, uint32_t v) {
	p[0] = (uint8_t)v;
	p[1] = (uint8_t)(v >> 8);
	p[2] = (uint8_t)(v >> 16);
	p[3] = (uint8_t)(v >> 24);
	return p + 4;
}

static uint8_t *put_fixed_64(uint8_t *p, uint64_t v) {
	p[0] = (uint8_t)v;
	p[1] = (uint8_t)(v >> 8);
	p[2] = (uint8_t)(v >> 16);
	p[3] = (uint8_t)(v >> 24);
	p[4] = (uint8_t)(v >> 32);
	p[5] = (uint8_t)(v >> 40);
	p[6] = (uint8_t)(v >> 48);
	p[7] = (uint8_t)(v >> 56);
	return p + 8;
}

int pb_encode(void *obj, const struct proto_message *type, char *data) {
	struct encode_stack stack[MAX_DEPTH];
	int depth = 0;
	const struct proto_field *f = type->fields;
	const struct proto_field *end = f + type->num_fields;
	int list_index = 0;
	char *msg = (char*)obj;
	uint8_t *p = (uint8_t*)data;

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
					p = put_varint(p, f->tag);
					*(p++) = 1;
				}
				break;
			case PROTO_ENUM:
			case PROTO_I32:
			case PROTO_U32: {
				uint32_t val = *(uint32_t*)(msg + f->offset);
				if (val) {
					p = put_varint(p, f->tag);
					p = put_varint(p, val);
				}
				break;
			}
			case PROTO_S32: {
				int32_t val = *(int32_t*)(msg + f->offset);
				if (val) {
					p = put_varint(p, f->tag);
					p = put_varint(p, encode_zigzag(val));
				}
				break;
			}
			case PROTO_FLOAT:
			case PROTO_SF32:
			case PROTO_F32: {
				uint32_t val = *(uint32_t*)(msg + f->offset);
				if (val) {
					p = put_varint(p, f->tag);
					p = put_fixed_32(p, val);
				}
				break;
			}
			case PROTO_S64: {
				int64_t val = *(int64_t*)(msg + f->offset);
				if (val) {
					p = put_varint(p, f->tag);
					p = put_varint_64(p, encode_zigzag_64(val));
				}
				break;
			}
			case PROTO_I64:
			case PROTO_U64:	{
				uint64_t val = *(uint64_t*)(msg + f->offset);
				if (val) {
					p = put_varint(p, f->tag);
					p = put_varint_64(p, val);
				}
				break;
			}
			case PROTO_DOUBLE:
			case PROTO_SF64:
			case PROTO_F64:	{
				uint64_t val = *(uint64_t*)(msg + f->offset);
				if (val) {
					p = put_varint(p, f->tag);
					p = put_fixed_64(p, val);
				}
				break;
			}
			case PROTO_BYTES:
			case PROTO_LIST_BOOL:
			case PROTO_STRING: {
				pb_bytes_t *bytes = (pb_bytes_t*)(msg + f->offset);
				if (bytes->len) {
					p = put_varint(p, f->tag);
					p = put_varint(p, bytes->len);
					memcpy(p, bytes->p, bytes->len);
					p += bytes->len;
				}
				break;
			}
			case PROTO_LIST_ENUM:
			case PROTO_LIST_U32:
			case PROTO_LIST_I32: {
				pb_uint_list *list = (pb_uint_list*)(msg + f->offset);
				if (list->len) {
					p = put_varint(p, f->tag);
					p = put_varint(p, list->_encoded);
					for (int i = 0; i < list->len; i++) {
						p = put_varint(p, list->v[i]);
					}
				}
				break;
			}
			case PROTO_LIST_S32: {
				pb_int_list *list = (pb_int_list*)(msg + f->offset);
				if (list->len) {
					p = put_varint(p, f->tag);
					p = put_varint(p, list->_encoded);
					for (int i = 0; i < list->len; i++) {
						p = put_varint(p, encode_zigzag(list->v[i]));
					}
				}
				break;
			}
			case PROTO_LIST_I64:
			case PROTO_LIST_U64: {
				pb_u64_list *list = (pb_u64_list*)(msg + f->offset);
				if (list->len) {
					p = put_varint(p, f->tag);
					p = put_varint(p, list->_encoded);
					for (int i = 0; i < list->len; i++) {
						p = put_varint_64(p, list->v[i]);
					}
				}
				break;
			}
			case PROTO_LIST_S64: {
				pb_i64_list *list = (pb_i64_list*)(msg + f->offset);
				if (list->len) {
					p = put_varint(p, f->tag);
					p = put_varint(p, list->_encoded);
					for (int i = 0; i < list->len; i++) {
						p = put_varint_64(p, encode_zigzag_64(list->v[i]));
					}
				}
				break;
			}
			case PROTO_LIST_FLOAT:
			case PROTO_LIST_SF32:
			case PROTO_LIST_F32: {
				pb_float_list *list = (pb_float_list*)(msg + f->offset);
				if (list->len) {
					p = put_varint(p, f->tag);
					p = put_varint(p, list->len * 4);
					memcpy(p, list->v, list->len * 4);
					p += list->len * 4;
				}
				break;
			}
			case PROTO_LIST_F64:
			case PROTO_LIST_SF64:
			case PROTO_LIST_DOUBLE: {
				pb_double_list *list = (pb_double_list*)(msg + f->offset);
				if (list->len) {
					p = put_varint(p, f->tag);
					p = put_varint(p, list->len * 8);
					memcpy(p, list->v, list->len * 8);
					p += list->len * 8;
				}
				break;
			}
			case PROTO_LIST_BYTES:
			case PROTO_LIST_STRING: {
				pb_string_list *list = (pb_string_list*)(msg + f->offset);
				for (int i = 0; i < list->len; i++) {
					const pb_string_t *s = &list->v[i];
					p = put_varint(p, f->tag);
					p = put_varint(p, s->len);
					memcpy(p, s->c_str, s->len);
					p += s->len;
				}
				break;
			}
			case PROTO_MESSAGE: {
				union pb_msg *child = *(union pb_msg**) (msg + f->offset);
				if (child == NULL || !child->encoded_size) {
					break;
				}
				p = put_varint(p, f->tag);
				p = put_varint(p, child->encoded_size);

				goto message_common;
			}
			case PROTO_POD:	{
				p = put_varint(p, f->tag);
				stack[depth].encoded_size.ptr = p++;

			message_common:
				stack[depth].next_field = f;
				stack[depth].field_end = end;
				stack[depth].msg = msg;

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
			next_message_in_list: {
				struct pb_message_list *msgs = (struct pb_message_list*) (msg + f->offset);
				if (list_index >= msgs->len) {
					break;
				}

				p = put_varint(p, f->tag);

				if (f->type == PROTO_LIST_POD) {
					stack[depth].encoded_size.ptr = p++;
				} else {
					p = put_varint(p, msgs->u.v[list_index]->encoded_size);
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
			return p - (uint8_t*)data;
		}

		depth--;

		f = stack[depth].next_field;
		end = stack[depth].field_end;
		msg = stack[depth].msg;
		uint8_t *szptr = stack[depth].encoded_size.ptr;

		switch (f->type) {
		case PROTO_LIST_POD:
			*szptr = (uint8_t)(p - szptr - 1);
			// fallthrough
		case PROTO_LIST_MESSAGE:
			list_index = stack[depth].next_index;
			goto next_message_in_list;

		case PROTO_POD:
			if (p == szptr + 1) {
				// remove the empty message
				p = szptr - varint_size(f->tag);
			} else {
				*szptr = (uint8_t)(p - szptr - 1);
			}
			// fallthrough
		case PROTO_MESSAGE:
		default:
			break;
		}

		f++;
	}
}

