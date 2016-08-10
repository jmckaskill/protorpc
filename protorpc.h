#pragma once
#include "protorpc-dll.h"
#include <os/str.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

// Some basic types

union pb_msg {
	void *prev;
	int maxsz;
};

struct pb_bytes {
	int len;
	uint8_t *p;
};

struct pb_string {
	int len;
	const char *p; // not null terminated
};

static inline struct pb_string pb_as_string(const char *str) {
	struct pb_string s;
	s.len = (int) strlen(str);
	s.p = str;
	return s;
}

static inline int pb_cmp2(struct pb_string a, const char *b, int len) {
	return a.len == len ? memcmp(a.p, b, len) : (int) (a.len - len);
}
static inline int pb_cmp(struct pb_string a, const char *b) {
	return pb_cmp2(a, b, (int) strlen(b));
}
static inline void str_addpb(str_t *s, struct pb_string a) {
	str_add2(s, a.p, a.len);
}

union pb_f32 {
	uint32_t u;
	int32_t i;
	float f;
};
union pb_f64 {
	uint64_t u;
	int64_t i;
	double f;
};

enum pb_wiretype {
	PB_WIRE_VARINT = 0,
	PB_WIRE_F32 = 1,
	PB_WIRE_VARIABLE = 2,
	PB_WIRE_F64 = 5,
};

// Object allocator

#define PB_PGSZ 0x1000U
#define PB_PAGE(x) ((uintptr_t)(x) & ~(uintptr_t)(PB_PGSZ-1))
#define PB_ALIGN8(x) ((x)+7) &~ (7U)
#define PB_ALLOC_INIT {(char*) UINTPTR_MAX, NULL}

struct pb_page;

typedef struct {
	char *next;
	struct pb_page *page;
} pb_alloc_t;

static inline void pb_alloc_init(pb_alloc_t *alloc) {
	alloc->next = (char*) UINTPTR_MAX;
	alloc->page = NULL;
}

PROTORPC_API void pb_alloc_destroy(pb_alloc_t *alloc);
PROTORPC_API void pb_alloc_clear(pb_alloc_t *alloc);

// pb_calloc allocates zeroed objects 
PROTORPC_API void *pb_calloc(pb_alloc_t *alloc, int n, size_t objsz);

// pb_reserve reserves room for one more in a growing vector of objects at the end of the memory pool
PROTORPC_API void *pb_reserve(pb_alloc_t *alloc, int have, size_t objsz);

// pb_commit commits the vector that has been growing using pb_reserve
PROTORPC_API void pb_commit(pb_alloc_t *alloc, int n, size_t objsz);



// Binary reading

PROTORPC_API const char *pb_get_u32(const char *p, const char *e, uint32_t *pv);
PROTORPC_API const char *pb_get_u64(const char *p, const char *e, uint64_t *pv);
PROTORPC_API const char *pb_get_s32(const char *p, const char *e, int32_t *pv);
PROTORPC_API const char *pb_get_s64(const char *p, const char *e, int64_t *pv);
PROTORPC_API const char *pb_get_f32(const char *p, const char *e, union pb_f32 *pv);
PROTORPC_API const char *pb_get_f64(const char *p, const char *e, union pb_f64 *pv);
PROTORPC_API const char *pb_get_string(const char *p, const char *e, struct pb_string *pv);

static inline const char *pb_get_bytes(const char *p, const char *e, struct pb_bytes *pv) {
	return pb_get_string(p, e, (struct pb_string*) pv);
}

PROTORPC_API const char *pb_get_packed_bool(const char *p, const char *e, pb_alloc_t *a, bool **pv, int *plen);
PROTORPC_API const char *pb_get_packed_u32(const char *p, const char *e, pb_alloc_t *a, uint32_t **pv, int *plen);
PROTORPC_API const char *pb_get_packed_u64(const char *p, const char *e, pb_alloc_t *a, uint64_t **pv, int *plen);
PROTORPC_API const char *pb_get_packed_s32(const char *p, const char *e, pb_alloc_t *a, int32_t **pv, int *plen);
PROTORPC_API const char *pb_get_packed_s64(const char *p, const char *e, pb_alloc_t *a, int64_t **pv, int *plen);
PROTORPC_API const char *pb_get_packed_f32(const char *p, const char *e, pb_alloc_t *a, union pb_f32 **pv, int *plen);
PROTORPC_API const char *pb_get_packed_f64(const char *p, const char *e, pb_alloc_t *a, union pb_f64 **pv, int *plen);

static inline int pb_cmp_tag_1(const char *p, const char *e, uint8_t v) {
	return (p < e) ? (*(uint8_t*)p - v) : 1;
}
static inline int pb_cmp_tag_2(const char *p, const char *e, uint16_t v) {
	return (p < e) ? (*(uint16_t*) p - v) : 1;
}
static inline int pb_cmp_tag_3(const char *p, const char *e, uint32_t v) {
	return (p < e) ? ((*(int32_t*) p & 0xFFFFFF) - (int32_t) v) : 1;
}
static inline int pb_cmp_tag_4(const char *p, const char *e, uint32_t v) {
	return (p < e) ? (*(int32_t*) p - (int32_t) v) : 1;
}

PROTORPC_API int pb_skipto_1(const char **p, const char *e, uint8_t tag);
PROTORPC_API int pb_skipto_2(const char **p, const char *e, uint16_t tag);
PROTORPC_API int pb_skipto_3(const char **p, const char *e, uint32_t tag);
PROTORPC_API int pb_skipto_4(const char **p, const char *e, uint32_t tag);

PROTORPC_API const char *pb_toend_1(const char *p, const char *e);
PROTORPC_API const char *pb_toend_2(const char *p, const char *e);
PROTORPC_API const char *pb_toend_3(const char *p, const char *e);

static inline const char *pb_get_bool(const char *p, const char *e, bool *pv) {
	uint32_t u;
	p = pb_get_u32(p, e, &u);
	*pv = (u != 0);
	return p;
}



// Binary writing

PROTORPC_API char *pb_put_u32(char *p, uint32_t v);
PROTORPC_API char *pb_put_u64(char *p, uint64_t v);
PROTORPC_API char *pb_put_s32(char *p, int32_t v);
PROTORPC_API char *pb_put_s64(char *p, int64_t v);

static inline char *pb_put_bool(char *p, bool v) {
	*p = v ? 1 : 0;
	return p + 1;
}
static inline char *pb_put_f32(char *p, union pb_f32 v) {
	*(uint32_t*) p = v.u;
	return p + 4;
}
static inline char *pb_put_f64(char *p, union pb_f64 v) {
	*(uint64_t*) p = v.u;
	return p + 8;
}
static inline char *pb_put_string(char *p, struct pb_string v) {
	p = pb_put_u32(p, v.len);
	memcpy(p, v.p, v.len);
	return p + v.len;
}
static inline char *pb_put_bytes(char *p, struct pb_bytes v) {
	p = pb_put_u32(p, v.len);
	memcpy(p, v.p, v.len);
	return p + v.len;
}

PROTORPC_API char *pb_put_packed_u32(char *p, const uint32_t *v, int len);
PROTORPC_API char *pb_put_packed_u64(char *p, const uint64_t *v, int len);
PROTORPC_API char *pb_put_packed_s32(char *p, const int32_t *v, int len);
PROTORPC_API char *pb_put_packed_s64(char *p, const int64_t *v, int len);

static inline char *pb_put_packed_bool(char *p, const bool *v, int len) {
	static_assert(sizeof(bool) == 1, "bool size");
	p = pb_put_u32(p, len);
	memcpy(p, v, len);
	return p + len;
}
static inline char *pb_put_packed_f32(char *p, const union pb_f32 *v, int len) {
	p = pb_put_u32(p, len * 4);
	memcpy(p, v, len * 4);
	return p + len * 4;
}
static inline char *pb_put_packed_f64(char *p, const union pb_f64 *v, int len) {
	p = pb_put_u64(p, len * 8);
	memcpy(p, v, len * 8);
	return p + len * 8;
}

PROTORPC_API int pb_u32_size(int sz);
PROTORPC_API void pb_put_finish(char *p, char *start, int szlen);

static inline char *pb_put_tag_1(char *p, uint8_t v) {
	*(uint8_t*)p = v;
	return p + 1;
}
static inline char *pb_put_tag_2(char *p, uint16_t v) {
	*(uint16_t*) p = v;
	return p + 2;
}
static inline char *pb_put_tag_3(char *p, uint32_t v) {
	*(uint32_t*) p = v;
	return p + 3;
}
static inline char *pb_put_tag_4(char *p, uint32_t v) {
	*(uint32_t*) p = v;
	return p + 4;
}



// JSON reading

PROTORPC_API char pb_errret[];

PROTORPC_API char *pb_parse_base64(char *p, struct pb_bytes *v);

PROTORPC_API char *pb_parse_bool(char *p, bool *v);
PROTORPC_API char *pb_parse_i32(char *p, int32_t *v);
PROTORPC_API char *pb_parse_u32(char *p, uint32_t *v);
PROTORPC_API char *pb_parse_i64(char *p, int64_t *v);
PROTORPC_API char *pb_parse_u64(char *p, uint64_t *v);
PROTORPC_API char *pb_parse_float(char *p, float *v);
PROTORPC_API char *pb_parse_double(char *p, double *v);
PROTORPC_API char *pb_parse_bytes(char *p, struct pb_bytes *v);
PROTORPC_API char *pb_parse_string(char *p, struct pb_string *v);

PROTORPC_API char *pb_parse_array_bool(char *p, pb_alloc_t *a, bool const **pv, int *plen);
PROTORPC_API char *pb_parse_array_i32(char *p, pb_alloc_t *a, int32_t const **pv, int *plen);
PROTORPC_API char *pb_parse_array_u32(char *p, pb_alloc_t *a, uint32_t const **pv, int *plen);
PROTORPC_API char *pb_parse_array_i64(char *p, pb_alloc_t *a, int64_t const **pv, int *plen);
PROTORPC_API char *pb_parse_array_u64(char *p, pb_alloc_t *a, uint64_t const **pv, int *plen);
PROTORPC_API char *pb_parse_array_float(char *p, pb_alloc_t *a, float const **pv, int *plen);
PROTORPC_API char *pb_parse_array_double(char *p, pb_alloc_t *a, double const **pv, int *plen);
PROTORPC_API char *pb_parse_array_bytes(char *p, pb_alloc_t *a, struct pb_bytes const **pv, int *plen);
PROTORPC_API char *pb_parse_array_string(char *p, pb_alloc_t *a, struct pb_string const **pv, int *plen);

PROTORPC_API bool pb_parse_array(char **p);
PROTORPC_API bool pb_parse_map(char **p);
PROTORPC_API bool pb_more_array(char **p);
PROTORPC_API uint32_t pb_parse_enum(char **p, struct pb_string *v, uint32_t mul);
PROTORPC_API uint32_t pb_parse_field(char **p, struct pb_string *v, uint32_t mul);
PROTORPC_API char *pb_parse_skip(char *p);



// JSON writing

static inline int pb_base64_size(int sz) {
	return (sz * 4 + 3) / 3;
}

PROTORPC_API char *pb_print_base64(char *p, const uint8_t *v, int n);

PROTORPC_API char *pb_print_bool(char *p, bool v);
PROTORPC_API char *pb_print_u32(char *p, uint32_t v);
PROTORPC_API char *pb_print_i32(char *p, int32_t v);
PROTORPC_API char *pb_print_u64(char *p, uint64_t v);
PROTORPC_API char *pb_print_i64(char *p, int64_t v);
PROTORPC_API char *pb_print_float(char *p, float v);
PROTORPC_API char *pb_print_double(char *p, double v);

PROTORPC_API void pb_print_string(str_t *a, struct pb_string v);
PROTORPC_API void pb_print_bytes(str_t *a, struct pb_bytes v);

PROTORPC_API void pb_print_array_end(str_t *a);
PROTORPC_API void pb_print_map_end(str_t *a);

PROTORPC_API int pb_pretty_print(str_t *out, const char *in, int len);

static inline char *pb_print_array_end_i(char *p) {
	if (p[-1] == ',') {
		p--;
	}
	p[0] = ']';
	p[1] = ',';
	return p + 2;
}



// RPC

struct rpc_publisher;
struct rpc_server;

typedef int(*rpc_post_callback)(pb_alloc_t *obj, str_t *resp, char *body, int bodysz);

// TODO support detached streams
PROTORPC_API struct rpc_server *rpc_server_new();
PROTORPC_API void rpc_server_delete(struct rpc_server *m);
PROTORPC_API void rpc_handle_post(struct rpc_server *m, const char *path, rpc_post_callback cb);
PROTORPC_API void rpc_handle_stream(struct rpc_server *m, const char *path, struct rpc_publisher *pub);
PROTORPC_API int rpc_listen(struct rpc_server *m, int port);
PROTORPC_API void rpc_detached_serve(struct rpc_server *m);
PROTORPC_API void rpc_serve(struct rpc_server *m);

PROTORPC_API struct rpc_publisher *rpc_publisher_new();
PROTORPC_API void rpc_publisher_delete(struct rpc_publisher *m);
PROTORPC_API str_t *rpc_publish_start(struct rpc_publisher *m);
PROTORPC_API void rpc_publish_finish(struct rpc_publisher *m);


// Websocket streams
struct rpc_stream;

PROTORPC_API struct rpc_stream *rpc_stream_new();
PROTORPC_API void rpc_stream_delete(struct rpc_stream *m);
PROTORPC_API str_t *rpc_stream_start_json(struct rpc_stream *m);
PROTORPC_API int rpc_stream_finish_json(struct rpc_stream *m);
PROTORPC_API char *rpc_stream_start_proto(struct rpc_stream *m, int maxsz);
PROTORPC_API int rpc_stream_finish_proto(struct rpc_stream *m, char *p);
PROTORPC_API char* rpc_read_json(struct rpc_stream *m);
PROTORPC_API bool rpc_read_proto(struct rpc_stream *m, struct pb_bytes *msg);
