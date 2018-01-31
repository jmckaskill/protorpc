#include "plugin.h"

static const struct proto_field fields_FileDescriptorProto[] = {
	{ {0, ""}, PROTO_STRING, offsetof(struct FileDescriptorProto, name), 10, NULL, -1 },
	{ {0, ""}, PROTO_STRING, offsetof(struct FileDescriptorProto, package), 18, NULL, -1 },
	{ {0, ""}, PROTO_LIST_STRING, offsetof(struct FileDescriptorProto, dependency), 26, NULL, -1 },
	{ {0, ""}, PROTO_LIST_MESSAGE, offsetof(struct FileDescriptorProto, message_type), 34, &type_DescriptorProto, -1 },
	{ {0, ""}, PROTO_LIST_MESSAGE, offsetof(struct FileDescriptorProto, enum_type), 42, &type_EnumDescriptorProto, -1 },
	{ {0, ""}, PROTO_LIST_MESSAGE, offsetof(struct FileDescriptorProto, service), 50, &type_ServiceDescriptorProto, -1 },
	{ {0, ""}, PROTO_STRING, offsetof(struct FileDescriptorProto, syntax), 98, NULL, -1 }
};
const struct proto_message type_FileDescriptorProto = {
	sizeof(struct FileDescriptorProto),
	sizeof(fields_FileDescriptorProto) / sizeof(struct proto_field),
	fields_FileDescriptorProto
};

static const struct proto_field fields_DescriptorProto[] = {
	{ {0, ""}, PROTO_STRING, offsetof(struct DescriptorProto, name), 10, NULL, -1 },
	{ {0, ""}, PROTO_LIST_MESSAGE, offsetof(struct DescriptorProto, field), 18, &type_FieldDescriptorProto, -1 },
	{ {0, ""}, PROTO_LIST_MESSAGE, offsetof(struct DescriptorProto, nested_type), 26, &type_DescriptorProto, -1 },
	{ {0, ""}, PROTO_LIST_MESSAGE, offsetof(struct DescriptorProto, enum_type), 34, &type_EnumDescriptorProto, -1 },
	{ {0, ""}, PROTO_LIST_MESSAGE, offsetof(struct DescriptorProto, oneof_decl), 66, &type_OneofDescriptorProto, -1 },
};
const struct proto_message type_DescriptorProto = {
	sizeof(struct DescriptorProto),
	sizeof(fields_DescriptorProto) / sizeof(struct proto_field),
	fields_DescriptorProto
};

static const struct proto_field fields_FieldDescriptorProto[] = {
	{ {0, ""}, PROTO_STRING, offsetof(struct FieldDescriptorProto, name), 10, NULL, -1 },
	{ {0, ""}, PROTO_I32, offsetof(struct FieldDescriptorProto, number), 24, NULL, -1 },
	{ {0, ""}, PROTO_ENUM, offsetof(struct FieldDescriptorProto, label), 32, NULL, -1 },
	{ {0, ""}, PROTO_ENUM, offsetof(struct FieldDescriptorProto, type), 40, NULL, -1 },
	{ {0, ""}, PROTO_STRING, offsetof(struct FieldDescriptorProto, type_name), 50, NULL, -1 },
	{ {0, ""}, PROTO_OPTIONAL_U32, offsetof(struct FieldDescriptorProto, oneof_index), 72, NULL, -1 },
	{ {0, ""}, PROTO_STRING, offsetof(struct FieldDescriptorProto, json_name), 82, NULL, -1 }
};
const struct proto_message type_FieldDescriptorProto = {
	sizeof(struct FieldDescriptorProto),
	sizeof(fields_FieldDescriptorProto) / sizeof(struct proto_field),
	fields_FieldDescriptorProto
};

static const struct proto_field fields_OneofDescriptorProto[] = {
	{ {0, ""}, PROTO_STRING, offsetof(struct OneofDescriptorProto, name), 10, NULL, -1 },
};
const struct proto_message type_OneofDescriptorProto = {
	sizeof(struct OneofDescriptorProto),
	sizeof(fields_OneofDescriptorProto) / sizeof(struct proto_field),
	fields_OneofDescriptorProto
};

static const struct proto_field fields_EnumDescriptorProto[] = {
	{ {0, ""}, PROTO_STRING, offsetof(struct EnumDescriptorProto, name), 10, NULL, -1 },
	{ {0, ""}, PROTO_LIST_MESSAGE, offsetof(struct EnumDescriptorProto, value), 18, &type_EnumValueDescriptorProto, -1 },
};
const struct proto_message type_EnumDescriptorProto = {
	sizeof(struct EnumDescriptorProto),
	sizeof(fields_EnumDescriptorProto) / sizeof(struct proto_field),
	fields_EnumDescriptorProto
};

static const struct proto_field fields_EnumValueDescriptorProto[] = {
	{ {0, ""}, PROTO_STRING, offsetof(struct EnumValueDescriptorProto, name), 10, NULL, -1 },
	{ {0, ""}, PROTO_I32, offsetof(struct EnumValueDescriptorProto, number), 16, NULL, -1 },
};
const struct proto_message type_EnumValueDescriptorProto = {
	sizeof(struct EnumValueDescriptorProto),
	sizeof(fields_EnumValueDescriptorProto) / sizeof(struct proto_field),
	fields_EnumValueDescriptorProto
};

static const struct proto_field fields_ServiceDescriptorProto[] = {
	{ {0, ""}, PROTO_STRING, offsetof(struct ServiceDescriptorProto, name), 10, NULL, -1 },
	{ {0, ""}, PROTO_LIST_MESSAGE, offsetof(struct ServiceDescriptorProto, method), 18, &type_MethodDescriptorProto, -1 },
};
const struct proto_message type_ServiceDescriptorProto = {
	sizeof(struct ServiceDescriptorProto),
	sizeof(fields_ServiceDescriptorProto) / sizeof(struct proto_field),
	fields_ServiceDescriptorProto
};

static const struct proto_field fields_MethodDescriptorProto[] = {
	{ {0, ""}, PROTO_STRING, offsetof(struct MethodDescriptorProto, name), 10, NULL, -1 },
	{ {0, ""}, PROTO_STRING, offsetof(struct MethodDescriptorProto, input_type), 18, NULL, -1 },
	{ {0, ""}, PROTO_STRING, offsetof(struct MethodDescriptorProto, output_type), 26, NULL, -1 },
	{ {0, ""}, PROTO_BOOL, offsetof(struct MethodDescriptorProto, client_streaming), 40, NULL, -1 },
	{ {0, ""}, PROTO_BOOL, offsetof(struct MethodDescriptorProto, server_streaming), 48, NULL, -1 }
};
const struct proto_message type_MethodDescriptorProto = {
	sizeof(struct MethodDescriptorProto),
	sizeof(fields_MethodDescriptorProto) / sizeof(struct proto_field),
	fields_MethodDescriptorProto
};

static const struct proto_field fields_CodeGeneratorRequest[] = {
	{ {0, ""}, PROTO_LIST_STRING, offsetof(struct CodeGeneratorRequest, file_to_generate), 10, NULL, -1 },
	{ {0, ""}, PROTO_LIST_MESSAGE, offsetof(struct CodeGeneratorRequest, proto_file), 122, &type_FileDescriptorProto, -1 }
};
const struct proto_message type_CodeGeneratorRequest = {
	sizeof(struct CodeGeneratorRequest),
	sizeof(fields_CodeGeneratorRequest) / sizeof(struct proto_field),
	fields_CodeGeneratorRequest
};






