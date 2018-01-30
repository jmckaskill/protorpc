#pragma once

#include <stdint.h>

struct pb_buf_t {
	uint8_t *next;
	uint8_t *end;
};

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
} pb_bytes;

typedef struct {
	int len;
	const char *c_str;
} pb_string;

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

typedef struct {int len; bool const *v;} pb_bool_list;
typedef struct {int len; uint32_t const *v;} pb_uint_list;
typedef struct {int len; int32_t const *v;} pb_int_list;
typedef struct {int len; uint64_t const *v;} pb_u64_list;
typedef struct {int len; int64_t const *v;} pb_i64_list;
typedef struct {int len; float const *v;} pb_float_list;
typedef struct {int len; double const *v;} pb_double_list;
typedef struct {int len; pb_string const *v;} pb_string_list;
typedef struct {int len; pb_bytes const *v;} pb_bytes_list;

enum proto_field_type {
	PROTO_BOOL,
	PROTO_U32,
	PROTO_I32,
	PROTO_S32,
	PROTO_F32,
	PROTO_U64,
	PROTO_I64,
	PROTO_S64,
	PROTO_F64,
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
	PROTO_LIST_U64,
	PROTO_LIST_I64,
	PROTO_LIST_S64,
	PROTO_LIST_F64,
	PROTO_LIST_FLOAT,
	PROTO_LIST_DOUBLE,
	PROTO_LIST_STRING,
	PROTO_LIST_BYTES,
	PROTO_LIST_ENUM,
	PROTO_LIST_POD,
	PROTO_LIST_MESSAGE,
};

struct proto_field {
	enum proto_field_type type;
	size_t offset;
	unsigned tag;
	const struct proto_message *message;
};

struct proto_message {
	size_t datasz;
	size_t num_fields;
	const struct proto_field *fields;
};

void *pb_decode(pb_buf_t *obj, const struct proto_message *type, const char *data, int sz);


