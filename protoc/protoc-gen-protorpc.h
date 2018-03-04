#include "plugin.h"
#include <protorpc/str.h>

typedef struct proto_type proto_type;

struct proto_type {
	str_t proto_type;
	str_t c_type;
	str_t list_type;
	float max_proto_size;
	bool max_proto_size_calculated;
	bool defined;
	bool is_pod;
	bool is_empty;
	DescriptorProto *msg;
	EnumDescriptorProto *en;
	ServiceDescriptorProto *svc;
	const FileDescriptorProto *file;
};

int compare_string(const char *sa, int salen, const char *sb, int sblen);
proto_type **get_all_types(int *pnum);
proto_type *get_type(pb_string type_name);
proto_type *get_field_type(const FieldDescriptorProto *f);
void finish_types();
void insert_file_types(const FileDescriptorProto *file);
const char *field_decode_type(const FieldDescriptorProto *f, const proto_type *ft);
uint32_t get_tag(const FieldDescriptorProto *f);



