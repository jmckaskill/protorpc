#include "protorpc-example/example.proto.h"

const proto_method proto_Example_ping = {
	{31, "/twirp/com.example.Example/ping"},
	0,
	&proto_EchoRequest,
	&proto_EchoRequest
};
static const pb_string *by_name_Example[] = {
	&proto_Example_ping.path,
};
const proto_service proto_Example = {
	1,
	by_name_Example
};

static const struct proto_field fields_EchoRequest[] = {
	{{4, "ping"}, PROTO_STRING, offsetof(EchoRequest, ping), 10, NULL, -1}
};
static const pb_string *by_name_EchoRequest[] = {
	&fields_EchoRequest[0].json_name
};
const proto_message proto_EchoRequest = {
	sizeof(EchoRequest),
	1,
	fields_EchoRequest,
	by_name_EchoRequest
};

