#pragma once
#include <protorpc/protorpc.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct TestPod TestPod;
typedef struct TestMessage TestMessage;
typedef struct TestService TestService;
typedef struct TestMessage_MbEntry TestMessage_MbEntry;
typedef struct TestMessage_MdEntry TestMessage_MdEntry;
typedef struct TestMessage_MfEntry TestMessage_MfEntry;
typedef struct TestMessage_MbyEntry TestMessage_MbyEntry;
typedef struct TestMessage_MenEntry TestMessage_MenEntry;
typedef struct TestMessage_Mf32Entry TestMessage_Mf32Entry;
typedef struct TestMessage_Mf64Entry TestMessage_Mf64Entry;
typedef struct TestMessage_Mi32Entry TestMessage_Mi32Entry;
typedef struct TestMessage_Mi64Entry TestMessage_Mi64Entry;
typedef struct TestMessage_MmsgEntry TestMessage_MmsgEntry;
typedef struct TestMessage_MpodEntry TestMessage_MpodEntry;
typedef struct TestMessage_Ms32Entry TestMessage_Ms32Entry;
typedef struct TestMessage_Ms64Entry TestMessage_Ms64Entry;
typedef struct TestMessage_MstrEntry TestMessage_MstrEntry;
typedef struct TestMessage_Mu32Entry TestMessage_Mu32Entry;
typedef struct TestMessage_Mu64Entry TestMessage_Mu64Entry;
typedef struct TestMessage_Msf32Entry TestMessage_Msf32Entry;
typedef struct TestMessage_Msf64Entry TestMessage_Msf64Entry;

extern const proto_message proto_TestPod;
extern const proto_enum proto_TestEnum;
extern const proto_message proto_TestMessage;
extern const proto_service proto_TestService;
extern const proto_method proto_TestService_rpc2;
extern const proto_method proto_TestService_rpc1;
extern const proto_message proto_TestMessage_MbEntry;
extern const proto_message proto_TestMessage_MdEntry;
extern const proto_message proto_TestMessage_MfEntry;
extern const proto_message proto_TestMessage_MbyEntry;
extern const proto_message proto_TestMessage_MenEntry;
extern const proto_message proto_TestMessage_Mf32Entry;
extern const proto_message proto_TestMessage_Mf64Entry;
extern const proto_message proto_TestMessage_Mi32Entry;
extern const proto_message proto_TestMessage_Mi64Entry;
extern const proto_message proto_TestMessage_MmsgEntry;
extern const proto_message proto_TestMessage_MpodEntry;
extern const proto_message proto_TestMessage_Ms32Entry;
extern const proto_message proto_TestMessage_Ms64Entry;
extern const proto_message proto_TestMessage_MstrEntry;
extern const proto_message proto_TestMessage_Mu32Entry;
extern const proto_message proto_TestMessage_Mu64Entry;
extern const proto_message proto_TestMessage_Msf32Entry;
extern const proto_message proto_TestMessage_Msf64Entry;

enum TestPod_foo {
	TESTPOD_U = 1,
	TESTPOD_I = 2
};

struct TestPod {
	enum TestPod_foo foo_type;
	union {
		unsigned u;
		int i;
	} foo;
};

enum TestEnum {
	ENUM_A = 0,
	ENUM_B = 1,
	ENUM_C = 2
};
typedef enum TestEnum TestEnum;

struct TestMessage {
	pb_msg _pbhdr;
	bool b;
	unsigned u32;
	uint64_t u64;
	int i32;
	int64_t i64;
	int s32;
	int64_t s64;
	uint32_t f32;
	uint64_t f64;
	int32_t sf32;
	int64_t sf64;
	float f;
	double d;
	TestEnum en;
	pb_bytes by;
	pb_string str;
	pb_bool_list rb;
	pb_u32_list ru32;
	pb_u64_list ru64;
	pb_i32_list ri32;
	pb_i64_list ri64;
	pb_i32_list rs32;
	pb_i64_list rs64;
	pb_u32_list rf32;
	pb_u64_list rf64;
	pb_i32_list rsf32;
	pb_i64_list rsf64;
	pb_float_list rf;
	pb_double_list rd;
	pb_bytes_list rby;
	pb_string_list rstr;
	struct {int len; TestEnum *v; int _encoded;} ren;
	TestMessage *msg;
	TestPod pod;
	struct {int len; TestMessage **v;} rmsg;
	struct {int len; TestPod *v;} rpod;
	struct {int len; TestMessage_MbEntry *v;} mb;
	struct {int len; TestMessage_Mu32Entry *v;} mu32;
	struct {int len; TestMessage_Mu64Entry *v;} mu64;
	struct {int len; TestMessage_Mi32Entry *v;} mi32;
	struct {int len; TestMessage_Mi64Entry *v;} mi64;
	struct {int len; TestMessage_Ms32Entry *v;} ms32;
	struct {int len; TestMessage_Ms64Entry *v;} ms64;
	struct {int len; TestMessage_Mf32Entry *v;} mf32;
	struct {int len; TestMessage_Mf64Entry *v;} mf64;
	struct {int len; TestMessage_Msf32Entry *v;} msf32;
	struct {int len; TestMessage_Msf64Entry *v;} msf64;
	struct {int len; TestMessage_MfEntry *v;} mf;
	struct {int len; TestMessage_MdEntry *v;} md;
	struct {int len; TestMessage_MbyEntry **v;} mby;
	struct {int len; TestMessage_MstrEntry **v;} mstr;
	struct {int len; TestMessage_MenEntry *v;} men;
	struct {int len; TestMessage_MmsgEntry **v;} mmsg;
	struct {int len; TestMessage_MpodEntry *v;} mpod;
};

struct TestService {
	int (*rpc2)(TestService *svc, pb_allocator *obj, const TestPod *in, TestMessage *out);
	int (*rpc1)(TestService *svc, pb_allocator *obj, const TestMessage *in, TestPod *out);
};

struct TestMessage_MbEntry {
	unsigned key;
	bool value;
};

struct TestMessage_MdEntry {
	unsigned key;
	double value;
};

struct TestMessage_MfEntry {
	unsigned key;
	float value;
};

struct TestMessage_MbyEntry {
	pb_msg _pbhdr;
	unsigned key;
	pb_bytes value;
};

struct TestMessage_MenEntry {
	unsigned key;
	TestEnum value;
};

struct TestMessage_Mf32Entry {
	unsigned key;
	uint32_t value;
};

struct TestMessage_Mf64Entry {
	unsigned key;
	uint64_t value;
};

struct TestMessage_Mi32Entry {
	unsigned key;
	int value;
};

struct TestMessage_Mi64Entry {
	unsigned key;
	int64_t value;
};

struct TestMessage_MmsgEntry {
	pb_msg _pbhdr;
	unsigned key;
	TestMessage *value;
};

struct TestMessage_MpodEntry {
	unsigned key;
	TestPod value;
};

struct TestMessage_Ms32Entry {
	unsigned key;
	int value;
};

struct TestMessage_Ms64Entry {
	unsigned key;
	int64_t value;
};

struct TestMessage_MstrEntry {
	pb_msg _pbhdr;
	unsigned key;
	pb_string value;
};

struct TestMessage_Mu32Entry {
	unsigned key;
	unsigned value;
};

struct TestMessage_Mu64Entry {
	unsigned key;
	uint64_t value;
};

struct TestMessage_Msf32Entry {
	unsigned key;
	int32_t value;
};

struct TestMessage_Msf64Entry {
	unsigned key;
	int64_t value;
};

#ifdef __cplusplus
}
#endif

