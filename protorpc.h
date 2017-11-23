#pragma once
#include "http-parser.h"
#include "protobuf.h"

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
    const char *error_string;
    uint64_t content_length;
    uint64_t left_in_chunk;
    unsigned have_etag : 1;
    unsigned multipart_form : 1;
    unsigned expect_continue : 1;
    unsigned version_1_0 : 1;
    unsigned have_multipart_header : 1;
	uint64_t etag;
    struct {int len; char buf[64];} login;
    struct {int len; char buf[6];} lang;
	struct {int len; char buf[256];} boundary;
    struct {int len; char buf[256];} name;
};

// error codes for pr_parse* functions
#define PR_FINISHED 0
#define PR_CONTINUE 1
#define PR_ERROR -1

int pr_parse_request(struct pr_http *h, char *buf, int *avail);
int pr_parse_body(struct pr_http *h, char **data, int *sz);
