#pragma once
#include <protorpc/protorpc.h>

typedef struct FileDescriptorProto FileDescriptorProto;
typedef struct DescriptorProto DescriptorProto;
typedef struct FieldDescriptorProto FieldDescriptorProto;
typedef struct OneofDescriptorProto OneofDescriptorProto;
typedef struct EnumDescriptorProto EnumDescriptorProto;
typedef struct EnumValueDescriptorProto EnumValueDescriptorProto;
typedef struct ServiceDescriptorProto ServiceDescriptorProto;
typedef struct MethodDescriptorProto MethodDescriptorProto;
typedef struct CodeGeneratorRequest CodeGeneratorRequest;

struct FileDescriptorProto {
	FileDescriptorProto *_next;
	int _encsz;
	pb_string	name;
	pb_string	package;
	pb_string_list	dependency;
	DescriptorProto *message_type;
	EnumDescriptorProto *enum_type;
	ServiceDescriptorProto *service;
	pb_string	syntax;
};
struct DescriptorProto {
	DescriptorProto *_next;
	int _encsz;
	pb_string	name;
	FieldDescriptorProto *field;
	DescriptorProto *nested_type;
	EnumDescriptorProto *enum_type;
	OneofDescriptorProto *oneof_decl;
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
typedef enum FieldDescriptorProto_Type FieldDescriptorProto_Type;

enum FieldDescriptorProto_Label {
	LABEL_OPTIONAL = 1,
	LABEL_REQUIRED = 2,
	LABEL_REPEATED = 3
};
typedef enum FieldDescriptorProto_Label FieldDescriptorProto_Label;

struct FieldDescriptorProto {
	FieldDescriptorProto *_next;
	int _encsz;
	pb_string	name;
	int32_t	number;
	FieldDescriptorProto_Label	label;
	FieldDescriptorProto_Type	type;
	pb_string	type_name;
	pb_opt_uint	oneof_index;
	pb_string	json_name;
};
struct OneofDescriptorProto {
	OneofDescriptorProto *_next;
	int _encsz;
	pb_string	name;
};
struct EnumDescriptorProto {
	EnumDescriptorProto *_next;
	int _encsz;
	pb_string	name;
	EnumValueDescriptorProto *value;
};
struct EnumValueDescriptorProto {
	EnumValueDescriptorProto *_next;
	int _encsz;
	pb_string	name;
	int32_t	number;
};
struct ServiceDescriptorProto {
	ServiceDescriptorProto *_next;
	int _encsz;
	pb_string	name;
	MethodDescriptorProto *method;
};
struct MethodDescriptorProto {
	MethodDescriptorProto *_next;
	int _encsz;
	pb_string	name;
	pb_string	input_type;
	pb_string	output_type;
	bool	client_streaming;
	bool	server_streaming;
};

struct CodeGeneratorRequest {
	CodeGeneratorRequest *_next;
	int _encsz;
	pb_string_list file_to_generate;
	FileDescriptorProto *proto_file;
};

extern const proto_message type_FileDescriptorProto;
extern const proto_message type_DescriptorProto;
extern const proto_message type_FieldDescriptorProto;
extern const proto_message type_OneofDescriptorProto;
extern const proto_message type_EnumDescriptorProto;
extern const proto_message type_EnumValueDescriptorProto;
extern const proto_message type_ServiceDescriptorProto;
extern const proto_message type_MethodDescriptorProto;
extern const proto_message type_CodeGeneratorRequest;





