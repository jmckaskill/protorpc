#include "protoc-gen-protorpc.h"
#include <assert.h>

struct parser {
    pb_string_t str;
    uint32_t off;
    const struct FieldDescriptorProto *field;
    const struct EnumValueDescriptorProto *en;
};


void do_parse_enum(str_t *o, const struct type *t, bool define) {
    str_addf(o, "const char *pb_parse_%s(const char *p, %s *v)", t->json_suffix.c_str, t->c_type.c_str);
    if (!define) {
        str_add(o, ";" EOL);
        return;
    }
    str_add(o, " {" EOL);

    struct hash_entry *hashes = (struct hash_entry*) calloc(t->en->value.len, sizeof(struct hash_entry));
    for (int i = 0; i < t->en->value.len; i++) {
        hashes[i].str = t->en->value.v[i]->name;
    }

    uint32_t hashsz, hashmul;
    calc_hash_values(hashes, t->en->value.len, &hashmul, &hashsz);

    str_add(o, "\tpb_string_t val;" EOL);
    str_addf(o, "\t*v = (%s) 0;" EOL, t->c_type.c_str);
    str_addf(o, "\tswitch (pb_parse_enum(&p, &val, %u) %% %u) {" EOL, hashmul, hashsz);

    for (int i = 0; i < t->en->value.len; i++) {
		struct hash_entry *h = &hashes[i];
		const struct EnumValueDescriptorProto *v = t->en->value.v[i];
        str_addf(o, "\tcase %u:" EOL, h->off);
        str_addf(o, "\t\tif (!pb_cmp(val, \"%s\")) {" EOL, h->str.c_str);
        str_addf(o, "\t\t\t*v = (%s) %d;" EOL, t->c_type.c_str, v->number);
        str_add(o, "\t\t}" EOL);
        str_add(o, "\t\tbreak;" EOL);
    }

    str_add(o, "\t}" EOL); // switch
    str_add(o, "\treturn p;" EOL);
    str_add(o, "}" EOL);

    free(hashes);
}

void do_parse(str_t *o, const struct type *t, bool define) {
    str_addf(o, "const char *pb_parse_%s(const char *p", t->json_suffix.c_str);
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
	str_add(o, "\t(void) m;" EOL);

    struct hash_entry *hashes = (struct hash_entry*) calloc(t->msg->field.len, sizeof(struct hash_entry));

    for (int i = 0; i < t->msg->field.len; i++) {
        hashes[i].str = t->msg->field.v[i]->name;
    }

    uint32_t hashsz, hashmul;
    calc_hash_values(hashes, t->msg->field.len, &hashmul, &hashsz);
    
    str_add(o, "\tif (!pb_parse_map(&p)) {" EOL);
    str_add(o, "\t\treturn p;" EOL);
    str_add(o, "\t}" EOL);
    str_add(o, "\tfor (;;) {" EOL);
    str_add(o, "\t\tpb_string_t key;" EOL);
    str_addf(o, "\t\tswitch (pb_parse_field(&p, &key, %u) %% %u) {" EOL, hashmul, hashsz);
    str_add(o, "\t\tcase 0:" EOL);
    str_add(o, "\t\t\treturn p;" EOL);

    for (int i = 0; i < t->msg->field.len; i++) {
        struct hash_entry *h = &hashes[i];
        const struct FieldDescriptorProto *f = t->msg->field.v[i];
        const struct type *ft = get_field_type(f);

        static str_t mbr = STR_INIT;
        str_set(&mbr, "m->");
        str_addstr(&mbr, f->name);

        str_addf(o, "\t\tcase %u:" EOL, h->off);
        str_addf(o, "\t\t\tif (pb_cmp(key, \"%s\")) {" EOL, h->str.c_str);
        str_add(o, "\t\t\t\tgoto unknown;" EOL);
        str_add(o, "\t\t\t}"  EOL);

        if (f->oneof_index_set) {
            pb_string_t oneof = t->msg->oneof_decl.v[f->oneof_index]->name;
            str_add(o, "\t\t\tm->");
            str_addstr(o, oneof);
            str_add(o, "_type = ");
            pb_string_t ps = {t->proto_suffix.len, t->proto_suffix.c_str};
            to_upper(o, ps);
            str_add(o, "_");
            to_upper(o, f->name);
            str_add(o, ";" EOL);

            str_set(&mbr, "m->");
            str_addstr(&mbr, oneof);
            str_addch(&mbr, '.');
            str_addstr(&mbr, f->name);
        }

        if (f->label == LABEL_REPEATED) {
			if (ft->msg && !ft->pod_message) {
                str_add(o, "\t\t\tif (pb_parse_array(&p)) {" EOL);
                str_addf(o, "\t\t\t\t%s *prev = NULL;" EOL, ft->c_type.c_str);
                str_add(o, "\t\t\t\tdo {" EOL);
                str_addf(o, "\t\t\t\t\t%s *c = (%s*) pb_calloc(obj, sizeof(%s));" EOL, ft->c_type.c_str, ft->c_type.c_str, ft->c_type.c_str);
				str_add(o, "\t\t\t\t\tif (!c) {return pb_errret;}" EOL);
                str_addf(o, "\t\t\t\t\tp = pb_parse_%s(p, obj, c);" EOL, ft->json_suffix.c_str);
                str_addf(o, "\t\t\t\t\t%s.len++;" EOL, mbr.c_str);
                str_add(o, "\t\t\t\t\tc->pb_hdr.prev = prev;" EOL);
                str_add(o, "\t\t\t\t\tprev = c;" EOL);
                str_add(o, "\t\t\t\t} while (pb_more_array(&p));" EOL);
                str_add(o, EOL);
                str_addf(o, "\t\t\t\t%s.v = (const %s**) pb_calloc(obj, %s.len * sizeof(%s*));" EOL, mbr.c_str, ft->c_type.c_str, mbr.c_str, ft->c_type.c_str);
				str_addf(o, "\t\t\t\tif (!%s.v) {return pb_errret;}" EOL, mbr.c_str);
                str_add(o, EOL);
                str_addf(o, "\t\t\t\tfor (int i = %s.len - 1; i >= 0; i--) {" EOL, mbr.c_str);
                str_addf(o, "\t\t\t\t\t%s.v[i] = prev;" EOL, mbr.c_str);
                str_addf(o, "\t\t\t\t\tprev = (%s*) prev->pb_hdr.prev;" EOL, ft->c_type.c_str);
                str_add(o, "\t\t\t\t}" EOL);
                str_add(o, "\t\t\t}" EOL);
			} else if (ft->en || ft->pod_message) {
                str_add(o, "\t\t\tif (pb_parse_array(&p)) {" EOL);
                str_add(o, "\t\t\t\tint n = 0;" EOL);
				str_addf(o, "\t\t\t\t%s.v = (%s*) obj->next;" EOL, mbr.c_str, ft->c_type.c_str);
                str_add(o, "\t\t\t\tdo {" EOL);
				str_addf(o, "\t\t\t\t\tif (obj->next + n * sizeof(%s) > obj->end) {return pb_errret;}" EOL, ft->c_type.c_str);
				str_addf(o, "\t\t\t\t\tp = pb_parse_%s(p, (%s*) &%s.v[n++]); " EOL, ft->json_suffix.c_str, ft->c_type.c_str, mbr.c_str);
                str_add(o, "\t\t\t\t} while (pb_more_array(&p));" EOL);
				str_addf(o, "\t\t\t\tobj->next += n * sizeof(%s);" EOL, ft->c_type.c_str);
                str_addf(o, "\t\t\t\t%s.len = n;" EOL, mbr.c_str);
                str_add(o, "\t\t\t}" EOL);
			} else {
                str_addf(o, "\t\t\tp = pb_parse_array_%s(p, obj, &%s.v, &%s.len);" EOL, ft->json_suffix.c_str, mbr.c_str, mbr.c_str);
			}
        } else if (ft->msg && !ft->pod_message) {
            str_addf(o, "\t\t\t%s = (%s*) pb_calloc(obj, sizeof(%s));" EOL, mbr.c_str, ft->c_type.c_str, ft->c_type.c_str);
			str_addf(o, "\t\t\tif (!%s) {return pb_errret;}" EOL, mbr.c_str);
            str_addf(o, "\t\t\tp = pb_parse_%s(p, obj, (%s*) %s);" EOL, ft->json_suffix.c_str, ft->c_type.c_str, mbr.c_str);
        } else {
            str_addf(o, "\t\t\tp = pb_parse_%s(p, &%s);" EOL, ft->json_suffix.c_str, mbr.c_str);
        }
        str_add(o, "\t\t\tcontinue;" EOL);
    }
    str_add(o, "\t\tdefault:" EOL);
    if (t->msg->field.len) {
		str_add(o, "\t\tunknown:" EOL);
	}
    str_add(o, "\t\t\tp = pb_parse_skip(p);" EOL);
    str_add(o, "\t\t}" EOL); // switch
    str_add(o, "\t}" EOL); // for
    str_add(o, "}" EOL); // function

    free(hashes);
}
