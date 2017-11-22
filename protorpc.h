#pragma once
#include "http-parser.h"
#include "protobuf.h"

#if 0
#define RPC_IN_BUFSZ (16*1024)
#define RPC_OUT_BUFSZ (16*1024)
#define RPC_DECODE_BUFSZ (16*1024)

struct rpc_message {
	struct http_parser http;
	struct { int len; char buf[RPC_IN_BUFSZ]; } in;
	struct { int len; char buf[RPC_OUT_BUFSZ]; } out;
	struct { int len; char buf[RPC_DECODE_BUFSZ]; } msg;
};

PROTO_API int rpc_process(struct rpc_server *s, const void *data, size_t sz);

typedef int(*rpc_read_fn)(void* udata, void *buf, size_t sz);

typedef int(*rpc_post_callback)(pb_buf_t *obj, pb_buf_t *resp, char *body, int bodysz);

PROTO_API struct rpc_server *rpc_server_new();
PROTO_API void rpc_server_delete(struct rpc_server *m);
PROTO_API void rpc_handle_post(struct rpc_server *m, const char *path, rpc_post_callback cb);
PROTO_API int rpc_listen(struct rpc_server *m, int port);
PROTO_API void rpc_detached_serve(struct rpc_server *m);
PROTO_API void rpc_serve(struct rpc_server *m);


#endif