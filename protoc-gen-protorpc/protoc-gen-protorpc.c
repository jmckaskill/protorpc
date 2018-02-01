#include "protoc-gen-protorpc.h"
#include <protorpc/ptr-array.h>

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

#define ALLOC_SIZE (1*1024*1024)

static void add_upper(str_t *o, const char *str) {
	while (*str) {
		if ('a' <= *str && *str <= 'z') {
			str_addch(o, *str + 'A' - 'a');
		} else {
			str_addch(o, *str);
		}
		str++;
	}
}

static void declare_oneof(str_t *o, const proto_type *t, unsigned oneof_idx) {
	OneofDescriptorProto *oneof = t->msg->oneof_decl.v[oneof_idx];

	str_add(o, EOL);
	str_addf(o, "enum %s_%s {" EOL, t->c_type.c_str, oneof->name.c_str);
	bool first = true;

	for (int i = 0; i < t->msg->field.len; i++) {
		FieldDescriptorProto *f = t->msg->field.v[i];
		if (f->oneof_index.set && f->oneof_index.val == oneof_idx) {
			if (!first) {
				str_add(o, "," EOL);
			}
			first = false;
			str_addch(o, '\t');
			add_upper(o, t->c_type.c_str);
			str_addch(o, '_');
			add_upper(o, f->name.c_str);
			str_addf(o, " = %d", f->number);
		}
	}

	str_add(o, EOL "};" EOL);
}

static void append_field(str_t *o, const FieldDescriptorProto *f) {
	proto_type *ft = get_field_type(f);
	if (f->label == LABEL_REPEATED) {
		if (ft->is_pod) {
			str_addf(o, "\tstruct {int len; %s *v;} %s;" EOL, ft->c_type.c_str, f->name.c_str);
		} else if (ft->msg) {
			str_addf(o, "\tstruct {int len; %s **v;} %s;" EOL, ft->c_type.c_str, f->name.c_str);
		} else if (ft->en) {
			str_addf(o, "\tstruct {int len; %s *v; int _encoded;} %s;" EOL, ft->c_type.c_str, f->name.c_str);
		} else {
			str_addf(o, "\t%s %s;" EOL, ft->list_type.c_str, f->name.c_str);
		}
	} else {
		if (ft->msg && !ft->is_pod) {
			str_addf(o, "\t%s *%s;" EOL, ft->c_type.c_str, f->name.c_str);
		} else {
			// including enums & pods
			str_addf(o, "\t%s %s;" EOL, ft->c_type.c_str, f->name.c_str);
		}
	}
}

static void define_message(str_t *o, proto_type *t) {
	for (int i = 0; i < t->msg->field.len; i++) {
		FieldDescriptorProto *f = t->msg->field.v[i];
		proto_type *ft = get_field_type(f);
		if (f->label != LABEL_REPEATED && ft->is_pod && !ft->defined) {
			define_message(o, ft);
		}
	}

	for (int i = 0; i < t->msg->oneof_decl.len; i++) {
		declare_oneof(o, t, i);
	}

	str_add(o, EOL);
	str_addf(o, "struct %s {" EOL, t->c_type.c_str);
	if (!t->is_pod) {
		str_add(o, "\tpb_msg _pbhdr;" EOL);
	}
	for (int i = 0; i < t->msg->field.len;) {
		FieldDescriptorProto *f = t->msg->field.v[i];

		if (f->oneof_index.set) {
			unsigned oneof = f->oneof_index.val;
			const char *oneof_name = t->msg->oneof_decl.v[oneof]->name.c_str;
			str_addf(o, "\tenum %s_%s %s_type;" EOL, t->c_type.c_str, oneof_name, oneof_name);
			str_add(o, "\tunion {" EOL);

			while (i < t->msg->field.len) {
				f = t->msg->field.v[i];
				if (!f->oneof_index.set || f->oneof_index.val != oneof) {
					break;
				}
				str_add(o, "\t");
				append_field(o, f);
				i++;
			}

			str_addf(o, "\t} %s;" EOL, oneof_name);
		} else {
			append_field(o, f);
			i++;
		}
	}
	str_add(o, "};" EOL);
	t->defined = true;
}

static void define_enum(str_t *o, const proto_type *t) {
	str_add(o, EOL);
	str_addf(o, "enum %s {" EOL, t->c_type.c_str);
	for (int i = 0; i < t->en->value.len; i++) {
		EnumValueDescriptorProto *v = t->en->value.v[i];
		if (i) {
			str_add(o, "," EOL);
		}
		str_addf(o, "\t%s = %d", v->name.c_str, v->number);
	}
	str_add(o, EOL "};" EOL);
}

static void define_service(str_t *o, const proto_type *t) {
	str_add(o, EOL);
	str_addf(o, "struct %s {" EOL, t->c_type.c_str);
	for (int i = 0; i < t->svc->method.len; i++) {
		MethodDescriptorProto *m = t->svc->method.v[i];
		proto_type *it = get_type(m->input_type);
		proto_type *ot = get_type(m->output_type);
		str_addf(o, "\tint (*%s)(%s *svc, pb_allocator *obj, const %s *in, %s *out);" EOL,
			m->name.c_str, t->c_type.c_str, it->c_type.c_str, ot->c_type.c_str);
	}
	str_add(o, "};" EOL);
}

static void write_header(str_t *o, const FileDescriptorProto *f) {
	int num;
	proto_type **types = get_all_types(&num);

	// includes
	str_add(o, "#pragma once" EOL);
	str_add(o, "#include <protorpc/protorpc.h>" EOL);

	for (int i = 0; i < f->dependency.len; i++) {
		str_add(o, "#include \"");
		str_addstr(o, f->dependency.v[i]);
		str_add(o, ".h\"" EOL);
	}

	str_add(o, "#ifdef __cplusplus" EOL);
	str_add(o, "extern \"C\" {" EOL);
	str_add(o, "#endif" EOL);

	// typedefs
	str_add(o, EOL);
	for (int i = 0; i < num; i++) {
		proto_type *t = types[i];
		if (t->file == f && (t->msg || t->svc)) {
			str_addf(o, "typedef struct %s %s;" EOL, t->c_type.c_str, t->c_type.c_str);
		} else if (t->file == f && t->en) {
			str_addf(o, "typedef enum %s %s;" EOL, t->c_type.c_str, t->c_type.c_str);
		}
	}

	// typeinfos
	str_add(o, EOL);
	for (int i = 0; i < num; i++) {
		proto_type *t = types[i];
		if (t->file == f && t->msg) {
			str_addf(o, "extern const proto_message proto_%s;" EOL, t->c_type.c_str);
		} else if (t->file == f && t->en) {
			str_addf(o, "extern const proto_enum proto_%s;" EOL, t->c_type.c_str);
		} else if (t->file == f && t->svc) {
			str_addf(o, "extern const proto_service proto_%s;" EOL, t->c_type.c_str);
		}
	}

	// struct/enum definitions
	for (int i = 0; i < num; i++) {
		proto_type *t = types[i];
		if (t->file == f && t->msg) {
			define_message(o, t);
		} else if (t->file == f && t->en) {
			define_enum(o, t);
		} else if (t->file == f && t->svc) {
			define_service(o, t);
		}
	}

	str_add(o, EOL);
	str_add(o, "#ifdef __cplusplus" EOL);
	str_add(o, "}" EOL);
	str_add(o, "#endif" EOL);
	str_add(o, EOL);
}

static int cmp_enum_number(const void **a, const void **b) {
	EnumValueDescriptorProto *va = *(EnumValueDescriptorProto**)a;
	EnumValueDescriptorProto *vb = *(EnumValueDescriptorProto**)b;
	return va->number - vb->number;
}

static int cmp_enum_name(const void **a, const void **b) {
	EnumValueDescriptorProto *va = *(EnumValueDescriptorProto**)a;
	EnumValueDescriptorProto *vb = *(EnumValueDescriptorProto**)b;
	return compare_string(va->name.c_str, va->name.len, vb->name.c_str, vb->name.len);
}

static int cmp_field_number(const void **a, const void **b) {
	FieldDescriptorProto *fa = *(FieldDescriptorProto**)a;
	FieldDescriptorProto *fb = *(FieldDescriptorProto**)b;
	return fa->number - fb->number;
}

static int cmp_field_name(const void **a, const void **b) {
	FieldDescriptorProto *fa = *(FieldDescriptorProto**)a;
	FieldDescriptorProto *fb = *(FieldDescriptorProto**)b;
	return compare_string(fa->json_name.c_str, fa->json_name.len, fb->json_name.c_str, fb->json_name.len);
}

static int cmp_method_name(const void *a, const void *b) {
	MethodDescriptorProto *ma = *(MethodDescriptorProto**)a;
	MethodDescriptorProto *mb = *(MethodDescriptorProto**)b;
	return compare_string(ma->name.c_str, ma->name.len, mb->name.c_str, mb->name.len);
}

static void do_fields_by_number(str_t *o, const proto_type *t) {
	str_addf(o, "static const struct proto_field fields_%s[] = {" EOL, t->c_type.c_str);
	for (int i = 0; i < t->msg->field.len; i++) {
		FieldDescriptorProto *f = t->msg->field.v[i];
		proto_type *ft = get_field_type(f);
		uint32_t tag = get_tag(f);
		const char *type_enum = field_decode_type(f, ft);
		const char *field_name = f->name.c_str;
		if (f->oneof_index.set) {
			field_name = t->msg->oneof_decl.v[f->oneof_index.val]->name.c_str;
		}
		if (i) {
			str_addf(o, "," EOL);
		}
		str_addf(o, "\t{{%d, \"%s\"}, %s, offsetof(%s, %s), %u, ", f->json_name.len, f->json_name.c_str, type_enum, t->c_type.c_str, field_name, tag);
		if (ft->msg || f->type == TYPE_ENUM) {
			str_addf(o, "&proto_%s,", ft->c_type.c_str);
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

static void do_fields_by_name(str_t *o, const proto_type *t) {
	str_addf(o, "static const pb_string *by_name_%s[] = {" EOL, t->c_type.c_str);
	for (int i = 0; i < t->msg->field.len; i++) {
		FieldDescriptorProto *f = t->msg->field.v[i];
		if (i) {
			str_addf(o, "," EOL);
		}
		str_addf(o, "\t&fields_%s[%d].json_name", t->c_type.c_str, f->by_number_index);
	}
	str_addf(o, EOL "};" EOL);
}

static void do_typeinfo(str_t *o, const proto_type *t) {
	str_addf(o, "const proto_message proto_%s = {" EOL, t->c_type.c_str);
	str_addf(o, "\tsizeof(%s)," EOL, t->c_type.c_str);
	str_addf(o, "\t%d," EOL, t->msg->field.len);
	str_addf(o, "\tfields_%s," EOL, t->c_type.c_str);
	str_addf(o, "\tby_name_%s" EOL, t->c_type.c_str);
	str_addf(o, "};" EOL);
}

static void do_enum_by_number(str_t *o, const proto_type *t) {
	str_addf(o, "static const proto_enum_value values_%s[] = {" EOL, t->c_type.c_str);
	for (int i = 0; i < t->en->value.len; i++) {
		EnumValueDescriptorProto *v = t->en->value.v[i];
		if (i) {
			str_addf(o, "," EOL);
		}
		str_addf(o, "\t{{%d, \"%s\"}, %d}", v->name.len, v->name.c_str, v->number);
	}
	str_add(o, EOL "};" EOL);
}

static void do_enum_by_name(str_t *o, const proto_type *t) {
	str_addf(o, "static const pb_string *by_name_%s[] = {" EOL, t->c_type.c_str);
	for (int i = 0; i < t->en->value.len; i++) {
		const struct EnumValueDescriptorProto *v = t->en->value.v[i];
		if (i) {
			str_addf(o, "," EOL);
		}
		str_addf(o, "\t{&values_%s[%d].name}", t->c_type.c_str, v->by_number_index);
	}
	str_add(o, EOL "};" EOL);
}

static void do_enuminfo(str_t *o, const proto_type *t) {
	str_addf(o, "const proto_enum proto_%s = {" EOL, t->c_type.c_str);
	str_addf(o, "\t%d," EOL, t->en->value.len);
	str_addf(o, "\tvalues_%s," EOL, t->c_type.c_str);
	str_addf(o, "\tby_name_%s" EOL, t->c_type.c_str);
	str_addf(o, "};" EOL);
}

static void do_method_by_index(str_t *o, const proto_type *t) {
	for (int i = 0; i < t->svc->method.len; i++) {
		// Note the proto_type has a leading . that we want to remove
		MethodDescriptorProto *m = t->svc->method.v[i];
		proto_type *it = get_type(m->input_type);
		proto_type *ot = get_type(m->output_type);
		str_addf(o, "static const proto_method method_%s_%s = {" EOL, t->c_type.c_str, m->name.c_str);
		str_addf(o, "\t{%d, \"/twirp/%s/%s\"}," EOL,
			7 /*/twirp*/ + (t->proto_type.len-1) + 1 + m->name.len, t->proto_type.c_str+1, m->name.c_str);
		str_addf(o, "\t%d," EOL, i);
		str_addf(o, "\t&proto_%s," EOL, it->c_type.c_str);
		str_addf(o, "\t&proto_%s" EOL, ot->c_type.c_str);
		str_add(o, "};" EOL);
	}
}

static void do_method_by_name(str_t *o, const proto_type *t) {
	str_addf(o, "static const pb_string *by_name_%s[] = {" EOL, t->c_type.c_str);
	for (int i = 0; i < t->svc->method.len; i++) {
		MethodDescriptorProto *m = t->svc->method.v[i];
		str_addf(o, "\t&method_%s_%s.path," EOL, t->c_type.c_str, m->name.c_str);
	}
	str_addf(o, "};" EOL);
}

static void do_svcinfo(str_t *o, const proto_type *t) {
	str_addf(o, "const proto_service proto_%s = {" EOL, t->c_type.c_str);
	str_addf(o, "\t%d," EOL, t->svc->method.len);
	str_addf(o, "\tby_name_%s" EOL, t->c_type.c_str);
	str_addf(o, "};" EOL);
}

static void write_source(str_t *o, const FileDescriptorProto *f) {
	int num;
	proto_type **types = get_all_types(&num);

	// includes
	str_add(o, "#include \"");
	str_addstr(o, f->name);
	str_add(o, ".h\"" EOL);

	// typeinfo
	for (int i = 0; i < num; i++) {
		proto_type *t = types[i];

		if (t->file == f && t->msg) {
			str_add(o, EOL);
			pa_sort(t->msg->field, &cmp_field_number);

			for (int j = 0; j < t->msg->field.len; j++) {
				t->msg->field.v[j]->by_number_index = j;
			}

			do_fields_by_number(o, t);

			pa_sort(t->msg->field, &cmp_field_name);

			do_fields_by_name(o, t);
			do_typeinfo(o, t);

		} else if (t->file == f && t->en) {
			str_add(o, EOL);
			pa_sort(t->en->value, &cmp_enum_number);

			for (int j = 0; j < t->en->value.len; j++) {
				t->en->value.v[j]->by_number_index = j;
			}

			do_enum_by_number(o, t);

			pa_sort(t->en->value, &cmp_enum_name);

			do_enum_by_name(o, t);
			do_enuminfo(o, t);

		} else if (t->file == f && t->svc) {
			str_add(o, EOL);
			do_method_by_index(o, t);
			pa_sort(t->svc->method, &cmp_method_name);
			do_method_by_name(o, t);
			do_svcinfo(o, t);
		}
	}

	str_add(o, EOL);
}

static FileDescriptorProto *find_file(const CodeGeneratorRequest *r, pb_string name) {
	for (int i = 0; i < r->proto_file.len; i++) {
		if (str_equals(r->proto_file.v[i]->name, name)) {
			return r->proto_file.v[i];
		}
	}
	return NULL;
}

static void write_file(const char *fn, const char *data, int len) {
	FILE *f = fopen(fn, "wb");
	if (!f) {
		fprintf(stderr, "failed to open %s\n", fn);
		exit(2);
	}
	fwrite(data, len, 1, f);
	fclose(f);
}

int main(int argc, char *argv[]) {
	if (argc >= 3) {
		// we've been called directly
		// call protoc with the arguments which will then call the backend back
		return exec_protoc(argv[0], argv[1], argv[2]);
	}
#ifdef _WIN32
	_setmode(_fileno(stdin), _O_BINARY);
#endif

	str_t in = STR_INIT;
	str_fread_all(&in, stdin);

	pb_allocator alloc;
	alloc.next = malloc(ALLOC_SIZE);
	alloc.end = alloc.next + ALLOC_SIZE;

	CodeGeneratorRequest *r = (CodeGeneratorRequest*)pb_decode(&alloc, &type_CodeGeneratorRequest, in.c_str, in.len);

	for (int i = 0; i < r->proto_file.len; i++) {
		insert_file_types(r->proto_file.v[i]);
	}
	finish_types();

	str_t data = STR_INIT;
	str_t fn = STR_INIT;

	for (int i = 0; i < r->file_to_generate.len; i++) {
		FileDescriptorProto *f = find_file(r, r->file_to_generate.v[i]);

		str_clear(&data);
		write_header(&data, f);

		str_setstr(&fn, f->name);
		str_add(&fn, ".h");
		write_file(fn.c_str, data.c_str, data.len);

		str_clear(&data);
		write_source(&data, f);

		str_setstr(&fn, f->name);
		str_add(&fn, ".c");
		write_file(fn.c_str, data.c_str, data.len);
	}


}