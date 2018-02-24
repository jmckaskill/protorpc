#pragma once
#include <stdint.h>
#include <stdbool.h>

void start_test(int *argc, char *argv[]);
void test_error(int err);
int finish_test();

void assert_true(bool a, const char *astr, const char *file, int line);
void expect_int_eq(int64_t a, int64_t b, const char *astr, const char *bstr, const char *file, int line);
void expect_uint_eq(uint64_t a, uint64_t b, const char *astr, const char *bstr);
void expect_int_gt(int64_t a, int64_t b, const char *astr, const char *bstr);
void expect_int_lt(int64_t a, int64_t b, const char *astr, const char *bstr);
void expect_str_eq(const char *a, const char *b, const char *astr, const char *bstr, const char *file, int line);
void expect_bytes_eq(const void *a, int alen, const void *b, int blen, const char *astr, const char *bstr, const char *file, int line);
void expect_float_eq(double a, double b, const char *astr, const char *bstr, const char *file, int line);
void expect_ptr_eq(const void *a, const void *b, const char *astr, const char *bstr, const char *file, int line);

#define ASSERT_TRUE(A) assert_true((A), #A, __FILE__, __LINE__)
#define EXPECT_EQ(A, B) expect_int_eq((A), (B), #A, #B, __FILE__, __LINE__)
#define EXPECT_STREQ(A, B) expect_str_eq((A), (B), #A, #B, __FILE__, __LINE__)
#define EXPECT_BYTES(A, ALEN, B, BLEN) expect_bytes_eq((A), (ALEN), (B), (BLEN), #A, #B, __FILE__, __LINE__)
#define EXPECT_FLOAT_EQ(A, B) expect_float_eq((A), (B), #A, #B, __FILE__, __LINE__)
#define EXPECT_PTREQ(A, B) expect_ptr_eq((A), (B), #A, #B, __FILE__, __LINE__)

