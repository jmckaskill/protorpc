#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct http http;

typedef void(*http_on_header)(http *h, const char *key, const char *val);

struct http {
	http_on_header on_header;
	int64_t content_length;
	int64_t length_remaining;

	unsigned path_complete : 1;
	unsigned headers_complete : 1;
	unsigned request_complete : 1;
	unsigned response_complete : 1;

	unsigned expect_continue : 1;
	unsigned connection_close : 1;
	unsigned have_nextch : 1;
	unsigned version_1_0 : 1;

	struct {int len; char c_str[12];} method;
	struct {int len; char c_str[256];} path;

	char nextch;
	char *rxbuf;
	int rxbufsz;
	int rxused;

	char *txbuf;
	int txbufsz;
	char *txnext;
	int txleft;
};


void http_reset(http *h, char *rxbuf, int rxbufsz, char *txbuf, int txbufsz, http_on_header on_header);
void http_next_request(http *h);

char *http_recv_buffer(http *h, int *plen);
int http_received(http *h, int rxlen);

char *http_send_buffer(http *h, int *plen);
int http_sent(http *h, int txlen);

char *http_request_data(http *h, int *plen);
void http_consume_data(http *h, int used);

char *http_start_response(http *h, int *plen);
void http_finish_response(http *h, int sts, int len);

void http_send_data(http *h, char *p, int len);


#ifdef __cplusplus
}
#endif


