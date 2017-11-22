#pragma once
#include <protorpc/protorpc.h>
#ifdef __cplusplus
extern "C" {
#endif
struct FileDescriptorSet {
	union pb_msg pb_hdr;
	struct {int len; struct FileDescriptorProto const **v;}	file;
};
struct FileDescriptorProto {
	union pb_msg pb_hdr;
	struct pb_string	name;
	struct pb_string	package;
	struct {int len; struct pb_string const *v;}	dependency;
	struct {int len; int32_t const *v;}	public_dependency;
	struct {int len; int32_t const *v;}	weak_dependency;
	struct {int len; struct DescriptorProto const **v;}	message_type;
	struct {int len; struct EnumDescriptorProto const **v;}	enum_type;
	struct {int len; struct ServiceDescriptorProto const **v;}	service;
	struct {int len; struct FieldDescriptorProto const **v;}	extension;
	struct FileOptions const*	options;
	struct SourceCodeInfo const*	source_code_info;
	struct pb_string	syntax;
};
struct DescriptorProto_ExtensionRange {
	int32_t	start;
	int32_t	end;
};
struct DescriptorProto_ReservedRange {
	int32_t	start;
	int32_t	end;
};
struct DescriptorProto {
	union pb_msg pb_hdr;
	struct pb_string	name;
	struct {int len; struct FieldDescriptorProto const **v;}	field;
	struct {int len; struct FieldDescriptorProto const **v;}	extension;
	struct {int len; struct DescriptorProto const **v;}	nested_type;
	struct {int len; struct EnumDescriptorProto const **v;}	enum_type;
	struct {int len; struct DescriptorProto_ExtensionRange const *v;}	extension_range;
	struct {int len; struct OneofDescriptorProto const **v;}	oneof_decl;
	struct MessageOptions const*	options;
	struct {int len; struct DescriptorProto_ReservedRange const *v;}	reserved_range;
	struct {int len; struct pb_string const *v;}	reserved_name;
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
	struct pb_string	name;
	int32_t	number;
	enum FieldDescriptorProto_Label	label;
	enum FieldDescriptorProto_Type	type;
	struct pb_string	type_name;
	struct pb_string	extendee;
	struct pb_string	default_value;
	int32_t	oneof_index;
	struct pb_string	json_name;
	struct FieldOptions const*	options;
	bool oneof_index_set;
};
struct OneofDescriptorProto {
	union pb_msg pb_hdr;
	struct pb_string	name;
	struct OneofOptions const*	options;
};
struct EnumDescriptorProto {
	union pb_msg pb_hdr;
	struct pb_string	name;
	struct {int len; struct EnumValueDescriptorProto const **v;}	value;
	struct EnumOptions const*	options;
};
struct EnumValueDescriptorProto {
	union pb_msg pb_hdr;
	struct pb_string	name;
	int32_t	number;
	struct EnumValueOptions const*	options;
};
struct ServiceDescriptorProto {
	union pb_msg pb_hdr;
	struct pb_string	name;
	struct {int len; struct MethodDescriptorProto const **v;}	method;
	struct ServiceOptions const*	options;
};
struct MethodDescriptorProto {
	union pb_msg pb_hdr;
	struct pb_string	name;
	struct pb_string	input_type;
	struct pb_string	output_type;
	struct MethodOptions const*	options;
	bool	client_streaming;
	bool	server_streaming;
};
enum FileOptions_OptimizeMode {
	SPEED = 1,
	CODE_SIZE = 2,
	LITE_RUNTIME = 3
};
struct FileOptions {
	union pb_msg pb_hdr;
	struct pb_string	java_package;
	struct pb_string	java_outer_classname;
	bool	java_multiple_files;
	bool	java_generate_equals_and_hash;
	bool	java_string_check_utf8;
	enum FileOptions_OptimizeMode	optimize_for;
	struct pb_string	go_package;
	bool	cc_generic_services;
	bool	java_generic_services;
	bool	py_generic_services;
	bool	deprecated;
	bool	cc_enable_arenas;
	struct pb_string	objc_class_prefix;
	struct pb_string	csharp_namespace;
	struct {int len; struct UninterpretedOption const **v;}	uninterpreted_option;
};
struct MessageOptions {
	union pb_msg pb_hdr;
	bool	message_set_wire_format;
	bool	no_standard_descriptor_accessor;
	bool	deprecated;
	bool	map_entry;
	struct {int len; struct UninterpretedOption const **v;}	uninterpreted_option;
};
enum FieldOptions_CType {
	STRING = 0,
	CORD = 1,
	STRING_PIECE = 2
};
enum FieldOptions_JSType {
	JS_NORMAL = 0,
	JS_STRING = 1,
	JS_NUMBER = 2
};
struct FieldOptions {
	union pb_msg pb_hdr;
	enum FieldOptions_CType	ctype;
	bool	packed;
	enum FieldOptions_JSType	jstype;
	bool	lazy;
	bool	deprecated;
	bool	weak;
	struct {int len; struct UninterpretedOption const **v;}	uninterpreted_option;
	uint32_t	array;
	bool packed_set;
};
struct OneofOptions {
	union pb_msg pb_hdr;
	struct {int len; struct UninterpretedOption const **v;}	uninterpreted_option;
};
struct EnumOptions {
	union pb_msg pb_hdr;
	bool	allow_alias;
	bool	deprecated;
	struct {int len; struct UninterpretedOption const **v;}	uninterpreted_option;
};
struct EnumValueOptions {
	union pb_msg pb_hdr;
	bool	deprecated;
	struct {int len; struct UninterpretedOption const **v;}	uninterpreted_option;
};
struct ServiceOptions {
	union pb_msg pb_hdr;
	bool	deprecated;
	struct {int len; struct UninterpretedOption const **v;}	uninterpreted_option;
};
struct MethodOptions {
	union pb_msg pb_hdr;
	bool	deprecated;
	struct {int len; struct UninterpretedOption const **v;}	uninterpreted_option;
	bool	binary;
};
struct UninterpretedOption_NamePart {
	union pb_msg pb_hdr;
	struct pb_string	name_part;
	bool	is_extension;
};
struct UninterpretedOption {
	union pb_msg pb_hdr;
	struct {int len; struct UninterpretedOption_NamePart const **v;}	name;
	struct pb_string	identifier_value;
	uint64_t	positive_int_value;
	int64_t	negative_int_value;
	double	double_value;
	struct pb_bytes	string_value;
	struct pb_string	aggregate_value;
};
struct SourceCodeInfo_Location {
	union pb_msg pb_hdr;
	struct {int len; int32_t const *v;}	path;
	struct {int len; int32_t const *v;}	span;
	struct pb_string	leading_comments;
	struct pb_string	trailing_comments;
	struct {int len; struct pb_string const *v;}	leading_detached_comments;
};
struct SourceCodeInfo {
	union pb_msg pb_hdr;
	struct {int len; struct SourceCodeInfo_Location const **v;}	location;
};
struct GeneratedCodeInfo_Annotation {
	union pb_msg pb_hdr;
	struct {int len; int32_t const *v;}	path;
	struct pb_string	source_file;
	int32_t	begin;
	int32_t	end;
};
struct GeneratedCodeInfo {
	union pb_msg pb_hdr;
	struct {int len; struct GeneratedCodeInfo_Annotation const **v;}	annotation;
};

int pb_get_FileDescriptorSet(const char *p, const char *e, pb_buf_t *obj, struct FileDescriptorSet *m);

int pb_get_FileDescriptorProto(const char *p, const char *e, pb_buf_t *obj, struct FileDescriptorProto *m);

int pb_get_DescriptorProto(const char *p, const char *e, pb_buf_t *obj, struct DescriptorProto *m);

int pb_get_DescriptorProto_ExtensionRange(const char *p, const char *e, struct DescriptorProto_ExtensionRange *m);

int pb_get_DescriptorProto_ReservedRange(const char *p, const char *e, struct DescriptorProto_ReservedRange *m);

int pb_get_FieldDescriptorProto(const char *p, const char *e, pb_buf_t *obj, struct FieldDescriptorProto *m);

int pb_get_OneofDescriptorProto(const char *p, const char *e, pb_buf_t *obj, struct OneofDescriptorProto *m);

int pb_get_EnumDescriptorProto(const char *p, const char *e, pb_buf_t *obj, struct EnumDescriptorProto *m);

int pb_get_EnumValueDescriptorProto(const char *p, const char *e, pb_buf_t *obj, struct EnumValueDescriptorProto *m);

int pb_get_ServiceDescriptorProto(const char *p, const char *e, pb_buf_t *obj, struct ServiceDescriptorProto *m);

int pb_get_MethodDescriptorProto(const char *p, const char *e, pb_buf_t *obj, struct MethodDescriptorProto *m);

int pb_get_FileOptions(const char *p, const char *e, pb_buf_t *obj, struct FileOptions *m);

int pb_get_MessageOptions(const char *p, const char *e, pb_buf_t *obj, struct MessageOptions *m);

int pb_get_FieldOptions(const char *p, const char *e, pb_buf_t *obj, struct FieldOptions *m);

int pb_get_OneofOptions(const char *p, const char *e, pb_buf_t *obj, struct OneofOptions *m);

int pb_get_EnumOptions(const char *p, const char *e, pb_buf_t *obj, struct EnumOptions *m);

int pb_get_EnumValueOptions(const char *p, const char *e, pb_buf_t *obj, struct EnumValueOptions *m);

int pb_get_ServiceOptions(const char *p, const char *e, pb_buf_t *obj, struct ServiceOptions *m);

int pb_get_MethodOptions(const char *p, const char *e, pb_buf_t *obj, struct MethodOptions *m);

int pb_get_UninterpretedOption(const char *p, const char *e, pb_buf_t *obj, struct UninterpretedOption *m);

int pb_get_UninterpretedOption_NamePart(const char *p, const char *e, pb_buf_t *obj, struct UninterpretedOption_NamePart *m);

int pb_get_SourceCodeInfo(const char *p, const char *e, pb_buf_t *obj, struct SourceCodeInfo *m);

int pb_get_SourceCodeInfo_Location(const char *p, const char *e, pb_buf_t *obj, struct SourceCodeInfo_Location *m);

int pb_get_GeneratedCodeInfo(const char *p, const char *e, pb_buf_t *obj, struct GeneratedCodeInfo *m);

int pb_get_GeneratedCodeInfo_Annotation(const char *p, const char *e, pb_buf_t *obj, struct GeneratedCodeInfo_Annotation *m);
#ifdef __cplusplus
}
#endif
