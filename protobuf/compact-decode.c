#include "compact.h"
#include <string.h>

#define WIRE_VARINT 0
#define WIRE_FIXED_64 1
#define WIRE_VARIABLE 2
#define WIRE_FIXED_32 5

struct in {
	const uint8_t *next;
	const uint8_t *end;
};

static inline char *align(char *p, size_t align) {
	return (char*) (((uintptr_t)p + (align - 1)) &~(align - 1));
}

static inline char *buf_calloc(pb_buf_t *b, size_t sz, size_t alignment) {
	char *p = align(b->next, alignment);
	char *n = p + sz;
	if (n > b->end) {
		return NULL;
	}
	memset(p, 0, sz);
	b->next = n;
	return p;
}

static int get_varint(struct in *in, unsigned *pv) {
    size_t shift = 0;
    unsigned v = 0;
	while (in->next < in->end) {
        unsigned u = (unsigned) *in->next;
		in->next++;
        v |= (u & 0x7F) << shift;
        if (!(u & 0x80)) {
			*pv = v;
            return 0;
        }
        shift += 7;
    }
	return -1;
}

static int get_varint_64(struct in *in, uint64_t *pv) {
    size_t shift = 0;
    uint64_t v = 0;
	while (in->next < in->end) {
		uint64_t u = (uint64_t) *in->next;
		in->next++;
        v |= (u & 0x7F) << shift;
        if (!(u & 0x80)) {
			*pv = v;
            return 0;
        }
        shift += 7;
    }
	return -1;
}

static bool still_in_list(struct in *in, unsigned tag) {
	const uint8_t *begin = in->next;
	unsigned have;
	if (get_varint(in, &have) || have != tag) {
		in->next = begin;
		return false;
	}
	return true;
}

static int get_fixed_32(struct in *in, uint32_t *pu) {
	if (in->next + 4 > in->end) {
		return -1;
	}
	*pu = ((uint32_t) in->next[0])
		| (((uint32_t) in->next[1]) << 8)
		| (((uint32_t) in->next[2]) << 16)
		| (((uint32_t) in->next[3]) << 24);
	in->next += 4;
	return 0;
}

static int get_fixed_64(struct in *in, uint64_t *pu) {
	if (in->next + 8 > in->end) {
		return -1;
	}
	*pu = ((uint64_t) in->next[0])
		| (((uint64_t) in->next[1]) << 8)
		| (((uint64_t) in->next[2]) << 16)
		| (((uint64_t) in->next[3]) << 24)
		| (((uint64_t) in->next[4]) << 32)
		| (((uint64_t) in->next[5]) << 40)
		| (((uint64_t) in->next[6]) << 48)
		| (((uint64_t) in->next[7]) << 56);
	in->next += 8;
	return 0;
}

static int decode_zigzag(unsigned u) {
	return (u >> 1) ^ -(int)(u & 1);
}

static int64_t decode_zigzag_64(uint64_t u) {
	return (u >> 1) ^ -(int64_t)(u & 1);
}

static int get_bytes(struct in *in, pb_bytes_t *p) {
	unsigned len;
	if (get_varint(in, &len)) {
		return -1;
	}
	if (in->next + len > in->end) {
		return -1;
	}
	p->p = in->next;
	p->len = len;
	in->next += len;
	return 0;
}

static int get_varint_list(struct in *in, pb_buf_t *obj, pb_uint_list *plist) {
	pb_bytes_t bytes;
	if (get_bytes(in, &bytes)) {
		return -1;
	}
	unsigned *pv = (unsigned*) align(obj->next, sizeof(unsigned));
	struct in data;
	data.next = bytes.p;
	data.end = data.next + bytes.len;
	int num = 0;

	while (data.next < data.end) {
		if ((char*) (pv + num + 1) > obj->end) {
			return -1;
		}
		if (get_varint(&data, &pv[num++])) {
			return -1;
		}
	}
	obj->next = (char*) (pv + num);

	plist->len = num;
	plist->v = pv;
	return 0;
}

static int get_varint_list_64(struct in *in, pb_buf_t *obj, pb_u64_list *plist) {
	pb_bytes_t bytes;
	if (get_bytes(in, &bytes)) {
		return -1;
	}
	uint64_t *pv = (uint64_t*) align(obj->next, sizeof(uint64_t));
	struct in data;
	data.next = bytes.p;
	data.end = data.next + bytes.len;
	int num = 0;

	while (data.next < data.end) {
		if ((char*) (pv + num + 1) > obj->end) {
			return -1;
		}
		if (get_varint_64(&data, &pv[num++])) {
			return -1;
		}
	}
	obj->next = (char*) (pv + num);

	plist->len = num;
	plist->v = pv;
	return 0;
}

static int skip(struct in *in, unsigned tag) {
    unsigned sz;
    switch (tag & 7) {
    case WIRE_VARINT:
		while (in->next < in->end && (*in->next & 0x80)) {
			in->next++;
		}
		in->next++;
        break;
	case WIRE_FIXED_64:
        in->next += 8;
        break;
    case WIRE_VARIABLE:
		if (get_varint(in, &sz)) {
			return -1;
		}
        in->next += sz;
        break;
	case WIRE_FIXED_32:
        in->next += 4;
        break;
    default:
		return -1;
    }
    if (in->next > in->end) {
		return -1;
    }
    return 0;
}

struct stack_entry {
	const struct proto_field *next_field;
	const struct proto_field *field_end;
	char *msg;
	const uint8_t *data_end;
	int next_index;
};

#define MAX_DEPTH 8
#define OBJ_ALIGN 8

void *pb_decode(pb_buf_t *obj, const struct proto_message *type, const char *data, int sz) {
	int depth = 0;
	struct stack_entry stack[MAX_DEPTH];
	const struct proto_field *f = type->fields;
	const struct proto_field *end = f + type->num_fields;
	char *objstart = obj->next;
	struct in in;
	in.next = (const uint8_t*) data;
	in.end = in.next + sz;

	char *msg = buf_calloc(obj, type->datasz, OBJ_ALIGN);
	if (!msg) {
		goto err;
	}

	for (;;) {
		while (f < end) {
			// perform a merge sort between the incoming tag and the structure fields
			for (;;) {
				if (in.next >= in.end) {
					// ran out of incoming data
					goto end_of_fields;
				}
				unsigned have;
				if (get_varint(&in, &have)) {
					goto err;
				}
				while (have > f->tag) {
					if (++f == end) {
						// ran out of fields
						goto end_of_fields;
					}
				}
				if (have == f->tag) {
					break;
				}
				if (skip(&in, have)) {
					goto err;
				}
			}

			if (f->oneof >= 0) {
				// bottom three bits of the tag is the wire type, we just want the field number
				unsigned* oneof = (unsigned*)(msg + f->oneof);
				if (*oneof) {
					// multiple fields from the same union in the message
					goto err;
				}
				*oneof = (f->tag >> 3);
			}

			unsigned u;
			uint64_t u64;
			pb_bytes_t bytes;

			switch (f->type) {
			case PROTO_BOOL:
				if (get_varint(&in, &u)) {
					goto err;
				}
				*(bool*)(msg + f->offset) = (u != 0);
				break;
			case PROTO_U32:
			case PROTO_I32:
			case PROTO_ENUM:
				if (get_varint(&in, (unsigned*)(msg + f->offset))) {
					goto err;
				}
				break;
			case PROTO_S32:
				if (get_varint(&in, &u)) {
					goto err;
				}
				*(int*)(msg + f->offset) = decode_zigzag(u);
				break;
			case PROTO_FLOAT:
			case PROTO_F32:
			case PROTO_SF32:
				if (get_fixed_32(&in, (uint32_t*)(msg + f->offset))) {
					goto err;
				}
				break;
			case PROTO_U64:
			case PROTO_I64:
				if (get_varint_64(&in, (uint64_t*)(msg + f->offset))) {
					goto err;
				}
				break;
			case PROTO_S64:
				if (get_varint_64(&in, &u64)) {
					goto err;
				}
				*(int64_t*)(msg + f->offset) = decode_zigzag_64(u64);
				break;
			case PROTO_SF64:
			case PROTO_F64:
			case PROTO_DOUBLE:
				if (get_fixed_64(&in, (uint64_t*)(msg + f->offset))) {
					goto err;
				}
				break;
			case PROTO_BYTES:
			case PROTO_STRING:
				if (get_bytes(&in, (pb_bytes_t*)(msg + f->offset))) {
					goto err;
				}
				break;
			case PROTO_POD:
			case PROTO_MESSAGE: {
				if (get_bytes(&in, &bytes)) {
					goto err;
				}

				stack[depth].next_field = f+1;
				stack[depth].field_end = end;
				stack[depth].msg = msg;
				stack[depth].data_end = in.end;

				if (++depth == MAX_DEPTH) {
					goto err;
				}

				const struct proto_message *ct = f->message;

				if (f->type == PROTO_POD) {
					msg = (msg + f->offset);
				} else {
					char *child = buf_calloc(obj, ct->datasz, OBJ_ALIGN);
					if (!child) {
						goto err;
					}
					*(char**)(msg + f->offset) = child;
					msg = child;
				}

				in.next = bytes.p;
				in.end = bytes.p + bytes.len;
				f = ct->fields;
				end = f + ct->num_fields;
				continue;
			}
			case PROTO_LIST_U32:
			case PROTO_LIST_I32:
			case PROTO_LIST_ENUM:
				if (get_varint_list(&in, obj, (pb_uint_list*) (msg + f->offset))) {
					goto err;
				}
				break;
			case PROTO_LIST_S32:{
				pb_uint_list *list = (pb_uint_list*) (msg + f->offset);
				if (get_varint_list(&in, obj, list)) {
					goto err;
				}
				int32_t *iv = (int32_t*)list->v;
				for (int i = 0; i < list->len; i++) {
					iv[i] = decode_zigzag(list->v[i]);
				}
				break;
			}
			case PROTO_LIST_U64:
			case PROTO_LIST_I64:
				if (get_varint_list_64(&in, obj, (pb_u64_list*) (msg + f->offset))) {
					goto err;
				}
				break;
			case PROTO_LIST_S64:{
				pb_u64_list *list = (pb_u64_list*)(msg + f->offset);
				if (get_varint_list_64(&in, obj, list)) {
					goto err;
				}
				int64_t *iv = (int64_t*)list->v;
				for (int i = 0; i < list->len; i++) {
					iv[i] = decode_zigzag_64(list->v[i]);
				}
				break;
			}
			case PROTO_LIST_BOOL:
				if (get_bytes(&in, (pb_bytes_t*) (msg + f->offset))) {
					goto err;
				}
				break;
			case PROTO_LIST_F32:
			case PROTO_LIST_SF32:
			case PROTO_LIST_FLOAT: {
				if (get_bytes(&in, &bytes) || (bytes.len & 3)) {
					goto err;
				}
				pb_float_list *list = (pb_float_list*) (msg + f->offset);
				list->len = bytes.len / 4;
				list->v = (float*) bytes.p;
				break;
			}
			case PROTO_LIST_F64:
			case PROTO_LIST_SF64:
			case PROTO_LIST_DOUBLE: {
				if (get_bytes(&in, &bytes) || (bytes.len & 7)) {
					goto err;
				}
				pb_double_list *list = (pb_double_list*) (msg + f->offset);
				list->len = bytes.len / 8;
				list->v = (double*) bytes.p;
				break;
			}
			case PROTO_LIST_BYTES:
			case PROTO_LIST_STRING: {
				pb_bytes_t *v = (pb_bytes_t*) align(obj->next, sizeof(void*));
				int num = 0;
				do {
					if ((char*) (v + num + 1) > obj->end) {
						goto err;
					}
					if (get_bytes(&in, &v[num++])) {
						goto err;
					}
				} while (still_in_list(&in, f->tag));

				obj->next = (char*)(v + num);

				pb_bytes_list *list = (pb_bytes_list*) (msg + f->offset);
				list->len = num;
				list->v = v;
				break;
			}
next_message_in_list:
			case PROTO_LIST_MESSAGE:
			case PROTO_LIST_POD: {
				if (get_bytes(&in, &bytes)) {
					goto err;
				}

				stack[depth].next_field = f;
				stack[depth].field_end = end;
				stack[depth].msg = msg;
				stack[depth].data_end = in.end;

				if (++depth == MAX_DEPTH) {
					goto err;
				}

				const struct proto_message *ct = f->message;

				if (f->type == PROTO_LIST_POD) {
					struct pb_pod_list *list = (struct pb_pod_list*) (msg + f->offset);
					if (!list->data) {
						list->data = align(obj->next, OBJ_ALIGN);
					}
					msg = list->data + (list->len * ct->datasz);
					memset(msg, 0, ct->datasz);
					list->len++;
					if (list->data + (list->len * ct->datasz) > obj->end) {
						goto err;
					}
				} else {
					union pb_msg *child = (union pb_msg*) buf_calloc(obj, ct->datasz, OBJ_ALIGN);
					if (!child) {
						goto err;
					}
					struct pb_message_list *list = (struct pb_message_list*) (msg + f->offset);
					list->len++;
					child->prev = list->u.last;
					list->u.last = child;
					msg = (char*)child;
				}

				in.next = bytes.p;
				in.end = bytes.p + bytes.len;
				f = ct->fields;
				end = f + ct->num_fields;
				continue;
			}
			default:
				goto err;
			}

			f++;
		}
		
end_of_fields:
		if (!depth) {
			return msg;
		}

		depth--;
		msg = stack[depth].msg;
		f = stack[depth].next_field;
		end = stack[depth].field_end;
		in.next = in.end;
		in.end = stack[depth].data_end;

		if (f->type == PROTO_LIST_MESSAGE || f->type == PROTO_LIST_POD) {
			if (still_in_list(&in, f->tag)) {
				goto next_message_in_list;
			}

			// we've finished the list, we should commit it
			if (f->type == PROTO_LIST_POD) {
				struct pb_pod_list *list = (struct pb_pod_list*) (msg + f->offset);
				obj->next = list->data + (list->len * f->message->datasz);
			} else {
				struct pb_message_list *list = (struct pb_message_list*) (msg + f->offset);
				union pb_msg **v = (union pb_msg**) buf_calloc(obj, list->len * sizeof(union pb_msg*), sizeof(void*));
				if (!v) {
					goto err;
				}
				union pb_msg *iter = list->u.last;
				for (int i = list->len-1; i >= 0; i--) {
					v[i] = iter;
					iter = iter->prev;
				}
				list->u.v = v;
			}

			f++;
		}
	}

err:
	obj->next = objstart;
	return NULL;
}


void pb_terminate(void *obj, const struct proto_message *type, char *data, int sz) {
	char *msg = (char*)obj;
	int depth = 0;
	struct stack_entry stack[MAX_DEPTH];
	const struct proto_field *f = type->fields;
	const struct proto_field *end = f + type->num_fields;
	int list_index = 0;

	// If the incoming message ends in a string, then terminating that string
	// will result in a zero being written one byte after the message data.
	// Rather then require that everyone test that corner case, we will always
	// write a trailing terminator to make sure the user can handle it.

	data[sz] = '\0';

	for (;;) {
		while (f < end) {
			if (f->oneof >= 0) {
				// oneof type is set to the field number
				// bottom three bits of the tag is the wire type
				unsigned *oneof = (unsigned*)(msg + f->oneof);
				if (*oneof != (f->tag >> 3)) {
					continue;
				}
			}
			switch (f->type) {
			case PROTO_BYTES:
			case PROTO_STRING: {
				pb_string_t *str = (pb_string_t *)(msg + f->offset);
				char *p = (char*)str->c_str;
				if (p) {
					p[str->len] = '\0';
				} else {
					str->c_str = "";
				}
				break;
			}
			case PROTO_LIST_STRING:
			case PROTO_LIST_BYTES: {
				pb_string_list *list = (pb_string_list *)(msg + f->offset);
				for (int i = 0; i < list->len; i++) {
					pb_string_t *str = (pb_string_t*) &list->v[i];
					char *p = (char*)str->c_str;
					if (p) {
						p[str->len] = '\0';
					} else {
						str->c_str = "";
					}
				}
				break;
			}
			case PROTO_MESSAGE:{
				char *child = *(char**)(msg + f->offset);
				if (!child) {
					break;
				}

				stack[depth].field_end = end;
				stack[depth].next_field = f + 1;
				stack[depth].msg = msg;

				if (++depth == MAX_DEPTH) {
					return;
				}

				const struct proto_message *ct = f->message;
				f = ct->fields;
				end = f + ct->num_fields;
				msg = child;
				continue;
			}
			case PROTO_LIST_MESSAGE:
				list_index = 0;
				goto next_list_message;
			next_list_message: {
				struct pb_message_list *list = (struct pb_message_list*) (msg + f->offset);
				if (list_index >= list->len) {
					break;
				}

				stack[depth].field_end = end;
				stack[depth].next_field = f;
				stack[depth].msg = msg;
				stack[depth].next_index = list_index + 1;

				if (++depth == MAX_DEPTH) {
					return;
				}

				const struct proto_message *ct = f->message;
				f = ct->fields;
				end = f + ct->num_fields;
				msg = (char*) list->u.v[list_index];
				continue;
			}
			default:
				break;
			}

			f++;
		}

		if (!depth) {
			return;
		}

		depth--;

		f = stack[depth].next_field;
		end = stack[depth].field_end;
		msg = stack[depth].msg;

		if (f->type == PROTO_LIST_MESSAGE) {
			list_index = stack[depth].next_index;
			goto next_list_message;
		}
	}
}
