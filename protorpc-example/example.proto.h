#pragma once
#include <protorpc/protorpc.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct Empty Empty;
typedef struct Example Example;
typedef struct EchoRequest EchoRequest;

extern const proto_message proto_Empty;
extern const proto_service proto_Example;
extern const proto_method proto_Example_ping;
extern const proto_method proto_Example_error;
extern const proto_message proto_EchoRequest;

struct Empty {
	char _pbempty;
};

struct Example {
	int (*ping)(Example *svc, pb_allocator *obj, const EchoRequest *in, EchoRequest *out);
	int (*error)(Example *svc, pb_allocator *obj, const Empty *in, Empty *out);
};

struct EchoRequest {
	pb_msg _pbhdr;
	pb_string ping;
};

#ifdef __cplusplus
}
#endif

