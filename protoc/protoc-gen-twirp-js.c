#include "protorpc/protorpc.h"
#include "protorpc/str.h"
#include "protorpc/ptr-array.h"
#include "exec.h"
#include "plugin.h"
#include <stdio.h>

#define ALLOC_SIZE (1*1024*1024)

static bool is_alphanum(char ch) {
	return ('0' <= ch && ch <= '9') || ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z');
}

static void write_variable(str_t *o, const char *ns, const char *type) {
	if (ns) {
		while (*ns) {
			str_addch(o, is_alphanum(*ns) ? *ns : '_');
			ns++;
		}
	}
	while (*type) {
		str_addch(o, is_alphanum(*type) ? *type : '_');
		type++;
	}
}

static void define_enum(str_t *o, const char *ns, const EnumDescriptorProto *en) {
	str_add(o, "\tvar ");
	write_variable(o, ns, en->name.c_str);
	str_add(o, " = {\n");
	for (EnumValueDescriptorProto *v = en->value; v != NULL; v = v->_next) {
		str_addf(o, "\t\t[%d]: \"%s\",\n", v->number, v->name.c_str);
	}
	str_add(o, "\t};\n");
}

static void define_nested_enum(str_t *o, str_t *ns, const DescriptorProto *m) {
	int nslen = ns->len;
	str_addstr(ns, m->name);
	str_addch(ns, '.');
	for (EnumDescriptorProto *en = m->enum_type; en != NULL; en = en->_next) {
		define_enum(o, ns->c_str, en);
	}
	for (DescriptorProto *n = m->nested_type; n != NULL; n = n->_next) {
		define_nested_enum(o, ns, n);
	}
	str_setlen(ns, nslen);
}

#define FIELD_REPEATED 16
#define FIELD_EXTRA 32
#define FIELD_BOOL 0
#define FIELD_UINT32 1
#define FIELD_INT32 2
#define FIELD_SINT32 3
#define FIELD_ENUM 4
#define FIELD_UINT64 5
#define FIELD_INT64 6
#define FIELD_SINT64 7
#define FIELD_FIXED32 8
#define FIELD_SFIXED32 9
#define FIELD_FIXED64 10
#define FIELD_SFIXED64 11
#define FIELD_FLOAT 12
#define FIELD_DOUBLE 13
#define FIELD_STRING 14
#define FIELD_BYTES 15

static int field_type(const FieldDescriptorProto *f) {
	int repeated = (f->label == LABEL_REPEATED) ? FIELD_REPEATED : 0;
	switch (f->type) {
	case TYPE_DOUBLE:
		return repeated | FIELD_DOUBLE;
	case TYPE_FLOAT:
		return repeated | FIELD_FLOAT;
	case TYPE_INT64:
		return repeated | FIELD_INT64;
	case TYPE_UINT64:
		return repeated | FIELD_UINT64;
	case TYPE_INT32:
		return repeated | FIELD_INT32;
	case TYPE_FIXED64:
		return repeated | FIELD_FIXED64;
	case TYPE_FIXED32:
		return repeated | FIELD_FIXED32;
	case TYPE_BOOL:
		return repeated | FIELD_BOOL;
	case TYPE_STRING:
		return repeated | FIELD_STRING;
	case TYPE_MESSAGE:
		return repeated | FIELD_BYTES | FIELD_EXTRA;
	case TYPE_BYTES:
		return repeated | FIELD_BYTES;
	case TYPE_UINT32:
		return repeated | FIELD_UINT32;
	case TYPE_ENUM:
		return repeated | FIELD_ENUM | FIELD_EXTRA;
	case TYPE_SFIXED32:
		return repeated | FIELD_SFIXED32;
	case TYPE_SFIXED64:
		return repeated | FIELD_SFIXED64;
	case TYPE_SINT32:
		return repeated | FIELD_SINT32;
	case TYPE_SINT64:
		return repeated | FIELD_SINT64;
	default:
		return 0;
	}
}

#define WIRE_VARINT 0
#define WIRE_FIXED_64 1
#define WIRE_VARIABLE 2
#define WIRE_FIXED_32 5

static uint32_t field_wire_type(const struct FieldDescriptorProto *f) {
	if (f->label == LABEL_REPEATED) {
		return WIRE_VARIABLE;
	}
	switch (f->type) {
	case TYPE_FIXED32:
	case TYPE_SFIXED32:
	case TYPE_FLOAT:
		return WIRE_FIXED_32;
	case TYPE_FIXED64:
	case TYPE_SFIXED64:
	case TYPE_DOUBLE:
		return WIRE_FIXED_64;
	case TYPE_STRING:
	case TYPE_BYTES:
	case TYPE_MESSAGE:
		return WIRE_VARIABLE;
	default:
		return WIRE_VARINT;
	}
}

uint32_t field_tag(const struct FieldDescriptorProto *f) {
	return field_wire_type(f) | (f->number << 3);
}

static int compare_field_number(const void *a, const void *b) {
	FieldDescriptorProto *fa = *(FieldDescriptorProto**)a;
	FieldDescriptorProto *fb = *(FieldDescriptorProto**)b;
	return fa->number - fb->number;
}

static void define_message(str_t *o, str_t *ns, const DescriptorProto *m) {
	int fnum = 0;
	for (FieldDescriptorProto *f = m->field; f != NULL; f = f->_next) {
		fnum++;
	}

	FieldDescriptorProto **fields = (FieldDescriptorProto**)calloc(fnum, sizeof(FieldDescriptorProto*));
	int i = 0;
	for (FieldDescriptorProto *f = m->field; f != NULL; f = f->_next) {
		fields[i++] = f;
	}

	qsort(fields, fnum, sizeof(fields[0]), &compare_field_number);

	str_add(o, "\tvar ");
	write_variable(o, ns->c_str, m->name.c_str);
	str_add(o, " = {\n");

	for (i = 0; i < fnum; i++) {
		FieldDescriptorProto *f = fields[i];
		int tag = field_tag(f);
		int type = field_type(f);
		str_addf(o, "\t\t%s: [%d,%d", f->json_name.c_str, tag, type);
		if (f->type == TYPE_ENUM) {
			str_add(o, ",");
			write_variable(o, NULL, f->type_name.c_str + 1);
		} else if (type & FIELD_EXTRA) {
			str_add(o, ",0");
		}
		str_add(o, "],\n");
	}
	str_add(o, "\t};\n");

	// define nested messages
	int nslen = ns->len;
	str_addstr(ns, m->name);
	str_addch(ns, '.');
	for (DescriptorProto *n = m->nested_type; n != NULL; n = n->_next) {
		define_message(o, ns, n);
	}
	str_setlen(ns, nslen);
}

static void link_message(str_t *o, str_t *ns, const DescriptorProto *m) {
	for (FieldDescriptorProto *f = m->field; f != NULL; f = f->_next) {
		if (f->type == TYPE_MESSAGE) {
			str_addf(o, "\t");
			write_variable(o, ns->c_str, m->name.c_str);
			str_addf(o, ".%s[2] = ", f->json_name.c_str);
			write_variable(o, NULL, f->type_name.c_str + 1);
			str_add(o, ";\n");
		}
	}

	// link nested messages
	int nslen = ns->len;
	str_addstr(ns, m->name);
	str_addch(ns, '.');
	for (DescriptorProto *n = m->nested_type; n != NULL; n = n->_next) {
		link_message(o, ns, n);
	}
	str_setlen(ns, nslen);
}

static void register_message(str_t *o, const char *ns, const DescriptorProto *m) {
	str_add(o, "\tproto.");
	write_variable(o, ns, m->name.c_str);
	str_add(o, " = ");
	write_variable(o, ns, m->name.c_str);
	str_add(o, ";\n");
}

static void define_service(str_t *o, const char *ns, const ServiceDescriptorProto *s) {
	str_add(o, "\tproto.");
	write_variable(o, ns, s->name.c_str);
	str_add(o, " = {\n");
	str_addf(o, "\t\t_basePath: \"/twirp/%s%s/\",\n", ns, s->name.c_str);
	for (MethodDescriptorProto *m = s->method; m != NULL; m = m->_next) {
		str_addf(o, "\t\t%s: [\"%s\", ", m->name.c_str, m->name.c_str);
		write_variable(o, NULL, m->input_type.c_str+1);
		str_add(o, ", ");
		write_variable(o, NULL, m->output_type.c_str+1);
		str_add(o, "],\n");
	}
	str_add(o, "\t};\n");
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

static void set_namespace(str_t *ns, const FileDescriptorProto *f) {
	str_setstr(ns, f->package);
	if (ns->len) {
		str_addch(ns, '.');
	}
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

	CodeGeneratorRequest *r = (CodeGeneratorRequest*) pb_decode(&alloc, &type_CodeGeneratorRequest, in.c_str, in.len);

	str_t o = STR_INIT;
	str_t ns = STR_INIT;
	str_add(&o, "(function(proto){\n");

	// define enums
	for (FileDescriptorProto *f = r->proto_file; f != NULL; f = f->_next) {
		set_namespace(&ns, f);
		for (EnumDescriptorProto *en = f->enum_type; en != NULL; en = en->_next) {
			define_enum(&o, ns.c_str, en);
		}
		for (DescriptorProto *m = f->message_type; m != NULL; m = m->_next) {
			define_nested_enum(&o, &ns, m);
		}
	}

	// define messages
	for (FileDescriptorProto *f = r->proto_file; f != NULL; f = f->_next) {
		set_namespace(&ns, f);
		for (DescriptorProto *m = f->message_type; m != NULL; m = m->_next) {
			define_message(&o, &ns, m);
		}
	}

	// link messages
	for (FileDescriptorProto *f = r->proto_file; f != NULL; f = f->_next) {
		set_namespace(&ns, f);
		for (DescriptorProto *m = f->message_type; m != NULL; m = m->_next) {
			link_message(&o, &ns, m);
		}
	}

	// register messages
	for (FileDescriptorProto *f = r->proto_file; f != NULL; f = f->_next) {
		set_namespace(&ns, f);
		for (DescriptorProto *m = f->message_type; m != NULL; m = m->_next) {
			register_message(&o, ns.c_str, m);
		}
	}

	// define services
	for (FileDescriptorProto *f = r->proto_file; f != NULL; f = f->_next) {
		set_namespace(&ns, f);
		for (ServiceDescriptorProto *s = f->service; s != NULL; s = s->_next) {
			define_service(&o, ns.c_str, s);
		}
	}



	str_add(&o, "})(proto);\n\n");
	
	str_t fn = STR_INIT;
	str_setstr(&fn, r->file_to_generate.v[0]);
	str_add(&fn, ".js");
	write_file(fn.c_str, o.c_str, o.len);

	return 0;
}
