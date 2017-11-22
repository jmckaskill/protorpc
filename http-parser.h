#pragma once

#include <stdint.h>

enum http_method {
    HTTP_METHOD_UNSET,
    HTTP_METHOD_GET,
    HTTP_METHOD_POST,
};

enum http_version {
    HTTP_VERSION_UNSET,
    HTTP_VERSION_1_0,
    HTTP_VERSION_1_1,
};

enum http_length_type {
    HTTP_LENGTH_UNSET,
    HTTP_LENGTH_CHUNKED,
    HTTP_LENGTH_FIXED,
    HTTP_LENGTH_CLOSE,
};

// low 32 bits mod time in 8 ascii bytes followed by
// low 32 bits file size in 8 ascii bytes 
#define HTTP_ETAG_LENGTH 16

// must be memset to 0 before each request
struct http_parser {
    enum http_method method;
    enum http_version version;
    enum http_length_type length_type;
    const char *error_string;
	int64_t content_length;
    unsigned have_connection_upgrade : 1;
	unsigned have_ws_upgrade : 1;
	unsigned have_ws_key : 1;
    unsigned want_ws_accept : 1;
	unsigned have_etag : 1;
    unsigned expect_continue : 1;
	uint64_t etag;
	int pathsz;
    struct {int len; char buf[32];} ws_key;
    struct {int len; char buf[256];} path;
    struct {int len; char buf[64];} login;
    struct {int len; char buf[6];} lang;
};

// error codes for http_parse_request function
#define HTTP_FINISHED 0
#define HTTP_CONTINUE 1
#define HTTP_ERROR -1

int http_parse_request(struct http_parser *h, char **next, int *avail);
const char *http_method_string(enum http_method method);

struct multipart_parser {
	char *next_out;
	int avail_out;
	unsigned have_boundary : 1;
	unsigned have_header : 1;
	struct {int len; char buf[256];} boundary;
	struct {int len; char buf[256];} name;
};

int parse_multipart(struct multipart_parser *m, char **next, int *avail);
