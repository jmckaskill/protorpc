#include "protoc-gen-protorpc.h"

void do_nonzero(str_t *o, const struct type *t, bool define) {
	if (!t->pod_message) {
		return;
	}

	str_addf(o, "bool pb_nonzero_%s(%s const *m)", t->json_suffix.buf, t->c_type.buf);
	if (!define) {
		str_add(o, ";" EOL);
		return;
	}

	static str_t mbr = STR_INIT;

	str_add(o, " {" EOL);
	str_add(o, "\treturn false");

	for (int i = 0; i < t->msg->oneof_decl.len; i++) {
		str_add(o, EOL "\t    || m->");
		str_addstr(o, t->msg->oneof_decl.v[i]->name);
		str_add(o, "_type");
	}

	int32_t oneof_index = -1;

	for (int i = 0; i < t->msg->field.len; i++) {
		const struct FieldDescriptorProto *f = t->msg->field.v[i];
		// only want to check the first member of a oneof
		if (f->oneof_index_set && oneof_index == f->oneof_index) {
			continue;
		} else if (f->oneof_index_set) {
			oneof_index = f->oneof_index;
		} else {
			oneof_index = -1;
		}

		str_set(&mbr, "m->");
		str_addstr(&mbr, f->name);

		if (f->oneof_index_set) {
			pb_string_t oneof = t->msg->oneof_decl.v[f->oneof_index]->name;
			str_set(&mbr, "m->");
            str_addstr(&mbr, oneof);
            str_addch(&mbr, '.');
            str_addstr(&mbr, f->name);
		}

		const struct type *ft = get_field_type(f);
		str_add(o, EOL);
		if (f->label == LABEL_REPEATED || f->type == TYPE_STRING || f->type == TYPE_BYTES) {
			str_addf(o, "\t    || %s.len", mbr.buf);
		} else if (ft->pod_message) {
			str_addf(o, "\t    || pb_nonzero_%s(&%s)", ft->json_suffix.buf, mbr.buf);
		} else {
			str_addf(o, "\t    || %s", mbr.buf);
		}
	}
	str_add(o, ";" EOL);
	str_add(o, "}" EOL);
}

void do_print(str_t *o, const struct type *t, bool define) {
    str_addf(o, "int pb_print_%s(pb_buf_t *a, %s const *m)", t->json_suffix.buf, t->c_type.buf);
    if (!define) {
        str_add(o, ";" EOL);
		return;
    }
    str_add(o, " {" EOL);
	
    // calc size of fixed members
    int fixedsz = 0;
    for (int i = 0; i < t->msg->field.len; i++) {
        const struct FieldDescriptorProto *f = t->msg->field.v[i];
        const struct type *ft = get_field_type(f);
        if (f->label != LABEL_REPEATED && isfinite(ft->max_print_size)) {
            fixedsz += f->name.len + 3 + (int) ft->max_print_size; // "key":<val>
        }
    }

    // print out the fixed members
	str_addf(o, "\tchar *p = pb_appendsz(a, %u);" EOL, fixedsz+1); // +1 is for the opening brace
	str_add(o, "\tif (!p) {return -1;}" EOL);
    str_add(o, "\t*p++ = '{';" EOL);

    static str_t mbr = STR_INIT;

    for (int i = 0; i < t->msg->field.len; i++) {
        const struct FieldDescriptorProto *f = t->msg->field.v[i];
        const struct type *ft = get_field_type(f);
        if (f->label != LABEL_REPEATED && isfinite(ft->max_print_size)) {
            str_set(&mbr, "m->");
            str_addstr(&mbr, f->name);

            if (f->oneof_index_set) {
                pb_string_t oneof = t->msg->oneof_decl.v[f->oneof_index]->name;
                str_add(o, "\tif(m->");
                str_addstr(o, oneof);
                str_add(o, "_type == ");
                pb_string_t ps = {t->proto_suffix.len, t->proto_suffix.buf};
                to_upper(o, ps);
                str_add(o, "_");
                to_upper(o, f->name);
                str_add(o, ") {" EOL);

                str_set(&mbr, "m->");
                str_addstr(&mbr, oneof);
                str_addch(&mbr, '.');
                str_addstr(&mbr, f->name);
            } else {
                str_addf(o, "\tif (%s) {" EOL, mbr.buf);
            }

            str_addf(o, "\t\tmemcpy(p, \"\\\"%s\\\":\", %u);" EOL, f->name.buf, f->name.len + 3 /*"":*/);
            str_addf(o, "\t\tp += %u;" EOL, f->name.len + 3);
            str_addf(o, "\t\tp = pb_print_%s(p, %s);" EOL, ft->json_suffix.buf, mbr.buf);
            str_add(o, "\t}" EOL);
        }
    }

	str_add(o, "\ta->next = p;" EOL);

    // now everything else
    for (int i = 0; i < t->msg->field.len; i++) {
        const struct FieldDescriptorProto *f = t->msg->field.v[i];
        const struct type *ft = get_field_type(f);
        if (f->label != LABEL_REPEATED && isfinite(ft->max_print_size)) {
            continue;
        }
        
        str_set(&mbr, "m->");
        str_addstr(&mbr, f->name);

        if (f->oneof_index_set) {
            pb_string_t oneof = t->msg->oneof_decl.v[f->oneof_index]->name;
            str_add(o, "\tif(m->");
            str_addstr(o, oneof);
            str_add(o, "_type == ");
            pb_string_t ps = {t->proto_suffix.len, t->proto_suffix.buf};
            to_upper(o, ps);
            str_add(o, "_");
            to_upper(o, f->name);
            str_add(o, ") {" EOL);

            str_set(&mbr, "m->");
            str_addstr(&mbr, oneof);
            str_addch(&mbr, '.');
            str_addstr(&mbr, f->name);

		} else if (f->label == LABEL_REPEATED || f->type == TYPE_STRING || f->type == TYPE_BYTES) {
			str_addf(o, "\tif (%s.len) {" EOL, mbr.buf);
		} else if (ft->pod_message) {
			str_addf(o, "\tif (pb_nonzero_%s(&%s)) {" EOL, ft->json_suffix.buf, mbr.buf);
        } else {
            str_addf(o, "\tif (%s) {" EOL, mbr.buf);
        }

		str_addf(o, "\t\tif (pb_append(a, \"\\\"%s\\\":\", %d)) {return -1;}" EOL, f->name.buf, f->name.len + 3);

        if (f->label == LABEL_REPEATED) {
			if (isfinite(ft->max_print_size)) {
				str_addf(o, "\t\tchar *mp = pb_appendsz(a, 3 /*[],*/ + %s.len * %d);" EOL, mbr.buf, (int) ft->max_print_size);
				str_add(o, "\t\tif (!mp) {return -1;}" EOL);
                str_add(o, "\t\t*mp++ = '[';" EOL);
                str_addf(o, "\t\tfor (int i = 0; i < %s.len; i++) {" EOL, mbr.buf);
                str_addf(o, "\t\t\tmp = pb_print_%s(mp, %s.v[i]);" EOL, ft->json_suffix.buf, mbr.buf);
                str_add(o, "\t\t}" EOL);
                str_add(o, "\t\tmp = pb_print_array_end_i(mp);" EOL);
				str_add(o, "\t\ta->next = mp;" EOL);
			} else {
				str_add(o, "\t\tif (pb_append(a, \"[\", 1)) {return -1;}" EOL);
                str_addf(o, "\t\tfor (int i = 0; i < %s.len; i++) {" EOL, mbr.buf);
				str_addf(o, "\t\t\tif (pb_print_%s(a, ", ft->json_suffix.buf);
				if (ft->pod_message) {
					str_addch(o, '&');
				}
				str_addf(o, "%s.v[i])) {return -1;}" EOL, mbr.buf);
				str_add(o, "\t\t\ta->next[-1] = ','; // replace trailing newline" EOL);
                str_add(o, "\t\t}" EOL);
				str_add(o, "\t\tif (pb_print_array_end(a)) {return -1;}" EOL);
			}
        } else {
			str_addf(o, "\t\tif (pb_print_%s(a, ", ft->json_suffix.buf);
			if (ft->pod_message) {
				str_addch(o, '&');
			}
			str_addf(o, "%s)) {return -1;}" EOL, mbr.buf);
			str_add(o, "\t\ta->next[-1] = ','; // replace trailing newline" EOL);
        }

        str_add(o, "\t}" EOL);
    }
    str_add(o, "\treturn pb_print_map_end(a);" EOL);
    str_add(o, "}" EOL);
}

void do_print_enum(str_t *o, const struct type *t, bool define) {
    str_addf(o, "char *pb_print_%s(char *p, %s v)", t->json_suffix.buf, t->c_type.buf);
    if (!define) {
        str_add(o, ";" EOL);
        return;
    }
    str_add(o, "{" EOL);
    str_add(o, "\tswitch ((int32_t) v) {" EOL);
    for (int i = 0; i < t->en->value.len; i++) {
        const struct EnumValueDescriptorProto *v = t->en->value.v[i];
        // "enum",
        str_addf(o, "\tcase %d:" EOL, v->number);
        str_addf(o, "\t\tmemcpy(p, \"\\\"%s\\\",\", %u);" EOL, v->name.buf, v->name.len + 3 /*"",*/);
        str_addf(o, "\t\treturn p + %u;" EOL, v->name.len + 3);
    }
    str_add(o, "\tdefault:" EOL);
	// "",
    str_add(o, "\t\tmemcpy(p, \"\\\"\\\",\", 3);" EOL);
    str_add(o, "\t\treturn p + 3;" EOL);
    str_add(o, "\t}" EOL);
    str_add(o, "}" EOL);
}
