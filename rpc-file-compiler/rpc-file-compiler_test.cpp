#include <protorpc/protorpc.h>
#include <gtest/gtest.h>
#include <zlib/zlib.h>

extern "C" const proto_dir dir_rpc_test_data;

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

TEST(protorpc, compiler) {
	int respsz;
	const char *path = "/rpc-test-data/test.4CDDE84B7A.js";
	const char *resp = pb_lookup_file(&dir_rpc_test_data, path, strlen(path), &respsz);
	char buf[4096];
	memcpy(buf, resp, respsz);
	size_t hdrsz = strlen(test_js_header);
	char first = buf[hdrsz];
	buf[hdrsz] = 0;
	EXPECT_STREQ(test_js_header, buf);
	buf[hdrsz] = first;

	char decoded[4096];
	z_stream z;
	memset(&z, 0, sizeof(z));
	inflateInit2(&z, 16 + MAX_WBITS);
	z.next_in = (uint8_t*)(resp + hdrsz);
	z.avail_in = respsz - hdrsz;
	z.next_out = (uint8_t*) decoded;
	z.avail_out = sizeof(decoded);
	EXPECT_EQ(Z_STREAM_END, inflate(&z, Z_FINISH));

	*z.next_out = 0;
	EXPECT_STREQ(test_js_data, decoded);
	EXPECT_EQ(sizeof(test_js_data) - 1, (char*) z.next_out - decoded);

	// Test 404 response
	path = "/rpc-test-data/test.js";
	resp = pb_lookup_file(&dir_rpc_test_data, path, strlen(path), &respsz);
	EXPECT_EQ(NULL, resp);
}

