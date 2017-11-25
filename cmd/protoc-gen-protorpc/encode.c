#include "protoc-gen-protorpc.h"

void do_maxsz(str_t *o, const struct type *t, bool define) {
    str_addf(o, "int pb_maxsz_%s(%s const *m)", t->proto_suffix.c_str, t->c_type.c_str);
    if (!define) {
        str_add(o, ";" EOL);
        return;
    }
    str_add(o, " {" EOL);
    str_add(o, "\tint n = 0;" EOL);

    static str_t mbr = STR_INIT;

    for (int i = 0; i < t->msg->field.len; i++) {
        const struct FieldDescriptorProto *f = t->msg->field.v[i];
        const struct type *ft = get_field_type(f);
        uint32_t tagsz = get_tag_size(f);

        str_set(&mbr, "m->");
        str_addstr(&mbr, f->name);

        if (f->oneof_index_set) {
            pb_string_t oneof = t->msg->oneof_decl.v[f->oneof_index]->name;
            str_add(o, "\tif(m->");
            str_addstr(o, oneof);
            str_add(o, "_type == ");
            to_upper(o, t->proto_suffix.c_str);
            str_add(o, "_");
            to_upper(o, f->name.c_str);
            str_add(o, ") {" EOL);

            str_set(&mbr, "m->");
            str_addstr(&mbr, oneof);
            str_addch(&mbr, '.');
            str_addstr(&mbr, f->name);
		} else if (f->label == LABEL_REPEATED || f->type == TYPE_STRING || f->type == TYPE_BYTES) {
			str_addf(o, "\tif (%s.len) {" EOL, mbr.c_str);
		} else if (ft->pod_message) {
			str_addf(o, "\tif (pb_nonzero_%s(&%s)) {" EOL, ft->proto_suffix.c_str, mbr.c_str);
        } else {
            str_addf(o, "\tif (%s) {" EOL, mbr.c_str);
        }

        if (f->label == LABEL_REPEATED) {
            if (is_field_packed(t, f)) {
                str_addf(o, "\t\tn += %u + (%d * %s.len);" EOL, tagsz, (int) ft->max_proto_size, mbr.c_str);
            } else if (isfinite(ft->max_proto_size)) {
                str_addf(o, "\t\tn += (%u + %d) * %s.len;" EOL, tagsz, (int) ft->max_proto_size, mbr.c_str);
            } else if (ft->msg) {
                str_addf(o, "\t\tfor (int i = 0; i < %s.len; i++) {" EOL, mbr.c_str);
                str_addf(o, "\t\t\tn += %u + pb_maxsz_%s(%s.v[i]);" EOL, tagsz, ft->proto_suffix.c_str, mbr.c_str);
                str_add(o, "\t\t}" EOL);
            } else {
				// bytes and strings
				str_addf(o, "\t\tfor (int i = 0; i < %s.len; i++) {" EOL, mbr.c_str);
				str_addf(o, "\t\t\tn += %u + 5 + %s.v[i].len;" EOL, tagsz, mbr.c_str);
				str_add(o, "\t\t}" EOL);
			}
        } else if (isfinite(ft->max_proto_size)) {
            str_addf(o, "\t\tn += %u + %d;" EOL, tagsz, (int) ft->max_proto_size);
        } else if (ft->msg) {
            str_addf(o, "\t\tn += %u + pb_maxsz_%s(%s);" EOL, tagsz, ft->proto_suffix.c_str, mbr.c_str);
        } else {
            // bytes and strings
            str_addf(o, "\t\tn += %u + 5 + %s.len;" EOL, tagsz, mbr.c_str);
        }

        str_add(o, "\t}" EOL);
    }
    str_add(o, "\tn += pb_u32_size(n);" EOL);
    str_add(o, "\t((union pb_msg*) m)->maxsz = n;" EOL);
    str_add(o, "\treturn n;" EOL);
    str_add(o, "}" EOL);
}

void do_encode(str_t *o, const struct type *t, bool define) {
    str_addf(o, "char *pb_encode_%s(char *p, %s const *m)", t->proto_suffix.c_str, t->c_type.c_str);
    if (!define) {
        str_add(o, ";" EOL);
        return;
    }
    str_add(o, " {" EOL);

    static str_t mbr = STR_INIT;

    for (int i = 0; i < t->msg->field.len; i++) {
        const struct FieldDescriptorProto *f = t->msg->field.v[i];
        const struct type *ft = get_field_type(f);
        uint32_t tagsz = get_tag_size(f);
        uint32_t tag = get_tag(f);

        str_set(&mbr, "m->");
        str_addstr(&mbr, f->name);

        if (f->oneof_index_set) {
            pb_string_t oneof = t->msg->oneof_decl.v[f->oneof_index]->name;
            str_add(o, "\tif(m->");
            str_addstr(o, oneof);
            str_add(o, "_type == ");
            to_upper(o, t->proto_suffix.c_str);
            str_add(o, "_");
            to_upper(o, f->name.c_str);
            str_add(o, ") {" EOL);

            str_set(&mbr, "m->");
            str_addstr(&mbr, oneof);
            str_addch(&mbr, '.');
            str_addstr(&mbr, f->name);
        } else if (f->label == LABEL_REPEATED || f->type == TYPE_STRING || f->type == TYPE_BYTES) {
			str_addf(o, "\tif (%s.len) {" EOL, mbr.c_str);
		} else if (ft->pod_message) {
			str_addf(o, "\tif (pb_nonzero_%s(&%s)) {" EOL, ft->proto_suffix.c_str, mbr.c_str);
        } else {
            str_addf(o, "\tif (%s) {" EOL, mbr.c_str);
        }

		if (f->label == LABEL_REPEATED) {
			if (ft->pod_message) {
				str_addf(o, "\t\tfor (int i = 0; i < %s.len; i++) {" EOL, mbr.c_str);
				str_addf(o, "\t\t\tp = pb_put_tag_%u(p, %u);" EOL, tagsz, tag);
				str_add(o, "\t\t\tchar *start = p+1;" EOL);
				str_addf(o, "\t\t\tp = pb_encode_%s(start, &%s.v[i]);" EOL, ft->proto_suffix.c_str, mbr.c_str);
				str_add(o, "\t\t\tstart[-1] = (char) (p - start);" EOL);
				str_add(o, "\t\t}" EOL);
			} else if (ft->msg) {
				str_addf(o, "\t\tfor (int i = 0; i < %s.len; i++) {" EOL, mbr.c_str);
				str_addf(o, "\t\t\tp = pb_put_tag_%u(p, %u);" EOL, tagsz, tag);
				str_addf(o, "\t\t\tint szlen = pb_u32_size(%s.v[i]->pb_hdr.maxsz);" EOL, mbr.c_str);
				str_add(o, "\t\t\tchar *start = p+szlen;" EOL);
				str_addf(o, "\t\t\tp = pb_encode_%s(start, %s.v[i]);" EOL, ft->proto_suffix.c_str, mbr.c_str);
				str_add(o, "\t\t\tpb_put_finish(p, start, szlen);" EOL);
				str_add(o, "\t\t}" EOL);
			} else if (is_field_packed(t, f)) {
				str_addf(o, "\t\tp = pb_put_tag_%u(p, %u);" EOL, tagsz, tag);
				str_addf(o, "\t\tp = pb_put_packed_%s(p, ", ft->proto_suffix.c_str);
				get_proto_cast(o, f, 1, 1);
				str_addf(o, " %s.v, %s.len);" EOL, mbr.c_str, mbr.c_str);
			} else {
				str_addf(o, "\t\tfor (int i = 0; i < %s.len; i++) {" EOL, mbr.c_str);
				str_addf(o, "\t\t\tp = pb_put_tag_%u(p, %u);" EOL, tagsz, tag);
				str_addf(o, "\t\t\tp = pb_put_%s(p, ", ft->proto_suffix.c_str);
				get_proto_cast(o, f, 0, 1);
				str_addf(o, " %s.v[i]);" EOL, mbr.c_str);
				str_add(o, "\t\t}" EOL);
			}
		} else if (ft->pod_message) {
			str_addf(o, "\t\tp = pb_put_tag_%u(p, %u);" EOL, tagsz, tag);
			str_add(o, "\t\tchar *start = p+1;" EOL);
			str_addf(o, "\t\tp = pb_encode_%s(start, &%s);" EOL, ft->proto_suffix.c_str, mbr.c_str);
			str_add(o, "\t\tstart[-1] = (char) (p - start);" EOL);
        } else if (ft->msg) {
            str_addf(o, "\t\tp = pb_put_tag_%u(p, %u);" EOL, tagsz, tag);
            str_addf(o, "\t\tint szlen = pb_u32_size(%s->pb_hdr.maxsz);" EOL, mbr.c_str);
            str_add(o, "\t\tchar *start = p+szlen;" EOL);
			str_addf(o, "\t\tp = pb_encode_%s(start, %s);" EOL, ft->proto_suffix.c_str, mbr.c_str);
            str_add(o, "\t\tpb_put_finish(p, start, szlen);" EOL);
        } else {
            str_addf(o, "\t\tp = pb_put_tag_%u(p, %u);" EOL, tagsz, tag);
            str_addf(o, "\t\tp = pb_put_%s(p, ", ft->proto_suffix.c_str);
            get_proto_cast(o, f, 0, 1);
            str_addf(o, " %s);" EOL, mbr.c_str);
        }

        str_add(o, "\t}" EOL);
    }

    str_add(o, "\treturn p;" EOL);
    str_add(o, "}" EOL);
}
