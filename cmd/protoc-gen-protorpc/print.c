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
		str_add(o, " ||" EOL);
		str_add(o, "\t\tm->");
		str_addpb(o, t->msg->oneof_decl.v[i]->name);
		str_add(o, "_type");
	}

	for (int i = 0; i < t->msg->field.len; i++) {
		const struct FieldDescriptorProto *f = t->msg->field.v[i];
		if (f->oneof_index_set) {
			continue;
		}

		str_set(&mbr, "m->");
		str_addpb(&mbr, f->name);

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
    str_addf(o, "void pb_print_%s(str_t *a, %s const *m)", t->json_suffix.buf, t->c_type.buf);
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
	str_addf(o, "\tstr_grow(a, a->len + %u);" EOL, fixedsz+1); // +1 is for the opening brace
	str_add(o, "\tchar *p = a->buf + a->len;" EOL);
    str_add(o, "\t*p++ = '{';" EOL);

    static str_t mbr = STR_INIT;

    for (int i = 0; i < t->msg->field.len; i++) {
        const struct FieldDescriptorProto *f = t->msg->field.v[i];
        const struct type *ft = get_field_type(f);
        if (f->label != LABEL_REPEATED && isfinite(ft->max_print_size)) {
            str_set(&mbr, "m->");
            str_addpb(&mbr, f->name);

            if (f->oneof_index_set) {
                struct pb_string oneof = t->msg->oneof_decl.v[f->oneof_index]->name;
                str_add(o, "\tif(m->");
                str_addpb(o, oneof);
                str_add(o, "_type == ");
                to_upper(o, t->proto_suffix.buf, t->proto_suffix.len);
                str_add(o, "_");
                to_upper(o, f->name.p, f->name.len);
                str_add(o, ") {" EOL);

                str_set(&mbr, "m->");
                str_addpb(&mbr, oneof);
                str_addch(&mbr, '.');
                str_addpb(&mbr, f->name);
            } else {
                str_addf(o, "\tif (%s) {" EOL, mbr.buf);
            }

            str_addf(o, "\t\tmemcpy(p, \"\\\"%.*s\\\":\", %u);" EOL, f->name.len, f->name.p, f->name.len + 3 /*"":*/);
            str_addf(o, "\t\tp += %u;" EOL, f->name.len + 3);
            str_addf(o, "\t\tp = pb_print_%s(p, %s);" EOL, ft->json_suffix.buf, mbr.buf);
            str_add(o, "\t}" EOL);
        }
    }

	str_add(o, "\tstr_setlen(a, (unsigned) (p - a->buf));" EOL);

    // now everything else
    for (int i = 0; i < t->msg->field.len; i++) {
        const struct FieldDescriptorProto *f = t->msg->field.v[i];
        const struct type *ft = get_field_type(f);
        if (f->label != LABEL_REPEATED && isfinite(ft->max_print_size)) {
            continue;
        }
        
        str_set(&mbr, "m->");
        str_addpb(&mbr, f->name);

        if (f->oneof_index_set) {
            struct pb_string oneof = t->msg->oneof_decl.v[f->oneof_index]->name;
            str_add(o, "\tif(m->");
            str_addpb(o, oneof);
            str_add(o, "_type == ");
            to_upper(o, t->proto_suffix.buf, t->proto_suffix.len);
            str_add(o, "_");
            to_upper(o, f->name.p, f->name.len);
            str_add(o, ") {" EOL);

            str_set(&mbr, "m->");
            str_addpb(&mbr, oneof);
            str_addch(&mbr, '.');
            str_addpb(&mbr, f->name);

		} else if (f->label == LABEL_REPEATED || f->type == TYPE_STRING || f->type == TYPE_BYTES) {
			str_addf(o, "\tif (%s.len) {" EOL, mbr.buf);
		} else if (ft->pod_message) {
			str_addf(o, "\tif (pb_nonzero_%s(&%s)) {" EOL, ft->json_suffix.buf, mbr.buf);
        } else {
            str_addf(o, "\tif (%s) {" EOL, mbr.buf);
        }

		str_addf(o, "\t\tstr_add(a, \"\\\"%.*s\\\":\");" EOL, f->name.len, f->name.p);

        if (f->label == LABEL_REPEATED) {
			if (isfinite(ft->max_print_size)) {
				str_addf(o, "\t\tstr_grow(a, a->len + 3 /*[],*/ + %s.len * %d);" EOL, mbr.buf, (int) ft->max_print_size);
				str_add(o, "\t\tchar *mp = a->buf + a->len;" EOL);
                str_add(o, "\t\t*mp++ = '[';" EOL);
                str_addf(o, "\t\tfor (int i = 0; i < %s.len; i++) {" EOL, mbr.buf);
                str_addf(o, "\t\t\tmp = pb_print_%s(mp, %s.v[i]);" EOL, ft->json_suffix.buf, mbr.buf);
                str_add(o, "\t\t}" EOL);
                str_add(o, "\t\tmp = pb_print_array_end_i(mp);" EOL);
				str_add(o, "\t\ta->len = (int) (mp - a->buf);" EOL);
			} else {
                str_add(o, "\t\tstr_addch(a, '[');" EOL);
                str_addf(o, "\t\tfor (int i = 0; i < %s.len; i++) {" EOL, mbr.buf);
				str_addf(o, "\t\t\tpb_print_%s(a, ", ft->json_suffix.buf);
				if (ft->pod_message) {
					str_addch(o, '&');
				}
                str_addf(o, "%s.v[i]);" EOL, mbr.buf);
				str_add(o, "\t\t\ta->buf[a->len-1] = ','; // replace trailing newline" EOL);
                str_add(o, "\t\t}" EOL);
                str_add(o, "\t\tpb_print_array_end(a);" EOL);
			}
        } else {
			str_addf(o, "\t\tpb_print_%s(a, ", ft->json_suffix.buf);
			if (ft->pod_message) {
				str_addch(o, '&');
			}
			str_addf(o, "%s); " EOL, mbr.buf);
			str_add(o, "\t\ta->buf[a->len-1] = ','; // replace trailing newline" EOL);
        }

        str_add(o, "\t}" EOL);
    }
    str_add(o, "\tpb_print_map_end(a);" EOL);
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
        str_addf(o, "\t\tmemcpy(p, \"\\\"%.*s\\\",\", %u);" EOL, v->name.len, v->name.p, v->name.len + 3 /*"",*/);
        str_addf(o, "\t\treturn p + %u;" EOL, v->name.len + 3);
    }
    str_add(o, "\tdefault:" EOL);
	// "",
    str_add(o, "\t\tmemcpy(p, \"\\\"\\\",\", 3);" EOL);
    str_add(o, "\t\treturn p + 3;" EOL);
    str_add(o, "\t}" EOL);
    str_add(o, "}" EOL);
}
