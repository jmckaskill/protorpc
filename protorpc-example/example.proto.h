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
extern const proto_method proto_Example_EchoStream;
extern const proto_method proto_Example_GenerateError;
extern const proto_message proto_EchoRequest;

struct Empty {
	char _pbempty;
};

struct Example {
	int (*Echo)(Example *svc, http *h, EchoRequest *in, EchoRequest *out);
	int (*EchoStream)(Example *svc, http *h, EchoRequest *in);
	int (*GenerateError)(Example *svc, http *h, Empty *in);
};

struct EchoRequest {
	EchoRequest *_next;
	int _encsz;
	pb_string ping;
};

#ifdef __cplusplus
}
#endif

