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

char *pb_parse_FileDescriptorSet(char *p, pb_alloc_t *a, struct FileDescriptorSet *m);
int pb_fparse_FileDescriptorSet(const char *fn, str_t *b, pb_alloc_t *a, struct FileDescriptorSet *m);
void pb_print_FileDescriptorSet(str_t *a, struct FileDescriptorSet const *m);
void pb_get_FileDescriptorSet(const char *p, const char *e, pb_alloc_t *obj, struct FileDescriptorSet *m);
char *pb_encode_FileDescriptorSet(char *p, struct FileDescriptorSet const *m);
int pb_maxsz_FileDescriptorSet(struct FileDescriptorSet const *m);

char *pb_parse_FileDescriptorProto(char *p, pb_alloc_t *a, struct FileDescriptorProto *m);
int pb_fparse_FileDescriptorProto(const char *fn, str_t *b, pb_alloc_t *a, struct FileDescriptorProto *m);
void pb_print_FileDescriptorProto(str_t *a, struct FileDescriptorProto const *m);
void pb_get_FileDescriptorProto(const char *p, const char *e, pb_alloc_t *obj, struct FileDescriptorProto *m);
char *pb_encode_FileDescriptorProto(char *p, struct FileDescriptorProto const *m);
int pb_maxsz_FileDescriptorProto(struct FileDescriptorProto const *m);

char *pb_parse_DescriptorProto(char *p, pb_alloc_t *a, struct DescriptorProto *m);
int pb_fparse_DescriptorProto(const char *fn, str_t *b, pb_alloc_t *a, struct DescriptorProto *m);
void pb_print_DescriptorProto(str_t *a, struct DescriptorProto const *m);
void pb_get_DescriptorProto(const char *p, const char *e, pb_alloc_t *obj, struct DescriptorProto *m);
char *pb_encode_DescriptorProto(char *p, struct DescriptorProto const *m);
int pb_maxsz_DescriptorProto(struct DescriptorProto const *m);

bool pb_nonzero_DescriptorProto_ExtensionRange(struct DescriptorProto_ExtensionRange const *m);
char *pb_parse_DescriptorProto_ExtensionRange(char *p, struct DescriptorProto_ExtensionRange *m);
int pb_fparse_DescriptorProto_ExtensionRange(const char *fn, str_t *b, struct DescriptorProto_ExtensionRange *m);
void pb_print_DescriptorProto_ExtensionRange(str_t *a, struct DescriptorProto_ExtensionRange const *m);
void pb_get_DescriptorProto_ExtensionRange(const char *p, const char *e, struct DescriptorProto_ExtensionRange *m);
char *pb_encode_DescriptorProto_ExtensionRange(char *p, struct DescriptorProto_ExtensionRange const *m);
int pb_maxsz_DescriptorProto_ExtensionRange(struct DescriptorProto_ExtensionRange const *m);

bool pb_nonzero_DescriptorProto_ReservedRange(struct DescriptorProto_ReservedRange const *m);
char *pb_parse_DescriptorProto_ReservedRange(char *p, struct DescriptorProto_ReservedRange *m);
int pb_fparse_DescriptorProto_ReservedRange(const char *fn, str_t *b, struct DescriptorProto_ReservedRange *m);
void pb_print_DescriptorProto_ReservedRange(str_t *a, struct DescriptorProto_ReservedRange const *m);
void pb_get_DescriptorProto_ReservedRange(const char *p, const char *e, struct DescriptorProto_ReservedRange *m);
char *pb_encode_DescriptorProto_ReservedRange(char *p, struct DescriptorProto_ReservedRange const *m);
int pb_maxsz_DescriptorProto_ReservedRange(struct DescriptorProto_ReservedRange const *m);

char *pb_parse_FieldDescriptorProto(char *p, pb_alloc_t *a, struct FieldDescriptorProto *m);
int pb_fparse_FieldDescriptorProto(const char *fn, str_t *b, pb_alloc_t *a, struct FieldDescriptorProto *m);
void pb_print_FieldDescriptorProto(str_t *a, struct FieldDescriptorProto const *m);
void pb_get_FieldDescriptorProto(const char *p, const char *e, pb_alloc_t *obj, struct FieldDescriptorProto *m);
char *pb_encode_FieldDescriptorProto(char *p, struct FieldDescriptorProto const *m);
int pb_maxsz_FieldDescriptorProto(struct FieldDescriptorProto const *m);

char *pb_print_FieldDescriptorProto_Type(char *p, enum FieldDescriptorProto_Type v);
char *pb_parse_FieldDescriptorProto_Type(char *p, enum FieldDescriptorProto_Type *v);

char *pb_print_FieldDescriptorProto_Label(char *p, enum FieldDescriptorProto_Label v);
char *pb_parse_FieldDescriptorProto_Label(char *p, enum FieldDescriptorProto_Label *v);

char *pb_parse_OneofDescriptorProto(char *p, pb_alloc_t *a, struct OneofDescriptorProto *m);
int pb_fparse_OneofDescriptorProto(const char *fn, str_t *b, pb_alloc_t *a, struct OneofDescriptorProto *m);
void pb_print_OneofDescriptorProto(str_t *a, struct OneofDescriptorProto const *m);
void pb_get_OneofDescriptorProto(const char *p, const char *e, pb_alloc_t *obj, struct OneofDescriptorProto *m);
char *pb_encode_OneofDescriptorProto(char *p, struct OneofDescriptorProto const *m);
int pb_maxsz_OneofDescriptorProto(struct OneofDescriptorProto const *m);

char *pb_parse_EnumDescriptorProto(char *p, pb_alloc_t *a, struct EnumDescriptorProto *m);
int pb_fparse_EnumDescriptorProto(const char *fn, str_t *b, pb_alloc_t *a, struct EnumDescriptorProto *m);
void pb_print_EnumDescriptorProto(str_t *a, struct EnumDescriptorProto const *m);
void pb_get_EnumDescriptorProto(const char *p, const char *e, pb_alloc_t *obj, struct EnumDescriptorProto *m);
char *pb_encode_EnumDescriptorProto(char *p, struct EnumDescriptorProto const *m);
int pb_maxsz_EnumDescriptorProto(struct EnumDescriptorProto const *m);

char *pb_parse_EnumValueDescriptorProto(char *p, pb_alloc_t *a, struct EnumValueDescriptorProto *m);
int pb_fparse_EnumValueDescriptorProto(const char *fn, str_t *b, pb_alloc_t *a, struct EnumValueDescriptorProto *m);
void pb_print_EnumValueDescriptorProto(str_t *a, struct EnumValueDescriptorProto const *m);
void pb_get_EnumValueDescriptorProto(const char *p, const char *e, pb_alloc_t *obj, struct EnumValueDescriptorProto *m);
char *pb_encode_EnumValueDescriptorProto(char *p, struct EnumValueDescriptorProto const *m);
int pb_maxsz_EnumValueDescriptorProto(struct EnumValueDescriptorProto const *m);

char *pb_parse_ServiceDescriptorProto(char *p, pb_alloc_t *a, struct ServiceDescriptorProto *m);
int pb_fparse_ServiceDescriptorProto(const char *fn, str_t *b, pb_alloc_t *a, struct ServiceDescriptorProto *m);
void pb_print_ServiceDescriptorProto(str_t *a, struct ServiceDescriptorProto const *m);
void pb_get_ServiceDescriptorProto(const char *p, const char *e, pb_alloc_t *obj, struct ServiceDescriptorProto *m);
char *pb_encode_ServiceDescriptorProto(char *p, struct ServiceDescriptorProto const *m);
int pb_maxsz_ServiceDescriptorProto(struct ServiceDescriptorProto const *m);

char *pb_parse_MethodDescriptorProto(char *p, pb_alloc_t *a, struct MethodDescriptorProto *m);
int pb_fparse_MethodDescriptorProto(const char *fn, str_t *b, pb_alloc_t *a, struct MethodDescriptorProto *m);
void pb_print_MethodDescriptorProto(str_t *a, struct MethodDescriptorProto const *m);
void pb_get_MethodDescriptorProto(const char *p, const char *e, pb_alloc_t *obj, struct MethodDescriptorProto *m);
char *pb_encode_MethodDescriptorProto(char *p, struct MethodDescriptorProto const *m);
int pb_maxsz_MethodDescriptorProto(struct MethodDescriptorProto const *m);

char *pb_parse_FileOptions(char *p, pb_alloc_t *a, struct FileOptions *m);
int pb_fparse_FileOptions(const char *fn, str_t *b, pb_alloc_t *a, struct FileOptions *m);
void pb_print_FileOptions(str_t *a, struct FileOptions const *m);
void pb_get_FileOptions(const char *p, const char *e, pb_alloc_t *obj, struct FileOptions *m);
char *pb_encode_FileOptions(char *p, struct FileOptions const *m);
int pb_maxsz_FileOptions(struct FileOptions const *m);

char *pb_print_FileOptions_OptimizeMode(char *p, enum FileOptions_OptimizeMode v);
char *pb_parse_FileOptions_OptimizeMode(char *p, enum FileOptions_OptimizeMode *v);

char *pb_parse_MessageOptions(char *p, pb_alloc_t *a, struct MessageOptions *m);
int pb_fparse_MessageOptions(const char *fn, str_t *b, pb_alloc_t *a, struct MessageOptions *m);
void pb_print_MessageOptions(str_t *a, struct MessageOptions const *m);
void pb_get_MessageOptions(const char *p, const char *e, pb_alloc_t *obj, struct MessageOptions *m);
char *pb_encode_MessageOptions(char *p, struct MessageOptions const *m);
int pb_maxsz_MessageOptions(struct MessageOptions const *m);

char *pb_parse_FieldOptions(char *p, pb_alloc_t *a, struct FieldOptions *m);
int pb_fparse_FieldOptions(const char *fn, str_t *b, pb_alloc_t *a, struct FieldOptions *m);
void pb_print_FieldOptions(str_t *a, struct FieldOptions const *m);
void pb_get_FieldOptions(const char *p, const char *e, pb_alloc_t *obj, struct FieldOptions *m);
char *pb_encode_FieldOptions(char *p, struct FieldOptions const *m);
int pb_maxsz_FieldOptions(struct FieldOptions const *m);

char *pb_print_FieldOptions_CType(char *p, enum FieldOptions_CType v);
char *pb_parse_FieldOptions_CType(char *p, enum FieldOptions_CType *v);

char *pb_print_FieldOptions_JSType(char *p, enum FieldOptions_JSType v);
char *pb_parse_FieldOptions_JSType(char *p, enum FieldOptions_JSType *v);

char *pb_parse_OneofOptions(char *p, pb_alloc_t *a, struct OneofOptions *m);
int pb_fparse_OneofOptions(const char *fn, str_t *b, pb_alloc_t *a, struct OneofOptions *m);
void pb_print_OneofOptions(str_t *a, struct OneofOptions const *m);
void pb_get_OneofOptions(const char *p, const char *e, pb_alloc_t *obj, struct OneofOptions *m);
char *pb_encode_OneofOptions(char *p, struct OneofOptions const *m);
int pb_maxsz_OneofOptions(struct OneofOptions const *m);

char *pb_parse_EnumOptions(char *p, pb_alloc_t *a, struct EnumOptions *m);
int pb_fparse_EnumOptions(const char *fn, str_t *b, pb_alloc_t *a, struct EnumOptions *m);
void pb_print_EnumOptions(str_t *a, struct EnumOptions const *m);
void pb_get_EnumOptions(const char *p, const char *e, pb_alloc_t *obj, struct EnumOptions *m);
char *pb_encode_EnumOptions(char *p, struct EnumOptions const *m);
int pb_maxsz_EnumOptions(struct EnumOptions const *m);

char *pb_parse_EnumValueOptions(char *p, pb_alloc_t *a, struct EnumValueOptions *m);
int pb_fparse_EnumValueOptions(const char *fn, str_t *b, pb_alloc_t *a, struct EnumValueOptions *m);
void pb_print_EnumValueOptions(str_t *a, struct EnumValueOptions const *m);
void pb_get_EnumValueOptions(const char *p, const char *e, pb_alloc_t *obj, struct EnumValueOptions *m);
char *pb_encode_EnumValueOptions(char *p, struct EnumValueOptions const *m);
int pb_maxsz_EnumValueOptions(struct EnumValueOptions const *m);

char *pb_parse_ServiceOptions(char *p, pb_alloc_t *a, struct ServiceOptions *m);
int pb_fparse_ServiceOptions(const char *fn, str_t *b, pb_alloc_t *a, struct ServiceOptions *m);
void pb_print_ServiceOptions(str_t *a, struct ServiceOptions const *m);
void pb_get_ServiceOptions(const char *p, const char *e, pb_alloc_t *obj, struct ServiceOptions *m);
char *pb_encode_ServiceOptions(char *p, struct ServiceOptions const *m);
int pb_maxsz_ServiceOptions(struct ServiceOptions const *m);

char *pb_parse_MethodOptions(char *p, pb_alloc_t *a, struct MethodOptions *m);
int pb_fparse_MethodOptions(const char *fn, str_t *b, pb_alloc_t *a, struct MethodOptions *m);
void pb_print_MethodOptions(str_t *a, struct MethodOptions const *m);
void pb_get_MethodOptions(const char *p, const char *e, pb_alloc_t *obj, struct MethodOptions *m);
char *pb_encode_MethodOptions(char *p, struct MethodOptions const *m);
int pb_maxsz_MethodOptions(struct MethodOptions const *m);

char *pb_parse_UninterpretedOption(char *p, pb_alloc_t *a, struct UninterpretedOption *m);
int pb_fparse_UninterpretedOption(const char *fn, str_t *b, pb_alloc_t *a, struct UninterpretedOption *m);
void pb_print_UninterpretedOption(str_t *a, struct UninterpretedOption const *m);
void pb_get_UninterpretedOption(const char *p, const char *e, pb_alloc_t *obj, struct UninterpretedOption *m);
char *pb_encode_UninterpretedOption(char *p, struct UninterpretedOption const *m);
int pb_maxsz_UninterpretedOption(struct UninterpretedOption const *m);

char *pb_parse_UninterpretedOption_NamePart(char *p, pb_alloc_t *a, struct UninterpretedOption_NamePart *m);
int pb_fparse_UninterpretedOption_NamePart(const char *fn, str_t *b, pb_alloc_t *a, struct UninterpretedOption_NamePart *m);
void pb_print_UninterpretedOption_NamePart(str_t *a, struct UninterpretedOption_NamePart const *m);
void pb_get_UninterpretedOption_NamePart(const char *p, const char *e, pb_alloc_t *obj, struct UninterpretedOption_NamePart *m);
char *pb_encode_UninterpretedOption_NamePart(char *p, struct UninterpretedOption_NamePart const *m);
int pb_maxsz_UninterpretedOption_NamePart(struct UninterpretedOption_NamePart const *m);

char *pb_parse_SourceCodeInfo(char *p, pb_alloc_t *a, struct SourceCodeInfo *m);
int pb_fparse_SourceCodeInfo(const char *fn, str_t *b, pb_alloc_t *a, struct SourceCodeInfo *m);
void pb_print_SourceCodeInfo(str_t *a, struct SourceCodeInfo const *m);
void pb_get_SourceCodeInfo(const char *p, const char *e, pb_alloc_t *obj, struct SourceCodeInfo *m);
char *pb_encode_SourceCodeInfo(char *p, struct SourceCodeInfo const *m);
int pb_maxsz_SourceCodeInfo(struct SourceCodeInfo const *m);

char *pb_parse_SourceCodeInfo_Location(char *p, pb_alloc_t *a, struct SourceCodeInfo_Location *m);
int pb_fparse_SourceCodeInfo_Location(const char *fn, str_t *b, pb_alloc_t *a, struct SourceCodeInfo_Location *m);
void pb_print_SourceCodeInfo_Location(str_t *a, struct SourceCodeInfo_Location const *m);
void pb_get_SourceCodeInfo_Location(const char *p, const char *e, pb_alloc_t *obj, struct SourceCodeInfo_Location *m);
char *pb_encode_SourceCodeInfo_Location(char *p, struct SourceCodeInfo_Location const *m);
int pb_maxsz_SourceCodeInfo_Location(struct SourceCodeInfo_Location const *m);

char *pb_parse_GeneratedCodeInfo(char *p, pb_alloc_t *a, struct GeneratedCodeInfo *m);
int pb_fparse_GeneratedCodeInfo(const char *fn, str_t *b, pb_alloc_t *a, struct GeneratedCodeInfo *m);
void pb_print_GeneratedCodeInfo(str_t *a, struct GeneratedCodeInfo const *m);
void pb_get_GeneratedCodeInfo(const char *p, const char *e, pb_alloc_t *obj, struct GeneratedCodeInfo *m);
char *pb_encode_GeneratedCodeInfo(char *p, struct GeneratedCodeInfo const *m);
int pb_maxsz_GeneratedCodeInfo(struct GeneratedCodeInfo const *m);

char *pb_parse_GeneratedCodeInfo_Annotation(char *p, pb_alloc_t *a, struct GeneratedCodeInfo_Annotation *m);
int pb_fparse_GeneratedCodeInfo_Annotation(const char *fn, str_t *b, pb_alloc_t *a, struct GeneratedCodeInfo_Annotation *m);
void pb_print_GeneratedCodeInfo_Annotation(str_t *a, struct GeneratedCodeInfo_Annotation const *m);
void pb_get_GeneratedCodeInfo_Annotation(const char *p, const char *e, pb_alloc_t *obj, struct GeneratedCodeInfo_Annotation *m);
char *pb_encode_GeneratedCodeInfo_Annotation(char *p, struct GeneratedCodeInfo_Annotation const *m);
int pb_maxsz_GeneratedCodeInfo_Annotation(struct GeneratedCodeInfo_Annotation const *m);
#ifdef __cplusplus
}
#endif
