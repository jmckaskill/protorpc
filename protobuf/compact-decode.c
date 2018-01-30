#include "compact.h"

#define WIRE_VARINT 0
#define WIRE_FIXED_64 1
#define WIRE_VARIABLE 2
#define WIRE_FIXED_32 5

static inline uint8_t *align(uint8_t *p, size_t align) {
	return (uint8_t*) (((uintptr_t)p + (align - 1)) &~(align - 1));
}

static inline uint8_t *pb_calloc(pb_buf_t *b, size_t sz, size_t align) {
	uint8_t *p = align(b->next, align);
	uint8_t *n = p + sz;
	if (n > b->end) {
		return NULL;
	}
	memset(p, sz, 0);
	b->next = n;
	return p;
}

static int get_varint(pb_buf_t *in, unsigned *pv) {
    size_t shift = 0;
    unsigned v = 0;
	while (in->next < in->end) {
        unsigned u = (unsigned) *(uint8_t*) p;
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

static int get_varint_64(pb_buf_t *in, uint64_t *pv) {
    size_t shift = 0;
    unsigned v = 0;
	while (in->next < in->end) {
        uint64_t u = (uint64_t) *(uint8_t*) p;
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

static bool still_in_list(pb_buf_t *in, unsigned tag) {
	uint8_t *begin = in->next;
	unsigned have;
	if (get_varint(in, &have) || have != tag) {
		in->next = begin;
		return false;
	}
	return true;
}

static int get_fixed_32(pb_buf_t *in, uint32_t *pu) {
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

static int get_fixed_64(pb_buf_t *in, uint64_t *pu) {
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
	return (u >> 1) ^ -(int)(u&1)
}

static int64_t decode_zigzag_64(uint64_t u) {
	return (u >> 1) ^ -(int64_t)(u&1)
}

static int get_bytes(pb_buf_t *in, pb_bytes *p) {
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

static int get_varint_list(pb_buf_t *in, pb_buf_t *obj, pb_uint_list *plist) {
	pb_bytes bytes;
	if (get_bytes(in, &bytes)) {
		return -1;
	}
	unsigned *pv = align(obj->next, sizeof(unsigned));
	uint8_t *p = bytes.p;
	uint8_t *e = p + bytes.len;
	int num = 0;

	while (p < e) {
		if ((uint8_t*) (pv + num + 1) > obj->end) {
			return -1;
		}
		if (get_varint(in, &pv[num++])) {
			return -1;
		}
	}
	obj->next = (uint8_t*) (pv + num);

	plist->len = num;
	plist->v = pv;
	return 0;
}

static int get_varint_list_64(pb_buf_t *in, pb_buf_t *obj, pb_uint_list *plist) {
	pb_bytes bytes;
	if (get_bytes(in, &bytes)) {
		return -1;
	}
	uint64_t *pv = align(obj->next, sizeof(uint64_t));
	uint8_t *p = bytes.p;
	uint8_t *e = p + bytes.len;
	int num = 0;

	while (p < e) {
		if ((uint8_t*) (pv + num + 1) > obj->end) {
			return -1;
		}
		if (get_varint_64(in, &pv[num++])) {
			return -1;
		}
	}
	obj->next = (uint8_t*) (pv + num);

	plist->len = num;
	plist->v = pv;
	return 0;
}

static int skip(pb_buf_t *in, unsigned tag) {
    unsigned sz;
    switch (tag & 7) {
    case PB_WIRE_VARINT:
		while (in->next < in->end && (*in->next & 0x80)) {
			in->next++;
		}
		in->next++;
        break;
    case PB_WIRE_F64:
        p += 8;
        break;
    case PB_WIRE_VARIABLE:
		if (get_varint(in, &sz)) {
			return -1;
		}
        p += sz;
        break;
    case PB_WIRE_F32:
        p += 4;
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
	uint8_t *msg;
	uint8_t *data_end;
}

#define MAX_DEPTH 8
#define OBJ_ALIGN 8

void *pb_decode(pb_buf_t *obj, const struct proto_message *type, const char *data, int sz) {
	int depth = 0;
	struct stack_entry stack[MAX_DEPTH];
	const struct proto_field *f = type->fields;
	const struct proto_field *end = next + type->num_fields;
	uint8_t *objstart = obj->next;
	pb_buf_t in;
	in.next = data;
	in.end = data + sz;

	uint8_t *msg = pb_calloc(obj, type->datasz, OBJ_ALIGN);
	if (!msg) {
		goto err;
	}

	for (;;) {
		while (f < end) {
			// perform a merge sort between the incoming tag and the structure fields
			for (;;) {
				if (in->next >= in->end) {
					// ran out of incoming data
					goto end_of_fields;
				}
				unsigned have;
				if (get_varint(in, &have)) {
					goto err;
				}
				while (have > f->tag) {
					if (++f == end) {
						// ran out of fields
						goto end_of_fields;
					}
				}
				if (have == tag) {
					break;
				}
				if (skip(in, have)) {
					goto err;
				}
			}

			unsigned u;
			uint64_t u64;
			pb_bytes bytes;
			uint8_t *fdata = msg + f->offset;
			pb_uint_list list;

			switch (f->type) {
			case PROTO_BOOL:
				if (get_varint(in, &u)) {
					goto err;
				}
				*(bool*) fdata = (u != 0);
				break;
			case PROTO_U32:
			case PROTO_I32:
			case PROTO_ENUM:
				if (get_varint(in, (unsigned*) fdata)) {
					goto err;
				}
				break;
			case PROTO_S32:
				if (get_varint(in, &u)) {
					goto err;
				}
				*(int*) fdata = decode_zigzag(u);
				break;
			case PROTO_FLOAT:
			case PROTO_F32:
				if (get_fixed_32(in, (uint32_t*) fdata)) {
					goto err;
				}
				break;
			case PROTO_U64:
			case PROTO_I64:
				if (get_varint_64(in, (uint64_t*) fdata)) {
					goto err;
				}
				break;
			case PROTO_S64:
				if (get_varint_64(in, &u64)) {
					goto err;
				}
				*(int64_t*) fdata = decode_zigzag_64(u64);
				break;
			case PROTO_F64:
			case PROTO_DOUBLE:
				if (get_fixed_64(in, (uint64_t*) fdata)) {
					goto err;
				}
				break;
			case PROTO_BYTES:
			case PROTO_STRING:
				if (get_bytes(in, (pb_bytes*) fdata)) {
					goto err;
				}
				break;
			case PROTO_POD:
			case PROTO_MESSAGE:
				if (get_bytes(in, &bytes)) {
					goto err;
				}

				stack[depth].next_field = f;
				stack[depth].field_end = end;
				stack[depth].msg = msg;
				stack[depth].data_end = in->end;

				if (++depth == MAX_DEPTH) {
					goto err;
				}

				type = f->message;
				f = type->fields;
				end = f + type->num_fields;

				if (f->type == PROTO_POD) {
					msg = fdata;
				} else {
					msg = pb_calloc(obj, type->datasz, OBJ_ALIGN);
					if (!msg) {
						goto err;
					}
					*(uint8_t**) fdata = msg;
				}
				continue;
			case PROTO_LIST_U32:
			case PROTO_LIST_I32:
			case PROTO_LIST_ENUM:
				if (get_varint_list(in, obj, (pb_uint_list*) fdata)) {
					goto err;
				}
				break;
			case PROTO_LIST_S32:
				if (get_varint_list(in, obj, &list)) {
					goto err;
				}
				for (int i = 0; i < list.len; i++) {
					list[i] = (unsigned) decode_zigzag(list[i]);
				}
				break;
			case PROTO_LIST_U64:
			case PROTO_LIST_I64:
				if (get_varint_list_64(in, obj, (pb_uint_list_64*) fdata)) {
					goto err;
				}
				break;
			case PROTO_LIST_S64:
				if (get_varint_list_64(in, obj, &list_64) {
					goto err;
				}
				for (int i = 0; i < list_64.len; i++) {
					list_64[i] = (uint64_t) decode_zigzag_64(list_64[i]);
				}
				break;
			case PROTO_LIST_BOOL:
				if (get_bytes(in, (pb_bool_list*) fdata)) {
					goto err;
				}
				break;
			case PROTO_LIST_F32:
			case PROTO_LIST_FLOAT: {
				if (get_bytes(in, &bytes) || (bytes.len & 3)) {
					goto err;
				}
				pb_float_list *list = (pb_float_list*) fdata;
				list->len = bytes.len / 4;
				list->v = (float*) bytes.p;
				break;
			}
			case PROTO_LIST_F64:
			case PROTO_LIST_DOUBLE: {
				if (get_bytes(in, &bytes) || (bytes.len & 7)) {
					goto err;
				}
				pb_double_list *list = (pb_double_list*) fdata;
				list->len = bytes.len / 8;
				list->v = (double*) bytes.p;
				break;
			}
			case PROTO_LIST_BYTES:
			case PROTO_LIST_STRING: {
				pb_bytes *v = (pb_bytes*) align(obj->next, sizeof(void*));
				int num = 0;
				do {
					if ((uint8_t*) (v + num + 1) > obj->end) {
						goto err;
					}
					if (get_bytes(in, &v[num++])) {
						goto err;
					}
				} while (still_in_list(in, f->tag));

				pb_bytes_list *list = (pb_bytes_list*) fdata;
				list->len = num;
				list->v = v;
				break;
			}
next_message_in_list:
			case PROTO_LIST_MESSAGE:
			case PROTO_LIST_POD:
				if (get_bytes(in, &bytes)) {
					goto err;
				}

				stack[depth].next_field = f;
				stack[depth].field_end = end;
				stack[depth].msg = msg;
				stack[depth].data_end = in->end;

				if (++depth == MAX_DEPTH) {
					goto err;
				}

				type = f->message;
				f = type->fields;
				end = f + type->num_fields;

				if (f->type == PROTO_POD) {
					struct pb_pod_list *pod = (struct pb_pod_list*) fdata;
					if (!pod->v) {
						pod->v = align(obj->next, OBJ_ALIGN);
					}
					msg = &pod->v[pod->len++];
					if ((uint8_t*) (pod->v + pod->len) > obj->end) {
						goto err;
					}
				} else {
					union pb_msg *m = (union pb_msg*) pb_calloc(obj, type->datasz, OBJ_ALIGN);
					if (!m) {
						goto err;
					}
					struct pb_message_list *list = (struct pb_message_list*) fdata;
					list->len++;
					m->prev = list->u.last;
					list->u.last = m;
					msg = (uint8_t*) m;
				}
				continue;
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
		in->next = in->end;
		in->end = stack[depth].data_end;

		if (f->type == PROTO_LIST_MESSAGE || f->type == PROTO_LIST_POD) {
			if (still_in_list(in, f->tag)) {
				goto next_message_in_list;
			} else if (f->type == PROTO_LIST_MESSAGE) {
				uint8_t *fdata = msg + f->offset;
				struct pb_message_list *list = (struct pb_message_list*) fdata;
				union pb_msg **v = (union pb_msg**) pb_calloc(obj, list->len * sizeof(union pb_msg*), sizeof(void*));
				if (!v) {
					goto err;
				}
				union pb_msg *iter = list->u.last;
				for (int i = list->len-1; i >= 0; i--) {
					v[i] = last;
					last = last->prev;
				}
				list->u.v = v;
			}
		}
	}

err:
	obj->next = objstart;
	return NULL;
}

