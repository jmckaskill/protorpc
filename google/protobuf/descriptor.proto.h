#pragma once
#include <protobuf.h>
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
	union pb_msg pb_hdr;
	int32_t	start;
	int32_t	end;
	struct ExtensionRangeOptions const*	options;
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
	struct {int len; struct DescriptorProto_ExtensionRange const **v;}	extension_range;
	struct {int len; struct OneofDescriptorProto const **v;}	oneof_decl;
	struct MessageOptions const*	options;
	struct {int len; struct DescriptorProto_ReservedRange const *v;}	reserved_range;
	struct {int len; struct pb_string const *v;}	reserved_name;
};
struct ExtensionRangeOptions {
	union pb_msg pb_hdr;
	struct {int len; struct UninterpretedOption const **v;}	uninterpreted_option;
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
struct EnumDescriptorProto_EnumReservedRange {
	int32_t	start;
	int32_t	end;
};
struct EnumDescriptorProto {
	union pb_msg pb_hdr;
	struct pb_string	name;
	struct {int len; struct EnumValueDescriptorProto const **v;}	value;
	struct EnumOptions const*	options;
	struct {int len; struct EnumDescriptorProto_EnumReservedRange const *v;}	reserved_range;
	struct {int len; struct pb_string const *v;}	reserved_name;
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
	bool	php_generic_services;
	bool	deprecated;
	bool	cc_enable_arenas;
	struct pb_string	objc_class_prefix;
	struct pb_string	csharp_namespace;
	struct pb_string	swift_prefix;
	struct pb_string	php_class_prefix;
	struct pb_string	php_namespace;
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
enum MethodOptions_IdempotencyLevel {
	IDEMPOTENCY_UNKNOWN = 0,
	NO_SIDE_EFFECTS = 1,
	IDEMPOTENT = 2
};
struct MethodOptions {
	union pb_msg pb_hdr;
	bool	deprecated;
	enum MethodOptions_IdempotencyLevel	idempotency_level;
	struct {int len; struct UninterpretedOption const **v;}	uninterpreted_option;
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
void pb_term_FileDescriptorSet(struct FileDescriptorSet *m);

int pb_get_FileDescriptorProto(const char *p, const char *e, pb_buf_t *obj, struct FileDescriptorProto *m);
void pb_term_FileDescriptorProto(struct FileDescriptorProto *m);

int pb_get_DescriptorProto(const char *p, const char *e, pb_buf_t *obj, struct DescriptorProto *m);
void pb_term_DescriptorProto(struct DescriptorProto *m);

int pb_get_DescriptorProto_ExtensionRange(const char *p, const char *e, pb_buf_t *obj, struct DescriptorProto_ExtensionRange *m);
void pb_term_DescriptorProto_ExtensionRange(struct DescriptorProto_ExtensionRange *m);

int pb_get_DescriptorProto_ReservedRange(const char *p, const char *e, struct DescriptorProto_ReservedRange *m);
void pb_term_DescriptorProto_ReservedRange(struct DescriptorProto_ReservedRange *m);

int pb_get_ExtensionRangeOptions(const char *p, const char *e, pb_buf_t *obj, struct ExtensionRangeOptions *m);
void pb_term_ExtensionRangeOptions(struct ExtensionRangeOptions *m);

int pb_get_FieldDescriptorProto(const char *p, const char *e, pb_buf_t *obj, struct FieldDescriptorProto *m);
void pb_term_FieldDescriptorProto(struct FieldDescriptorProto *m);

int pb_get_OneofDescriptorProto(const char *p, const char *e, pb_buf_t *obj, struct OneofDescriptorProto *m);
void pb_term_OneofDescriptorProto(struct OneofDescriptorProto *m);

int pb_get_EnumDescriptorProto(const char *p, const char *e, pb_buf_t *obj, struct EnumDescriptorProto *m);
void pb_term_EnumDescriptorProto(struct EnumDescriptorProto *m);

int pb_get_EnumDescriptorProto_EnumReservedRange(const char *p, const char *e, struct EnumDescriptorProto_EnumReservedRange *m);
void pb_term_EnumDescriptorProto_EnumReservedRange(struct EnumDescriptorProto_EnumReservedRange *m);

int pb_get_EnumValueDescriptorProto(const char *p, const char *e, pb_buf_t *obj, struct EnumValueDescriptorProto *m);
void pb_term_EnumValueDescriptorProto(struct EnumValueDescriptorProto *m);

int pb_get_ServiceDescriptorProto(const char *p, const char *e, pb_buf_t *obj, struct ServiceDescriptorProto *m);
void pb_term_ServiceDescriptorProto(struct ServiceDescriptorProto *m);

int pb_get_MethodDescriptorProto(const char *p, const char *e, pb_buf_t *obj, struct MethodDescriptorProto *m);
void pb_term_MethodDescriptorProto(struct MethodDescriptorProto *m);

int pb_get_FileOptions(const char *p, const char *e, pb_buf_t *obj, struct FileOptions *m);
void pb_term_FileOptions(struct FileOptions *m);

int pb_get_MessageOptions(const char *p, const char *e, pb_buf_t *obj, struct MessageOptions *m);
void pb_term_MessageOptions(struct MessageOptions *m);

int pb_get_FieldOptions(const char *p, const char *e, pb_buf_t *obj, struct FieldOptions *m);
void pb_term_FieldOptions(struct FieldOptions *m);

int pb_get_OneofOptions(const char *p, const char *e, pb_buf_t *obj, struct OneofOptions *m);
void pb_term_OneofOptions(struct OneofOptions *m);

int pb_get_EnumOptions(const char *p, const char *e, pb_buf_t *obj, struct EnumOptions *m);
void pb_term_EnumOptions(struct EnumOptions *m);

int pb_get_EnumValueOptions(const char *p, const char *e, pb_buf_t *obj, struct EnumValueOptions *m);
void pb_term_EnumValueOptions(struct EnumValueOptions *m);

int pb_get_ServiceOptions(const char *p, const char *e, pb_buf_t *obj, struct ServiceOptions *m);
void pb_term_ServiceOptions(struct ServiceOptions *m);

int pb_get_MethodOptions(const char *p, const char *e, pb_buf_t *obj, struct MethodOptions *m);
void pb_term_MethodOptions(struct MethodOptions *m);

int pb_get_UninterpretedOption(const char *p, const char *e, pb_buf_t *obj, struct UninterpretedOption *m);
void pb_term_UninterpretedOption(struct UninterpretedOption *m);

int pb_get_UninterpretedOption_NamePart(const char *p, const char *e, pb_buf_t *obj, struct UninterpretedOption_NamePart *m);
void pb_term_UninterpretedOption_NamePart(struct UninterpretedOption_NamePart *m);

int pb_get_SourceCodeInfo(const char *p, const char *e, pb_buf_t *obj, struct SourceCodeInfo *m);
void pb_term_SourceCodeInfo(struct SourceCodeInfo *m);

int pb_get_SourceCodeInfo_Location(const char *p, const char *e, pb_buf_t *obj, struct SourceCodeInfo_Location *m);
void pb_term_SourceCodeInfo_Location(struct SourceCodeInfo_Location *m);

int pb_get_GeneratedCodeInfo(const char *p, const char *e, pb_buf_t *obj, struct GeneratedCodeInfo *m);
void pb_term_GeneratedCodeInfo(struct GeneratedCodeInfo *m);

int pb_get_GeneratedCodeInfo_Annotation(const char *p, const char *e, pb_buf_t *obj, struct GeneratedCodeInfo_Annotation *m);
void pb_term_GeneratedCodeInfo_Annotation(struct GeneratedCodeInfo_Annotation *m);

#ifdef __cplusplus
}
#endif

