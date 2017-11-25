#include "protoc-gen-protorpc.h"

void do_term(str_t *o, const struct type *t, bool define) {
	str_addf(o, "void pb_term_%s(%s *m)", t->proto_suffix.buf, t->c_type.buf);
	if (!define) {
		str_add(o, ";" EOL);
		return;
	}
	str_add(o, " {" EOL);

	static str_t mbr = STR_INIT;

	for (int i = 0; i < t->msg->field.len; i++) {
		const struct FieldDescriptorProto *f = t->msg->field.v[i];
		const struct type *ft = get_field_type(f);

		int begin = o->len;
		bool remove = false;

		str_set(&mbr, "m->");
		str_addstr(&mbr, f->name);
		const char *pfx = "\t";

		if (f->oneof_index_set) {
			struct pb_string oneof = t->msg->oneof_decl.v[f->oneof_index]->name;
			str_add(o, "\tif(m->");
			str_addstr(o, oneof);
			str_add(o, "_type == ");
			struct pb_string ps = { t->proto_suffix.len, t->proto_suffix.buf };
			to_upper(o, ps);
			str_add(o, "_");
			to_upper(o, f->name);
			str_add(o, ") {" EOL);

			str_set(&mbr, "m->");
			str_addstr(&mbr, oneof);
			str_addch(&mbr, '.');
			str_addstr(&mbr, f->name);

			pfx = "\t\t";
		}

		if (f->label == LABEL_REPEATED) {
			if (ft->pod_message) {
				str_addf(o, "%sfor (int i = 0; i < %s.len; i++) {" EOL, pfx, mbr.buf);
				str_addf(o, "%s\tpb_term_%s((%s*) &%s.v[i]);" EOL, pfx, ft->proto_suffix.buf, ft->c_type.buf, mbr.buf);
				str_addf(o, "%s}" EOL, pfx);
			} else if (ft->msg) {
				str_addf(o, "%sfor (int i = 0; i < %s.len; i++) {" EOL, pfx, mbr.buf);
				str_addf(o, "%s\tpb_term_%s((%s*) %s.v[i]);" EOL, pfx, ft->proto_suffix.buf, ft->c_type.buf, mbr.buf);
				str_addf(o, "%s}" EOL, pfx);
			} else if (f->type == TYPE_STRING) {
				str_addf(o, "%sfor (int i = 0; i < %s.len; i++) {" EOL, pfx, mbr.buf);
				str_addf(o, "%s\tif (%s.v[i].buf) {" EOL, pfx, mbr.buf);
				str_addf(o, "%s\t\t((char*)%s.v[i].buf)[%s.v[i].len] = '\\0';" EOL, pfx, mbr.buf, mbr.buf);
				str_addf(o, "%s\t} else {" EOL, pfx);
				str_addf(o, "%s\t\t((struct pb_string*)%s.v)[i].buf = \"\";" EOL, pfx, mbr.buf);
				str_addf(o, "%s\t}" EOL, pfx);
				str_addf(o, "%s}" EOL, pfx);
			} else {
				remove = true;
			}
		} else if (ft->pod_message) {
			str_addf(o, "%spb_term_%s((%s*) &%s);" EOL, pfx, ft->proto_suffix.buf, ft->c_type.buf, mbr.buf);
		} else if (ft->msg) {
			str_addf(o, "%sif (%s) {" EOL, pfx, mbr.buf);
			str_addf(o, "%s\tpb_term_%s((%s*) %s);" EOL, pfx, ft->proto_suffix.buf, ft->c_type.buf, mbr.buf);
			str_addf(o, "%s}" EOL, pfx);
		} else if (f->type == TYPE_STRING) {
			str_addf(o, "%sif (%s.buf) {" EOL, pfx, mbr.buf);
			str_addf(o, "%s\t((char*)%s.buf)[%s.len] = '\\0';" EOL, pfx, mbr.buf, mbr.buf);
			str_addf(o, "%s} else {" EOL, pfx);
			str_addf(o, "%s\t%s.buf = \"\";" EOL, pfx, mbr.buf);
			str_addf(o, "%s}" EOL, pfx);
		} else {
			remove = true;
		}

		if (f->oneof_index_set) {
			str_add(o, "\t}" EOL);
		}

		if (remove) {
			str_setlen(o, begin);
		}
	}
	str_add(o, "}" EOL);
}
