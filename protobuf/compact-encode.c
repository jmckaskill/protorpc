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
	int encoded_size;
};

static int varint_size(unsigned val) {
	if (val < 0x80) {
		return 1;
	} else if (val < 0x4000) {
		return 2;
	} else if (val < 0x200000) {
		return 3;
	} else {
		return 4;
	}
}

static int varint_size_64(uint64_t val) {
	int ret = 0;
	while (val) {
		val >>= 7;
		ret += 1;
	}
	return ret;
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
			switch (f->type) {
			case PROTO_BOOL:
				if (*(bool*) (msg + f->offset)) {
					ret += varint_size(f->tag);
					ret += 1;
				}
				break;
			case PROTO_ENUM:
			case PROTO_S32:
			case PROTO_I32:
			case PROTO_U32: {
				uint32_t val = *(uint32_t*) (msg + f->offset);
				if (val) {
					ret += varint_size(f->tag);
					ret += varint_size(val);
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
			case PROTO_S64:
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
			case PROTO_LIST_S32:
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
			case PROTO_LIST_U64:
			case PROTO_LIST_I64:
			case PROTO_LIST_S64: {
				pb_u64_list *list = (pb_u64_list*) (msg + f->offset);
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
					ret += varint_size(list->len * 4);
					ret += list->len * 4;
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
				stack[depth].encoded_size = ret;

				if (++depth == MAX_DEPTH) {
					return -1;
				}

				if (f->type == PROTO_POD) {
					msg = msg + f->offset;
				} else {
					msg = *(char**) (msg + f->offset);
				}

				const struct proto_message *ct = f->message;
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
				stack[depth].encoded_size = ret;
				stack[depth].next_index = list_index + 1;

				if (++depth == MAX_DEPTH) {
					return -1;
				}

				const struct proto_message *ct = f->message;

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

		f = stack[depth].next_field;
		end = stack[depth].field_end;
		msg = stack[depth].msg;
		int enc = stack[depth].encoded_size;

		ret += varint_size(f->tag);
		ret += varint_size(enc);
		ret += enc;

		switch (f->type) {
		case PROTO_LIST_MESSAGE:
			child->maxsz = enc;
			// fallthrough
		case PROTO_LIST_POD:
			list_index = stack[depth].next_index;
			goto next_message_in_list;

		case PROTO_MESSAGE:
			child->maxsz = enc;
			// fallthrough
		case PROTO_POD:
		default:
			break;
		}

		f++;
	}

}
