#include "example.proto.h"

static const struct proto_field fields_Empty[] = {
	{0}
};
static const pb_string *by_name_Empty[] = {
	NULL,
};
const proto_message proto_Empty = {
	sizeof(Empty),
	sizeof(by_name_Empty)/sizeof(pb_string*) - 1,
	fields_Empty,
	by_name_Empty
};

const proto_method proto_Example_Echo = {
	{31, "/twirp/com.example.Example/Echo"},
	0,
	&proto_EchoRequest,
	&proto_EchoRequest,
};
const proto_method proto_Example_EchoStream = {
	{37, "/twirp/com.example.Example/EchoStream"},
	1,
	&proto_EchoRequest,
	NULL,
};
const proto_method proto_Example_GenerateError = {
	{40, "/twirp/com.example.Example/GenerateError"},
	2,
	&proto_Empty,
	NULL,
};
static const pb_string *methods_Example[] = {
	&proto_Example_Echo.path,
	&proto_Example_GenerateError.path,
	NULL,
};
static const pb_string *streams_Example[] = {
	&proto_Example_EchoStream.path,
	NULL,
};
const proto_service proto_Example = {
	sizeof(methods_Example)/sizeof(pb_string*) - 1,
	sizeof(streams_Example)/sizeof(pb_string*) - 1,
	methods_Example,
	streams_Example,
};

static const struct proto_field fields_EchoRequest[] = {
	{{4, "ping"}, PROTO_STRING, offsetof(EchoRequest, ping), 10, NULL, -1}
};
static const pb_string *by_name_EchoRequest[] = {
	&fields_EchoRequest[0].json_name,
	NULL,
};
const proto_message proto_EchoRequest = {
	sizeof(EchoRequest),
	sizeof(by_name_EchoRequest)/sizeof(pb_string*) - 1,
	fields_EchoRequest,
	by_name_EchoRequest
};

