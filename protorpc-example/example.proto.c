#include "example.proto.h"

static const struct proto_field fields_Empty[] = {
	{0}
};
static const pb_string *by_name_Empty[] = {
	NULL
};
const proto_message proto_Empty = {
	sizeof(Empty),
	0,
	fields_Empty,
	by_name_Empty
};

const proto_method proto_Example_ping = {
	{31, "/twirp/com.example.Example/ping"},
	0,
	&proto_EchoRequest,
	&proto_EchoRequest
};
const proto_method proto_Example_error = {
	{32, "/twirp/com.example.Example/error"},
	1,
	&proto_Empty,
	&proto_Empty
};
static const pb_string *by_name_Example[] = {
	&proto_Example_ping.path,
	&proto_Example_error.path,
};
const proto_service proto_Example = {
	2,
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

