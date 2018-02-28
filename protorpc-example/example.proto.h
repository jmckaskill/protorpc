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
extern const proto_method proto_Example_Echo;
extern const proto_method proto_Example_GenerateError;
extern const proto_message proto_EchoRequest;

struct Empty {
	char _pbempty;
};

struct Example {
	int (*Echo)(Example *svc, pb_allocator *obj, const EchoRequest *in, EchoRequest *out);
	int (*GenerateError)(Example *svc, pb_allocator *obj, const Empty *in, Empty *out);
};

struct EchoRequest {
	EchoRequest *_next;
	int _encsz;
	pb_string ping;
};

#ifdef __cplusplus
}
#endif

