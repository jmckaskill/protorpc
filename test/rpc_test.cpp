#include <gtest/gtest.h>
#include "../protorpc.h"
#include <string.h>

TEST(protorpc, http) {
    struct pr_http h;

    char get_request[] =
        "GET /foo HTTP/1.1\r\n"
        "\r\n";
    
    char *data = get_request;
    int sz = strlen(get_request);
    memset(&h, 0, sizeof(h));
    EXPECT_EQ(PR_FINISHED, pr_parse_request(&h, &data, &sz));
    EXPECT_EQ(0, sz);
    EXPECT_EQ(data, get_request + strlen(get_request));
    EXPECT_EQ(0, h.have_body);
    EXPECT_EQ(PR_HTTP_GET, h.method);
    EXPECT_STREQ("/foo", h.name.buf);
    EXPECT_EQ(4, h.name.len);

    char post[] = 
        "POST /api/foo HTTP/1.1\r\n"
        "Content-Length:6\r\n"
        "\r\n"
        "abcdef";

    data = post;
    sz = strlen(post);
    memset(&h, 0, sizeof(h));
    EXPECT_EQ(PR_FINISHED, pr_parse_request(&h, &data, &sz));
    EXPECT_EQ(6, sz);
    EXPECT_EQ(post + strlen(post) - 6, data);
    EXPECT_EQ(1, h.have_body);
    EXPECT_EQ(PR_HTTP_POST, h.method);
    EXPECT_STREQ("/api/foo", h.name.buf);
    EXPECT_EQ(8, h.name.len);

    EXPECT_EQ(PR_FINISHED, pr_parse_body(&h, &data, &sz));
    EXPECT_EQ(0, sz);
    EXPECT_EQ(post + strlen(post), data);
    EXPECT_EQ(post + strlen(post) - 6, h.body_chunk);
    EXPECT_EQ(6, h.chunk_size);
}
