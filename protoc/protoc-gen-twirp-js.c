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
		str_addch(o, '_');
	}
	while (*type) {
		str_addch(o, is_alphanum(*type) ? *type : '_');
		type++;
	}
}

static void define_enum(str_t *o, str_t *ns, const EnumDescriptorProto *en) {
	str_add(o, "\tvar ");
	write_variable(o, ns->c_str, en->name.c_str);
	str_add(o, " = {\n");
	for (int i = 0; i < en->value.len; i++) {
		EnumValueDescriptorProto *v = en->value.v[i];
		str_addf(o, "\t\t[%d]: \"%s\",\n", v->number, v->name.c_str);
	}
	str_add(o, "\t};\n");
}

static void define_nested_enum(str_t *o, str_t *ns, const DescriptorProto *m) {
	int nslen = ns->len;
	str_addch(ns, '.');
	str_addstr(ns, m->name);
	for (int i = 0; i < m->enum_type.len; i++) {
		define_enum(o, ns, m->enum_type.v[i]);
	}
	for (int i = 0; i < m->nested_type.len; i++) {
		define_nested_enum(o, ns, m->nested_type.v[i]);
	}
	str_setlen(ns, nslen);
}

static void register_enum(str_t *o, str_t *ns, const EnumDescriptorProto *en) {
	str_addf(o, "\t\t%s: ", en->name.c_str);
	write_variable(o, ns->c_str, en->name.c_str);
	str_add(o, ",\n");
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

static int compare_field_number(const void **a, const void **b) {
	FieldDescriptorProto *fa = *(FieldDescriptorProto**)a;
	FieldDescriptorProto *fb = *(FieldDescriptorProto**)b;
	return fa->number - fb->number;
}

static void define_message(str_t *o, str_t *ns, const DescriptorProto *m) {
	pa_sort(m->field, &compare_field_number);

	str_add(o, "\tvar ");
	write_variable(o, ns->c_str, m->name.c_str);
	str_add(o, " = [\n");

	for (int i = 0; i < m->field.len; i++) {
		FieldDescriptorProto *f = m->field.v[i];
		int tag = field_tag(f);
		int type = field_type(f);
		str_addf(o, "\t\t%d,%d,\"%s\",", tag, type, f->json_name.c_str);
		if (f->type == TYPE_ENUM) {
			write_variable(o, NULL, f->type_name.c_str + 1);
			str_add(o, ",");
		} else if (type & FIELD_EXTRA) {
			str_add(o, "null,");
		}
		str_add(o, "\n");
	}
	str_add(o, "\t];\n");

	// define nested messages
	int nslen = ns->len;
	str_addch(ns, '.');
	str_addstr(ns, m->name);
	for (int i = 0; i < m->nested_type.len; i++) {
		define_message(o, ns, m->nested_type.v[i]);
	}
	str_setlen(ns, nslen);
}

static void link_message(str_t *o, str_t *ns, const DescriptorProto *m) {
	for (int i = 0, off = 0; i < m->field.len; i++) {
		FieldDescriptorProto *f = m->field.v[i];
		int type = field_type(f);
		off += 3;
		if (f->type == TYPE_MESSAGE) {
			str_addf(o, "\t");
			write_variable(o, ns->c_str, m->name.c_str);
			str_addf(o, "[%d] = ", off);
			write_variable(o, NULL, f->type_name.c_str + 1);
			str_add(o, ";\n");
		}
		if (type & FIELD_EXTRA) {
			off++;
		}
	}

	// link nested messages
	int nslen = ns->len;
	str_addch(ns, '.');
	str_addstr(ns, m->name);
	for (int i = 0; i < m->nested_type.len; i++) {
		link_message(o, ns, m->nested_type.v[i]);
	}
	str_setlen(ns, nslen);
}

static void register_message(str_t *o, str_t *ns, const DescriptorProto *m) {
	str_addf(o, "\t\t%s: ", m->name.c_str);
	write_variable(o, ns->c_str, m->name.c_str);
	str_add(o, ",\n");
}

static void define_service(str_t *o, str_t *ns, const ServiceDescriptorProto *s) {
	str_add(o, "\tvar ");
	write_variable(o, ns->c_str, s->name.c_str);
	str_add(o, " = {\n");
	for (int i = 0; i < s->method.len; i++) {
		const MethodDescriptorProto *m = s->method.v[i];
		str_addf(o, "\t\t%s: [", m->name.c_str);
		write_variable(o, NULL, m->input_type.c_str+1);
		str_add(o, ",");
		write_variable(o, NULL, m->output_type.c_str+1);
		str_add(o, "],\n");
	}
	str_add(o, "\t};\n");
}

static void register_service(str_t *o, str_t *ns, const ServiceDescriptorProto *s) {
	str_addf(o, "\t\t%s: ", s->name.c_str);
	write_variable(o, ns->c_str, s->name.c_str);
	str_add(o, ",\n");
}

static void write_file(const char *fn, const char *data, int len) {
	FILE *f = fopen(fn, "w");
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
	str_fread_all(&in, stdin, STR_BINARY);

	pb_allocator alloc;
	alloc.next = malloc(ALLOC_SIZE);
	alloc.end = alloc.next + ALLOC_SIZE;

	CodeGeneratorRequest *r = (CodeGeneratorRequest*) pb_decode(&alloc, &type_CodeGeneratorRequest, in.c_str, in.len);

	str_t o = STR_INIT;
	str_t ns = STR_INIT;
	str_add(&o, "(function(proto){\n");

	// define enums
	for (int i = 0; i < r->proto_file.len; i++) {
		FileDescriptorProto *f = r->proto_file.v[i];
		str_setstr(&ns, f->package);
		for (int j = 0; j < f->enum_type.len; j++) {
			define_enum(&o, &ns, f->enum_type.v[j]);
		}
		for (int j = 0; j < f->message_type.len; j++) {
			define_nested_enum(&o, &ns, f->message_type.v[j]);
		}
	}

	// define messages
	for (int i = 0; i < r->proto_file.len; i++) {
		FileDescriptorProto *f = r->proto_file.v[i];
		str_setstr(&ns, f->package);
		for (int j = 0; j < f->message_type.len; j++) {
			define_message(&o, &ns, f->message_type.v[j]);
		}
	}

	// link messages
	for (int i = 0; i < r->proto_file.len; i++) {
		FileDescriptorProto *f = r->proto_file.v[i];
		str_setstr(&ns, f->package);
		for (int j = 0; j < f->message_type.len; j++) {
			link_message(&o, &ns, f->message_type.v[j]);
		}
	}

	// define services
	for (int i = 0; i < r->proto_file.len; i++) {
		FileDescriptorProto *f = r->proto_file.v[i];
		str_setstr(&ns, f->package);
		for (int j = 0; j < f->service.len; j++) {
			define_service(&o, &ns, f->service.v[j]);
		}
	}

	// register types
	for (int i = 0; i < r->proto_file.len; i++) {
		FileDescriptorProto *f = r->proto_file.v[i];
		str_setstr(&ns, f->package);
		str_addf(&o, "\tproto.register(\"%s\", {\n", f->package.c_str);
		for (int j = 0; j < f->enum_type.len; j++) {
			register_enum(&o, &ns, f->enum_type.v[j]);
		}
		str_add(&o, "\t},{\n");
		for (int j = 0; j < f->message_type.len; j++) {
			register_message(&o, &ns, f->message_type.v[j]);
		}
		str_add(&o, "\t},{\n");
		for (int j = 0; j < f->service.len; j++) {
			register_service(&o, &ns, f->service.v[j]);
		}
		str_add(&o, "\t});\n");
	}


	str_add(&o, "})(proto);\n\n");
	
	str_t fn = STR_INIT;
	str_setstr(&fn, r->file_to_generate.v[0]);
	str_add(&fn, ".js");
	write_file(fn.c_str, o.c_str, o.len);

	return 0;
}