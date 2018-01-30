#pragma once
#include <compact.h>

struct FileDescriptorProto {
	union pb_msg pb_hdr;
	pb_string_t	name;
	pb_string_t	package;
	struct {int len; pb_string_t const *v;}	dependency;
	struct {int len; struct DescriptorProto const **v;}	message_type;
	struct {int len; struct EnumDescriptorProto const **v;}	enum_type;
	struct {int len; struct ServiceDescriptorProto const **v;}	service;
	pb_string_t	syntax;
};
struct DescriptorProto {
	union pb_msg pb_hdr;
	pb_string_t	name;
	struct {int len; struct FieldDescriptorProto const **v;}	field;
	struct {int len; struct DescriptorProto const **v;}	nested_type;
	struct {int len; struct EnumDescriptorProto const **v;}	enum_type;
	struct {int len; struct OneofDescriptorProto const **v;}	oneof_decl;
};
enum FieldDescriptorProto_Type {
	TYPE_DOUBLE = 1,
	TYPE_FLOAT = 2,
	TYPE_INT64 = 3,
	TYPE_UINT64 = 4,
	TYPE_INT32 = 5,
	TYPE_FIXED64 = 6,
	TYPE_FIXED32 = 7,
	TYPE_BOOL = 8,
	TYPE_STRING = 9,
	TYPE_GROUP = 10,
	TYPE_MESSAGE = 11,
	TYPE_BYTES = 12,
	TYPE_UINT32 = 13,
	TYPE_ENUM = 14,
	TYPE_SFIXED32 = 15,
	TYPE_SFIXED64 = 16,
	TYPE_SINT32 = 17,
	TYPE_SINT64 = 18
};
enum FieldDescriptorProto_Label {
	LABEL_OPTIONAL = 1,
	LABEL_REQUIRED = 2,
	LABEL_REPEATED = 3
};
struct FieldDescriptorProto {
	union pb_msg pb_hdr;
	pb_string_t	name;
	int32_t	number;
	enum FieldDescriptorProto_Label	label;
	enum FieldDescriptorProto_Type	type;
	pb_string_t	type_name;
	pb_opt_uint	oneof_index;
	pb_string_t	json_name;
};
struct OneofDescriptorProto {
	union pb_msg pb_hdr;
	pb_string_t	name;
};
struct EnumDescriptorProto {
	union pb_msg pb_hdr;
	pb_string_t	name;
	struct {int len; struct EnumValueDescriptorProto const **v;}	value;
};
struct EnumValueDescriptorProto {
	union pb_msg pb_hdr;
	pb_string_t	name;
	int32_t	number;
};
struct ServiceDescriptorProto {
	union pb_msg pb_hdr;
	pb_string_t	name;
	struct {int len; struct MethodDescriptorProto const **v;}	method;
};
struct MethodDescriptorProto {
	union pb_msg pb_hdr;
	pb_string_t	name;
	pb_string_t	input_type;
	pb_string_t	output_type;
	bool	client_streaming;
	bool	server_streaming;
};

struct CodeGeneratorRequest {
	union pb_msg pb_hdr;
	pb_string_list file_to_generate;
	struct {int len; struct FileDescriptorProto const **v;}	proto_file;
};

extern const struct proto_message type_FileDescriptorProto;
extern const struct proto_message type_DescriptorProto;
extern const struct proto_message type_FieldDescriptorProto;
extern const struct proto_message type_OneofDescriptorProto;
extern const struct proto_message type_EnumDescriptorProto;
extern const struct proto_message type_EnumValueDescriptorProto;
extern const struct proto_message type_ServiceDescriptorProto;
extern const struct proto_message type_MethodDescriptorProto;
extern const struct proto_message type_CodeGeneratorRequest;





