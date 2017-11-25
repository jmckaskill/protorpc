#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

// Some basic types

#ifdef __cplusplus
extern "C" {
#endif

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
	const char *buf; // not null terminated
};

static inline struct pb_string pb_as_string(const char *str) {
	struct pb_string s;
	s.len = (int) strlen(str);
	s.buf = str;
	return s;
}

static inline int pb_cmp2(struct pb_string a, const char *b, int len) {
	return a.len == len ? memcmp(a.buf, b, len) : (int) (a.len - len);
}
static inline int pb_cmp(struct pb_string a, const char *b) {
	return pb_cmp2(a, b, (int) strlen(b));
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

struct pb_buf {
	char *next;
	char *end;
};

typedef struct pb_buf pb_buf_t;

static inline void *pb_alloc(pb_buf_t *b, size_t sz) {
	char *p = b->next;
	char *n = p + (((sz)+7) &~7U);
	if (n > b->end) {
		return NULL;
	}
	b->next = n;
	return p;
}

static inline void *pb_calloc(pb_buf_t *b, size_t sz) {
	void *p = pb_alloc(b, sz);
	if (p) {
		memset(p, 0, sz);
	}
	return p;
}

static inline char *pb_appendsz(pb_buf_t *b, int sz) {
	char *p = b->next;
	char *n = p + sz;
	if (n > b->end) {
		return NULL;
	}
	b->next = n;
	return p;
}

static inline int pb_append(pb_buf_t *b, const char *str, int sz) {
	char *p = pb_appendsz(b, sz);
	if (p) {
		memcpy(p, str, sz);
		return 0;
	} else {
		return -1;
	}
}

// Binary reading

const char *pb_get_u32(const char *p, const char *e, uint32_t *pv);
const char *pb_get_u64(const char *p, const char *e, uint64_t *pv);
const char *pb_get_s32(const char *p, const char *e, int32_t *pv);
const char *pb_get_s64(const char *p, const char *e, int64_t *pv);
const char *pb_get_f32(const char *p, const char *e, union pb_f32 *pv);
const char *pb_get_f64(const char *p, const char *e, union pb_f64 *pv);
const char *pb_get_string(const char *p, const char *e, struct pb_string *pv);

static inline const char *pb_get_bytes(const char *p, const char *e, struct pb_bytes *pv) {
	return pb_get_string(p, e, (struct pb_string*) pv);
}

const char *pb_get_packed_bool(const char *p, const char *e, pb_buf_t *a, bool **pv, int *plen);
const char *pb_get_packed_u32(const char *p, const char *e, pb_buf_t *a, uint32_t **pv, int *plen);
const char *pb_get_packed_u64(const char *p, const char *e, pb_buf_t *a, uint64_t **pv, int *plen);
const char *pb_get_packed_s32(const char *p, const char *e, pb_buf_t *a, int32_t **pv, int *plen);
const char *pb_get_packed_s64(const char *p, const char *e, pb_buf_t *a, int64_t **pv, int *plen);
const char *pb_get_packed_f32(const char *p, const char *e, pb_buf_t *a, union pb_f32 **pv, int *plen);
const char *pb_get_packed_f64(const char *p, const char *e, pb_buf_t *a, union pb_f64 **pv, int *plen);

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

int pb_skipto_1(const char **p, const char *e, uint8_t tag);
int pb_skipto_2(const char **p, const char *e, uint16_t tag);
int pb_skipto_3(const char **p, const char *e, uint32_t tag);
int pb_skipto_4(const char **p, const char *e, uint32_t tag);

const char *pb_toend_1(const char *p, const char *e);
const char *pb_toend_2(const char *p, const char *e);
const char *pb_toend_3(const char *p, const char *e);

static inline const char *pb_get_bool(const char *p, const char *e, bool *pv) {
	uint32_t u;
	p = pb_get_u32(p, e, &u);
	*pv = (u != 0);
	return p;
}



// Binary writing

char *pb_put_u32(char *p, uint32_t v);
char *pb_put_u64(char *p, uint64_t v);
char *pb_put_s32(char *p, int32_t v);
char *pb_put_s64(char *p, int64_t v);

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
	memcpy(p, v.buf, v.len);
	return p + v.len;
}
static inline char *pb_put_bytes(char *p, struct pb_bytes v) {
	p = pb_put_u32(p, v.len);
	memcpy(p, v.p, v.len);
	return p + v.len;
}

char *pb_put_packed_u32(char *p, const uint32_t *v, int len);
char *pb_put_packed_u64(char *p, const uint64_t *v, int len);
char *pb_put_packed_s32(char *p, const int32_t *v, int len);
char *pb_put_packed_s64(char *p, const int64_t *v, int len);

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

int pb_u32_size(int sz);
void pb_put_finish(char *p, char *start, int szlen);

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

extern const char pb_errret[];

const char *pb_parse_base64(const char *p, struct pb_bytes *v);

const char *pb_parse_bool(const char *p, bool *v);
const char *pb_parse_i32(const char *p, int32_t *v);
const char *pb_parse_u32(const char *p, uint32_t *v);
const char *pb_parse_i64(const char *p, int64_t *v);
const char *pb_parse_u64(const char *p, uint64_t *v);
const char *pb_parse_float(const char *p, float *v);
const char *pb_parse_double(const char *p, double *v);
const char *pb_parse_bytes(const char *p, struct pb_bytes *v);
const char *pb_parse_string(const char *p, struct pb_string *v);

const char *pb_parse_array_bool(const char *p, pb_buf_t *a, bool const **pv, int *plen);
const char *pb_parse_array_i32(const char *p, pb_buf_t *a, int32_t const **pv, int *plen);
const char *pb_parse_array_u32(const char *p, pb_buf_t *a, uint32_t const **pv, int *plen);
const char *pb_parse_array_i64(const char *p, pb_buf_t *a, int64_t const **pv, int *plen);
const char *pb_parse_array_u64(const char *p, pb_buf_t *a, uint64_t const **pv, int *plen);
const char *pb_parse_array_float(const char *p, pb_buf_t *a, float const **pv, int *plen);
const char *pb_parse_array_double(const char *p, pb_buf_t *a, double const **pv, int *plen);
const char *pb_parse_array_bytes(const char *p, pb_buf_t *a, struct pb_bytes const **pv, int *plen);
const char *pb_parse_array_string(const char *p, pb_buf_t *a, struct pb_string const **pv, int *plen);

bool pb_parse_array(const char **p);
bool pb_parse_map(const char **p);
bool pb_more_array(const char **p);
uint32_t pb_parse_enum(const char **p, struct pb_string *v, uint32_t mul);
uint32_t pb_parse_field(const char **p, struct pb_string *v, uint32_t mul);
const char *pb_parse_skip(const char *p);



// JSON writing

static inline int pb_base64_size(int sz) {
	return (sz * 4 + 3) / 3;
}

char *pb_print_base64(char *p, const uint8_t *v, int n);

char *pb_print_bool(char *p, bool v);
char *pb_print_u32(char *p, uint32_t v);
char *pb_print_i32(char *p, int32_t v);
char *pb_print_u64(char *p, uint64_t v);
char *pb_print_i64(char *p, int64_t v);
char *pb_print_float(char *p, float v);
char *pb_print_double(char *p, double v);

int pb_print_string(pb_buf_t *a, struct pb_string v);
int pb_print_bytes(pb_buf_t *a, struct pb_bytes v);

int pb_print_array_end(pb_buf_t *a);
int pb_print_map_end(pb_buf_t *a);

int pb_pretty_print(pb_buf_t *out, const char *in, int len);

static inline char *pb_print_array_end_i(char *p) {
	if (p[-1] == ',') {
		p--;
	}
	p[0] = ']';
	p[1] = ',';
	return p + 2;
}

#ifdef __cplusplus
}
#endif

