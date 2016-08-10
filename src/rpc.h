#pragma once

#include <protorpc/protorpc.h>
#include <os/str.h>

void rpc_get_message(struct rpc_publisher *m, str_t *out);
int rpc_subscribe(struct rpc_publisher *m, int fd, str_t *in);
int rpc_launch_ws_thread(int fd, str_t *in, rpc_post_callback cb);

