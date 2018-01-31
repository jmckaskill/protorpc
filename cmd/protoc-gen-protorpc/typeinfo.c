#include "protoc-gen-protorpc.h"

static const char *field_type(const struct FieldDescriptorProto *f, const struct type *ft) {
	if (f->label == LABEL_REPEATED) {
		if (ft->pod_message) {
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
		if (ft->pod_message) {
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

void do_fields_by_name(str_t *o, const struct type *t) {
	str_addf(o, "static const pb_string_t *by_name_%s[] = {" EOL, t->proto_suffix.c_str);
	for (int i = 0; i < t->msg->field.len; i++) {
		const struct FieldDescriptorProto *f = t->msg->field.v[i];
		if (i) {
			str_addf(o, "," EOL);
		}
		str_addf(o, "\t&fields_%s[%d].json_name", t->proto_suffix.c_str, f->by_number_index);
	}
	str_addf(o, EOL "};" EOL);
}

void do_fields_by_number(str_t *o, const struct type *t) {
	str_addf(o, "static const struct proto_field fields_%s[] = {" EOL, t->proto_suffix.c_str);
	for (int i = 0; i < t->msg->field.len; i++) {
		const struct FieldDescriptorProto *f = t->msg->field.v[i];
		const struct type *ft = get_field_type(f);
		uint32_t tag = get_tag(f);
		const char *type_enum = field_type(f, ft);
		const char *field_name = f->name.c_str;
		if (f->oneof_index.set) {
			field_name = t->msg->oneof_decl.v[f->oneof_index.val]->name.c_str;
		}
		if (i) {
			str_addf(o, "," EOL);
		}
		str_addf(o, "\t{{%d, \"%s\"}, %s, offsetof(%s, %s), %u, ", f->json_name.len, f->json_name.c_str, type_enum, t->c_type.c_str, field_name, tag);
		if (ft->msg) {
			str_addf(o, "&pb_type_%s,", ft->proto_suffix.c_str);
		} else if (f->type == TYPE_ENUM) {
			str_addf(o, "&pb_enum_%s,", ft->json_suffix.c_str);
		} else {
			str_addf(o, "NULL,");
		}
		if (f->oneof_index.set) {
			str_addf(o, " offsetof(%s, %s_type)}", t->c_type.c_str, field_name);
		} else {
			str_add(o, " -1}");
		}
	}
	str_addf(o, EOL "};" EOL);
}

void declare_typeinfo(str_t *o, const struct type *t) {
	str_addf(o, "extern const struct proto_message pb_type_%s;" EOL, t->proto_suffix.c_str);
}

void do_typeinfo(str_t *o, const struct type *t) {
	str_addf(o, "const struct proto_message pb_type_%s = {" EOL, t->proto_suffix.c_str);
	str_addf(o, "\tsizeof(%s)," EOL, t->c_type.c_str);
	str_addf(o, "\tsizeof(fields_%s) / sizeof(struct proto_field)," EOL, t->proto_suffix.c_str);
	str_addf(o, "\tfields_%s," EOL, t->proto_suffix.c_str);
	str_addf(o, "\tby_name_%s" EOL, t->proto_suffix.c_str);
	str_addf(o, "};" EOL);
}

void declare_enuminfo(str_t *o, const struct type *t) {
	str_addf(o, "extern const struct proto_enum pb_enum_%s;" EOL, t->json_suffix.c_str);
}

void do_enums_by_number(str_t *o, const struct type *t) {
	str_addf(o, "static const struct proto_enum_value values_%s[] = {" EOL, t->json_suffix.c_str);
	for (int i = 0; i < t->en->value.len; i++) {
		const struct EnumValueDescriptorProto *v = t->en->value.v[i];
		if (i) {
			str_addf(o, "," EOL);
		}
		str_addf(o, "\t{{%d, \"%s\"}, %d}", v->name.len, v->name.c_str, v->number);
	}
	str_add(o, EOL "};" EOL);
}

void do_enums_by_name(str_t *o, const struct type *t) {
	str_addf(o, "static const pb_string_t *by_name_%s[] = {" EOL, t->json_suffix.c_str);
	for (int i = 0; i < t->en->value.len; i++) {
		const struct EnumValueDescriptorProto *v = t->en->value.v[i];
		if (i) {
			str_addf(o, "," EOL);
		}
		str_addf(o, "\t{&values_%s[%d].name}", t->json_suffix.c_str, v->by_number_index);
	}
	str_add(o, EOL "};" EOL);
}

void do_enuminfo(str_t *o, const struct type *t) {
	str_addf(o, "const struct proto_enum pb_enum_%s = {" EOL, t->json_suffix.c_str);
	str_addf(o, "\tsizeof(values_%s) / sizeof(struct proto_enum_value)," EOL, t->json_suffix.c_str);
	str_addf(o, "\tvalues_%s," EOL, t->json_suffix.c_str);
	str_addf(o, "\tby_name_%s" EOL, t->json_suffix.c_str);
	str_addf(o, "};" EOL);
}

