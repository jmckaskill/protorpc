#include "test.proto.h"
#include <protorpc/http.h>
#include <protorpc/char-array.h>
#include <protorpc/test.h>
#include <stdlib.h>

// for zig zag types
// if negative = 2|x| - 1
// if positive = 2|x|

// tag sizes
// 0 through 15 - 1 byte
// 16 through 2047 - 2 bytes
// 2048 through 262143 - 3 bytes
// 262144 through 33554431 - 4 bytes
// 33554432 through max - 5 bytes


static const uint8_t test_pod_proto[] = {
	// i = -34, zigzag = 34*2-1 = 67
	0x10, 67
};

static const char test_pod_json[] =
	"{\n"
		"\t\"i\": -34\n"
	"}\n";

static const uint8_t test_proto[] = {
	// b = true
	0x08, 1,

	// i32 = -23 = 0xFFFFFFE9 = xF,x7F,x7F,x7F,x69
	0x10, 0xE9, 0xFF, 0xFF, 0xFF, 0xF,

	// s32 = -1234 = 2467 = 0x9A3 = 0x13,0x23
	0x18, 0xA3, 0x13,

	// sf32 = -34757 = 0xFFFF783B
	0x25, 0x3B, 0x78, 0xFF, 0xFF,

	// u32 = 1
	0x28, 1,

	// f32 = 34757 = 0x000087C5
	0x35, 0xC5, 0x87, 0, 0,

	// i64 = -3434565678781212898 = 0xD055F8B6567F571E = 1E 2E 7D 33 65 16 7E 2A 50 01
	0x38, 0x9E, 0xAE, 0xFD, 0xB3, 0xE5, 0x96, 0xFE, 0xAA, 0xD0, 0x01,

	// sf64 = -1234567890123456789 = 0x EEDD EF0B 8216 7EEB
	0x41, 0xEB, 0x7E, 0x16, 0x82, 0x0B, 0xEF, 0xDD, 0xEE,

	// s64 = -23585 = 47169 = xB841 = x02,x70,x41
	0x48, 0xC1, 0xF0, 0x02,

	// u64 = 10234 = 0x27FA = 0x4F,0x7A
	0x50, 0xFA, 0x4F,

	// f64 = 1234567890123456789 = 0x 1122 10F4 7DE9 8115
	0x59, 0x15, 0x81, 0xE9, 0x7D, 0xF4, 0x10, 0x22, 0x11,

	// f = 314 = 0x439d0000
	0x65, 0, 0, 0x9D, 0x43,

	// d = 3.141 = 0x4009 20C4 9BA5 E354
	0x69, 0x54, 0xE3, 0xA5, 0x9B, 0xC4, 0x20, 0x09, 0x40,

	// by = "abcde" = x61 x62 x63 x64 x65
	0x72, 5, 0x61, 0x62, 0x63, 0x64, 0x65,

	// str = "abc$¢€𝌆" = x61 x62 x63 x24 xC2A2 xE282AC xF09D8C86
	0x7A, 13, 0x61, 0x62, 0x63, 0x24, 0xC2, 0xA2, 0xE2, 0x82, 0xAC, 0xF0, 0x9D, 0x8C, 0x86,

	// en = ENUM_C = 2
	0x80, 1, 2,

	// msg = {b = true}
	0x8A, 1, 2, 0x8, 1,

	// pod = {i = -12}, -12 zigzag = 23
	0x92, 1, 2, 0x10, 23,

	// rb = [false,true,false]
	0xAA, 1, 3, 0, 1, 0,

	// ri32 = [-1,0,1] = [0xFFFFFFFF,0,1] = [xF x7F x7F x7F x7F, 0, 1]
	0xB2, 1, 7, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0, 1,

	// rs32 = [-10,0,10] = [19,0,20]
	0xBA, 1, 3, 19, 0, 20,

	// rsf32 = [-10,20,0] = [0xFFFFFFF6,20,0]
	0xC2, 1, 12, 0xF6, 0xFF, 0xFF, 0xFF, 20, 0, 0, 0, 0, 0, 0, 0,

	// ru32 = [1,2,3]
	0xCA, 1, 3, 1, 2, 3,

	// rf32 = [10,20,30]
	0xD2, 1, 12, 10, 0, 0, 0, 20, 0, 0, 0, 30, 0, 0, 0,
	
	// ri64 = [-2,0,2] = [0xFFFFFFFFFFFFFFFE,0,2] = [x01 x7F x7F x7F x7F x7F x7F x7F x7F x7E, 0, 2]
	0xDA, 1, 12, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0, 2,

	// rsf64 = [-100, 0, 100] = [0xFFFFFFFFFFFFFF9C, 0, 100]
	0xE2, 1, 24, 0x9C, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0,

	// rs64 = [-20,0,20] = [39,0,40]
	0xEA, 1, 3, 39, 0, 40,

	// ru64 = [3,4,5]
	0x92, 0xD, 3, 3, 4, 5,

	// rf64 = [30,40,50]
	0x9A, 0xD, 24, 30, 0, 0, 0, 0, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 50, 0, 0, 0, 0, 0, 0, 0,

	// rf = [3.5] = [0x40600000]
	0xA2, 0xD, 4, 0, 0, 0x60, 0x40,

	// rd = [1.1,2.2,3.3] = [0x3FF1 9999 9999 999A, 0x4001 9999 9999 999A, 0x400A 6666 6666 6666]
	0xAA, 0xD, 24,
		0x9A, 0x99, 0x99, 0x99, 0x99, 0x99, 0xF1, 0x3F, 
		0x9A, 0x99, 0x99, 0x99, 0x99, 0x99, 0x01, 0x40,
		0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x0A, 0x40,

	// rby = ["defgh", "abcde"] = [x64 x65 x66 x67 x68, x61 x62 x63 x64 x65]
	0xB2, 0xD, 5, 0x64, 0x65, 0x66, 0x67, 0x68,
	0xB2, 0xD, 5, 0x61, 0x62, 0x63, 0x64, 0x65,

	// rstr = ["ghikj","lmnop"] = [x67 x68 x69 x6b x6a, x6c x6d x6e x6f x70]
	0xBA, 0xD, 5, 0x67, 0x68, 0x69, 0x6B, 0x6A,
	0xBA, 0xD, 5, 0x6C, 0x6D, 0x6E, 0x6F, 0x70,

	// ren = [ENUM_C,ENUM_B,ENUM_A] = [2,1,0]
	0xC2, 0xD, 3, 2, 1, 0,

	// rmsg = [{b = true},{u64 = 10234},{}]; 10234 = 0x27FA = 0x4F,0x7A
	0xCA, 0xD, 2, 0x8, 1,
	0xCA, 0xD, 3, 0x50, 0xFA, 0x4F,
	0xCA, 0xD, 0,

	// rpod = [{u = 1},{i = -1}]
	0xD2, 0xD, 2, 0x8, 1,
	0xD2, 0xD, 2, 0x10, 1,
};

static const char test_json[] = 
	"{\n"
		"\t\"b\": true,\n"
		"\t\"i32\": -23,\n"
		"\t\"s32\": -1234,\n"
		"\t\"sf32\": -34757,\n"
		"\t\"u32\": 1,\n"
		"\t\"f32\": 34757,\n"
		"\t\"i64\": \"-3434565678781212898\",\n"
		"\t\"sf64\": \"-1234567890123456789\",\n"
		"\t\"s64\": \"-23585\",\n"
		"\t\"u64\": \"10234\",\n"
		"\t\"f64\": \"1234567890123456789\",\n"
		"\t\"f\": 314,\n"
		"\t\"d\": 3.141,\n"
		"\t\"by\": \"YWJjZGU\",\n"
		"\t\"str\": \"abc$¢€𝌆\",\n"
		"\t\"en\": \"ENUM_C\",\n"
		"\t\"msg\": {\n"
			"\t\t\"b\": true\n"
		"\t},\n"
		"\t\"pod\": {\n"
			"\t\t\"i\": -12\n"
		"\t},\n"
		"\t\"rb\": [\n"
			"\t\tfalse,\n"
			"\t\ttrue,\n"
			"\t\tfalse\n"
		"\t],\n"
		"\t\"ri32\": [\n"
			"\t\t-1,\n"
			"\t\t0,\n"
			"\t\t1\n"
		"\t],\n"
		"\t\"rs32\": [\n"
			"\t\t-10,\n"
			"\t\t0,\n"
			"\t\t10\n"
		"\t],\n"
		"\t\"rsf32\": [\n"
			"\t\t-10,\n"
			"\t\t20,\n"
			"\t\t0\n"
		"\t],\n"
		"\t\"ru32\": [\n"
			"\t\t1,\n"
			"\t\t2,\n"
			"\t\t3\n"
		"\t],\n"
		"\t\"rf32\": [\n"
			"\t\t10,\n"
			"\t\t20,\n"
			"\t\t30\n"
		"\t],\n"
		"\t\"ri64\": [\n"
			"\t\t\"-2\",\n"
			"\t\t\"0\",\n"
			"\t\t\"2\"\n"
		"\t],\n"
		"\t\"rsf64\": [\n"
			"\t\t\"-100\",\n"
			"\t\t\"0\",\n"
			"\t\t\"100\"\n"
		"\t],\n"
		"\t\"rs64\": [\n"
			"\t\t\"-20\",\n"
			"\t\t\"0\",\n"
			"\t\t\"20\"\n"
		"\t],\n"
		"\t\"ru64\": [\n"
			"\t\t\"3\",\n"
			"\t\t\"4\",\n"
			"\t\t\"5\"\n"
		"\t],\n"
		"\t\"rf64\": [\n"
			"\t\t\"30\",\n"
			"\t\t\"40\",\n"
			"\t\t\"50\"\n"
		"\t],\n"
		"\t\"rf\": [\n"
			"\t\t3.5\n"
		"\t],\n"
		"\t\"rd\": [\n"
			"\t\t1.1,\n"
			"\t\t2.2,\n"
			"\t\t3.3\n"
		"\t],\n"
		"\t\"rby\": [\n"
			"\t\t\"ZGVmZ2g\",\n"
			"\t\t\"YWJjZGU\"\n"
		"\t],\n"
		"\t\"rstr\": [\n"
			"\t\t\"ghikj\",\n"
			"\t\t\"lmnop\"\n"
		"\t],\n"
		"\t\"ren\": [\n"
			"\t\t\"ENUM_C\",\n"
			"\t\t\"ENUM_B\",\n"
			"\t\t\"ENUM_A\"\n"
		"\t],\n"
		"\t\"rmsg\": [\n"
			"\t\t{\n"
				"\t\t\t\"b\": true\n"
			"\t\t},\n"
			"\t\t{\n"
				"\t\t\t\"u64\": \"10234\"\n"
			"\t\t},\n"
			"\t\t{}\n"
		"\t],\n"
		"\t\"rpod\": [\n"
			"\t\t{\n"
				"\t\t\t\"u\": 1\n"
			"\t\t},\n"
			"\t\t{\n"
				"\t\t\t\"i\": -1\n"
			"\t\t}\n"
		"\t]\n"
	"}\n";

static void setup_message(TestMessage *m) {
	static bool rb[3] = { false, true, false };
	static uint32_t ru32[3] = { 1,2,3 };
	static uint64_t ru64[3] = { 3,4,5 };
	static int32_t ri32[3] = { -1,0,1 };
	static int64_t ri64[3] = { -2,0,2 };
	static int32_t rs32[3] = { -10,0,10 };
	static int64_t rs64[3] = { -20,0,20 };
	static uint32_t rf32[3] = { 10,20,30 };
	static uint64_t rf64[3] = { 30,40,50 };
	static int32_t rsf32[3] = { -10,20,0 };
	static int64_t rsf64[3] = { -100,0,100 };
	static float rf[1] = { 3.5 };
	static double rd[3] = { 1.1,2.2,3.3 };
	static pb_bytes rby[2] = { {5,(uint8_t*)"defgh"}, {5, (uint8_t*)"abcde"} };
	static pb_string rstr[2] = { {5, "ghikj"}, {5, "lmnop"} };
	static TestEnum ren[3] = { ENUM_C, ENUM_B, ENUM_A };

	static TestMessage msg2 = {0};
	msg2.b = true;
	static TestMessage msg3 = {0};
	msg3.u64 = 10234;
	static TestMessage emptymsg = {0};
	static TestMessage *rmsg[3] = { &msg2, &msg3, &emptymsg };

	static TestPod pod = {0};
	pod.foo_type = TESTPOD_I;
	pod.foo.i = -12;

	static TestPod rpod[2] = {0};
	rpod[0].foo_type = TESTPOD_U;
	rpod[0].foo.u = 1;
	rpod[1].foo_type = TESTPOD_I;
	rpod[1].foo.i = -1;

	m->b = true;
	m->u32 = 1;
	m->u64 = 10234;
	m->i32 = -23;
	m->i64 = INT64_C(-3434565678781212898);
	m->s32 = -1234;
	m->s64 = -23585;
	m->f32 = 34757;
	m->f64 = UINT64_C(1234567890123456789);
	m->sf32 = -34757;
	m->sf64 = INT64_C(-1234567890123456789);
	m->f = 314;
	m->d = 3.141;
	m->by.len = 5;
	m->by.p = (uint8_t*) "abcde";
	m->str.len = strlen("abc$¢€𝌆");
	m->str.c_str = "abc$¢€𝌆";
	m->en = ENUM_C;
	m->rb.len = 3;
	m->rb.v = rb;
	m->ru32.len = 3;
	m->ru32.v = ru32;
	m->ru64.len = 3;
	m->ru64.v = ru64;
	m->ri32.len = 3;
	m->ri32.v = ri32;
	m->ri64.len = 3;
	m->ri64.v = ri64;
	m->rs32.len = 3;
	m->rs32.v = rs32;
	m->rs64.len = 3;
	m->rs64.v = rs64;
	m->rf32.len = 3;
	m->rf32.v = rf32;
	m->rf64.len = 3;
	m->rf64.v = rf64;
	m->rsf32.len = 3;
	m->rsf32.v = rsf32;
	m->rsf64.len = 3;
	m->rsf64.v = rsf64;
	m->rf.len = 1;
	m->rf.v = rf;
	m->rd.len = 3;
	m->rd.v = rd;
	m->rby.len = 2;
	m->rby.v = rby;
	m->rstr.len = 2;
	m->rstr.v = rstr;
	m->ren.len = 3;
	m->ren.v = ren;
	m->msg = &msg2;
	m->pod = pod;
	m->rmsg.len = 3;
	m->rmsg.v = rmsg;
	m->rpod.len = 2;
	m->rpod.v = rpod;
}

static void setup_pod(TestPod *m) {
	m->foo_type = TESTPOD_I;
	m->foo.i = -34;
}

static void check_message(const struct TestMessage *m) {
	ASSERT_TRUE(m != NULL);
	EXPECT_EQ(true, m->b);
	EXPECT_EQ(1, m->u32);
	EXPECT_EQ(10234, m->u64);
	EXPECT_EQ(-23, m->i32);
	EXPECT_EQ(INT64_C(-3434565678781212898), m->i64);
	EXPECT_EQ(-1234, m->s32);
	EXPECT_EQ(-23585, m->s64);
	EXPECT_EQ(34757, m->f32);
	EXPECT_EQ(UINT64_C(1234567890123456789), m->f64);
	EXPECT_EQ(-34757, m->sf32);
	EXPECT_EQ(INT64_C(-1234567890123456789), m->sf64);
	EXPECT_FLOAT_EQ(314, m->f);
	EXPECT_FLOAT_EQ(3.141, m->d);
	EXPECT_EQ(5, m->by.len);
	EXPECT_STREQ("abcde", (char*)m->by.p);
	EXPECT_EQ(strlen("abc$¢€𝌆"), m->str.len);
	EXPECT_STREQ("abc$¢€𝌆", m->str.c_str);
	EXPECT_EQ(3, m->rb.len);
	EXPECT_EQ(false, m->rb.v[0]);
	EXPECT_EQ(true, m->rb.v[1]);
	EXPECT_EQ(false, m->rb.v[2]);
	EXPECT_EQ(3, m->ru32.len);
	EXPECT_EQ(1, m->ru32.v[0]);
	EXPECT_EQ(2, m->ru32.v[1]);
	EXPECT_EQ(3, m->ru32.v[2]);
	EXPECT_EQ(3, m->ru64.len);
	EXPECT_EQ(3, m->ru64.v[0]);
	EXPECT_EQ(4, m->ru64.v[1]);
	EXPECT_EQ(5, m->ru64.v[2]);
	EXPECT_EQ(3, m->ri32.len);
	EXPECT_EQ(-1, m->ri32.v[0]);
	EXPECT_EQ(0, m->ri32.v[1]);
	EXPECT_EQ(1, m->ri32.v[2]);
	EXPECT_EQ(3, m->ri64.len);
	EXPECT_EQ(-2, m->ri64.v[0]);
	EXPECT_EQ(0, m->ri64.v[1]);
	EXPECT_EQ(2, m->ri64.v[2]);
	EXPECT_EQ(3, m->rs32.len);
	EXPECT_EQ(-10, m->rs32.v[0]);
	EXPECT_EQ(0, m->rs32.v[1]);
	EXPECT_EQ(10, m->rs32.v[2]);
	EXPECT_EQ(3, m->rs64.len);
	EXPECT_EQ(-20, m->rs64.v[0]);
	EXPECT_EQ(0, m->rs64.v[1]);
	EXPECT_EQ(20, m->rs64.v[2]);
	EXPECT_EQ(3, m->rf32.len);
	EXPECT_EQ(10, m->rf32.v[0]);
	EXPECT_EQ(20, m->rf32.v[1]);
	EXPECT_EQ(30, m->rf32.v[2]);
	EXPECT_EQ(3, m->rf64.len);
	EXPECT_EQ(30, m->rf64.v[0]);
	EXPECT_EQ(40, m->rf64.v[1]);
	EXPECT_EQ(50, m->rf64.v[2]);
	EXPECT_EQ(3, m->rsf32.len);
	EXPECT_EQ(-10, m->rsf32.v[0]);
	EXPECT_EQ(20, m->rsf32.v[1]);
	EXPECT_EQ(0, m->rsf32.v[2]);
	EXPECT_EQ(3, m->rsf64.len);
	EXPECT_EQ(-100, m->rsf64.v[0]);
	EXPECT_EQ(0, m->rsf64.v[1]);
	EXPECT_EQ(100, m->rsf64.v[2]);
	EXPECT_EQ(1, m->rf.len);
	EXPECT_FLOAT_EQ(3.5, m->rf.v[0]);
	EXPECT_EQ(3, m->rd.len);
	EXPECT_FLOAT_EQ(1.1, m->rd.v[0]);
	EXPECT_FLOAT_EQ(2.2, m->rd.v[1]);
	EXPECT_FLOAT_EQ(3.3, m->rd.v[2]);
	EXPECT_EQ(2, m->rby.len);
	EXPECT_EQ(5, m->rby.v[0].len);
	EXPECT_STREQ("defgh", (char*) m->rby.v[0].p);
	EXPECT_EQ(5, m->rby.v[1].len);
	EXPECT_STREQ("abcde", (char*) m->rby.v[1].p);
	EXPECT_EQ(2, m->rstr.len);
	EXPECT_EQ(5, m->rstr.v[0].len);
	EXPECT_STREQ("ghikj", m->rstr.v[0].c_str);
	EXPECT_EQ(5, m->rstr.v[1].len);
	EXPECT_STREQ("lmnop", m->rstr.v[1].c_str);
	EXPECT_EQ(3, m->ren.len);
	EXPECT_EQ(ENUM_C, m->ren.v[0]);
	EXPECT_EQ(ENUM_B, m->ren.v[1]);
	EXPECT_EQ(ENUM_A, m->ren.v[2]);
	ASSERT_TRUE(m->msg != NULL);
	EXPECT_EQ(true, m->msg->b);
	EXPECT_EQ(TESTPOD_I, m->pod.foo_type);
	EXPECT_EQ(-12, m->pod.foo.i);
	EXPECT_EQ(3, m->rmsg.len);
	EXPECT_EQ(true, m->rmsg.v[0]->b);
	EXPECT_EQ(10234, m->rmsg.v[1]->u64);
	EXPECT_EQ(2, m->rpod.len);
	EXPECT_EQ(TESTPOD_U, m->rpod.v[0].foo_type);
	EXPECT_EQ(1, m->rpod.v[0].foo.u);
	EXPECT_EQ(TESTPOD_I, m->rpod.v[1].foo_type);
	EXPECT_EQ(-1, m->rpod.v[1].foo.i);
}

static void test_print() {
	struct TestMessage m = {0};
	setup_message(&m);

	char buf[4096];
	int sz = pb_print(&m, &proto_TestMessage, buf, sizeof(buf));
	EXPECT_EQ(strlen(test_json), sz);
	EXPECT_STREQ(test_json, buf);
}

static void test_encode_base64() {
	char buf[32];

	EXPECT_PTREQ(buf + 2, pb_encode_base64(buf, (uint8_t*) "a", 1));
	EXPECT_STREQ("YQ", buf);

	EXPECT_PTREQ(buf + 3, pb_encode_base64(buf, (uint8_t*) "ab", 2));
	EXPECT_STREQ("YWI", buf);

	EXPECT_PTREQ(buf + 4, pb_encode_base64(buf, (uint8_t*) "abc", 3));
	EXPECT_STREQ("YWJj", buf);
}

static void test_parse() {
	char objbuf[65536];
	pb_allocator obj = PB_INIT_ALLOCATOR(objbuf);
	char *json_in = strdup(test_json);
	struct TestMessage *m = (struct TestMessage*) pb_parse(&obj, &proto_TestMessage, json_in);
	check_message(m);
}

static void test_decode_base64() {
	pb_bytes by;
	char a[] = "YQ";
	EXPECT_PTREQ(a + 2, pb_decode_base64(a, &by));
	EXPECT_EQ(1, by.len);
	a[1] = 0;
	EXPECT_STREQ("a", (char*)by.p);

	char ab[] = "YWI";
	EXPECT_PTREQ(ab + 3, pb_decode_base64(ab, &by));
	EXPECT_EQ(2, by.len);
	ab[2] = 0;
	EXPECT_STREQ("ab", (char*)by.p);

	char abc[] = "YWJj";
	EXPECT_PTREQ(abc + 4, pb_decode_base64(abc, &by));
	EXPECT_EQ(3, by.len);
	abc[3] = 0;
	EXPECT_STREQ("abc", (char*)by.p);

	char apad[] = "YQ==";
	EXPECT_PTREQ(apad + 4, pb_decode_base64(apad, &by));
	EXPECT_EQ(1, by.len);
	apad[1] = 0;
	EXPECT_STREQ("a", (char*)by.p);

	char abpad[] = "YWI=";
	EXPECT_PTREQ(abpad + 4, pb_decode_base64(abpad, &by));
	EXPECT_EQ(2, by.len);
	abpad[2] = 0;
	EXPECT_STREQ("ab", (char*)by.p);
}

static void test_encode() {
	struct TestMessage m = {0};
	setup_message(&m);

	char buf[1024];
	memset(buf, 0xAB, sizeof(buf));

	int sz = pb_encoded_size(&m, &proto_TestMessage);
	EXPECT_EQ(sz, sizeof(test_proto));

	sz = pb_encode(&m, &proto_TestMessage, buf);
	EXPECT_EQ(sz, sizeof(test_proto));

	EXPECT_BYTES(buf, sz, test_proto, sizeof(test_proto));
}

static void test_decode() {
	char obuf[65536];
	pb_allocator obj = PB_INIT_ALLOCATOR(obuf);

	char *buf = (char*) malloc(sizeof(test_proto) + 1);
	memcpy(buf, test_proto, sizeof(test_proto));

	struct TestMessage *m = (struct TestMessage*) pb_decode(&obj, &proto_TestMessage, buf, sizeof(test_proto));
	check_message(m);

	free(buf);
}

static int test_rpc1(TestService *s, pb_allocator *a, const TestMessage *in, TestPod *out) {
	check_message(in);
	setup_pod(out);
	return 201;
}

static void test_dispatch() {
	char abuf[65536];
	char ibuf[4096];
	char obuf[4096];
	char tbuf[4096];

	pb_allocator obj = PB_INIT_ALLOCATOR(abuf);

	TestService svc = { 0 };
	svc.rpc1 = &test_rpc1;

	const char *path = "/twirp/com.example.TestService/rpc1";
	EXPECT_PTREQ(&proto_TestService_rpc1, pb_lookup_method(&svc, &proto_TestService, path, strlen(path)));

	// Try with protobufs
	int inlen = sizeof(test_proto);
	memcpy(ibuf, test_proto, inlen);
	int osz = pb_dispatch(&svc, &proto_TestService_rpc1, &obj, ibuf, inlen, obuf, sizeof(obuf));
	int tsz = sprintf(tbuf, "HTTP/1.1 201 \r\nContent-Type:application/protobuf\r\nContent-Length:%6d\r\n\r\n%.*s",
		(int) sizeof(test_pod_proto), (int) sizeof(test_pod_proto), test_pod_proto);

	EXPECT_BYTES(obuf, osz, tbuf, tsz);

	// Try with json
	inlen = strlen(test_json);
	memcpy(ibuf, test_json, inlen);
	osz = pb_dispatch(&svc, &proto_TestService_rpc1, &obj, ibuf, inlen, obuf, sizeof(obuf));
	tsz = sprintf(tbuf, "HTTP/1.1 201 \r\nContent-Type:application/json;charset=utf-8\r\nContent-Length:%6d\r\n\r\n%s",
		(int)strlen(test_pod_json), test_pod_json);

	EXPECT_BYTES(obuf, osz, tbuf, tsz);
}

static void test_http() {
	http h;
	char rxbuf[4096];
	http_reset(&h, rxbuf, sizeof(rxbuf), NULL); 

	// test normal one off request
	int len;
	char *rx = http_recv_buffer(&h, &len);
	EXPECT_PTREQ(rxbuf, rx);
	EXPECT_EQ(sizeof(rxbuf), len);
	EXPECT_EQ(HTTP_IDLE, h.state);

	len = sprintf(rxbuf, "POST / HTTP/1.1\r\nContent-Length:3\r\n\r\nabc");
	EXPECT_EQ(0, http_received(&h, len));
	EXPECT_EQ(HTTP_HEADERS_RECEIVED, h.state);
	EXPECT_EQ(3, h.content_length);

	// we haven't accepted the request so we shouldn't be getting
	// any data yet
	EXPECT_EQ(3, h.length_remaining);
	EXPECT_PTREQ(NULL, http_request_data(&h, &len));
	EXPECT_EQ(0, len);

	// now accept it
	EXPECT_EQ(0, http_send_continue(&h));
	EXPECT_EQ(HTTP_DATA_RECEIVED, h.state);
	rx = http_request_data(&h, &len);
	rx[len] = 0;
	EXPECT_EQ(3, len);
	EXPECT_STREQ("abc", rx);

	// queue the response
	static const char ok[] = "HTTP/1.1 200 OK\r\nContent-Length:0\r\n\r\n";
	EXPECT_EQ(0, http_send_response(&h, ok, strlen(ok)));
	EXPECT_EQ(HTTP_SENDING_RESPONSE, h.state);

	// send the response
	const char *tx = http_send_buffer(&h, &len);
	EXPECT_PTREQ(ok, tx);
	EXPECT_EQ(strlen(ok), len);

	// complete the send
	EXPECT_EQ(0, http_sent(&h, len));
	EXPECT_EQ(HTTP_RESPONSE_SENT, h.state);
	EXPECT_PTREQ(NULL, h.txnext);

	// test that there's no more
	tx = http_send_buffer(&h, &len);
	EXPECT_PTREQ(NULL, tx);
	EXPECT_EQ(0, len);




	// test pipelining whilst skipping the request payload
	// start the next request
	EXPECT_EQ(0, http_next_request(&h));
	EXPECT_EQ(HTTP_IDLE, h.state);

	rx = http_recv_buffer(&h, &len);
	len = sprintf(rx, "POST /foo HTTP/1.1\r\nContent-Length:3\r\n\r\ndefGET /bar HTTP/1.1\r\n\r\n");

	EXPECT_EQ(0, http_received(&h, len));
	EXPECT_EQ(HTTP_HEADERS_RECEIVED, h.state);
	EXPECT_STREQ("POST", h.method.c_str);
	EXPECT_STREQ("/foo", h.path.c_str);
	EXPECT_EQ(3, h.content_length);

	// dump the request payload
	static const char resp404[] = "HTTP/1.1 404 Not Found\r\nContent-Length:0\r\n\r\n";
	EXPECT_EQ(0, http_send_response(&h, resp404, strlen(resp404)));
	EXPECT_EQ(HTTP_SENDING_RESPONSE, h.state);

	// we shouldn't be getting any payload as we've dumped it
	EXPECT_PTREQ(NULL, http_request_data(&h, &len));
	EXPECT_EQ(0, len);

	tx = http_send_buffer(&h, &len);
	EXPECT_PTREQ(resp404, tx);
	EXPECT_EQ(strlen(resp404), len);

	EXPECT_EQ(0, http_sent(&h, len));
	EXPECT_EQ(HTTP_RESPONSE_SENT, h.state);
	// the header settings shouldn't have been cleared yet
	EXPECT_STREQ("POST", h.method.c_str);

	// continue on with parsing the next request
	EXPECT_EQ(0, http_next_request(&h));
	EXPECT_EQ(HTTP_HEADERS_RECEIVED, h.state);
	EXPECT_STREQ("GET", h.method.c_str);
	EXPECT_STREQ("/bar", h.path.c_str);

	EXPECT_EQ(0, http_send_response(&h, ok, strlen(ok)));
	EXPECT_EQ(HTTP_SENDING_RESPONSE, h.state);

	tx = http_send_buffer(&h, &len);
	EXPECT_PTREQ(ok, tx);
	EXPECT_EQ(strlen(ok), len);
	
	EXPECT_EQ(0, http_sent(&h, len));
	EXPECT_EQ(HTTP_RESPONSE_SENT, h.state);

	EXPECT_EQ(0, http_next_request(&h));
	EXPECT_EQ(HTTP_IDLE, h.state);



	// post with expect:100-continue
	rx = http_recv_buffer(&h, &len);
	len = sprintf(rx, "POST /continue HTTP/1.1\r\nContent-Length:5\r\nExpect:100-continue\r\n\r\n");

	EXPECT_EQ(0, http_received(&h, len));
	EXPECT_EQ(HTTP_HEADERS_RECEIVED, h.state);
	EXPECT_STREQ("/continue", h.path.c_str);
	EXPECT_EQ(1, h.expect_continue);

	EXPECT_EQ(0, http_send_continue(&h));
	EXPECT_EQ(HTTP_SENDING_CONTINUE, h.state);

	tx = http_send_buffer(&h, &len);
	EXPECT_STREQ("HTTP/1.1 100 Continue\r\n\r\n", tx);

	EXPECT_EQ(0, http_sent(&h, len));
	EXPECT_EQ(HTTP_RECEIVING_DATA, h.state);

	// send an early response - it won't be sent until the rest of the payload is received
	EXPECT_EQ(0, http_send_response(&h, ok, strlen(ok)));
	EXPECT_EQ(HTTP_RECEIVING_DATA, h.state);
	EXPECT_PTREQ(ok, h.txnext);
	EXPECT_PTREQ(NULL, http_send_buffer(&h, &len));

	rx = http_recv_buffer(&h, &len);
	EXPECT_PTREQ(rxbuf, rx);
	EXPECT_EQ(sizeof(rxbuf), len);

	// send the response data in two chunks. since we sent an early response it should be dumped
	len = sprintf(rx, "123");
	EXPECT_EQ(0, http_received(&h, len));
	EXPECT_EQ(HTTP_RECEIVING_DATA, h.state);

	http_request_data(&h, &len);
	EXPECT_EQ(0, len);

	// and the second chunk
	rx = http_recv_buffer(&h, &len);
	EXPECT_PTREQ(rxbuf, rx);
	EXPECT_EQ(sizeof(rxbuf), len);
	len = sprintf(rx, "45");
	EXPECT_EQ(0, http_received(&h, len));
	EXPECT_EQ(HTTP_SENDING_RESPONSE, h.state);

	tx = http_send_buffer(&h, &len);
	EXPECT_PTREQ(ok, tx);
	EXPECT_EQ(strlen(ok), len);
	EXPECT_EQ(0, http_sent(&h, len));

	

	// post a streamed upload/download
	// upload is three chunks:
	// 1 byte in initial request
	// 2 bytes in first chunk
	// 2 bytes in second chunk
	// download is two chunks:
	// 3 bytes and then 4 bytes
	EXPECT_EQ(0, http_next_request(&h));
	rx = http_recv_buffer(&h, &len);
	len = sprintf(rx, "POST /streamed HTTP/1.1\r\nContent-Length:5\r\n\r\n1");
	EXPECT_EQ(0, http_received(&h, len));
	EXPECT_EQ(HTTP_HEADERS_RECEIVED, h.state);

	EXPECT_EQ(0, http_send_continue(&h));
	EXPECT_EQ(HTTP_RECEIVING_DATA, h.state);

	// check that we have the first chunk
	EXPECT_STREQ("1", http_request_data(&h, &len));
	EXPECT_EQ(1, len);

	// upload the second chunk
	rx = http_recv_buffer(&h, &len);
	EXPECT_PTREQ(rxbuf+1, rx);
	EXPECT_EQ(sizeof(rxbuf)-1, len);
	len = sprintf(rx, "23");
	EXPECT_EQ(0, http_received(&h, len));
	EXPECT_EQ(HTTP_RECEIVING_DATA, h.state);

	// and check that we have the first two chunks
	EXPECT_STREQ("123", http_request_data(&h, &len));
	EXPECT_EQ(3, len);
	
	// consume some of the first two chunks
	// and see that we still have the rest of the first chunk
	EXPECT_EQ(0, http_consume_data(&h, 2));
	EXPECT_STREQ("3", http_request_data(&h, &len));
	EXPECT_EQ(1, len);

	// receive the rest of it
	rx = http_recv_buffer(&h, &len);
	EXPECT_PTREQ(rxbuf+1, rx);
	EXPECT_EQ(sizeof(rxbuf)-1, len);
	len = sprintf(rx, "45");
	EXPECT_EQ(0, http_received(&h, len));
	EXPECT_EQ(HTTP_DATA_RECEIVED, h.state);

	// we should have part of the first chunk and all of the second chunk still buffered
	EXPECT_STREQ("345", http_request_data(&h, &len));
	EXPECT_EQ(3, len);

	// if we consume it all, there should be none left
	EXPECT_EQ(0, http_consume_data(&h, len));
	EXPECT_EQ(0, h.length_remaining);
	http_request_data(&h, &len);
	EXPECT_EQ(0, len);

	// now start the download

	// first chunk
	static const char down1[] = "HTTP/1.1 200 OK\r\nContent-Length:7\r\n\r\nabc";
	EXPECT_EQ(0, http_send_response(&h, down1, strlen(down1)));
	EXPECT_EQ(HTTP_SENDING_RESPONSE, h.state);

	EXPECT_PTREQ(down1, http_send_buffer(&h, &len));
	EXPECT_EQ(strlen(down1), len);

	// send some of it
	EXPECT_EQ(0, http_sent(&h, 12));
	EXPECT_PTREQ(down1+12, http_send_buffer(&h, &len));
	EXPECT_EQ(strlen(down1)-12, len);

	// then the rest
	EXPECT_EQ(0, http_sent(&h, strlen(down1)-12));
	EXPECT_EQ(HTTP_RESPONSE_SENT, h.state);
	EXPECT_PTREQ(NULL, http_send_buffer(&h, &len));
	EXPECT_EQ(0, len);

	// then send the second chunk
	static const char down2[] = "defg";
	EXPECT_EQ(0, http_send_response(&h, down2, strlen(down2)));
	EXPECT_EQ(HTTP_SENDING_RESPONSE, h.state);
	EXPECT_PTREQ(down2, http_send_buffer(&h, &len));
	EXPECT_EQ(strlen(down2), len);
	EXPECT_EQ(0, http_sent(&h, len));
	EXPECT_EQ(HTTP_RESPONSE_SENT, h.state);
	EXPECT_PTREQ(NULL, http_send_buffer(&h, &len));
	EXPECT_EQ(0, len);
}

int main(int argc, char *argv[]) {
	start_test(&argc, argv);
	test_print();
	test_encode_base64();
	test_parse();
	test_decode_base64();
	test_encode();
	test_decode();
	test_dispatch();
	test_http();
	return finish_test();
}
