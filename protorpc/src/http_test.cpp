#include <gtest/gtest.h>
#include "../protorpc.h"
#include "../protobuf/test.proto.h"
#include <string.h>

TEST(protorpc, parse_request) {
	struct pr_http h = {};

    static const char get_request[] =
        "GET /foo HTTP/1.1\r\n"
        "\r\n";
    
	char inbuf[1024];
	pb_buf_t in = PB_INIT_BUF(inbuf);
	strcpy(inbuf, get_request);
	in.end = inbuf + strlen(get_request);
    EXPECT_EQ(PR_FINISHED, pr_parse_request(&h, &in));
	EXPECT_EQ(in.end, in.next);
    EXPECT_EQ(0, h.have_body);
    EXPECT_EQ(PR_HTTP_GET, h.method);
    EXPECT_STREQ("/foo", h.name.c_str);
    EXPECT_EQ(4, h.name.len);

    static const char post[] = 
        "POST /api/foo HTTP/1.1\r\n"
        "Content-Length:6\r\n"
        "\r\n"
        "abcdef";

	strcpy(inbuf, post);
	in.next = inbuf;
	in.end = inbuf + strlen(post);
    memset(&h, 0, sizeof(h));
    EXPECT_EQ(PR_FINISHED, pr_parse_request(&h, &in));
	EXPECT_EQ(in.end - 6, in.next);
    EXPECT_EQ(1, h.have_body);
    EXPECT_EQ(PR_HTTP_POST, h.method);
    EXPECT_STREQ("/api/foo", h.name.c_str);
    EXPECT_EQ(8, h.name.len);

	pb_buf_t chunk;
    EXPECT_EQ(PR_FINISHED, pr_parse_body(&h, &in, &chunk));
	EXPECT_EQ(in.end, in.next);
	EXPECT_EQ(in.end - 6, chunk.next);
	EXPECT_EQ(in.end, chunk.end);
}

static const char test_request[] = 
	"POST /TestService/Test HTTP/1.1\r\n"
	"Content-Length: 11\r\n"
	"\r\n"
	"{\"b\": true}";

static const char test_response[] =
"HTTP/1.1 200 \r\n"
"Content-Length:    11\r\n"
"\r\n"
"{\"u32\":15}\n";


struct TestServiceMock {
	struct TestService hdr;
	struct TestMessage in;
};

static int TestCallback(struct TestService *iface, pb_buf_t *obj, struct TestMessage const *in, struct TestMessage *out) {
	struct TestServiceMock *m = container_of(iface, struct TestServiceMock, hdr);
	out->u32 = 15;
	m->in = *in;
	return 0;
}

TEST(protorpc, dispatch) {
	struct TestServiceMock m = {};
	m.hdr.Test = &TestCallback;
	char inbuf[1024];
	char outbuf[1024];
	char objbuf[4096];
	pb_buf_t out = PB_INIT_BUF(outbuf);
	pb_buf_t obj = PB_INIT_BUF(objbuf);
	strcpy(inbuf, test_request);
	pb_buf_t in = { inbuf, inbuf + strlen(test_request) };
	struct pr_http h = {};
	pb_buf_t chunk;

	EXPECT_EQ(PR_FINISHED, pr_parse_request(&h, &in));
	EXPECT_EQ(PR_FINISHED, pr_parse_body(&h, &in, &chunk));
	chunk.end[0] = '\0';
	EXPECT_STREQ("{\"b\": true}", chunk.next);

	EXPECT_EQ(0, pr_start_response(&h, &out));
	EXPECT_EQ(0, rpc_TestService(&m.hdr, "/TestService/Test", chunk.next, &out, &obj));
	EXPECT_EQ(0, pr_finish_response(&h, outbuf, &out, 0));
	out.next[0] = '\0';
	EXPECT_STREQ(test_response, outbuf);
	EXPECT_EQ(sizeof(test_response) - 1, out.next - outbuf);
	EXPECT_EQ(m.in.b, true);
}
