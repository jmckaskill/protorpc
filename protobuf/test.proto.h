#pragma once
#include <protobuf.h>
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
	TESTPOD_U,
	TESTPOD_I,
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
	struct {int len; bool const *v;}	rb;
	struct {int len; uint32_t const *v;}	ru32;
	struct {int len; uint64_t const *v;}	ru64;
	struct {int len; int32_t const *v;}	ri32;
	struct {int len; int64_t const *v;}	ri64;
	struct {int len; int32_t const *v;}	rs32;
	struct {int len; int64_t const *v;}	rs64;
	struct {int len; uint32_t const *v;}	rf32;
	struct {int len; uint64_t const *v;}	rf64;
	struct {int len; int32_t const *v;}	rsf32;
	struct {int len; int64_t const *v;}	rsf64;
	struct {int len; float const *v;}	rf;
	struct {int len; double const *v;}	rd;
	struct {int len; pb_bytes_t const *v;}	rby;
	struct {int len; pb_string_t const *v;}	rstr;
	struct {int len; enum TestEnum const *v;}	ren;
	struct TestMessage const*	msg;
	struct TestPod	pod;
	struct {int len; struct TestMessage const **v;}	rmsg;
	struct {int len; struct TestPod const *v;}	rpod;
	struct {int len; struct TestMessage_MbEntry const *v;}	mb;
	struct {int len; struct TestMessage_Mu32Entry const *v;}	mu32;
	struct {int len; struct TestMessage_Mu64Entry const *v;}	mu64;
	struct {int len; struct TestMessage_Mi32Entry const *v;}	mi32;
	struct {int len; struct TestMessage_Mi64Entry const *v;}	mi64;
	struct {int len; struct TestMessage_Ms32Entry const *v;}	ms32;
	struct {int len; struct TestMessage_Ms64Entry const *v;}	ms64;
	struct {int len; struct TestMessage_Mf32Entry const *v;}	mf32;
	struct {int len; struct TestMessage_Mf64Entry const *v;}	mf64;
	struct {int len; struct TestMessage_Msf32Entry const *v;}	msf32;
	struct {int len; struct TestMessage_Msf64Entry const *v;}	msf64;
	struct {int len; struct TestMessage_MfEntry const *v;}	mf;
	struct {int len; struct TestMessage_MdEntry const *v;}	md;
	struct {int len; struct TestMessage_MbyEntry const **v;}	mby;
	struct {int len; struct TestMessage_MstrEntry const **v;}	mstr;
	struct {int len; struct TestMessage_MenEntry const *v;}	men;
	struct {int len; struct TestMessage_MmsgEntry const **v;}	mmsg;
	struct {int len; struct TestMessage_MpodEntry const *v;}	mpod;
};

char *pb_parse_TestMessage(char *p, pb_buf_t *obj, struct TestMessage *m);
int pb_print_TestMessage(pb_buf_t *a, struct TestMessage const *m);
int pb_get_TestMessage(const char *p, const char *e, pb_buf_t *obj, struct TestMessage *m);
void pb_term_TestMessage(struct TestMessage *m);
char *pb_encode_TestMessage(char *p, struct TestMessage const *m);
int pb_maxsz_TestMessage(struct TestMessage const *m);

bool pb_nonzero_TestMessage_MbEntry(struct TestMessage_MbEntry const *m);
char *pb_parse_TestMessage_MbEntry(char *p, struct TestMessage_MbEntry *m);
int pb_print_TestMessage_MbEntry(pb_buf_t *a, struct TestMessage_MbEntry const *m);
int pb_get_TestMessage_MbEntry(const char *p, const char *e, struct TestMessage_MbEntry *m);
void pb_term_TestMessage_MbEntry(struct TestMessage_MbEntry *m);
char *pb_encode_TestMessage_MbEntry(char *p, struct TestMessage_MbEntry const *m);
int pb_maxsz_TestMessage_MbEntry(struct TestMessage_MbEntry const *m);

bool pb_nonzero_TestMessage_Mu32Entry(struct TestMessage_Mu32Entry const *m);
char *pb_parse_TestMessage_Mu32Entry(char *p, struct TestMessage_Mu32Entry *m);
int pb_print_TestMessage_Mu32Entry(pb_buf_t *a, struct TestMessage_Mu32Entry const *m);
int pb_get_TestMessage_Mu32Entry(const char *p, const char *e, struct TestMessage_Mu32Entry *m);
void pb_term_TestMessage_Mu32Entry(struct TestMessage_Mu32Entry *m);
char *pb_encode_TestMessage_Mu32Entry(char *p, struct TestMessage_Mu32Entry const *m);
int pb_maxsz_TestMessage_Mu32Entry(struct TestMessage_Mu32Entry const *m);

bool pb_nonzero_TestMessage_Mu64Entry(struct TestMessage_Mu64Entry const *m);
char *pb_parse_TestMessage_Mu64Entry(char *p, struct TestMessage_Mu64Entry *m);
int pb_print_TestMessage_Mu64Entry(pb_buf_t *a, struct TestMessage_Mu64Entry const *m);
int pb_get_TestMessage_Mu64Entry(const char *p, const char *e, struct TestMessage_Mu64Entry *m);
void pb_term_TestMessage_Mu64Entry(struct TestMessage_Mu64Entry *m);
char *pb_encode_TestMessage_Mu64Entry(char *p, struct TestMessage_Mu64Entry const *m);
int pb_maxsz_TestMessage_Mu64Entry(struct TestMessage_Mu64Entry const *m);

bool pb_nonzero_TestMessage_Mi32Entry(struct TestMessage_Mi32Entry const *m);
char *pb_parse_TestMessage_Mi32Entry(char *p, struct TestMessage_Mi32Entry *m);
int pb_print_TestMessage_Mi32Entry(pb_buf_t *a, struct TestMessage_Mi32Entry const *m);
int pb_get_TestMessage_Mi32Entry(const char *p, const char *e, struct TestMessage_Mi32Entry *m);
void pb_term_TestMessage_Mi32Entry(struct TestMessage_Mi32Entry *m);
char *pb_encode_TestMessage_Mi32Entry(char *p, struct TestMessage_Mi32Entry const *m);
int pb_maxsz_TestMessage_Mi32Entry(struct TestMessage_Mi32Entry const *m);

bool pb_nonzero_TestMessage_Mi64Entry(struct TestMessage_Mi64Entry const *m);
char *pb_parse_TestMessage_Mi64Entry(char *p, struct TestMessage_Mi64Entry *m);
int pb_print_TestMessage_Mi64Entry(pb_buf_t *a, struct TestMessage_Mi64Entry const *m);
int pb_get_TestMessage_Mi64Entry(const char *p, const char *e, struct TestMessage_Mi64Entry *m);
void pb_term_TestMessage_Mi64Entry(struct TestMessage_Mi64Entry *m);
char *pb_encode_TestMessage_Mi64Entry(char *p, struct TestMessage_Mi64Entry const *m);
int pb_maxsz_TestMessage_Mi64Entry(struct TestMessage_Mi64Entry const *m);

bool pb_nonzero_TestMessage_Ms32Entry(struct TestMessage_Ms32Entry const *m);
char *pb_parse_TestMessage_Ms32Entry(char *p, struct TestMessage_Ms32Entry *m);
int pb_print_TestMessage_Ms32Entry(pb_buf_t *a, struct TestMessage_Ms32Entry const *m);
int pb_get_TestMessage_Ms32Entry(const char *p, const char *e, struct TestMessage_Ms32Entry *m);
void pb_term_TestMessage_Ms32Entry(struct TestMessage_Ms32Entry *m);
char *pb_encode_TestMessage_Ms32Entry(char *p, struct TestMessage_Ms32Entry const *m);
int pb_maxsz_TestMessage_Ms32Entry(struct TestMessage_Ms32Entry const *m);

bool pb_nonzero_TestMessage_Ms64Entry(struct TestMessage_Ms64Entry const *m);
char *pb_parse_TestMessage_Ms64Entry(char *p, struct TestMessage_Ms64Entry *m);
int pb_print_TestMessage_Ms64Entry(pb_buf_t *a, struct TestMessage_Ms64Entry const *m);
int pb_get_TestMessage_Ms64Entry(const char *p, const char *e, struct TestMessage_Ms64Entry *m);
void pb_term_TestMessage_Ms64Entry(struct TestMessage_Ms64Entry *m);
char *pb_encode_TestMessage_Ms64Entry(char *p, struct TestMessage_Ms64Entry const *m);
int pb_maxsz_TestMessage_Ms64Entry(struct TestMessage_Ms64Entry const *m);

bool pb_nonzero_TestMessage_Mf32Entry(struct TestMessage_Mf32Entry const *m);
char *pb_parse_TestMessage_Mf32Entry(char *p, struct TestMessage_Mf32Entry *m);
int pb_print_TestMessage_Mf32Entry(pb_buf_t *a, struct TestMessage_Mf32Entry const *m);
int pb_get_TestMessage_Mf32Entry(const char *p, const char *e, struct TestMessage_Mf32Entry *m);
void pb_term_TestMessage_Mf32Entry(struct TestMessage_Mf32Entry *m);
char *pb_encode_TestMessage_Mf32Entry(char *p, struct TestMessage_Mf32Entry const *m);
int pb_maxsz_TestMessage_Mf32Entry(struct TestMessage_Mf32Entry const *m);

bool pb_nonzero_TestMessage_Mf64Entry(struct TestMessage_Mf64Entry const *m);
char *pb_parse_TestMessage_Mf64Entry(char *p, struct TestMessage_Mf64Entry *m);
int pb_print_TestMessage_Mf64Entry(pb_buf_t *a, struct TestMessage_Mf64Entry const *m);
int pb_get_TestMessage_Mf64Entry(const char *p, const char *e, struct TestMessage_Mf64Entry *m);
void pb_term_TestMessage_Mf64Entry(struct TestMessage_Mf64Entry *m);
char *pb_encode_TestMessage_Mf64Entry(char *p, struct TestMessage_Mf64Entry const *m);
int pb_maxsz_TestMessage_Mf64Entry(struct TestMessage_Mf64Entry const *m);

bool pb_nonzero_TestMessage_Msf32Entry(struct TestMessage_Msf32Entry const *m);
char *pb_parse_TestMessage_Msf32Entry(char *p, struct TestMessage_Msf32Entry *m);
int pb_print_TestMessage_Msf32Entry(pb_buf_t *a, struct TestMessage_Msf32Entry const *m);
int pb_get_TestMessage_Msf32Entry(const char *p, const char *e, struct TestMessage_Msf32Entry *m);
void pb_term_TestMessage_Msf32Entry(struct TestMessage_Msf32Entry *m);
char *pb_encode_TestMessage_Msf32Entry(char *p, struct TestMessage_Msf32Entry const *m);
int pb_maxsz_TestMessage_Msf32Entry(struct TestMessage_Msf32Entry const *m);

bool pb_nonzero_TestMessage_Msf64Entry(struct TestMessage_Msf64Entry const *m);
char *pb_parse_TestMessage_Msf64Entry(char *p, struct TestMessage_Msf64Entry *m);
int pb_print_TestMessage_Msf64Entry(pb_buf_t *a, struct TestMessage_Msf64Entry const *m);
int pb_get_TestMessage_Msf64Entry(const char *p, const char *e, struct TestMessage_Msf64Entry *m);
void pb_term_TestMessage_Msf64Entry(struct TestMessage_Msf64Entry *m);
char *pb_encode_TestMessage_Msf64Entry(char *p, struct TestMessage_Msf64Entry const *m);
int pb_maxsz_TestMessage_Msf64Entry(struct TestMessage_Msf64Entry const *m);

bool pb_nonzero_TestMessage_MfEntry(struct TestMessage_MfEntry const *m);
char *pb_parse_TestMessage_MfEntry(char *p, struct TestMessage_MfEntry *m);
int pb_print_TestMessage_MfEntry(pb_buf_t *a, struct TestMessage_MfEntry const *m);
int pb_get_TestMessage_MfEntry(const char *p, const char *e, struct TestMessage_MfEntry *m);
void pb_term_TestMessage_MfEntry(struct TestMessage_MfEntry *m);
char *pb_encode_TestMessage_MfEntry(char *p, struct TestMessage_MfEntry const *m);
int pb_maxsz_TestMessage_MfEntry(struct TestMessage_MfEntry const *m);

bool pb_nonzero_TestMessage_MdEntry(struct TestMessage_MdEntry const *m);
char *pb_parse_TestMessage_MdEntry(char *p, struct TestMessage_MdEntry *m);
int pb_print_TestMessage_MdEntry(pb_buf_t *a, struct TestMessage_MdEntry const *m);
int pb_get_TestMessage_MdEntry(const char *p, const char *e, struct TestMessage_MdEntry *m);
void pb_term_TestMessage_MdEntry(struct TestMessage_MdEntry *m);
char *pb_encode_TestMessage_MdEntry(char *p, struct TestMessage_MdEntry const *m);
int pb_maxsz_TestMessage_MdEntry(struct TestMessage_MdEntry const *m);

char *pb_parse_TestMessage_MbyEntry(char *p, pb_buf_t *obj, struct TestMessage_MbyEntry *m);
int pb_print_TestMessage_MbyEntry(pb_buf_t *a, struct TestMessage_MbyEntry const *m);
int pb_get_TestMessage_MbyEntry(const char *p, const char *e, pb_buf_t *obj, struct TestMessage_MbyEntry *m);
void pb_term_TestMessage_MbyEntry(struct TestMessage_MbyEntry *m);
char *pb_encode_TestMessage_MbyEntry(char *p, struct TestMessage_MbyEntry const *m);
int pb_maxsz_TestMessage_MbyEntry(struct TestMessage_MbyEntry const *m);

char *pb_parse_TestMessage_MstrEntry(char *p, pb_buf_t *obj, struct TestMessage_MstrEntry *m);
int pb_print_TestMessage_MstrEntry(pb_buf_t *a, struct TestMessage_MstrEntry const *m);
int pb_get_TestMessage_MstrEntry(const char *p, const char *e, pb_buf_t *obj, struct TestMessage_MstrEntry *m);
void pb_term_TestMessage_MstrEntry(struct TestMessage_MstrEntry *m);
char *pb_encode_TestMessage_MstrEntry(char *p, struct TestMessage_MstrEntry const *m);
int pb_maxsz_TestMessage_MstrEntry(struct TestMessage_MstrEntry const *m);

bool pb_nonzero_TestMessage_MenEntry(struct TestMessage_MenEntry const *m);
char *pb_parse_TestMessage_MenEntry(char *p, struct TestMessage_MenEntry *m);
int pb_print_TestMessage_MenEntry(pb_buf_t *a, struct TestMessage_MenEntry const *m);
int pb_get_TestMessage_MenEntry(const char *p, const char *e, struct TestMessage_MenEntry *m);
void pb_term_TestMessage_MenEntry(struct TestMessage_MenEntry *m);
char *pb_encode_TestMessage_MenEntry(char *p, struct TestMessage_MenEntry const *m);
int pb_maxsz_TestMessage_MenEntry(struct TestMessage_MenEntry const *m);

char *pb_parse_TestMessage_MmsgEntry(char *p, pb_buf_t *obj, struct TestMessage_MmsgEntry *m);
int pb_print_TestMessage_MmsgEntry(pb_buf_t *a, struct TestMessage_MmsgEntry const *m);
int pb_get_TestMessage_MmsgEntry(const char *p, const char *e, pb_buf_t *obj, struct TestMessage_MmsgEntry *m);
void pb_term_TestMessage_MmsgEntry(struct TestMessage_MmsgEntry *m);
char *pb_encode_TestMessage_MmsgEntry(char *p, struct TestMessage_MmsgEntry const *m);
int pb_maxsz_TestMessage_MmsgEntry(struct TestMessage_MmsgEntry const *m);

bool pb_nonzero_TestMessage_MpodEntry(struct TestMessage_MpodEntry const *m);
char *pb_parse_TestMessage_MpodEntry(char *p, struct TestMessage_MpodEntry *m);
int pb_print_TestMessage_MpodEntry(pb_buf_t *a, struct TestMessage_MpodEntry const *m);
int pb_get_TestMessage_MpodEntry(const char *p, const char *e, struct TestMessage_MpodEntry *m);
void pb_term_TestMessage_MpodEntry(struct TestMessage_MpodEntry *m);
char *pb_encode_TestMessage_MpodEntry(char *p, struct TestMessage_MpodEntry const *m);
int pb_maxsz_TestMessage_MpodEntry(struct TestMessage_MpodEntry const *m);

bool pb_nonzero_TestPod(struct TestPod const *m);
char *pb_parse_TestPod(char *p, struct TestPod *m);
int pb_print_TestPod(pb_buf_t *a, struct TestPod const *m);
int pb_get_TestPod(const char *p, const char *e, struct TestPod *m);
void pb_term_TestPod(struct TestPod *m);
char *pb_encode_TestPod(char *p, struct TestPod const *m);
int pb_maxsz_TestPod(struct TestPod const *m);

char *pb_print_TestEnum(char *p, enum TestEnum v);
char *pb_parse_TestEnum(char *p, enum TestEnum *v);

struct TestService {
	int (*Test)(struct TestService*, pb_buf_t *obj, struct TestMessage const *in, struct TestMessage *out);
};

int rpc_TestService(struct TestService* rpc, const char *path, char *body, pb_buf_t *resp, pb_buf_t *obj);

#ifdef __cplusplus
}
#endif

