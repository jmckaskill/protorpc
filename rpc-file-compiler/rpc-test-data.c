#include <protorpc/protorpc.h>

static const uint8_t data_0[] = {
	0x48,0x54,0x54,0x50,0x2F,0x31,0x2E,0x31,0x20,0x32,0x30,0x30,0x20,0x4F,0x4B,0x0D, /*HTTP/1.1 200 OK.*/
	0x0A,0x43,0x6F,0x6E,0x74,0x65,0x6E,0x74,0x2D,0x4C,0x65,0x6E,0x67,0x74,0x68,0x3A, /*.Content-Length:*/
	0x33,0x30,0x0D,0x0A,0x43,0x6F,0x6E,0x74,0x65,0x6E,0x74,0x2D,0x54,0x79,0x70,0x65, /*30..Content-Type*/
	0x3A,0x74,0x65,0x78,0x74,0x2F,0x68,0x74,0x6D,0x6C,0x3B,0x63,0x68,0x61,0x72,0x73, /*:text/html;chars*/
	0x65,0x74,0x3D,0x75,0x74,0x66,0x2D,0x38,0x0D,0x0A,0x43,0x6F,0x6E,0x74,0x65,0x6E, /*et=utf-8..Conten*/
	0x74,0x2D,0x45,0x6E,0x63,0x6F,0x64,0x69,0x6E,0x67,0x3A,0x67,0x7A,0x69,0x70,0x0D, /*t-Encoding:gzip.*/
	0x0A,0x43,0x61,0x63,0x68,0x65,0x2D,0x43,0x6F,0x6E,0x74,0x72,0x6F,0x6C,0x3A,0x6E, /*.Cache-Control:n*/
	0x6F,0x2D,0x63,0x61,0x63,0x68,0x65,0x0D,0x0A,0x0D,0x0A,                          /*o-cache....*/
	0x1F,0x8B,0x08,0x00,0x00,0x00,0x00,0x00,0x02,0x03,0xB3,0xC9,0x28,0xC9,0xCD,0xB1, /*............(...*/
	0xE3,0xB2,0xD1,0x07,0xD3,0x00,0x5E,0x35,0x8C,0x98,0x0E,0x00,0x00,0x00,           /*......^5......*/
};

static const uint8_t data_1[] = {
	0x48,0x54,0x54,0x50,0x2F,0x31,0x2E,0x31,0x20,0x32,0x30,0x30,0x20,0x4F,0x4B,0x0D, /*HTTP/1.1 200 OK.*/
	0x0A,0x43,0x6F,0x6E,0x74,0x65,0x6E,0x74,0x2D,0x4C,0x65,0x6E,0x67,0x74,0x68,0x3A, /*.Content-Length:*/
	0x33,0x39,0x0D,0x0A,0x43,0x6F,0x6E,0x74,0x65,0x6E,0x74,0x2D,0x54,0x79,0x70,0x65, /*39..Content-Type*/
	0x3A,0x61,0x70,0x70,0x6C,0x69,0x63,0x61,0x74,0x69,0x6F,0x6E,0x2F,0x6A,0x61,0x76, /*:application/jav*/
	0x61,0x73,0x63,0x72,0x69,0x70,0x74,0x3B,0x63,0x68,0x61,0x72,0x73,0x65,0x74,0x3D, /*ascript;charset=*/
	0x75,0x74,0x66,0x2D,0x38,0x0D,0x0A,0x43,0x6F,0x6E,0x74,0x65,0x6E,0x74,0x2D,0x45, /*utf-8..Content-E*/
	0x6E,0x63,0x6F,0x64,0x69,0x6E,0x67,0x3A,0x67,0x7A,0x69,0x70,0x0D,0x0A,0x43,0x61, /*ncoding:gzip..Ca*/
	0x63,0x68,0x65,0x2D,0x43,0x6F,0x6E,0x74,0x72,0x6F,0x6C,0x3A,0x6D,0x61,0x78,0x2D, /*che-Control:max-*/
	0x61,0x67,0x65,0x3D,0x33,0x31,0x35,0x33,0x36,0x30,0x30,0x30,0x0D,0x0A,0x0D,0x0A, /*age=31536000....*/
	0x1F,0x8B,0x08,0x00,0x00,0x00,0x00,0x00,0x02,0x03,0xAB,0xE6,0x52,0x00,0x02,0xA5, /*............R...*/
	0xB4,0xFC,0x7C,0x25,0x2B,0x05,0xA5,0xA4,0xC4,0x22,0x25,0xAE,0x5A,0x2E,0x00,0xF1, /*..|%+...."%.Z...*/
	0x59,0x8F,0xBC,0x15,0x00,0x00,0x00,                                              /*Y......*/
};

// Source URL "/test.html"
static const proto_file file_0 = {
	{10, "/test.html"},
	(const char*) data_0,
	153
};

// Source URL "/test.js"
static const proto_file file_1 = {
	{19, "/test.4CDDE84B7A.js"},
	(const char*) data_1,
	183
};

static const pb_string *by_path[] = {
	&file_0.path,
	&file_1.path,
};

extern const proto_dir dir_rpc_test_data;

const proto_dir dir_rpc_test_data = {2, by_path};
