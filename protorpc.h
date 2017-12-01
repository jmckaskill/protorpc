#pragma once
#include <stdint.h>
#include <stddef.h>
#include "protobuf.h"

#ifdef __cplusplus
extern "C" {
#endif

enum pr_http_method {
    PR_HTTP_UNSET,
    PR_HTTP_GET,
    PR_HTTP_POST,
};

enum pr_http_length_type {
    PR_HTTP_LENGTH_UNSET,
    PR_HTTP_LENGTH_CHUNKED,
    PR_HTTP_LENGTH_FIXED,
    PR_HTTP_LENGTH_CLOSE,
};

// must be memset to 0 before each request
struct pr_http {
    enum pr_http_method method;
    enum pr_http_length_type length_type;
    uint64_t content_length;
    uint64_t left_in_chunk;
    uint64_t etag;
	unsigned flags;
    struct {int len; char c_str[64];} login;
    struct {int len; char c_str[6];} lang;
	struct {int len; char c_str[256];} boundary;
    struct {int len; char c_str[256];} name;
};

// flags
#define PR_FLAG_HAVE_HEADER 1
#define PR_FLAG_VERSION_1_0 2
#define PR_FLAG_HAVE_ETAG 4
#define PR_FLAG_HAVE_MULTIPART 8
#define PR_FLAG_EXPECT_CONTINUE 16

// error codes for pr_parse* functions
#define PR_FINISHED 0
#define PR_CONTINUE -1
#define PR_SEND_CONTINUE 100
#define PR_ERR_BAD_REQUEST 400
#define PR_ERR_LENGTH_REQUIRED 411
#define PR_ERR_TOO_LARGE 413
#define PR_ERR_BAD_EXPECTATION 417
#define PR_ERR_METHOD_NOT_IMPLEMENTED 501
#define PR_ERR_VERSION_NOT_SUPPORTED 505
#define PR_ERR_URI_TOO_LONG 414
#define PR_ERR_NOT_FOUND 404
#define PR_ERR_CLOSE 1000

int pr_parse_request(struct pr_http *h, pb_string_t *in, pb_buf_t *out);

int pr_start_response(struct pr_http *h, pb_buf_t *out);
int pr_finish_response(struct pr_http *h, char *hdr, pb_buf_t *out, int code);

uint32_t pr_hash(const char *path, uint32_t hashmul);

extern const char pr_continue[];

#ifdef __cplusplus
}
#endif
