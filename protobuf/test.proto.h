#pragma once
#include <protobuf.h>
#include <compact.h>
#include <protorpc.h>
#ifdef __cplusplus
extern "C" {
#endif
enum TestEnum {
	ENUM_A = 0,
	ENUM_B = 1,
	ENUM_C = 2
};
struct TestMessage_MbEntry {
	uint32_t	key;
	bool	value;
};
struct TestMessage_Mu32Entry {
	uint32_t	key;
	uint32_t	value;
};
struct TestMessage_Mu64Entry {
	uint32_t	key;
	uint64_t	value;
};
struct TestMessage_Mi32Entry {
	uint32_t	key;
	int32_t	value;
};
struct TestMessage_Mi64Entry {
	uint32_t	key;
	int64_t	value;
};
struct TestMessage_Ms32Entry {
	uint32_t	key;
	int32_t	value;
};
struct TestMessage_Ms64Entry {
	uint32_t	key;
	int64_t	value;
};
struct TestMessage_Mf32Entry {
	uint32_t	key;
	uint32_t	value;
};
struct TestMessage_Mf64Entry {
	uint32_t	key;
	uint64_t	value;
};
struct TestMessage_Msf32Entry {
	uint32_t	key;
	int32_t	value;
};
struct TestMessage_Msf64Entry {
	uint32_t	key;
	int64_t	value;
};
struct TestMessage_MfEntry {
	uint32_t	key;
	float	value;
};
struct TestMessage_MdEntry {
	uint32_t	key;
	double	value;
};
struct TestMessage_MbyEntry {
	union pb_msg pb_hdr;
	uint32_t	key;
	pb_bytes_t	value;
};
struct TestMessage_MstrEntry {
	union pb_msg pb_hdr;
	uint32_t	key;
	pb_string_t	value;
};
struct TestMessage_MenEntry {
	uint32_t	key;
	enum TestEnum	value;
};
struct TestMessage;
struct TestMessage_MmsgEntry {
	union pb_msg pb_hdr;
	uint32_t	key;
	struct TestMessage const*	value;
};
enum TestPod_foo_type {
	TESTPOD_U = 1,
	TESTPOD_I = 2,
};
struct TestPod {
	enum TestPod_foo_type foo_type;
	union {
		uint32_t u;
		int32_t i;
	} foo;
};
struct TestMessage_MpodEntry {
	uint32_t	key;
	struct TestPod	value;
};
struct TestMessage {
	union pb_msg pb_hdr;
	bool	b;
	uint32_t	u32;
	uint64_t	u64;
	int32_t	i32;
	int64_t	i64;
	int32_t	s32;
	int64_t	s64;
	uint32_t	f32;
	uint64_t	f64;
	int32_t	sf32;
	int64_t	sf64;
	float	f;
	double	d;
	enum TestEnum	en;
	pb_bytes_t	by;
	pb_string_t	str;
	struct {int len; bool const *v; int _encoded;}	rb;
	struct {int len; uint32_t const *v; int _encoded;}	ru32;
	struct {int len; uint64_t const *v; int _encoded;}	ru64;
	struct {int len; int32_t const *v; int _encoded;}	ri32;
	struct {int len; int64_t const *v; int _encoded;}	ri64;
	struct {int len; int32_t const *v; int _encoded;}	rs32;
	struct {int len; int64_t const *v; int _encoded;}	rs64;
	struct {int len; uint32_t const *v; int _encoded;}	rf32;
	struct {int len; uint64_t const *v; int _encoded;}	rf64;
	struct {int len; int32_t const *v; int _encoded;}	rsf32;
	struct {int len; int64_t const *v; int _encoded;}	rsf64;
	struct {int len; float const *v; int _encoded;}	rf;
	struct {int len; double const *v; int _encoded;}	rd;
	struct {int len; pb_bytes_t const *v; int _encoded;}	rby;
	struct {int len; pb_string_t const *v; int _encoded;}	rstr;
	struct {int len; enum TestEnum const *v; int _encoded;}	ren;
	struct TestMessage const*	msg;
	struct TestPod	pod;
	struct {int len; struct TestMessage const **v; int _encoded;}	rmsg;
	struct {int len; struct TestPod const *v; int _encoded;}	rpod;
	struct {int len; struct TestMessage_MbEntry const *v; int _encoded;}	mb;
	struct {int len; struct TestMessage_Mu32Entry const *v; int _encoded;}	mu32;
	struct {int len; struct TestMessage_Mu64Entry const *v; int _encoded;}	mu64;
	struct {int len; struct TestMessage_Mi32Entry const *v; int _encoded;}	mi32;
	struct {int len; struct TestMessage_Mi64Entry const *v; int _encoded;}	mi64;
	struct {int len; struct TestMessage_Ms32Entry const *v; int _encoded;}	ms32;
	struct {int len; struct TestMessage_Ms64Entry const *v; int _encoded;}	ms64;
	struct {int len; struct TestMessage_Mf32Entry const *v; int _encoded;}	mf32;
	struct {int len; struct TestMessage_Mf64Entry const *v; int _encoded;}	mf64;
	struct {int len; struct TestMessage_Msf32Entry const *v; int _encoded;}	msf32;
	struct {int len; struct TestMessage_Msf64Entry const *v; int _encoded;}	msf64;
	struct {int len; struct TestMessage_MfEntry const *v; int _encoded;}	mf;
	struct {int len; struct TestMessage_MdEntry const *v; int _encoded;}	md;
	struct {int len; struct TestMessage_MbyEntry const **v; int _encoded;}	mby;
	struct {int len; struct TestMessage_MstrEntry const **v; int _encoded;}	mstr;
	struct {int len; struct TestMessage_MenEntry const *v; int _encoded;}	men;
	struct {int len; struct TestMessage_MmsgEntry const **v; int _encoded;}	mmsg;
	struct {int len; struct TestMessage_MpodEntry const *v; int _encoded;}	mpod;
};

extern const struct proto_message pb_type_TestMessage;

extern const struct proto_message pb_type_TestMessage_MbEntry;

extern const struct proto_message pb_type_TestMessage_Mu32Entry;

extern const struct proto_message pb_type_TestMessage_Mu64Entry;

extern const struct proto_message pb_type_TestMessage_Mi32Entry;

extern const struct proto_message pb_type_TestMessage_Mi64Entry;

extern const struct proto_message pb_type_TestMessage_Ms32Entry;

extern const struct proto_message pb_type_TestMessage_Ms64Entry;

extern const struct proto_message pb_type_TestMessage_Mf32Entry;

extern const struct proto_message pb_type_TestMessage_Mf64Entry;

extern const struct proto_message pb_type_TestMessage_Msf32Entry;

extern const struct proto_message pb_type_TestMessage_Msf64Entry;

extern const struct proto_message pb_type_TestMessage_MfEntry;

extern const struct proto_message pb_type_TestMessage_MdEntry;

extern const struct proto_message pb_type_TestMessage_MbyEntry;

extern const struct proto_message pb_type_TestMessage_MstrEntry;

extern const struct proto_message pb_type_TestMessage_MenEntry;

extern const struct proto_message pb_type_TestMessage_MmsgEntry;

extern const struct proto_message pb_type_TestMessage_MpodEntry;

extern const struct proto_message pb_type_TestPod;

extern const struct proto_enum pb_enum_TestEnum;

struct TestService {
	int (*Test)(struct TestService*, pb_buf_t *obj, struct TestMessage const *in, struct TestMessage *out);
};

int rpc_TestService(struct TestService* rpc, const char *path, char *body, pb_buf_t *resp, pb_buf_t *obj);

#ifdef __cplusplus
}
#endif

