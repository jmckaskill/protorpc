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

char *pb_parse_TestMessage(char *p, pb_buf_t *obj, struct TestMessage *m);
int pb_print_TestMessage(pb_buf_t *a, struct TestMessage const *m);
extern const struct proto_message pb_type_TestMessage;

bool pb_nonzero_TestMessage_MbEntry(struct TestMessage_MbEntry const *m);
char *pb_parse_TestMessage_MbEntry(char *p, struct TestMessage_MbEntry *m);
int pb_print_TestMessage_MbEntry(pb_buf_t *a, struct TestMessage_MbEntry const *m);
extern const struct proto_message pb_type_TestMessage_MbEntry;

bool pb_nonzero_TestMessage_Mu32Entry(struct TestMessage_Mu32Entry const *m);
char *pb_parse_TestMessage_Mu32Entry(char *p, struct TestMessage_Mu32Entry *m);
int pb_print_TestMessage_Mu32Entry(pb_buf_t *a, struct TestMessage_Mu32Entry const *m);
extern const struct proto_message pb_type_TestMessage_Mu32Entry;

bool pb_nonzero_TestMessage_Mu64Entry(struct TestMessage_Mu64Entry const *m);
char *pb_parse_TestMessage_Mu64Entry(char *p, struct TestMessage_Mu64Entry *m);
int pb_print_TestMessage_Mu64Entry(pb_buf_t *a, struct TestMessage_Mu64Entry const *m);
extern const struct proto_message pb_type_TestMessage_Mu64Entry;

bool pb_nonzero_TestMessage_Mi32Entry(struct TestMessage_Mi32Entry const *m);
char *pb_parse_TestMessage_Mi32Entry(char *p, struct TestMessage_Mi32Entry *m);
int pb_print_TestMessage_Mi32Entry(pb_buf_t *a, struct TestMessage_Mi32Entry const *m);
extern const struct proto_message pb_type_TestMessage_Mi32Entry;

bool pb_nonzero_TestMessage_Mi64Entry(struct TestMessage_Mi64Entry const *m);
char *pb_parse_TestMessage_Mi64Entry(char *p, struct TestMessage_Mi64Entry *m);
int pb_print_TestMessage_Mi64Entry(pb_buf_t *a, struct TestMessage_Mi64Entry const *m);
extern const struct proto_message pb_type_TestMessage_Mi64Entry;

bool pb_nonzero_TestMessage_Ms32Entry(struct TestMessage_Ms32Entry const *m);
char *pb_parse_TestMessage_Ms32Entry(char *p, struct TestMessage_Ms32Entry *m);
int pb_print_TestMessage_Ms32Entry(pb_buf_t *a, struct TestMessage_Ms32Entry const *m);
extern const struct proto_message pb_type_TestMessage_Ms32Entry;

bool pb_nonzero_TestMessage_Ms64Entry(struct TestMessage_Ms64Entry const *m);
char *pb_parse_TestMessage_Ms64Entry(char *p, struct TestMessage_Ms64Entry *m);
int pb_print_TestMessage_Ms64Entry(pb_buf_t *a, struct TestMessage_Ms64Entry const *m);
extern const struct proto_message pb_type_TestMessage_Ms64Entry;

bool pb_nonzero_TestMessage_Mf32Entry(struct TestMessage_Mf32Entry const *m);
char *pb_parse_TestMessage_Mf32Entry(char *p, struct TestMessage_Mf32Entry *m);
int pb_print_TestMessage_Mf32Entry(pb_buf_t *a, struct TestMessage_Mf32Entry const *m);
extern const struct proto_message pb_type_TestMessage_Mf32Entry;

bool pb_nonzero_TestMessage_Mf64Entry(struct TestMessage_Mf64Entry const *m);
char *pb_parse_TestMessage_Mf64Entry(char *p, struct TestMessage_Mf64Entry *m);
int pb_print_TestMessage_Mf64Entry(pb_buf_t *a, struct TestMessage_Mf64Entry const *m);
extern const struct proto_message pb_type_TestMessage_Mf64Entry;

bool pb_nonzero_TestMessage_Msf32Entry(struct TestMessage_Msf32Entry const *m);
char *pb_parse_TestMessage_Msf32Entry(char *p, struct TestMessage_Msf32Entry *m);
int pb_print_TestMessage_Msf32Entry(pb_buf_t *a, struct TestMessage_Msf32Entry const *m);
extern const struct proto_message pb_type_TestMessage_Msf32Entry;

bool pb_nonzero_TestMessage_Msf64Entry(struct TestMessage_Msf64Entry const *m);
char *pb_parse_TestMessage_Msf64Entry(char *p, struct TestMessage_Msf64Entry *m);
int pb_print_TestMessage_Msf64Entry(pb_buf_t *a, struct TestMessage_Msf64Entry const *m);
extern const struct proto_message pb_type_TestMessage_Msf64Entry;

bool pb_nonzero_TestMessage_MfEntry(struct TestMessage_MfEntry const *m);
char *pb_parse_TestMessage_MfEntry(char *p, struct TestMessage_MfEntry *m);
int pb_print_TestMessage_MfEntry(pb_buf_t *a, struct TestMessage_MfEntry const *m);
extern const struct proto_message pb_type_TestMessage_MfEntry;

bool pb_nonzero_TestMessage_MdEntry(struct TestMessage_MdEntry const *m);
char *pb_parse_TestMessage_MdEntry(char *p, struct TestMessage_MdEntry *m);
int pb_print_TestMessage_MdEntry(pb_buf_t *a, struct TestMessage_MdEntry const *m);
extern const struct proto_message pb_type_TestMessage_MdEntry;

char *pb_parse_TestMessage_MbyEntry(char *p, pb_buf_t *obj, struct TestMessage_MbyEntry *m);
int pb_print_TestMessage_MbyEntry(pb_buf_t *a, struct TestMessage_MbyEntry const *m);
extern const struct proto_message pb_type_TestMessage_MbyEntry;

char *pb_parse_TestMessage_MstrEntry(char *p, pb_buf_t *obj, struct TestMessage_MstrEntry *m);
int pb_print_TestMessage_MstrEntry(pb_buf_t *a, struct TestMessage_MstrEntry const *m);
extern const struct proto_message pb_type_TestMessage_MstrEntry;

bool pb_nonzero_TestMessage_MenEntry(struct TestMessage_MenEntry const *m);
char *pb_parse_TestMessage_MenEntry(char *p, struct TestMessage_MenEntry *m);
int pb_print_TestMessage_MenEntry(pb_buf_t *a, struct TestMessage_MenEntry const *m);
extern const struct proto_message pb_type_TestMessage_MenEntry;

char *pb_parse_TestMessage_MmsgEntry(char *p, pb_buf_t *obj, struct TestMessage_MmsgEntry *m);
int pb_print_TestMessage_MmsgEntry(pb_buf_t *a, struct TestMessage_MmsgEntry const *m);
extern const struct proto_message pb_type_TestMessage_MmsgEntry;

bool pb_nonzero_TestMessage_MpodEntry(struct TestMessage_MpodEntry const *m);
char *pb_parse_TestMessage_MpodEntry(char *p, struct TestMessage_MpodEntry *m);
int pb_print_TestMessage_MpodEntry(pb_buf_t *a, struct TestMessage_MpodEntry const *m);
extern const struct proto_message pb_type_TestMessage_MpodEntry;

bool pb_nonzero_TestPod(struct TestPod const *m);
char *pb_parse_TestPod(char *p, struct TestPod *m);
int pb_print_TestPod(pb_buf_t *a, struct TestPod const *m);
extern const struct proto_message pb_type_TestPod;

char *pb_print_TestEnum(char *p, enum TestEnum v);
char *pb_parse_TestEnum(char *p, enum TestEnum *v);

struct TestService {
	int (*Test)(struct TestService*, pb_buf_t *obj, struct TestMessage const *in, struct TestMessage *out);
};

int rpc_TestService(struct TestService* rpc, const char *path, char *body, pb_buf_t *resp, pb_buf_t *obj);

#ifdef __cplusplus
}
#endif

