#pragma once

#include <protorpc/protorpc.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct http http;
typedef struct pb_allocator pb_allocator;

typedef void(*http_on_header)(http *h, const char *key, const char *val);

enum http_state {
	// waiting for the rest of the request headers
	// see http_received
	HTTP_RECEIVING_HEADERS,

	// request header has been fully processed,
	// application has not accepted the request yet
	// see http_send_response & http_send_continue
	HTTP_HEADERS_RECEIVED,

	// sending the 100 continue initial response
	// see http_sent
	HTTP_SENDING_CONTINUE,

	// application has accepted the request,
	// 100 continue (if applicable) has been sent,
	// not all payload bytes have been received
	// see http_received, http_request_data & http_consume_data
	HTTP_RECEIVING_DATA,

	// all payload bytes have been received
	// they may still be in the receive buffer
	// use http_send_response to proceed
	HTTP_DATA_RECEIVED,

	// all payload bytes have been received and consumed
	// by the application, waiting for reply
	HTTP_DATA_CONSUMED,

	// actively dumping the payload because we already have decided
	// on the response - typically an error
	HTTP_DUMPING_DATA,

	// all payload bytes have been received and removed from the receive buffer
	// actively sending the response
	HTTP_SENDING_RESPONSE,

	// response fully sent
	// waiting for application to start the next request
	HTTP_RESPONSE_SENT,

	// we are in a websocket and are buffering up the next
	// incoming message
	HTTP_RECEIVING_WEBSOCKET,

	// we've received a complete websocket message and are
	// waiting fro the user to consume it
	HTTP_WEBSOCKET_RECEIVED,
};

// struct http is the data structure to manage the server side of a
// single HTTP/1.0 or HTTP/1.1 connection. It does not allocate any
// memory and can be thrown out at any time.
// The HTTP stack goes through the following general steps. Each
// step is fully completed before continuing on to the next (e.g.
// no response is sent until the full request payload is received).
// 1. Receive the request headers (see http_received)
// 2. Wait for the application to agree/reject the request
//	  (see http_send_response and http_send_continue)
// 3. Send the 100-continue response
// 4. Receive the request payload
// 5. Send the response
struct http {
	// state is the exposed state presented to the application
	enum http_state state;

	http_on_header on_header;
	int64_t content_length;
	int64_t length_remaining;

	unsigned expect_continue : 1;
	unsigned expect_websocket : 1;

	unsigned connection_close : 1;
	unsigned have_nextch : 1;

	uint8_t websocket_flags;

	struct {int len; char c_str[12];} method;
	struct {int len; char c_str[256];} path;
	struct {int len; char c_str[128];} ws_response;

	char nextch;
	char *rxbuf;
	int rxbufsz;
	int rxused;

	const char *txnext;
	int txleft;

	const char *internal_error;

	pb_allocator obj;
	void *udata;
};

// http_reset initializes the http struct
// on_header is a callback called for every request header
// rxbuf and sz are the receive buffer
void http_reset(http *h, char *rxbuf, int rxsz, http_on_header on_header);

// http_next_request should be called once a request is finished
// (ie h->state == HTTP_RESPONSE_SENT) to start the next request
// the return code is non-zero if the session should be shutdown
int http_next_request(http *h);

// http_recv_buffer returns the current receive buffer and size.
// If the session is not in a receive state (i.e. sending or waiting
// for the application) then this returns a 0 length buffer.
// This call doesn't affect the session state and will return the
// same content if called multiple times in succession.
char *http_recv_buffer(const http *h, int *plen);

// http_received is called after data is received into the receive
// buffer. This processes the data. It will modify the state, call
// the on_header callback, etc. This function can be called with
// the raw return from a sockets recv call.
int http_received(http *h, int rxlen);

// http_send_buffer returns the current send buffer and size. If
// the session is not in a send state (i.e. receiving or waiting for the
// application), then this returns a 0 length buffer.
// This call doesn't affect the session state and will return the same
// content if called multiple times in succession.
const char *http_send_buffer(const http *h, int *plen);

// http_sent is called after data has been sent to the underlying TCP
// stack and can be removed from the transmit buffer. This function
// can be called with the raw return from a sockets send call.
int http_sent(http *h, int txlen);

// http_request_data returns the current request payload and size.
// This can be called once the system has started receiving data (i.e.
// HTTP_RECEIVING_DATA) and/or once the full payload is received (i.e.
// HTTP_DATA_RECEIVED).
char *http_request_data(http *h, int *plen);

// http_consume_data removes request data from the receive buffer. This
// is an optional step to free up room for more request data. It should
// only be called in the HTTP_RECEIVING_DATA or HTTP_DATA_RECEIVED states.
// The used parameter should not exceed the size returned by the last
// http_request_data call.
int http_consume_data(http *h, int used);

// http_send_response sends another response chunk. It's used in a few states:
// HTTP_HEADERS_RECEIVED - to reject the request
// HTTP_DATA_RECEIVED - to send a normal HTTP response
// HTTP_RESPONSE_SENT - to send another response chunk for streamed responses
// It must not be called at other times.
// The provided payload is the raw data stream including response headers,
// chunked encoding, etc.
int http_send_response(http *h, const char *p, int len);

// http_send_continue is used to accept a request and send the 100-continue
// message. It should only be called in the HTTP_HEADERS_RECEIVED state.
// Note that the 100 continue is only actually sent if the client has
// requested it, but the method should be called irrespectively.
// In the case of a websocket request this will initiate the websocket.
int http_send_continue(http *h);

// http_pump pumps data through BSD sockets send/recv calls
// returns
// 1 - data moved
// 0 - wait for more
// -1 - error
int http_pump(http *h, int fd);

int ws_send_binary(http *h, char *buf, int sz, void *obj, const proto_message *type);
int ws_send_json(http *h, char *buf, int sz, const void *obj, const proto_message *type, int indent);
int ws_send_text(http *h, char *buf, int sz, const char *text, int len);

#ifdef __cplusplus
}
#endif


