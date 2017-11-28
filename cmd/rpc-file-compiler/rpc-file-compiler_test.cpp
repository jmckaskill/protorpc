#include "../../protobuf.h"
#include <gtest/gtest.h>
#include <zlib/zlib.h>

extern "C" int rpc_test_data(const char *path, pb_buf_t *out);

static const char test_js_header[] =
"HTTP/1.1 200 OK\r\n"
"Content-Length:39\r\n"
"Content-Type:application/javascript\r\n"
"Content-Encoding:gzip\r\n"
"Cache-Control:max-age=31536000\r\n"
"\r\n";

static const char test_js_data[] = 
	"{\n"
	"    \"foo\": \"bar\"\n"
	"}\n";

static const char response_404[] =
"HTTP/1.1 404 Not Found\r\n"
"Content-Length:0\r\n"
"\r\n";

TEST(protorpc, compiler) {
	char buf[4096];
	pb_buf_t out = PB_INIT_BUF(buf);
	EXPECT_EQ(0, rpc_test_data("/rpc-test-data/test.4CDDE84B7A.js", &out));
	size_t hdrsz = strlen(test_js_header);
	char first = buf[hdrsz];
	buf[hdrsz] = 0;
	EXPECT_STREQ(test_js_header, buf);
	buf[hdrsz] = first;

	char decoded[4096];
	z_stream z;
	memset(&z, 0, sizeof(z));
	inflateInit2(&z, 16 + MAX_WBITS);
	z.next_in = (uint8_t*) buf + hdrsz;
	z.avail_in = out.next - (buf+hdrsz);
	z.next_out = (uint8_t*) decoded;
	z.avail_out = sizeof(decoded);
	EXPECT_EQ(Z_STREAM_END, inflate(&z, Z_FINISH));

	*z.next_out = 0;
	EXPECT_STREQ(test_js_data, decoded);
	EXPECT_EQ(sizeof(test_js_data) - 1, (char*) z.next_out - decoded);

	// Test 404 response
	out.next = buf;
	out.end = buf + sizeof(buf);
	EXPECT_EQ(0, rpc_test_data("/rpc-test-data/test.js", &out));
	EXPECT_EQ(buf + strlen(response_404), out.next);
	buf[strlen(response_404)] = 0;
	EXPECT_STREQ(response_404, buf);
}

