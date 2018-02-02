#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct pb_allocator pb_allocator;
typedef union pb_msg pb_msg;
typedef struct pb_bytes pb_bytes;
typedef struct pb_string pb_string;

struct pb_allocator {
	char *next;
	char *end;
};

#define PB_INIT_ALLOCATOR(buf) {buf, buf + sizeof(buf)}

// Messages are of the form
// struct my_message {
//   pb_msg _pbhdr;
//   fields...
// }
union pb_msg {
	pb_msg *previous;
	int encoded_size;
};

struct pb_bytes {
	int len;
	const uint8_t *p;
};

struct pb_string {
	int len;
	const char *c_str;
};

typedef struct {bool set; unsigned val;} pb_opt_uint;

typedef struct {int len; bool const *v;} pb_bool_list;
typedef struct {int len; uint32_t const *v; int _encoded;} pb_u32_list;
typedef struct {int len; int32_t const *v; int _encoded;} pb_i32_list;
typedef struct {int len; uint64_t const *v; int _encoded;} pb_u64_list;
typedef struct {int len; int64_t const *v; int _encoded;} pb_i64_list;
typedef struct {int len; float const *v;} pb_float_list;
typedef struct {int len; double const *v;} pb_double_list;
typedef struct {int len; pb_string const *v;} pb_string_list;
typedef struct {int len; pb_bytes const *v;} pb_bytes_list;


enum proto_field_type {
	PROTO_BOOL,
	PROTO_U32,
	PROTO_OPTIONAL_U32,
	PROTO_I32,
	PROTO_S32,
	PROTO_F32,
	PROTO_SF32,
	PROTO_U64,
	PROTO_I64,
	PROTO_S64,
	PROTO_F64,
	PROTO_SF64,
	PROTO_FLOAT,
	PROTO_DOUBLE,
	PROTO_STRING,
	PROTO_BYTES,
	PROTO_ENUM,
	PROTO_POD,
	PROTO_MESSAGE,
	PROTO_LIST_BOOL,
	PROTO_LIST_U32,
	PROTO_LIST_I32,
	PROTO_LIST_S32,
	PROTO_LIST_F32,
	PROTO_LIST_SF32,
	PROTO_LIST_U64,
	PROTO_LIST_I64,
	PROTO_LIST_S64,
	PROTO_LIST_F64,
	PROTO_LIST_SF64,
	PROTO_LIST_FLOAT,
	PROTO_LIST_DOUBLE,
	PROTO_LIST_STRING,
	PROTO_LIST_BYTES,
	PROTO_LIST_ENUM,
	PROTO_LIST_POD,
	PROTO_LIST_MESSAGE,
};

typedef struct proto_field proto_field;
typedef struct proto_enum proto_enum;
typedef struct proto_enum_value proto_enum_value;
typedef struct proto_message proto_message;
typedef struct proto_service proto_service;
typedef struct proto_method proto_method;

struct proto_enum_value {
	// name needs to be first to allow the parse binary_search to work
	pb_string name;
	int number;
};

struct proto_enum {
	size_t num_values;
	const struct proto_enum_value *values;
	const pb_string **by_name;
};

struct proto_field {
	// json_name needs to be first to allow the parse binary_search to work
	pb_string json_name;
	enum proto_field_type type;
	size_t offset;
	unsigned tag;
	const void *proto_type;
	int oneof;
};

struct proto_message {
	size_t datasz;
	size_t num_fields;
	const struct proto_field *fields;
	const pb_string **by_name;
};

typedef int(*proto_method_fn)(void *, pb_allocator*, const void*, void*);

struct proto_method {
	pb_string path;
	int offset;
	const proto_message *input;
	const proto_message *output;
};

struct proto_service {
	size_t num_methods;
	const pb_string **by_path;
};

void *pb_calloc(pb_allocator *obj, size_t num, size_t sz);
void *pb_decode(pb_allocator *obj, const proto_message *type, char *data, int sz);
int pb_encoded_size(void *obj, const proto_message *type);
int pb_encode(void *obj, const proto_message *type, char *data);
int pb_print(void *obj, const proto_message *type, char *buf, int sz);
void *pb_parse(pb_allocator *obj, const proto_message *type, char *p);

const proto_method *pb_lookup_method(void *svc, const proto_service *type, const char *path);
int pb_dispatch(void *svc, const proto_method *method, pb_allocator *obj, char *in, int insz, char *out, int outsz);

static inline int pb_base64_size(int sz) {
	return (sz * 4 + 3) / 3;
}

char *pb_encode_base64(char *out, const uint8_t *v, int n);
char *pb_decode_base64(char *text, pb_bytes *v);

#ifdef __cplusplus
}
#endif
