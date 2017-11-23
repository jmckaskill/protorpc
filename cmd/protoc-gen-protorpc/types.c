#include "protoc-gen-protorpc.h"

struct {map_t h; slice_t *keys; struct type **values;} g_named_types;
struct {map_t h; const struct DescriptorProto **keys; struct type **values;} g_struct_types;
struct {map_t h; const struct EnumDescriptorProto **keys; struct type **values;} g_enum_types;
struct {map_t h; const struct ServiceDescriptorProto **keys; struct type **values;} g_service_types;

static void join_underscore(str_t *c, struct pb_string tail) {
    if (c->len) {
        str_addch(c, '_');
	}
    str_addstr(c, tail);
}

static void insert_service(const struct ServiceDescriptorProto *svc, str_t *proto, str_t *c) {
    str_add(proto, ".");
    str_addstr(proto, svc->name);
	join_underscore(c, svc->name);
	struct type *t = NEW(struct type);
    str_addstr(&t->name, *proto);
    str_addstr(&t->c_type, *c);
    str_addstr(&t->proto_suffix, *c);
	str_addstr(&t->json_suffix, *c);
    str_addstr(&t->proto_type, *proto);
	t->svc = svc;

    imap_set(&g_service_types, svc, t);
}

static void insert_enum(const struct EnumDescriptorProto *en, str_t *proto, str_t *c) {
    str_add(proto, ".");
    str_addstr(proto, en->name);
	join_underscore(c, en->name);

    struct type *t = NEW(struct type);
    str_addstr(&t->name, *proto);
    str_add(&t->c_type, "enum ");
    str_addstr(&t->c_type, *c);
    str_add(&t->proto_suffix, "u32");
	str_addstr(&t->json_suffix, *c);
    t->en = en;
    t->max_proto_size = 5;
    str_add(&t->proto_type, "uint32_t");
    t->max_print_size = 0;
    for (int i = 0; i < en->value.len; i++) {
        if (en->value.v[i]->name.len > t->max_print_size) {
            t->max_print_size = (float) (en->value.v[i]->name.len);
        }
    }
    t->max_print_size += 4; // "ENUM_VALUE",\n ... "",\n are the extra four chars

    imap_set(&g_enum_types, en, t);
    smap_set(&g_named_types, t->name.buf, t->name.len, t);
}

static void insert_message(const struct DescriptorProto *msg, str_t *proto, str_t *c, bool default_packed, const struct FileDescriptorProto *file) {
    str_add(proto, ".");
    str_addstr(proto, msg->name);
	join_underscore(c, msg->name);

    int protosz = proto->len;
    int csz = c->len;

    struct type *t = NEW(struct type);
    str_addstr(&t->name, *proto);
    str_add(&t->c_type, "struct ");
    str_addstr(&t->c_type, *c);
    str_addstr(&t->proto_suffix, *c);
    t->msg = msg;
    t->max_print_size = INFINITY;
    t->max_proto_size = INFINITY;
    t->file = file;
    t->defined = false;
	t->default_packed = default_packed;

    if (!strcmp(proto->buf, ".google.protobuf.Timestamp")) {
        str_set(&t->c_type, "os_time_t");
        str_set(&t->proto_suffix, "time");
        t->msg = NULL;
        t->max_print_size = (float)(strlen("1972-01-01T10:00:20.123456789Z") + 3);
        t->max_proto_size = 1 + 1 + 10 + 1 + 10; // datasize + 2 tagged int64s
        t->max_proto_size_calculated = 1;

    } else if (!strcmp(proto->buf, ".google.protobuf.Duration")) {
        str_set(&t->c_type, "os_duration_t");
        str_set(&t->proto_suffix, "duration");
        t->msg = NULL;
        t->max_print_size = (float)(strlen("9223372036854775808.123456789s") + 3);
        t->max_proto_size = 1 + 1 + 10 + 1 + 10; // datasize + 2 tagged int64s
        t->max_proto_size_calculated = 1;

    } else if (!strcmp(proto->buf, ".google.protobuf.Empty")) {
        str_set(&t->c_type, "pb_empty_t");
        str_set(&t->proto_suffix, "empty");
		t->msg = NULL;
		t->max_print_size = 0;
		t->max_proto_size = 0;
		t->max_proto_size_calculated = 1;
	}

    if (msg && msg->options && msg->options->map_entry && msg->field.len == 2) {
        t->map_key = msg->field.v[0];
        t->map_value = msg->field.v[1];
    }

	t->json_suffix = t->proto_suffix;
	smap_set(&g_named_types, t->name.buf, t->name.len, t);
    
    if (t->msg) {
		imap_set(&g_struct_types, t->msg, t);

        for (int i = 0; i < t->msg->nested_type.len; i++) {
            str_setlen(proto, protosz);
            str_setlen(c, csz);
            insert_message(t->msg->nested_type.v[i], proto, c, default_packed, NULL);
        }
        
        for (int i = 0; i < t->msg->enum_type.len; i++) {
            str_setlen(proto, protosz);
            str_setlen(c, csz);
            insert_enum(t->msg->enum_type.v[i], proto, c);
        }
    }
}

void insert_file_types(const struct FileDescriptorProto *f) {
    str_t proto = STR_INIT;
    str_t c = STR_INIT;
    
    if (f->package.len) {
        str_add(&proto, ".");
        str_addstr(&proto, f->package);
    }

    int protosz = proto.len;
    int csz = c.len;
    bool default_packed = pb_cmp(f->syntax, "") && pb_cmp(f->syntax, "proto2");

    for (int i = 0; i < f->enum_type.len; i++) {
        str_setlen(&proto, protosz);
        str_setlen(&c, csz);
        insert_enum(f->enum_type.v[i], &proto, &c);
    }

    for (int i = 0; i < f->message_type.len; i++) {
        str_setlen(&proto, protosz);
        str_setlen(&c, csz);
        insert_message(f->message_type.v[i], &proto, &c, default_packed, f);
    }

	for (int i = 0; i < f->service.len; i++) {
        str_setlen(&proto, protosz);
        str_setlen(&c, csz);
		insert_service(f->service.v[i], &proto, &c);
	}
}

uint32_t get_tag_size(const struct FieldDescriptorProto *f) {
    if (f->number < 0x10) {
        return 1;
    } else if (f->number < 0x800) {
        return 2;
    } else if (f->number < 0x40000) {
        return 3;
    } else {
        return 4;
    }
}

static enum pb_wiretype get_wire_type(const struct FieldDescriptorProto *f) {
    if (f->label == LABEL_REPEATED) {
        return PB_WIRE_VARIABLE;
    }
    switch (f->type) {
    case TYPE_BOOL:
    case TYPE_INT32:
    case TYPE_UINT32:
    case TYPE_SINT32:
    case TYPE_ENUM:
    case TYPE_INT64:
    case TYPE_UINT64:
    case TYPE_SINT64:
        return PB_WIRE_VARINT;
    case TYPE_FIXED32:
    case TYPE_SFIXED32:
    case TYPE_FLOAT:
        return PB_WIRE_F32;
    case TYPE_FIXED64:
    case TYPE_SFIXED64:
    case TYPE_DOUBLE:
        return PB_WIRE_F64;
    case TYPE_STRING:
    case TYPE_BYTES:
    case TYPE_MESSAGE:
        return PB_WIRE_VARIABLE;
    default:
        assert(0);
        return PB_WIRE_VARIABLE;
    }
}

uint32_t get_tag(const struct FieldDescriptorProto *f) {
    enum pb_wiretype wire = get_wire_type(f);
    uint32_t val = (uint32_t)wire | (f->number << 3);
    char ret[4] = {0};
    pb_put_u32(ret, val);
    return *(uint32_t*) &ret[0];
}

static void calc_max_proto_size(struct type *t) {
    float sz = 0;
    // start with infinite size to handle recursive types
    t->max_proto_size = INFINITY;
    for (int i = 0; i < t->msg->field.len && isfinite(sz); i++) {
        const struct FieldDescriptorProto *f = t->msg->field.v[i];
        const struct type *ft = get_field_type(f);
        if (f->label == LABEL_REPEATED) {
            sz = INFINITY;
        } else {
            sz += (float) get_tag_size(f);
            sz += ft->max_proto_size;
        }
    }

    t->max_proto_size_calculated = true;
    if (isfinite(sz)) {
        t->max_proto_size = sz + pb_u32_size((uint32_t) sz);
		if (sz <= 0x7F) {
			t->pod_message = true;
		}
	}
}

#define X(str) STR_STATIC(str)
#define PRINT64 24 /*"-9223372036854775807",*/
#define PRINT32 12 /*-2147483648,*/

static struct type basic_types[19] = {
    {0}, // errors
    {X("double"), 8, 22 /*%.17g,*/, X("f64"), X("double"), X("union pb_f64")}, // TYPE_DOUBLE
    {X("float"), 4, 14 /*%.9g,*/, X("f32"), X("float"), X("union pb_f32")}, // TYPE_FLOAT
    {X("int64_t"), 10, PRINT64, X("u64"), X("i64"), X("uint64_t")}, // TYPE_INT64
    {X("uint64_t"), 10, PRINT64, X("u64"), X("u64"), STR_INIT}, // TYPE_UINT64
    {X("int32_t"), 5, PRINT32, X("u32"), X("i32"), X("uint32_t")}, // TYPE_INT32
    {X("uint64_t"), 8, PRINT64, X("f64"), X("u64"), X("union pb_f64")}, // TYPE_FIXED64
    {X("uint32_t"), 4, PRINT32, X("f32"), X("u32"), X("union pb_f32")}, // TYPE_FIXED32
    {X("bool"), 1, 6 /*false,*/, X("bool"), X("bool"), STR_INIT}, // TYPE_BOOL
    {X("struct pb_string"), INFINITY, INFINITY, X("string"), X("string"), X("struct pb_string")}, // TYPE_STRING
    {0}, // TYPE_GROUP - unused
    {0}, // TYPE_MESSAGE
    {X("struct pb_bytes"), INFINITY, INFINITY, X("bytes"), X("bytes"), X("struct pb_bytes")}, // TYPE_BYTES
    {X("uint32_t"), 5, PRINT32, X("u32"), X("u32"), STR_INIT}, // TYPE_UINT32
    {0}, // TYPE_ENUM - filled out separately
    {X("int32_t"), 4, PRINT32, X("f32"), X("i32"), X("union pb_f32")}, // TYPE_SFIXED32
    {X("int64_t"), 8, PRINT64, X("f64"), X("i64"), X("union pb_f64")}, // TYPE_SFIXED64
    {X("int32_t"), 5, PRINT32, X("s32"), X("i32"), STR_INIT}, // TYPE_SINT32
    {X("int64_t"), 10, PRINT64, X("s64"), X("i64"), STR_INIT}, // TYPE_SINT64
};

void get_proto_cast(str_t *out, const struct FieldDescriptorProto* f, int array_type, int value_type) {
    struct type *t = get_field_type(f);
    if (!t->proto_type.len) {
        return;
    }

    if (value_type) {
        str_add(out, "*");
    }
    str_add(out, "(");
    str_addstr(out, t->proto_type);

    if (array_type) {
        str_add(out, "*");
    }
    str_add(out, "*)");
    if (value_type) {
        str_add(out, "&");
    }
}

struct type *get_field_type(const struct FieldDescriptorProto *f) {
    if (f->type == TYPE_MESSAGE || f->type == TYPE_ENUM) {
        struct type *t = NULL;
        if (smap_get(&g_named_types, f->type_name.buf, f->type_name.len, &t) && t->msg && !t->max_proto_size_calculated) {
            calc_max_proto_size(t);
        }
        return t;
    } else {
        return &basic_types[f->type];
    }
}

struct type *get_struct_type(const struct DescriptorProto *p) {
    struct type *t = NULL;
    if (imap_get(&g_struct_types, p, &t) && !t->max_proto_size_calculated) {
        calc_max_proto_size(t);
    }
    return t;
}

struct type *get_enum_type(const struct EnumDescriptorProto *p) {
    struct type *t = NULL;
    imap_get(&g_enum_types, p, &t);
    return t;
}

struct type *get_service_type(const struct ServiceDescriptorProto *p) {
    struct type *t = NULL;
    imap_get(&g_service_types, p, &t);
    return t;
}

struct type *get_input_type(const struct MethodDescriptorProto *m) {
    struct type *t = NULL;
    smap_get(&g_named_types, m->input_type.buf, m->input_type.len, &t);
    return t;
}

struct type *get_output_type(const struct MethodDescriptorProto *m) {
	struct type *t= NULL;
    smap_get(&g_named_types, m->output_type.buf, m->output_type.len, &t);
    return t;
}

bool is_field_packed(const struct type *t, const struct FieldDescriptorProto *ft) {
	if (ft->label != LABEL_REPEATED) {
		return false;
	}
	if (ft->type == TYPE_MESSAGE || ft->type == TYPE_BYTES || ft->type == TYPE_STRING) {
		return false;
	}
	if (ft->options && ft->options->packed_set) {
		return ft->options->packed;
	}
	return t->default_packed;
}
