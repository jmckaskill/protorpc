#include "protoc-gen-protorpc.h"
#include <assert.h>

void to_upper(str_t *o, const char *s, size_t n) {
	for (size_t i = 0; i < n; i++) {
		if ('a' <= s[i] && s[i] <= 'z') {
            str_addch(o, s[i] - 'a' + 'A');
		} else {
            str_addch(o, s[i]);
		}
	}
}

void define_enum(str_t *o, const struct type *t) {
    str_addstr(o, &t->c_type);
    str_add(o, " {" EOL);

    for (int i = 0; i < t->en->value.len; i++) {
        const struct EnumValueDescriptorProto *v = t->en->value.v[i];
        str_add(o, "\t");
        str_addpb(o, v->name);
        str_add(o, " = ");
        str_addf(o, "%u", v->number);
        if (i < t->en->value.len-1) {
            str_add(o, ",");
        }
        str_add(o, EOL);
    }
    str_add(o, "};" EOL);
}

void declare_struct(str_t *o, const struct type *t) {
    str_addstr(o, &t->c_type);
    str_add(o, ";" EOL);
}

static void append_field_type(str_t *o, const struct FieldDescriptorProto *f) {
    const struct type *ft = get_field_type(f);
    if (f->label == LABEL_REPEATED) {
        str_add(o, "struct {int len; ");
        str_addstr(o, &ft->c_type);
        str_add(o, " const *");
        if (ft->msg && !ft->pod_message) {
            str_add(o, "*");
        }
        str_add(o, "v;}");
    } else if (ft->msg && !ft->pod_message) {
        str_addstr(o, &ft->c_type);
        str_add(o, " const*");
    } else {
        str_addstr(o, &ft->c_type);
    }
}

size_t declare_oneof(str_t *o, const struct type *t, int i) {
	int32_t oneof = t->msg->field.v[i]->oneof_index;
	struct pb_string name = t->msg->oneof_decl.v[oneof]->name;

    str_add(o, "enum ");
    str_addstr(o, &t->proto_suffix);
    str_add(o, "_");
    str_addpb(o, name);
    str_add(o, "_type {" EOL);

	while (i < t->msg->field.len && t->msg->field.v[i]->oneof_index_set && t->msg->field.v[i]->oneof_index == oneof) {
        const struct FieldDescriptorProto* f = t->msg->field.v[i];
        str_add(o, "\t");
        to_upper(o, t->proto_suffix.buf, t->proto_suffix.len);
        str_add(o, "_");
        to_upper(o, f->name.p, f->name.len);
        str_add(o, ",");
        str_add(o, EOL);
        i++;
	}

    str_add(o, "};" EOL);

	return i;
}

size_t define_oneof(str_t *o, const struct type *t, int i) {
    int32_t oneof = t->msg->field.v[i]->oneof_index;
    struct pb_string name = t->msg->oneof_decl.v[oneof]->name;

    str_add(o, "\tenum ");
    str_addstr(o, &t->proto_suffix);
    str_add(o, "_");
    str_addpb(o, name);
    str_add(o, "_type ");
    str_addpb(o, name);
    str_add(o, "_type;" EOL);

    str_add(o, "\tunion {" EOL);

    while (i < t->msg->field.len && t->msg->field.v[i]->oneof_index_set && t->msg->field.v[i]->oneof_index == oneof) {
        const struct FieldDescriptorProto* f = t->msg->field.v[i];
        str_add(o, "\t\t");
        append_field_type(o, f);
        str_add(o, " ");
        str_addpb(o, f->name);
        str_add(o, ";" EOL);
        i++;
    }

    str_add(o, "\t} ");
    str_addpb(o, name);
    str_add(o, ";" EOL);

    return i;
}

void define_struct(str_t *o, struct type *t) {
	if (t->defined) {
		return;
	}
	t->defined = true;

    for (int i = 0; i < t->msg->nested_type.len; i++) {
        define_struct(o, get_struct_type(t->msg->nested_type.v[i]));
    }

    for (int i = 0; i < t->msg->enum_type.len; i++) {
        define_enum(o, get_enum_type(t->msg->enum_type.v[i]));
    }

	for (int i = 0; i < t->msg->field.len; i++) {
		const struct FieldDescriptorProto *f = t->msg->field.v[i];
		if (f->oneof_index_set) {
			i = declare_oneof(o, t, i);
		}
		struct type *ft = get_field_type(f);
		if (ft->pod_message) {
			define_struct(o, ft);
		}
	}

    str_addstr(o, &t->c_type);
    str_add(o, " {" EOL);
    
	if (!t->pod_message) {
		str_add(o, "\tunion pb_msg pb_hdr;" EOL);
	}
    
    for (int i = 0; i < t->msg->field.len; i++) {
        const struct FieldDescriptorProto *f = t->msg->field.v[i];
        if (f->oneof_index_set) {
            i = define_oneof(o, t, i);
        } else {
            str_add(o, "\t");
            append_field_type(o, f);
            str_addch(o, '\t');
            str_addpb(o, f->name);
            str_add(o, ";" EOL);
        }
    }

    // hacks to support bootstrapping descriptor.proto as 0 are useful values for these fields
    if (!strcmp(t->name.buf, ".FieldDescriptorProto")) {
        str_add(o, "\tbool oneof_index_set;" EOL);

    } else if (!strcmp(t->name.buf, ".FieldOptions")) {
        str_add(o, "\tbool packed_set;" EOL);
	}

    str_add(o, "};" EOL);
}

void do_enum_funcs(str_t *o, const struct type *t, bool define) {
    str_add(o, EOL);
    do_print_enum(o, t, define);
    do_parse_enum(o, t, define);
}

static int cmp_field_number(const void *a, const void *b) {
    struct FieldDescriptorProto *fa = *(struct FieldDescriptorProto**) a;
    struct FieldDescriptorProto *fb = *(struct FieldDescriptorProto**) b;
    return fa->number - fb->number;
}

void do_struct_funcs(str_t *o, const struct type *t, bool define) {
    str_add(o, EOL);

	do_nonzero(o, t, define);
    do_parse(o, t, define);
	do_fparse(o, t, define);
    do_print(o, t, define);

    if (define) {
        qsort((struct FieldDescriptorProto**) t->msg->field.v, t->msg->field.len, sizeof(t->msg->field.v[0]), &cmp_field_number);
    }

    do_decode(o, t, define);
    do_encode(o, t, define);
    do_maxsz(o, t, define);

    for (int i = 0; i < t->msg->enum_type.len; i++) {
        do_enum_funcs(o, get_enum_type(t->msg->enum_type.v[i]), define);
    }
    for (int i = 0; i < t->msg->nested_type.len; i++) {
        do_struct_funcs(o, get_struct_type(t->msg->nested_type.v[i]), define);
    }
}

