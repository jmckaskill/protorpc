#pragma once

#include "google/protobuf/compiler/plugin.proto.h"
#include "../../protobuf.h"
#include "../../char-array.h"
#include "../str.h"
#include "../hash.h"
#include <math.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#define NEW(type) ((type*) calloc(1, sizeof(type)))

struct type {
    str_t c_type;
    float max_proto_size;
    float max_print_size;
    str_t proto_suffix;
	str_t json_suffix;
    str_t proto_type;
    str_t name;
    const struct DescriptorProto *msg;
    const struct EnumDescriptorProto *en;
	const struct ServiceDescriptorProto *svc;
    bool max_proto_size_calculated;
    const struct FileDescriptorProto *declared;
    bool defined;
    const struct FieldDescriptorProto *map_key, *map_value;
    const struct FileDescriptorProto *file;
    unsigned default_packed : 1;
	unsigned pod_message : 1;
	unsigned requires_allocator : 1;
};

#ifdef WIN32
#define EOL "\r\n"
#else
#define EOL "\n"
#endif

void insert_file_types(const struct FileDescriptorProto *s);
struct type *get_struct_type(const struct DescriptorProto *s);
struct type *get_enum_type(const struct EnumDescriptorProto *e);
struct type *get_service_type(const struct ServiceDescriptorProto *s);
struct type *get_field_type(const struct FieldDescriptorProto *f);
struct type *get_input_type(const struct MethodDescriptorProto *m);
struct type *get_output_type(const struct MethodDescriptorProto *m);
uint32_t get_tag_size(const struct FieldDescriptorProto *f);
uint32_t get_tag(const struct FieldDescriptorProto *f);
void get_proto_cast(str_t *out, const struct FieldDescriptorProto* f, int array_type, int value_type);
void to_upper(str_t *out, pb_string_t s);
bool is_field_packed(const struct type *t, const struct FieldDescriptorProto *ft);

void define_enum(str_t *out, const struct type *t);
void declare_struct(str_t *out, const struct type *t);
void define_struct(str_t *out, struct type *t);

void do_enum_funcs(str_t *out, const struct type *t, bool define);
void do_struct_funcs(str_t *out, const struct type *t, bool define, bool decode_only);

void do_maxsz(str_t *out, const struct type *t, bool define);
void do_decode(str_t *out, const struct type *t, bool define);
void do_term(str_t *out, const struct type *t, bool define);
void do_parse_enum(str_t *out, const struct type *t, bool define);
void do_print_enum(str_t *out, const struct type *t, bool define);
void do_parse(str_t *out, const struct type *t, bool define);
void do_print(str_t *out, const struct type *t, bool define);
void do_encode(str_t *out, const struct type *t, bool define);
void do_server(str_t *out, const struct type *t, bool define);
void do_client(str_t *out, const struct type *t, bool define);
void do_nonzero(str_t *o, const struct type *t, bool define);

int exec_protoc(char *my_exe, char *protoc_exe, char *filename);

struct hash_entry {
	pb_string_t str;
	uint32_t off;
};

void calc_hash_values(struct hash_entry *entries, int num, uint32_t *hashmul, uint32_t *hashsz);

