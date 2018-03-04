#include "protoc-gen-protorpc.h"
#include <protorpc/ptr-vector.h>
#include <protorpc/ptr-array.h>
#include <math.h>
#include <assert.h>

static ptr_vector all_types;

int compare_string(const char *sa, int salen, const char *sb, int sblen) {
	int diff = salen - sblen;
	if (!diff) {
		diff = memcmp(sa, sb, salen);
	}
	return diff;
}

static int compare_proto_type(const void **a, const void **b) {
	proto_type *ta = *(proto_type**)a;
	proto_type *tb = *(proto_type**)b;
	return compare_string(ta->proto_type.c_str, ta->proto_type.len, tb->proto_type.c_str, tb->proto_type.len);
}

static int lookup_proto_type(const void *key, const void **pt) {
	pb_string *str = (pb_string*)key;
	proto_type *t = *(proto_type**)pt;
	return compare_string(str->c_str, str->len, t->proto_type.c_str, t->proto_type.len);
}

proto_type **get_all_types(int *pnum) {
	*pnum = all_types.len;
	return (proto_type**) all_types.v;
}

proto_type *get_type(pb_string type_name) {
	return (proto_type*)pa_search(all_types, &lookup_proto_type, &type_name);
}

#define X(str) STR_STATIC(str)

static proto_type basic_types[19] = {
	{ 0 }, // errors
	{ X(""), X("double"),	X("pb_double_list"), 8, true}, // TYPE_DOUBLE
	{ X(""), X("float"),	X("pb_float_list"), 4, true }, // TYPE_FLOAT
	{ X(""), X("int64_t"),	X("pb_i64_list"), 10, true }, // TYPE_INT64
	{ X(""), X("uint64_t"), X("pb_u64_list"), 10, true }, // TYPE_UINT64
	{ X(""), X("int"),		X("pb_i32_list"), 5, true }, // TYPE_INT32
	{ X(""), X("uint64_t"), X("pb_u64_list"), 8, true }, // TYPE_FIXED64
	{ X(""), X("uint32_t"), X("pb_u32_list"), 4, true }, // TYPE_FIXED32
	{ X(""), X("bool"),		X("pb_bool_list"), 1, true }, // TYPE_BOOL
	{ X(""), X("pb_string"),X("pb_string_list"), INFINITY, true }, // TYPE_STRING
	{ 0 }, // TYPE_GROUP
	{ 0 }, // TYPE_MESSAGE
	{ X(""), X("pb_bytes"), X("pb_bytes_list"), INFINITY, true }, // TYPE_BYTES
	{ X(""), X("unsigned"), X("pb_u32_list"), 5, true }, // TYPE_UINT32
	{ 0 }, // TYPE_ENUM
	{ X(""), X("int32_t"),	X("pb_i32_list"), 4, true }, // TYPE_SFIXED32
	{ X(""), X("int64_t"),	X("pb_i64_list"), 8, true }, // TYPE_SFIXED64
	{ X(""), X("int"),		X("pb_i32_list"), 5, true }, // TYPE_SINT32
	{ X(""), X("int64_t"),	X("pb_i64_list"), 10, true }, // TYPE_SINT64
};

proto_type *get_field_type(const FieldDescriptorProto *f) {
	proto_type *ret = NULL;
	if (f->type == TYPE_MESSAGE || f->type == TYPE_ENUM) {
		ret = get_type(f->type_name);
	} else if (f->type <= TYPE_SINT64) {
		ret = &basic_types[f->type];
	}
	assert(ret != NULL);
	return ret;
}

static float get_varint_size(uint32_t v) {
	if (v < 0x80) {
		return 1;
	} else if (v < 0x4000) {
		return 2;
	} else if (v < 0x200000) {
		return 3;
	} else if (v < 0x10000000) {
		return 4;
	} else {
		return 5;
	}
}

static float compute_proto_size(proto_type *t) {
	if (t->max_proto_size_calculated) {
		return t->max_proto_size;
	}

	float sz = 0;
	// start with infinity to handle recursive types
	t->max_proto_size_calculated = true;
	t->max_proto_size = INFINITY;

	for (FieldDescriptorProto *f = t->msg->field.first; f != NULL && isfinite(sz); f = f->_next) {
		proto_type *ft = get_field_type(f);
		if (f->label == LABEL_REPEATED) {
			sz = INFINITY;
		} else {
			sz += get_varint_size(f->number << 3);
			sz += compute_proto_size(ft);
		}
	}

	if (isfinite(sz)) {
		if (sz == 0) {
			t->is_empty = true;
		}
		sz += get_varint_size((uint32_t)sz);
		if (sz <= 32) {
			t->is_pod = true;
		}
	}

	t->max_proto_size = sz;
	return sz;
}

void finish_types() {
	pa_sort(all_types, &compare_proto_type);
	for (int i = 0; i < all_types.len; i++) {
		compute_proto_size((proto_type*)all_types.v[i]);
	}
}

static proto_type *create_type(const str_t *proto, const str_t *c) {
	proto_type *t = (proto_type*)calloc(1, sizeof(proto_type));
	str_init(&t->proto_type);
	str_init(&t->c_type);
	str_init(&t->list_type);
	str_setstr(&t->proto_type, *proto);
	str_setstr(&t->c_type, *c);
	pv_append(&all_types, t);
	return t;
}

static void join(str_t *c, pb_string tail, char joinchar) {
	if (c->len && tail.len && c->c_str[c->len-1] != joinchar) {
		str_addch(c, joinchar);
	}
	str_addstr(c, tail);
}

static void insert_service(const FileDescriptorProto *file, ServiceDescriptorProto *s, str_t *proto, str_t *c) {
	size_t protobase = proto->len;
	size_t cbase = c->len;
	join(proto, s->name, '.');
	join(c, s->name, '_');

	proto_type *t = create_type(proto, c);
	t->max_proto_size_calculated = true;
	t->max_proto_size = INFINITY;
	t->svc = s;
	t->file = file;

	str_setlen(proto, protobase);
	str_setlen(c, cbase);
}

static void insert_enum(const FileDescriptorProto *file, EnumDescriptorProto *en, str_t *proto, str_t *c) {
	size_t protobase = proto->len;
	size_t cbase = c->len;
	join(proto, en->name, '.');
	join(c, en->name, '_');

	proto_type *t = create_type(proto, c);
	t->max_proto_size_calculated = true;
	t->max_proto_size = 5;
	t->en = en;
	t->file = file;

	str_setlen(proto, protobase);
	str_setlen(c, cbase);
}

static void insert_message(const FileDescriptorProto *file, DescriptorProto *m, str_t *proto, str_t *c) {
	size_t protobase = proto->len;
	size_t cbase = c->len;
	join(proto, m->name, '.');
	join(c, m->name, '_');

	proto_type *t = create_type(proto, c);
	t->msg = m;
	t->file = file;

	for (DescriptorProto *n = m->nested_type.first; n != NULL; n = n->_next) {
		insert_message(file, n, proto, c);
	}
	for (EnumDescriptorProto *en = m->enum_type.first; en != NULL; en = en->_next) {
		insert_enum(file, en, proto, c);
	}

	str_setlen(proto, protobase);
	str_setlen(c, cbase);
}

void insert_file_types(const FileDescriptorProto *file) {
	static str_t proto = STR_INIT;
	static str_t c = STR_INIT;

	str_clear(&c);
	str_set(&proto, ".");
	str_addstr(&proto, file->package);

	for (DescriptorProto *m = file->message_type.first; m != NULL; m = m->_next) {
		insert_message(file, m, &proto, &c);
	}

	for (EnumDescriptorProto *en = file->enum_type.first; en != NULL; en = en->_next) {
		insert_enum(file, en, &proto, &c);
	}

	for (ServiceDescriptorProto *s = file->service.first; s != NULL; s = s->_next) {
		insert_service(file, s, &proto, &c);
	}
}

#define WIRE_VARINT 0
#define WIRE_FIXED_64 1
#define WIRE_VARIABLE 2
#define WIRE_FIXED_32 5

static uint32_t get_wire_type(const struct FieldDescriptorProto *f) {
	if (f->label == LABEL_REPEATED) {
		return WIRE_VARIABLE;
	}
	switch (f->type) {
	case TYPE_FIXED32:
	case TYPE_SFIXED32:
	case TYPE_FLOAT:
		return WIRE_FIXED_32;
	case TYPE_FIXED64:
	case TYPE_SFIXED64:
	case TYPE_DOUBLE:
		return WIRE_FIXED_64;
	case TYPE_STRING:
	case TYPE_BYTES:
	case TYPE_MESSAGE:
		return WIRE_VARIABLE;
	default:
		return WIRE_VARINT;
	}
}

uint32_t get_tag(const struct FieldDescriptorProto *f) {
	return get_wire_type(f) | (f->number << 3);
}

const char *field_decode_type(const FieldDescriptorProto *f, const proto_type *ft) {
	if (f->label == LABEL_REPEATED) {
		if (ft->is_pod) {
			return "PROTO_LIST_POD";
		} else {
			switch (f->type) {
			case TYPE_DOUBLE:
				return "PROTO_LIST_DOUBLE";
			case TYPE_FLOAT:
				return "PROTO_LIST_FLOAT";
			case TYPE_INT64:
				return "PROTO_LIST_I64";
			case TYPE_UINT64:
				return "PROTO_LIST_U64";
			case TYPE_INT32:
				return "PROTO_LIST_I32";
			case TYPE_FIXED64:
				return "PROTO_LIST_F64";
			case TYPE_FIXED32:
				return "PROTO_LIST_F32";
			case TYPE_BOOL:
				return "PROTO_LIST_BOOL";
			case TYPE_STRING:
				return "PROTO_LIST_STRING";
			case TYPE_MESSAGE:
				return "PROTO_LIST_MESSAGE";
			case TYPE_BYTES:
				return "PROTO_LIST_BYTES";
			case TYPE_UINT32:
				return "PROTO_LIST_U32";
			case TYPE_ENUM:
				return "PROTO_LIST_ENUM";
			case TYPE_SFIXED32:
				return "PROTO_LIST_SF32";
			case TYPE_SFIXED64:
				return "PROTO_LIST_SF64";
			case TYPE_SINT32:
				return "PROTO_LIST_S32";
			case TYPE_SINT64:
				return "PROTO_LIST_S64";
			default:
				return "";
			}
		}
	} else {
		if (ft->is_pod) {
			return "PROTO_POD";
		} else {
			switch (f->type) {
			case TYPE_DOUBLE:
				return "PROTO_DOUBLE";
			case TYPE_FLOAT:
				return "PROTO_FLOAT";
			case TYPE_INT64:
				return "PROTO_I64";
			case TYPE_UINT64:
				return "PROTO_U64";
			case TYPE_INT32:
				return "PROTO_I32";
			case TYPE_FIXED64:
				return "PROTO_F64";
			case TYPE_FIXED32:
				return "PROTO_F32";
			case TYPE_BOOL:
				return "PROTO_BOOL";
			case TYPE_STRING:
				return "PROTO_STRING";
			case TYPE_MESSAGE:
				return "PROTO_MESSAGE";
			case TYPE_BYTES:
				return "PROTO_BYTES";
			case TYPE_UINT32:
				return "PROTO_U32";
			case TYPE_ENUM:
				return "PROTO_ENUM";
			case TYPE_SFIXED32:
				return "PROTO_SF32";
			case TYPE_SFIXED64:
				return "PROTO_SF64";
			case TYPE_SINT32:
				return "PROTO_S32";
			case TYPE_SINT64:
				return "PROTO_S64";
			default:
				return "";
			}
		}
	}
}






