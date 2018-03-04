#include "protoc-gen-protorpc.h"
#include "exec.h"
#include <protorpc/ptr-array.h>

#define ALLOC_SIZE (1*1024*1024)

static void define_enum(str_t *o, proto_type *t) {
	if (t->defined) {
		return;
	}

	str_add(o, "\n");
	str_addf(o, "enum %s {\n", t->c_type.c_str);
	for (EnumValueDescriptorProto *v = t->en->value.first; v != NULL; v = v->_next) {
		if (v != t->en->value.first) {
			str_add(o, ",\n");
		}
		str_addf(o, "\t%s = %d", v->name.c_str, v->number);
	}
	str_add(o, "\n};\n");
	str_addf(o, "typedef enum %s %s;\n", t->c_type.c_str, t->c_type.c_str);
	t->defined = true;
}

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

static OneofDescriptorProto *get_oneof(const DescriptorProto *msg, unsigned idx) {
	OneofDescriptorProto *v = msg->oneof_decl.first;
	while (idx && v) {
		v = v->_next;
	}
	if (v == NULL) {
		fprintf(stderr, "invalid oneof\n");
		exit(2);
	}
	return v;
}

static void declare_oneof(str_t *o, const proto_type *t, OneofDescriptorProto* oneof, unsigned oneof_idx) {
	str_add(o, "\n");
	str_addf(o, "enum %s_%s {\n", t->c_type.c_str, oneof->name.c_str);
	bool first = true;

	for (FieldDescriptorProto *f = t->msg->field.first; f != NULL; f = f->_next) {
		if (f->oneof_index.set && f->oneof_index.val == oneof_idx) {
			if (!first) {
				str_add(o, ",\n");
			}
			first = false;
			str_addch(o, '\t');
			add_upper(o, t->c_type.c_str);
			str_addch(o, '_');
			add_upper(o, f->name.c_str);
			str_addf(o, " = %d", f->number);
		}
	}

	str_add(o, "\n};\n");
}

static void append_field(str_t *o, const FieldDescriptorProto *f) {
	proto_type *ft = get_field_type(f);
	if (f->label == LABEL_REPEATED) {
		if (ft->is_pod) {
			str_addf(o, "\tstruct {int len; %s *v;} %s;\n", ft->c_type.c_str, f->name.c_str);
		} else if (ft->msg) {
			str_addf(o, "\tstruct {int len; %s *first;} %s;\n", ft->c_type.c_str, f->name.c_str);
		} else if (ft->en) {
			str_addf(o, "\tstruct {int len; %s *v; int _encoded;} %s;\n", ft->c_type.c_str, f->name.c_str);
		} else {
			str_addf(o, "\t%s %s;\n", ft->list_type.c_str, f->name.c_str);
		}
	} else {
		if (ft->msg && !ft->is_pod) {
			str_addf(o, "\t%s *%s;\n", ft->c_type.c_str, f->name.c_str);
		} else {
			// including enums & pods
			str_addf(o, "\t%s %s;\n", ft->c_type.c_str, f->name.c_str);
		}
	}
}

static void define_message(str_t *o, proto_type *t) {
	if (t->defined) {
		return;
	}

	for (FieldDescriptorProto *f = t->msg->field.first; f != NULL; f = f->_next) {
		proto_type *ft = get_field_type(f);
		if (f->label != LABEL_REPEATED && ft->is_pod && ft->file == t->file) {
			define_message(o, ft);
		} else if (f->type == TYPE_ENUM && ft->file == t->file) {
			define_enum(o, ft);
		}
	}

	unsigned oneof_idx = 0;
	for (OneofDescriptorProto *n = t->msg->oneof_decl.first; n != NULL; n = n->_next) {
		declare_oneof(o, t, n, oneof_idx++);
	}

	str_add(o, "\n");
	str_addf(o, "struct %s {\n", t->c_type.c_str);
	if (!t->is_pod) {
		str_addf(o, "\t%s *_next;\n", t->c_type.c_str);
		str_add(o, "\tint _encsz;\n");
	} else if (t->msg->field.len == 0) {
		str_add(o, "\tchar _pbempty;\n");
	}

	for (FieldDescriptorProto *f = t->msg->field.first; f != NULL;) {
		if (f->oneof_index.set) {
			unsigned oneof = f->oneof_index.val;
			const char *oneof_name = get_oneof(t->msg, oneof)->name.c_str;
			str_addf(o, "\tenum %s_%s %s_type;\n", t->c_type.c_str, oneof_name, oneof_name);
			str_add(o, "\tunion {\n");

			while (f) {
				if (!f->oneof_index.set || f->oneof_index.val != oneof) {
					break;
				}
				str_add(o, "\t");
				append_field(o, f);
				f = f->_next;
			}

			str_addf(o, "\t} %s;\n", oneof_name);
		} else {
			append_field(o, f);
			f = f->_next;
		}
	}
	str_add(o, "};\n");
	t->defined = true;
}

static void define_service(str_t *o, const proto_type *t) {
	str_add(o, "\n");
	str_addf(o, "struct %s {\n", t->c_type.c_str);
	for (MethodDescriptorProto *m = t->svc->method.first; m != NULL; m = m->_next) {
		proto_type *it = get_type(m->input_type);
		proto_type *ot = get_type(m->output_type);
		if (m->client_streaming || m->server_streaming || ot->is_empty) {
			str_addf(o, "\tint (*%s)(%s *svc, http *h, %s *in);\n",
				m->name.c_str, t->c_type.c_str, it->c_type.c_str);
		} else {
			str_addf(o, "\tint (*%s)(%s *svc, http *h, %s *in, %s *out);\n",
				m->name.c_str, t->c_type.c_str, it->c_type.c_str, ot->c_type.c_str);
		}
	}
	str_add(o, "};\n");
}

static void write_header(str_t *o, const FileDescriptorProto *f) {
	int num;
	proto_type **types = get_all_types(&num);

	// includes
	str_add(o, "#pragma once\n");
	str_add(o, "#include <protorpc/protorpc.h>\n");

	for (int i = 0; i < f->dependency.len; i++) {
		str_add(o, "#include \"");
		str_addstr(o, f->dependency.v[i]);
		str_add(o, ".h\"\n");
	}

	str_add(o, "#ifdef __cplusplus\n");
	str_add(o, "extern \"C\" {\n");
	str_add(o, "#endif\n");

	// typedefs
	str_add(o, "\n");
	for (int i = 0; i < num; i++) {
		proto_type *t = types[i];
		if (t->file == f && (t->msg || t->svc)) {
			str_addf(o, "typedef struct %s %s;\n", t->c_type.c_str, t->c_type.c_str);
		}
	}

	// typeinfos
	str_add(o, "\n");
	for (int i = 0; i < num; i++) {
		proto_type *t = types[i];
		if (t->file == f && t->msg) {
			str_addf(o, "extern const proto_message proto_%s;\n", t->c_type.c_str);
		} else if (t->file == f && t->en) {
			str_addf(o, "extern const proto_enum proto_%s;\n", t->c_type.c_str);
		} else if (t->file == f && t->svc) {
			str_addf(o, "extern const proto_service proto_%s;\n", t->c_type.c_str);
			for (MethodDescriptorProto *m = t->svc->method.first; m != NULL; m = m->_next) {
				str_addf(o, "extern const proto_method proto_%s_%s;\n", t->c_type.c_str, m->name.c_str);
			}
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

	str_add(o, "\n");
	str_add(o, "#ifdef __cplusplus\n");
	str_add(o, "}\n");
	str_add(o, "#endif\n");
	str_add(o, "\n");
}

typedef struct EnumValue EnumValue;

struct EnumValue {
	const EnumValueDescriptorProto *pb;
	int by_number_index;
};

static int cmp_enum_number(const void *a, const void *b) {
	EnumValue *va = (EnumValue*)a;
	EnumValue *vb = (EnumValue*)b;
	return va->pb->number - vb->pb->number;
}

static int cmp_enum_name(const void *a, const void *b) {
	EnumValue *va = (EnumValue*)a;
	EnumValue *vb = (EnumValue*)b;
	return compare_string(va->pb->name.c_str, va->pb->name.len, vb->pb->name.c_str, vb->pb->name.len);
}

typedef struct Field Field;

struct Field {
	const FieldDescriptorProto *pb;
	int by_number_index;
};

static int cmp_field_number(const void *a, const void *b) {
	Field *fa = (Field*)a;
	Field *fb = (Field*)b;
	return fa->pb->number - fb->pb->number;
}

static int cmp_field_json_name(const void *a, const void *b) {
	Field *fa = (Field*)a;
	Field *fb = (Field*)b;
	return compare_string(fa->pb->json_name.c_str, fa->pb->json_name.len, fb->pb->json_name.c_str, fb->pb->json_name.len);
}

typedef struct Method Method;

struct Method {
	const MethodDescriptorProto *pb;
};

static int cmp_method_name(const void *a, const void *b) {
	Method *ma = (Method*)a;
	Method *mb = (Method*)b;
	return compare_string(ma->pb->name.c_str, ma->pb->name.len, mb->pb->name.c_str, mb->pb->name.len);
}

static void do_fields_by_number(str_t *o, const proto_type *t, Field *fields, int num) {
	str_addf(o, "static const struct proto_field fields_%s[] = {\n", t->c_type.c_str);
	for (int i = 0; i < num; i++) {
		const FieldDescriptorProto *f = fields[i].pb;
		proto_type *ft = get_field_type(f);
		uint32_t tag = get_tag(f);
		const char *type_enum = field_decode_type(f, ft);
		const char *field_name = f->name.c_str;
		if (f->oneof_index.set) {
			field_name = get_oneof(t->msg, f->oneof_index.val)->name.c_str;
		}
		if (i) {
			str_addf(o, ",\n");
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
	if (!num) {
		str_addf(o, "\t{0}");
	}
	str_addf(o, "\n};\n");
}

static void do_fields_by_name(str_t *o, const proto_type *t, Field *fields, int num) {
	str_addf(o, "static const pb_string *by_name_%s[] = {\n", t->c_type.c_str);
	for (int i = 0; i < num; i++) {
		str_addf(o, "\t&fields_%s[%d].json_name,\n", t->c_type.c_str, fields[i].by_number_index);
	}
	str_addf(o, "\tNULL,\n");
	str_addf(o, "};\n");
}

static void do_typeinfo(str_t *o, const proto_type *t) {
	str_addf(o, "const proto_message proto_%s = {\n", t->c_type.c_str);
	str_addf(o, "\tsizeof(%s),\n", t->c_type.c_str);
	str_addf(o, "\tsizeof(by_name_%s)/sizeof(pb_string*) - 1,\n", t->c_type.c_str);
	str_addf(o, "\tfields_%s,\n", t->c_type.c_str);
	str_addf(o, "\tby_name_%s\n", t->c_type.c_str);
	str_addf(o, "};\n");
}

static void do_enum_by_number(str_t *o, const proto_type *t, const EnumValue *values, int num) {
	str_addf(o, "static const proto_enum_value values_%s[] = {\n", t->c_type.c_str);
	for (int i = 0; i < num; i++) {
		const EnumValueDescriptorProto *v = values[i].pb;
		str_addf(o, "\t{{%d, \"%s\"}, %d},\n", v->name.len, v->name.c_str, v->number);
	}
	str_add(o, "};\n");
}

static void do_enum_by_name(str_t *o, const proto_type *t, const EnumValue *values, int num) {
	str_addf(o, "static const pb_string *by_name_%s[] = {\n", t->c_type.c_str);
	for (int i = 0; i < num; i++) {
		str_addf(o, "\t&values_%s[%d].name,\n", t->c_type.c_str, values[i].by_number_index);
	}
	str_add(o, "\tNULL,\n");
	str_add(o, "};\n");
}

static void do_enuminfo(str_t *o, const proto_type *t) {
	str_addf(o, "const proto_enum proto_%s = {\n", t->c_type.c_str);
	str_addf(o, "\tsizeof(by_name_%s)/sizeof(pb_string*) - 1,\n", t->c_type.c_str);
	str_addf(o, "\tvalues_%s,\n", t->c_type.c_str);
	str_addf(o, "\tby_name_%s\n", t->c_type.c_str);
	str_addf(o, "};\n");
}

static void do_method_by_index(str_t *o, const proto_type *t) {
	int method_idx = 0;
	for (MethodDescriptorProto *m = t->svc->method.first; m != NULL; m = m->_next, method_idx++) {
		// Note the proto_type has a leading . that we want to remove
		proto_type *it = get_type(m->input_type);
		proto_type *ot = get_type(m->output_type);
		str_addf(o, "const proto_method proto_%s_%s = {\n", t->c_type.c_str, m->name.c_str);
		str_addf(o, "\t{%d, \"/twirp/%s/%s\"},\n",
			7 /*/twirp*/ + (t->proto_type.len - 1) + 1 + m->name.len, t->proto_type.c_str + 1, m->name.c_str);
		str_addf(o, "\t%d,\n", method_idx);
		str_addf(o, "\t&proto_%s,\n", it->c_type.c_str);
		if (m->client_streaming || m->server_streaming || ot->is_empty) {
			str_add(o, "\tNULL,\n");
		} else {
			str_addf(o, "\t&proto_%s,\n", ot->c_type.c_str);
		}
		str_add(o, "};\n");
	}
}

static void do_methods_by_name(str_t *o, const proto_type *t, Method *methods, int num) {
	str_addf(o, "static const pb_string *methods_%s[] = {\n", t->c_type.c_str);
	for (int i = 0; i < num; i++) {
		const MethodDescriptorProto *m = methods[i].pb;
		if (!m->client_streaming && !m->server_streaming) {
			str_addf(o, "\t&proto_%s_%s.path,\n", t->c_type.c_str, m->name.c_str);
		}
	}
	str_add(o, "\tNULL,\n");
	str_addf(o, "};\n");
}

static void do_streams_by_name(str_t *o, const proto_type *t, Method *methods, int num) {
	str_addf(o, "static const pb_string *streams_%s[] = {\n", t->c_type.c_str);
	for (int i = 0; i < num; i++) {
		const MethodDescriptorProto *m = methods[i].pb;
		if (m->client_streaming || m->server_streaming) {
			str_addf(o, "\t&proto_%s_%s.path,\n", t->c_type.c_str, m->name.c_str);
		}
	}
	str_add(o, "\tNULL,\n");
	str_addf(o, "};\n");
}

static void do_svcinfo(str_t *o, const proto_type *t) {
	str_addf(o, "const proto_service proto_%s = {\n", t->c_type.c_str);
	str_addf(o, "\tsizeof(methods_%s)/sizeof(pb_string*) - 1,\n", t->c_type.c_str);
	str_addf(o, "\tsizeof(streams_%s)/sizeof(pb_string*) - 1,\n", t->c_type.c_str);
	str_addf(o, "\tmethods_%s,\n", t->c_type.c_str);
	str_addf(o, "\tstreams_%s,\n", t->c_type.c_str);
	str_addf(o, "};\n");
}
static void write_source(str_t *o, const FileDescriptorProto *f) {
	int num;
	proto_type **types = get_all_types(&num);

	// include - don't include the directory name
	str_add(o, "#include \"");
	char *slash = str_rfind_char(f->name, '/');
	str_add(o, slash ? (slash + 1) : f->name.c_str);
	str_add(o, ".h\"\n");

	// typeinfo
	for (int i = 0; i < num; i++) {
		proto_type *t = types[i];

		if (t->file == f && t->msg) {
			str_add(o, "\n");

			int fnum = t->msg->field.len;
			Field *fields = (Field*)calloc(fnum, sizeof(Field));

			int j = 0;
			for (FieldDescriptorProto *fld = t->msg->field.first; fld != NULL; fld = fld->_next) {
				fields[j++].pb = fld;
			}

			qsort(fields, fnum, sizeof(fields[0]), &cmp_field_number);

			for (j = 0; j < fnum; j++) {
				fields[j].by_number_index = j;
			}

			do_fields_by_number(o, t, fields, fnum);

			qsort(fields, fnum, sizeof(fields[0]), &cmp_field_json_name);

			do_fields_by_name(o, t, fields, fnum);
			do_typeinfo(o, t);

			free(fields);

		} else if (t->file == f && t->en) {
			str_add(o, "\n");

			int vnum = t->en->value.len;
			EnumValue *values = (EnumValue*)calloc(vnum, sizeof(EnumValue));

			int j = 0;
			for (EnumValueDescriptorProto *v = t->en->value.first; v != NULL; v = v->_next) {
				values[j++].pb = v;
			}

			qsort(values, vnum, sizeof(values[0]), &cmp_enum_number);

			for (j = 0; j < vnum; j++) {
				values[j].by_number_index = j;
			}

			do_enum_by_number(o, t, values, vnum);

			qsort(values, vnum, sizeof(values[0]), &cmp_enum_name);

			do_enum_by_name(o, t, values, vnum);
			do_enuminfo(o, t);

			free(values);

		} else if (t->file == f && t->svc) {
			str_add(o, "\n");
			do_method_by_index(o, t);

			int mnum = t->svc->method.len;
			Method *methods = (Method*)calloc(mnum, sizeof(Method));

			int j = 0;
			for (MethodDescriptorProto *m = t->svc->method.first; m != NULL; m = m->_next) {
				methods[j++].pb = m;
			}

			qsort(methods, mnum, sizeof(methods[0]), &cmp_method_name);

			do_methods_by_name(o, t, methods, mnum);
			do_streams_by_name(o, t, methods, mnum);
			do_svcinfo(o, t);
		}
	}

	str_add(o, "\n");
}

static FileDescriptorProto *find_file(const CodeGeneratorRequest *r, pb_string name) {
	for (FileDescriptorProto *f = r->proto_file.first; f != NULL; f = f->_next) {
		if (str_equals(f->name, name)) {
			return f;
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
		char *my_exe = argv[0];
		char *protoc_exe = argv[1];
		char **files = argv + 2;
		int num = 0;
		while (files[num] && strcmp("-o", files[num])) {
			num++;
		}
		// call protoc with the arguments which will then call the backend back
		return exec_protoc(my_exe, protoc_exe, files, num);
	}

	str_t in = STR_INIT;
	str_fread_all(&in, stdin);

	pb_allocator alloc;
	alloc.next = malloc(ALLOC_SIZE);
	alloc.end = alloc.next + ALLOC_SIZE;

	CodeGeneratorRequest *r = (CodeGeneratorRequest*)pb_decode(&alloc, &type_CodeGeneratorRequest, in.c_str, in.len);

	for (FileDescriptorProto *f = r->proto_file.first; f != NULL; f = f->_next) {
		insert_file_types(f);
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
