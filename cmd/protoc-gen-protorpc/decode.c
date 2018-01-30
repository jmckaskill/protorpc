#include "protoc-gen-protorpc.h"

static void decode_field(str_t *o, const struct type *t, const struct FieldDescriptorProto* f, uint32_t* tagsection) {
    const struct type *ft = get_field_type(f);
    uint32_t tagsz = get_tag_size(f);
    uint32_t tag = get_encoded_tag(f);

    static str_t mbr = STR_INIT;
    str_set(&mbr, "m->");
    str_addstr(&mbr, f->name);

    if (*tagsection == 1 && *tagsection < tagsz) {
        str_add(o, "\tp = pb_toend_1(p,e);" EOL);
        (*tagsection)++;
    }
    if (*tagsection == 2 && *tagsection < tagsz) {
        str_add(o, "\tp = pb_toend_2(p,e);" EOL);
        (*tagsection)++;
    }
    if (*tagsection == 3 && *tagsection < tagsz) {
        str_add(o, "\tp = pb_toend_3(p,e);" EOL);
        (*tagsection)++;
    }

    str_addf(o, "\tif (pb_skipto_%u(&p, e, %u)) {" EOL, tagsz, tag);
    
    // hacks to support bootstapping descriptor.proto as 0 are useful values for these fields
    if (!strcmp(t->name.c_str, ".google.protobuf.FieldDescriptorProto") && !pb_cmp(f->name, "oneof_index.val")) {
        str_add(o, "\t\tm->oneof_index.set = true;" EOL);
    } else if (!strcmp(t->name.c_str, ".google.protobuf.FieldOptions") && !pb_cmp(f->name, "packed")) {
		str_add(o, "\t\tm->packed_set = true;" EOL);
	}


    if (f->oneof_index.set) {
		pb_string_t oneof = t->msg->oneof_decl.v[f->oneof_index.val]->name;
		str_add(o, "\t\tm->");
		str_addstr(o, oneof);
		str_add(o, "_type = ");
		to_upper(o, t->proto_suffix.c_str);
		str_add(o, "_");
		to_upper(o, f->name.c_str);
		str_add(o, ";" EOL);

		str_set(&mbr, "m->");
		str_addstr(&mbr, oneof);
		str_addch(&mbr, '.');
		str_addstr(&mbr, f->name);
    }

	if (f->label == LABEL_REPEATED) {
		if (ft->pod_message) {
			str_addf(o, "\t\t%s.v = (%s*) obj->next;" EOL, mbr.c_str, ft->c_type.c_str);
			str_add(o, "\t\tdo {" EOL);
			str_addf(o, "\t\t\tif (obj->next + %s.len * sizeof(%s) > obj->end) {" EOL, mbr.c_str, ft->c_type.c_str);
			str_add(o, "\t\t\t\treturn -1;" EOL);
			str_add(o, "\t\t\t}" EOL);
			str_add(o, "\t\t\tpb_string_t msg;" EOL);
			str_addf(o, "\t\t\tp = pb_get_string(p + %u, e, &msg);" EOL, tagsz);
			str_addf(o, "\t\t\tif (pb_get_%s(msg.c_str, msg.c_str + msg.len, (%s*) &%s.v[%s.len])) {" EOL, ft->proto_suffix.c_str, ft->c_type.c_str, mbr.c_str, mbr.c_str);
			str_add(o, "\t\t\t\treturn -1;" EOL);
			str_add(o, "\t\t\t}" EOL);
			str_addf(o, "\t\t\t%s.len++;" EOL, mbr.c_str);
			str_addf(o, "\t\t} while (!pb_cmp_tag_%u(p, e, %u));" EOL, tagsz, tag);
			str_add(o, EOL);
			str_addf(o, "\t\tobj->next += %s.len * sizeof(%s);" EOL, mbr.c_str, ft->c_type.c_str);
		} else if (ft->msg) {
			str_addf(o, "\t\t%s *prev = NULL;" EOL, ft->c_type.c_str);
			str_add(o, "\t\tdo {" EOL);
			str_add(o, "\t\t\tpb_string_t msg;" EOL);
			str_addf(o, "\t\t\tp = pb_get_string(p + %u, e, &msg);" EOL, tagsz);
			str_addf(o, "\t\t\t%s *c = (%s*) pb_calloc(obj, sizeof(%s));" EOL, ft->c_type.c_str, ft->c_type.c_str, ft->c_type.c_str);
			str_addf(o, "\t\t\tif (!c || pb_get_%s(msg.c_str, msg.c_str + msg.len, obj, c)) {" EOL, ft->proto_suffix.c_str);
			str_add(o, "\t\t\t\treturn -1;" EOL);
			str_add(o, "\t\t\t}" EOL);
			str_addf(o, "\t\t\t%s.len++;" EOL, mbr.c_str);
			str_add(o, "\t\t\tc->pb_hdr.prev = prev;" EOL);
			str_add(o, "\t\t\tprev = c;" EOL);
			str_addf(o, "\t\t} while (!pb_cmp_tag_%u(p, e, %u));" EOL, tagsz, tag);
			str_add(o, EOL);
			str_addf(o, "\t\t%s.v = (const %s**) pb_calloc(obj, %s.len * sizeof(%s*));" EOL, mbr.c_str, ft->c_type.c_str, mbr.c_str, ft->c_type.c_str);
			str_addf(o, "\t\tif (!%s.v) {" EOL, mbr.c_str);
			str_add(o, "\t\t\treturn -1;" EOL);
			str_add(o, "\t\t}" EOL);
			str_add(o, EOL);
			str_addf(o, "\t\tfor (int i = %s.len - 1; i >= 0; i--) {" EOL, mbr.c_str);
			str_addf(o, "\t\t\t%s.v[i] = prev;" EOL, mbr.c_str);
			str_addf(o, "\t\t\tprev = (%s*) prev->pb_hdr.prev;" EOL, ft->c_type.c_str);
			str_add(o, "\t\t}" EOL);
		} else if (is_field_packed(t, f)) {
			str_addf(o, "\t\tp = pb_get_packed_%s(p + %u, e, obj, ", ft->proto_suffix.c_str, tagsz);
			get_proto_cast(o, f, 1, 0);
			str_addf(o, "&%s.v, &%s.len);" EOL, mbr.c_str, mbr.c_str);
		} else {
			str_addf(o, "\t\t%s.v = (%s*) obj->next;" EOL, mbr.c_str, ft->c_type.c_str);
			str_add(o, "\t\tdo {" EOL);
			str_addf(o, "\t\t\tif (obj->next + %s.len * sizeof(%s) > obj->end) {" EOL, mbr.c_str, ft->c_type.c_str);
			str_add(o, "\t\t\t\treturn -1;" EOL);
			str_add(o, "\t\t\t}" EOL);
			str_addf(o, "\t\t\tp = pb_get_%s(p + %u, e, ", ft->proto_suffix.c_str, tagsz);
			get_proto_cast(o, f, 0, 0);
			str_addf(o, "&%s.v[%s.len]);" EOL, mbr.c_str, mbr.c_str);
			str_addf(o, "\t\t\t%s.len++;" EOL, mbr.c_str);
			str_addf(o, "\t\t} while (!pb_cmp_tag_%u(p, e, %u));" EOL, tagsz, tag);
			str_add(o, EOL);
			str_addf(o, "\t\tobj->next += %s.len * sizeof(%s);" EOL, mbr.c_str, ft->c_type.c_str);
		}
    } else if (ft->msg) {
        str_add(o, "\t\tpb_string_t msg;" EOL);
        str_addf(o, "\t\tp = pb_get_string(p + %u, e, &msg);" EOL, tagsz);
		if (ft->pod_message) {
			str_addf(o, "\t\tif (pb_get_%s(msg.c_str, msg.c_str + msg.len, &%s)) {" EOL, ft->proto_suffix.c_str, mbr.c_str);
			str_add(o, "\t\t\treturn -1;" EOL);
			str_add(o, "\t\t}" EOL);
		} else {
			str_addf(o, "\t\t%s = (%s*) pb_calloc(obj, sizeof(%s));" EOL, mbr.c_str, ft->c_type.c_str, ft->c_type.c_str);
			str_addf(o, "\t\tif (!%s || pb_get_%s(msg.c_str, msg.c_str + msg.len, obj, (%s*) %s)) {" EOL, mbr.c_str, ft->proto_suffix.c_str, ft->c_type.c_str, mbr.c_str);
			str_add(o, "\t\t\treturn -1;" EOL);
			str_add(o, "\t\t}" EOL);
		}
    } else {
        str_addf(o, "\t\tp = pb_get_%s(p + %u, e, ", ft->proto_suffix.c_str, tagsz);
        get_proto_cast(o, f, 0, 0);
        str_addf(o, "&%s);" EOL, mbr.c_str);
    }

    str_add(o, "\t}" EOL);
}

void do_decode(str_t *o, const struct type *t, bool define) {
	str_addf(o, "int pb_get_%s(const char *p, const char *e", t->proto_suffix.c_str);
	if (!t->pod_message) {
		str_add(o, ", pb_buf_t *obj");
	}
	str_addf(o, ", %s *m)", t->c_type.c_str);
    if (!define) {
        str_add(o, ";" EOL);
        return;
    }
    str_add(o, " {" EOL);
	if (!t->pod_message) {
		str_add(o, "\t(void) obj;" EOL);
	}

    uint32_t tagsection = 1;
    for (int i = 0; i < t->msg->field.len; i++) {
        decode_field(o, t, t->msg->field.v[i], &tagsection);
    }

	str_add(o, "\treturn 0;" EOL);
    str_add(o, "}" EOL);
}
