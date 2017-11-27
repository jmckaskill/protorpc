#include <gtest/gtest.h>
#include "../protorpc.h"
#include "../protobuf/test.proto.h"
#include <string.h>

TEST(protorpc, parse_request) {
    struct pr_http h;

    static const char get_request[] =
        "GET /foo HTTP/1.1\r\n"
        "\r\n";
    
    const char *data = get_request;
    int sz = strlen(get_request);
    const char *end = data + sz;
    memset(&h, 0, sizeof(h));
    EXPECT_EQ(PR_FINISHED, pr_parse_request(&h, &data, &sz));
    EXPECT_EQ(0, sz);
    EXPECT_EQ(data, end);
    EXPECT_EQ(0, h.have_body);
    EXPECT_EQ(PR_HTTP_GET, h.method);
    EXPECT_STREQ("/foo", h.name.c_str);
    EXPECT_EQ(4, h.name.len);

    static const char post[] = 
        "POST /api/foo HTTP/1.1\r\n"
        "Content-Length:6\r\n"
        "\r\n"
        "abcdef";

    data = post;
    sz = strlen(post);
    end = data + sz;
    memset(&h, 0, sizeof(h));
    EXPECT_EQ(PR_FINISHED, pr_parse_request(&h, &data, &sz));
    EXPECT_EQ(6, sz);
    EXPECT_EQ(end - 6, data);
    EXPECT_EQ(1, h.have_body);
    EXPECT_EQ(PR_HTTP_POST, h.method);
    EXPECT_STREQ("/api/foo", h.name.c_str);
    EXPECT_EQ(8, h.name.len);

    EXPECT_EQ(PR_FINISHED, pr_parse_body(&h, &data, &sz));
    EXPECT_EQ(0, sz);
    EXPECT_EQ(end, data);
    EXPECT_EQ(end - 6, h.body_chunk);
    EXPECT_EQ(6, h.chunk_size);
}

static const char test_request[] = "{\"b\": true}";

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
	char msg[1024];
	char objbuf[4096];
	pb_buf_t resp = PB_INIT_BUF(msg);
	pb_buf_t obj = PB_INIT_BUF(objbuf);
	strcpy((char*)msg, test_request);
	EXPECT_EQ(0, rpc_TestService(&m.hdr, "/TestService/Test", msg, &resp, &obj));
}
