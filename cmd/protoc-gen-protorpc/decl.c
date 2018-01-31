#include "protoc-gen-protorpc.h"
#include <assert.h>

void to_upper(str_t *o, const char *s) {
	while (*s) {
		if ('a' <= *s && *s <= 'z') {
            str_addch(o, *s - 'a' + 'A');
		} else {
            str_addch(o, *s);
		}
		s++;
	}
}

void define_enum(str_t *o, const struct type *t) {
    str_addstr(o, t->c_type);
    str_add(o, " {" EOL);

    for (int i = 0; i < t->en->value.len; i++) {
        const struct EnumValueDescriptorProto *v = t->en->value.v[i];
        str_add(o, "\t");
        str_addstr(o, v->name);
        str_add(o, " = ");
        str_addf(o, "%u", v->number);
        if (i < t->en->value.len-1) {
            str_add(o, ",");
        }
        str_add(o, EOL);
    }
    str_add(o, "};" EOL);
}

void declare_struct(str_t *o, struct type *t) {
	if (!t->declared && !t->defined) {
		str_addstr(o, t->c_type);
		str_add(o, ";" EOL);
		t->declared = true;
	}
}

static void append_field_type(str_t *o, const struct FieldDescriptorProto *f) {
    const struct type *ft = get_field_type(f);
    if (f->label == LABEL_REPEATED) {
        str_add(o, "struct {int len; ");
        str_addstr(o, ft->c_type);
        str_add(o, " const *");
        if (ft->msg && !ft->pod_message) {
            str_add(o, "*");
        }
		str_add(o, "v; int _encoded;}");
    } else if (ft->msg && !ft->pod_message) {
        str_addstr(o, ft->c_type);
        str_add(o, " const*");
    } else {
        str_addstr(o, ft->c_type);
    }
}

size_t declare_oneof(str_t *o, const struct type *t, int i) {
	uint32_t oneof = t->msg->field.v[i]->oneof_index.val;
	pb_string_t name = t->msg->oneof_decl.v[oneof]->name;

    str_add(o, "enum ");
    str_addstr(o, t->proto_suffix);
    str_add(o, "_");
    str_addstr(o, name);
    str_add(o, "_type {" EOL);

	while (i < t->msg->field.len && t->msg->field.v[i]->oneof_index.set && t->msg->field.v[i]->oneof_index.val == oneof) {
        const struct FieldDescriptorProto* f = t->msg->field.v[i];
        str_add(o, "\t");
        to_upper(o, t->proto_suffix.c_str);
        str_add(o, "_");
        to_upper(o, f->name.c_str);
        str_addf(o, " = %u,", f->number);
        str_add(o, EOL);
        i++;
	}

    str_add(o, "};" EOL);

	return i;
}

size_t define_oneof(str_t *o, const struct type *t, int i) {
    uint32_t oneof = t->msg->field.v[i]->oneof_index.val;
    pb_string_t name = t->msg->oneof_decl.v[oneof]->name;

    str_add(o, "\tenum ");
    str_addstr(o, t->proto_suffix);
    str_add(o, "_");
    str_addstr(o, name);
    str_add(o, "_type ");
    str_addstr(o, name);
    str_add(o, "_type;" EOL);

    str_add(o, "\tunion {" EOL);

    while (i < t->msg->field.len && t->msg->field.v[i]->oneof_index.set && t->msg->field.v[i]->oneof_index.val == oneof) {
        const struct FieldDescriptorProto* f = t->msg->field.v[i];
        str_add(o, "\t\t");
        append_field_type(o, f);
        str_add(o, " ");
        str_addstr(o, f->name);
        str_add(o, ";" EOL);
        i++;
    }

    str_add(o, "\t} ");
    str_addstr(o, name);
    str_add(o, ";" EOL);

    return i;
}

void define_struct(str_t *o, struct type *t) {
	if (t->defined) {
		return;
	}

    for (int i = 0; i < t->msg->nested_type.len; i++) {
        define_struct(o, get_struct_type(t->msg->nested_type.v[i]));
    }

    for (int i = 0; i < t->msg->enum_type.len; i++) {
        define_enum(o, get_enum_type(t->msg->enum_type.v[i]));
    }

	for (int i = 0; i < t->msg->field.len; i++) {
		const struct FieldDescriptorProto *f = t->msg->field.v[i];
		if (f->oneof_index.set) {
			i = declare_oneof(o, t, i);
		}
		struct type *ft = get_field_type(f);
		if (ft->file == t->file) {
			if (ft->pod_message) {
				define_struct(o, ft);
			} else if (ft->msg) {
				declare_struct(o, ft);
			}
		}
	}

    str_addstr(o, t->c_type);
    str_add(o, " {" EOL);
    
	if (!t->pod_message) {
		str_add(o, "\tunion pb_msg pb_hdr;" EOL);
	}
    
    for (int i = 0; i < t->msg->field.len; i++) {
        const struct FieldDescriptorProto *f = t->msg->field.v[i];
        if (f->oneof_index.set) {
            i = define_oneof(o, t, i);
        } else {
            str_add(o, "\t");
            append_field_type(o, f);
            str_addch(o, '\t');
            str_addstr(o, f->name);
            str_add(o, ";" EOL);
        }
    }

    // hacks to support bootstrapping descriptor.proto as 0 are useful values for these fields
    if (str_test(t->name, ".google.protobuf.FieldDescriptorProto")) {
        str_add(o, "\tbool oneof_index.set;" EOL);

    } else if (str_test(t->name, ".google.protobuf.FieldOptions")) {
        str_add(o, "\tbool packed_set;" EOL);
	}

    str_add(o, "};" EOL);
	t->defined = true;
}

static int cmp_enum_number(const void *a, const void *b) {
	struct EnumValueDescriptorProto *va = *(struct EnumValueDescriptorProto**) a;
	struct EnumValueDescriptorProto *vb = *(struct EnumValueDescriptorProto**) b;
	return va->number - vb->number;
}

static int cmp_enum_name(const void *a, const void *b) {
	struct EnumValueDescriptorProto *va = *(struct EnumValueDescriptorProto**) a;
	struct EnumValueDescriptorProto *vb = *(struct EnumValueDescriptorProto**) b;
	int delta = va->name.len - vb->name.len;
	if (!delta) {
		delta = memcmp(va->name.c_str, vb->name.c_str, va->name.len);
	}
	return delta;
}

void do_enum_funcs(str_t *o, const struct type *t, bool define) {
    str_add(o, EOL);
	if (!define) {
		declare_enuminfo(o, t);
	} else {
		qsort((struct EnumValueDescriptorProto**) t->en->value.v, t->en->value.len, sizeof(t->en->value.v[0]), &cmp_enum_number);
		for (int i = 0; i < t->en->value.len; i++) {
			struct EnumValueDescriptorProto *v = (struct EnumValueDescriptorProto*) t->en->value.v[i];
			v->by_number_index = i;
		}

		do_enums_by_number(o, t);
	
		qsort((struct EnumValueDescriptorProto**) t->en->value.v, t->en->value.len, sizeof(t->en->value.v[0]), &cmp_enum_name);
		do_enums_by_name(o, t);
		do_enuminfo(o, t);
	}
}

static int cmp_field_number(const void *a, const void *b) {
    struct FieldDescriptorProto *fa = *(struct FieldDescriptorProto**) a;
    struct FieldDescriptorProto *fb = *(struct FieldDescriptorProto**) b;
    return fa->number - fb->number;
}

static int cmp_field_name(const void *a, const void *b) {
	struct FieldDescriptorProto *fa = *(struct FieldDescriptorProto**) a;
	struct FieldDescriptorProto *fb = *(struct FieldDescriptorProto**) b;
	int delta = fa->json_name.len - fb->json_name.len;
	if (!delta) {
		delta = memcmp(fa->json_name.c_str, fb->json_name.c_str, fa->json_name.len);
	}
	return delta;
}

void do_struct_funcs(str_t *o, const struct type *t, bool define) {
    str_add(o, EOL);

	if (!define) {
		declare_typeinfo(o, t);
	} else {
        qsort((struct FieldDescriptorProto**) t->msg->field.v, t->msg->field.len, sizeof(t->msg->field.v[0]), &cmp_field_number);

		for (int i = 0; i < t->msg->field.len; i++) {
			struct FieldDescriptorProto *f = (struct FieldDescriptorProto*) t->msg->field.v[i];
			f->by_number_index = i;
		}

		do_fields_by_number(o, t);

		qsort((struct FieldDescriptorProto**) t->msg->field.v, t->msg->field.len, sizeof(t->msg->field.v[0]), &cmp_field_name);

		do_fields_by_name(o, t);
		do_typeinfo(o, t);
    }

	for (int i = 0; i < t->msg->enum_type.len; i++) {
		do_enum_funcs(o, get_enum_type(t->msg->enum_type.v[i]), define);
	}

    for (int i = 0; i < t->msg->nested_type.len; i++) {
        do_struct_funcs(o, get_struct_type(t->msg->nested_type.v[i]), define);
    }
}

