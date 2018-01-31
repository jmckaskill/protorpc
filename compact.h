#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <protobuf.h>

#if 0
typedef struct {
	uint8_t *next;
	uint8_t *end;
} pb_buf_t;

// Messages are of the form
// struct my_message {
//   union pb_msg pb_msg;
//   fields...
// }
union pb_msg {
	union pb_msg *prev;
	int maxsz;
};

typedef struct {
	int len;
	const uint8_t *p;
} pb_bytes_t;

typedef struct {
	int len;
	const char *c_str;
} pb_string_t;
#endif

// lists are of the form
// struct {int len; <type> const *v;} field;
struct pb_message_list {
	int len;
	union {
		union pb_msg **v;
		union pb_msg *last;
	} u;
};

struct pb_pod_list {
	int len;
	char *data;
};

typedef struct {bool set; unsigned val;} pb_opt_uint;

typedef struct {int len; bool const *v;} pb_bool_list;
typedef struct {int len; uint32_t const *v; int _encoded;} pb_uint_list;
typedef struct {int len; int32_t const *v; int _encoded;} pb_int_list;
typedef struct {int len; uint64_t const *v; int _encoded;} pb_u64_list;
typedef struct {int len; int64_t const *v; int _encoded;} pb_i64_list;
typedef struct {int len; float const *v;} pb_float_list;
typedef struct {int len; double const *v;} pb_double_list;
typedef struct {int len; pb_string_t const *v;} pb_string_list;
typedef struct {int len; pb_bytes_t const *v;} pb_bytes_list;

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

struct proto_enum_value {
	int number;
	const char *name;
};

struct proto_enum {
	size_t num_values;
	const struct proto_enum_value *values;
};

struct proto_field {
	enum proto_field_type type;
	size_t offset;
	unsigned tag;
	const void *proto_type;
	int oneof;
	const char *json_name;
};

struct proto_message {
	size_t datasz;
	size_t num_fields;
	const struct proto_field *fields;
};

#ifdef __cplusplus
extern "C" {
#endif

void *pb_decode(pb_buf_t *obj, const struct proto_message *type, char *data, int sz);
int pb_encoded_size(void *obj, const struct proto_message *type);
int pb_encode(void *obj, const struct proto_message *type, char *data);
int pb_print(void *obj, const struct proto_message *type, char *buf, int sz);

#ifdef __cplusplus
}
#endif
