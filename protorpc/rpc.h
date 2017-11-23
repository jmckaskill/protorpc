#pragma once

#include "../protorpc.h"

#ifdef PROTORPC_INCLUDE_STREAMS
void rpc_get_message(struct rpc_publisher *m, pb_buf_t *out);
int rpc_subscribe(struct rpc_publisher *m, int fd, pb_buf_t *in);
int rpc_launch_ws_thread(int fd, pb_buf_t *in, rpc_post_callback cb);
#endif


