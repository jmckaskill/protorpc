#pragma once
#include <protorpc/protorpc.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct Example Example;
typedef struct EchoRequest EchoRequest;

extern const proto_service proto_Example;
extern const proto_method proto_Example_ping;
extern const proto_message proto_EchoRequest;

struct Example {
	int (*ping)(Example *svc, pb_allocator *obj, const EchoRequest *in, EchoRequest *out);
};

struct EchoRequest {
	pb_msg _pbhdr;
	pb_string ping;
};

#ifdef __cplusplus
}
#endif

