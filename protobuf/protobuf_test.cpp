#include <gtest/gtest.h>
#include "test.proto.h"

// wire types
#define VAR 0
#define F64 1
#define LEN 2
#define F32 5

// for zig zag types
// if negative = 2|x| - 1
// if positive = 2|x|

// tag sizes
// 0 through 15 - 1 byte
// 16 through 2047 - 2 bytes
// 2048 through 262143 - 3 bytes
// 262144 through 33554431 - 4 bytes
// 33554432 through max - 5 bytes

#define TAG1(TAG,TYPE)    ((TAG << 3) | TYPE)
#define TAG2_LO(TAG,TYPE) (0x80 | ((TAG & 15) << 3) | TYPE)
#define TAG2_HI(TAG)      (TAG >> 4)
#define TAG3_LO(TAG,TYPE) (0x80 | ((TAG & 15) << 3) | TYPE)
#define TAG3_MID(TAG)     (0x80 | (TAG >> 4))
#define TAG3_HI(TAG)      (TAG >> 11)

static const uint8_t test_proto[] = {
	// b = true
	TAG1(1, VAR), 1,

	// -23 = 0xFFFFFFE9 = xF,x7F,x7F,x7F,x69
	TAG1(2, VAR), 0xE9, 0xFF, 0xFF, 0xFF, 0xF,

	// s32 = -1234 = 2469 = 0x9A3 = 0x13,0x23
	TAG1(3, VAR), 0xA3, 0x13,

	// sf32 = -34757 = 0xFFFF783B
	TAG1(4, F32), 0x3B, 0x78, 0xFF, 0xFF,

	// u32 = 1
	TAG1(5, VAR), 1,

	// f32 = -34757 = 0x000087C5
	TAG1(6, F32), 0xC5, 0x87, 0, 0,

	// i64 = -34 = 0xFFFFFFFFFFFFFFDE = x01,x7F,x7F,x7F,x7F,x7F,x7F,x7F,x7F,x5E
	TAG1(7, VAR), 0xDE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01,

	// sf64 = -575859 = 0xFFFFFFFFFFF7368D
	TAG1(8, F64), 0x8D, 0x36, 0xF7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,

	// s64 = -23585 = 47169 = xB841 = x02,x70,x41
	TAG1(9, VAR), 0xC1, 0xF0, 0x02,

	// u64 = 10234 = 0x27FA = 0x4F,0x7A
	TAG1(10, VAR), 0xFA, 0x4F,

	// f64 = 575859 = 0x000000000008C973
	TAG1(11, F64), 0x73, 0xC9, 0x08, 0, 0, 0, 0, 0,

	// f = 314 = 0x439d0000
	TAG1(12, F32), 0, 0, 0x9D, 0x43,

	// d = 3.141 = 0x4009 20C4 9BA5 E354
	TAG1(13, F64), 0x54, 0xE3, 0xA5, 0x9B, 0xC4, 0x20, 0x09, 0x40,

	// by = "abcde" = x61 x62 x63 x64 x65
	TAG1(14, LEN), 5, 0x61, 0x62, 0x63, 0x64, 0x65,

	// str = "abcde" = x61 x62 x63 x64 x65
	TAG1(15, LEN), 5, 0x61, 0x62, 0x63, 0x64, 0x65,

	// en = ENUM_C = 2
	TAG2_LO(16, VAR), TAG2_HI(16), 2,

	// msg = {b = true}
	TAG2_LO(17, LEN), TAG2_HI(17), 2, TAG1(1, VAR), 1,

	// pod = {u = 34}
	TAG2_LO(18, LEN), TAG2_HI(18), 2, TAG1(1, VAR), 34,

	// rb = [false,true,false]
	TAG2_LO(21, LEN), TAG2_HI(21), 3, 0, 1, 0,

	// ri32 = [-1,0,1] = [0xFFFFFFFF,0,1] = [xF x7F x7F x7F x7F, 0, 1]
	TAG2_LO(22, LEN), TAG2_HI(22), 7, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0, 1,

	// rs32 = [-10,0,10] = [19,0,20]
	TAG2_LO(23, LEN), TAG2_HI(23), 3, 19, 0, 20,

	// rsf32 = [-10,20,0] = [0xFFFFFFF6,20,0]
	TAG2_LO(24, LEN), TAG2_HI(24), 12, 0xF6, 0xFF, 0xFF, 0xFF, 20, 0, 0, 0, 0, 0, 0, 0,

	// ru32 = [1,2,3]
	TAG2_LO(25, LEN), TAG2_HI(25), 3, 1, 2, 3,

	// rf32 = [10,20,30]
	TAG2_LO(26, LEN), TAG2_HI(26), 12, 10, 0, 0, 0, 20, 0, 0, 0, 30, 0, 0, 0,
	
	// ri64 = [-2,0,2] = [0xFFFFFFFFFFFFFFFE,0,2] = [x01 x7F x7F x7F x7F x7F x7F x7F x7F x7E, 0, 2]
	TAG2_LO(27, LEN), TAG2_HI(27), 12, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0, 2,

	// rsf64 = [-100, 0, 100] = [0xFFFFFFFFFFFFFF9C, 0, 100]
	TAG2_LO(28, LEN), TAG2_HI(28), 24, 0x9C, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0,

	// rs64 = [-20,0,20] = [39,0,40]
	TAG2_LO(29, LEN), TAG2_HI(29), 3, 39, 0, 40,

	// ru64 = [3,4,5]
	TAG2_LO(210, LEN), TAG2_HI(210), 3, 3, 4, 5,

	// rf64 = [30,40,50]
	TAG2_LO(211, LEN), TAG2_HI(211), 24, 30, 0, 0, 0, 0, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 50, 0, 0, 0, 0, 0, 0, 0,

	// rf = [3.5] = [0x40600000]
	TAG2_LO(212, LEN), TAG2_HI(212), 4, 0, 0, 0x60, 0x40,

	// rd = [1.1,2.2,3.3] = [0x3FF1 9999 9999 999A, 0x4001 9999 9999 999A, 0x400A 6666 6666 6666]
	TAG2_LO(213, LEN), TAG2_HI(213), 24,
		0x9A, 0x99, 0x99, 0x99, 0x99, 0x99, 0xF1, 0x3F, 
		0x9A, 0x99, 0x99, 0x99, 0x99, 0x99, 0x01, 0x40,
		0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x0A, 0x40,

	// rby = ["defgh", "abcde"] = [x64 x65 x66 x67 x68, x61 x62 x63 x64 x65]
	TAG2_LO(214, LEN), TAG2_HI(214), 5, 0x64, 0x65, 0x66, 0x67, 0x68,
	TAG2_LO(214, LEN), TAG2_HI(214), 5, 0x61, 0x62, 0x63, 0x64, 0x65,

	// rstr = ["ghikj","lmnop"] = [x67 x68 x69 x6b x6a, x6c x6d x6e x6f x70]
	TAG2_LO(215, LEN), TAG2_HI(215), 5, 0x67, 0x68, 0x69, 0x6B, 0x6A,
	TAG2_LO(215, LEN), TAG2_HI(215), 5, 0x6C, 0x6D, 0x6E, 0x6F, 0x70,

	// ren = [ENUM_C,ENUM_B,ENUM_A] = [2,1,0]
	TAG2_LO(216, LEN), TAG2_HI(216), 3, 2, 1, 0,

	// rmsg = [{u64 = 10234}]; 10234 = 0x27FA = 0x4F,0x7A
	TAG2_LO(217, LEN), TAG2_HI(217), 3, TAG1(10, VAR), 0xFA, 0x4F,

	// rpod = [{u = 1},{i = -1}]
	TAG2_LO(218, LEN), TAG2_HI(218), 2, TAG1(1, VAR), 1,
	TAG2_LO(218, LEN), TAG2_HI(218), 2, TAG1(2, VAR), 1,
};

static const char test_json[] = 
	"{"
		"\"b\":true,"
		"\"u32\":1,"
		"\"u64\":\"10234\","
		"\"i32\":-23,"
		"\"i64\":\"-34\","
		"\"s32\":-1234,"
		"\"s64\":\"-23585\","
		"\"f32\":34757,"
		"\"f64\":\"575859\","
		"\"sf32\":-34757,"
		"\"sf64\":\"-575859\","
		"\"f\":314,"
		"\"d\":3.141,"
		"\"en\":\"ENUM_C\","
		"\"by\":\"YWJjZGU\","
		"\"str\":\"abcde\","
		"\"rb\":[false,true,false],"
		"\"ru32\":[1,2,3],"
		"\"ru64\":[\"3\",\"4\",\"5\"],"
		"\"ri32\":[-1,0,1],"
		"\"ri64\":[\"-2\",\"0\",\"2\"],"
		"\"rs32\":[-10,0,10],"
		"\"rs64\":[\"-20\",\"0\",\"20\"],"
		"\"rf32\":[10,20,30],"
		"\"rf64\":[\"30\",\"40\",\"50\"],"
		"\"rsf32\":[-10,20,0],"
		"\"rsf64\":[\"-100\",\"0\",\"100\"],"
		"\"rf\":[3.5],"
		"\"rd\":[1.1,2.2,3.3],"
		"\"rby\":[\"ZGVmZ2g\",\"YWJjZGU\"],"
		"\"rstr\":[\"ghikj\",\"lmnop\"],"
		"\"ren\":[\"ENUM_C\",\"ENUM_B\",\"ENUM_A\"],"
		"\"msg\":{"
			"\"b\":true"
		"},"
		"\"pod\":{\"u\":34},"
		"\"rmsg\":[{\"u64\":\"10234\"}],"
		"\"rpod\":[{\"u\":1},{\"i\":-1}]"
	"}\n";


static const char test_pretty[] = 
	"{\n"
		"\t\"b\": true,\n"
		"\t\"u32\": 1,\n"
		"\t\"u64\": \"10234\",\n"
		"\t\"i32\": -23,\n"
		"\t\"i64\": \"-34\",\n"
		"\t\"s32\": -1234,\n"
		"\t\"s64\": \"-23585\",\n"
		"\t\"f32\": 34757,\n"
		"\t\"f64\": \"575859\",\n"
		"\t\"sf32\": -34757,\n"
		"\t\"sf64\": \"-575859\",\n"
		"\t\"f\": 314,\n"
		"\t\"d\": 3.141,\n"
		"\t\"en\": \"ENUM_C\",\n"
		"\t\"by\": \"YWJjZGU\",\n"
		"\t\"str\": \"abcde\",\n"
		"\t\"rb\": [\n"
			"\t\tfalse,\n"
			"\t\ttrue,\n"
			"\t\tfalse\n"
		"\t],\n"
		"\t\"ru32\": [\n"
			"\t\t1,\n"
			"\t\t2,\n"
			"\t\t3\n"
		"\t],\n"
		"\t\"ru64\": [\n"
			"\t\t\"3\",\n"
			"\t\t\"4\",\n"
			"\t\t\"5\"\n"
		"\t],\n"
		"\t\"ri32\": [\n"
			"\t\t-1,\n"
			"\t\t0,\n"
			"\t\t1\n"
		"\t],\n"
		"\t\"ri64\": [\n"
			"\t\t\"-2\",\n"
			"\t\t\"0\",\n"
			"\t\t\"2\"\n"
		"\t],\n"
		"\t\"rs32\": [\n"
			"\t\t-10,\n"
			"\t\t0,\n"
			"\t\t10\n"
		"\t],\n"
		"\t\"rs64\": [\n"
			"\t\t\"-20\",\n"
			"\t\t\"0\",\n"
			"\t\t\"20\"\n"
		"\t],\n"
		"\t\"rf32\": [\n"
			"\t\t10,\n"
			"\t\t20,\n"
			"\t\t30\n"
		"\t],\n"
		"\t\"rf64\": [\n"
			"\t\t\"30\",\n"
			"\t\t\"40\",\n"
			"\t\t\"50\"\n"
		"\t],\n"
		"\t\"rsf32\": [\n"
			"\t\t-10,\n"
			"\t\t20,\n"
			"\t\t0\n"
		"\t],\n"
		"\t\"rsf64\": [\n"
			"\t\t\"-100\",\n"
			"\t\t\"0\",\n"
			"\t\t\"100\"\n"
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
		"\t\"msg\": {\n"
			"\t\t\"b\": true\n"
		"\t},\n"
		"\t\"pod\": {\n"
			"\t\t\"u\": 34\n"
		"\t},\n"
		"\t\"rmsg\": [{\n"
				"\t\t\t\"u64\": \"10234\"\n"
			"\t\t}\n"
		"\t],\n"
		"\t\"rpod\": [{\n"
				"\t\t\t\"u\": 1\n"
			"\t\t},{\n"
				"\t\t\t\"i\": -1\n"
			"\t\t}\n"
		"\t]\n"
	"}\n";

static void setup_message(struct TestMessage *m) {
	static const bool rb[3] = { false, true, false };
	static const uint32_t ru32[3] = { 1,2,3 };
	static const uint64_t ru64[3] = { 3,4,5 };
	static const int32_t ri32[3] = { -1,0,1 };
	static const int64_t ri64[3] = { -2,0,2 };
	static const int32_t rs32[3] = { -10,0,10 };
	static const int64_t rs64[3] = { -20,0,20 };
	static const uint32_t rf32[3] = { 10,20,30 };
	static const uint64_t rf64[3] = { 30,40,50 };
	static const int32_t rsf32[3] = { -10,20,0 };
	static const int64_t rsf64[3] = { -100,0,100 };
	static const float rf[1] = { 3.5 };
	static const double rd[3] = { 1.1,2.2,3.3 };
	static const pb_bytes_t rby[2] = { {5,(uint8_t*)"defgh"}, {5, (uint8_t*)"abcde"} };
	static const pb_string_t rstr[2] = { {5, "ghikj"}, {5, "lmnop"} };
	static const enum TestEnum ren[3] = { ENUM_C, ENUM_B, ENUM_A };

	static struct TestMessage msg2 = {};
	msg2.b = true;

	static struct TestMessage msg3 = {};
	msg3.u64 = 10234;

	static const struct TestMessage *rmsg[1] = { &msg3 };

	static struct TestPod pod = {};
	pod.foo_type = TESTPOD_U;
	pod.foo.u = 34;

	static struct TestPod rpod[2] = {};
	rpod[0].foo_type = TESTPOD_U;
	rpod[0].foo.u = 1;
	rpod[1].foo_type = TESTPOD_I;
	rpod[1].foo.i = -1;

	m->b = true;
	m->u32 = 1;
	m->u64 = 10234;
	m->i32 = -23;
	m->i64 = -34;
	m->s32 = -1234;
	m->s64 = -23585;
	m->f32 = 34757;
	m->f64 = 575859;
	m->sf32 = -34757;
	m->sf64 = -575859;
	m->f = 314;
	m->d = 3.141;
	m->by.len = 5;
	m->by.p = (uint8_t*) "abcde";
	m->str.len = 5;
	m->str.c_str = "abcde";
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
	m->rmsg.len = 1;
	m->rmsg.v = rmsg;
	m->rpod.len = 2;
	m->rpod.v = rpod;
}

static void check_message(const struct TestMessage *m) {
	EXPECT_EQ(true, m->b);
	EXPECT_EQ(1, m->u32);
	EXPECT_EQ(10234, m->u64);
	EXPECT_EQ(-23, m->i32);
	EXPECT_EQ(-34, m->i64);
	EXPECT_EQ(-1234, m->s32);
	EXPECT_EQ(-23585, m->s64);
	EXPECT_EQ(34757, m->f32);
	EXPECT_EQ(575859, m->f64);
	EXPECT_EQ(-34757, m->sf32);
	EXPECT_EQ(-575859, m->sf64);
	EXPECT_EQ(314, m->f);
	EXPECT_EQ(3.141, m->d);
	EXPECT_EQ(5, m->by.len);
	EXPECT_STREQ("abcde", (char*)m->by.p);
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
	EXPECT_EQ(3.5, m->rf.v[0]);
	EXPECT_EQ(3, m->rd.len);
	EXPECT_EQ(1.1, m->rd.v[0]);
	EXPECT_EQ(2.2, m->rd.v[1]);
	EXPECT_EQ(3.3, m->rd.v[2]);
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
	EXPECT_EQ(TESTPOD_U, m->pod.foo_type);
	EXPECT_EQ(34, m->pod.foo.u);
	EXPECT_EQ(1, m->rmsg.len);
	EXPECT_EQ(10234, m->rmsg.v[0]->u64);
	EXPECT_EQ(2, m->rpod.len);
	EXPECT_EQ(TESTPOD_U, m->rpod.v[0].foo_type);
	EXPECT_EQ(1, m->rpod.v[0].foo.u);
	EXPECT_EQ(TESTPOD_I, m->rpod.v[1].foo_type);
	EXPECT_EQ(-1, m->rpod.v[1].foo.i);
}

TEST(protobuf, print) {
	struct TestMessage m = {};
	setup_message(&m);

	char buf[4096], pbuf[4096];
	pb_buf_t pr = PB_INIT_BUF(buf);
	pb_buf_t pp = PB_INIT_BUF(pbuf);

	EXPECT_EQ(0, pb_print_TestMessage(&pr, &m));
	EXPECT_EQ(0, pb_pretty_print(&pp, (char*) buf, pr.next - buf));
	ASSERT_EQ(0, pb_append(&pr, "\0", 1));
	ASSERT_EQ(0, pb_append(&pp, "\0", 1));
	EXPECT_STREQ(test_json, (char*) buf);
	EXPECT_STREQ(test_pretty, (char*) pbuf);
}

TEST(protobuf, print_bytes) {
	char buf[16];
	pb_buf_t b;
	pb_bytes_t by;

	by.len = 1;
	by.p = (uint8_t*) "a";
	b.next = buf;
	b.end = buf + sizeof(buf);
	EXPECT_EQ(0, pb_print_bytes(&b, by));
	*b.next = 0;
	EXPECT_EQ(buf + 5, b.next);
	EXPECT_STREQ("\"YQ\",", (char*) buf);

	by.len = 2;
	by.p = (uint8_t*) "ab";
	b.next = buf;
	b.end = buf + sizeof(buf);
	EXPECT_EQ(0, pb_print_bytes(&b, by));
	*b.next = 0;
	EXPECT_EQ(buf + 6, b.next);
	EXPECT_STREQ("\"YWI\",", (char*) buf);

	by.len = 3;
	by.p = (uint8_t*) "abc";
	b.next = buf;
	b.end = buf + sizeof(buf);
	EXPECT_EQ(0, pb_print_bytes(&b, by));
	*b.next = 0;
	EXPECT_EQ(buf + 7, b.next);
	EXPECT_STREQ("\"YWJj\",", (char*) buf);
}

TEST(protobuf, parse) {
	char objbuf[4096];
	pb_buf_t obj = PB_INIT_BUF(objbuf);
	char *json_in = strdup(test_json);
	size_t in_len = strlen(json_in) - 1; // ignore the trailing newline
	struct TestMessage m = {};
	EXPECT_EQ(json_in + in_len, pb_parse_TestMessage(json_in, &obj, &m));
	check_message(&m);
}

TEST(protobuf, parse_bytes) {
	pb_bytes_t by;
	char a[] = "\"YQ\"";
	EXPECT_EQ(a + 4, pb_parse_bytes(a, &by));
	EXPECT_EQ(1, by.len);
	EXPECT_STREQ("a", (char*)by.p);

	char ab[] = "\"YWI\"";
	EXPECT_EQ(ab + 5, pb_parse_bytes(ab, &by));
	EXPECT_EQ(2, by.len);
	EXPECT_STREQ("ab", (char*)by.p);

	char abc[] = "\"YWJj\"";
	EXPECT_EQ(abc + 6, pb_parse_bytes(abc, &by));
	EXPECT_EQ(3, by.len);
	EXPECT_STREQ("abc", (char*)by.p);

	char apad[] = "\"YQ==\"";
	EXPECT_EQ(apad + 6, pb_parse_bytes(apad, &by));
	EXPECT_EQ(1, by.len);
	EXPECT_STREQ("a", (char*)by.p);

	char abpad[] = "\"YWI=\"";
	EXPECT_EQ(abpad + 6, pb_parse_bytes(abpad, &by));
	EXPECT_EQ(2, by.len);
	EXPECT_STREQ("ab", (char*)by.p);
}

bool operator==(pb_bytes_t a, pb_bytes_t b) {
	return a.len == b.len && !memcmp(a.p, b.p, a.len);
}

std::ostream& operator<<(std::ostream& o, pb_bytes_t m) {
	std::ios_base::fmtflags flags = o.flags();
	std::streamsize prec = o.precision();
	char fill = o.fill();
	o << "bytes{";
	o << std::hex << std::setfill('0') << std::uppercase;	
	for (int i = 0; i < m.len; i++) {
		if (i && !(i&1)) {o << " ";}
		o << std::setw(2) << (int) m.p[i];
	}
	o << "}";
	o.flags(flags);
	o.precision(prec);
	o.fill(fill);
	return o;
}

TEST(protobuf, encode) {
	struct TestMessage m = {};
	setup_message(&m);

	char buf[1024];
	ASSERT_LT(pb_maxsz_TestMessage(&m), (int) sizeof(buf));

	char *end = pb_encode_TestMessage(buf, &m);

	pb_bytes_t test = {sizeof(test_proto), test_proto};
	pb_bytes_t have = {(int) (end - buf), (uint8_t*) buf};
	EXPECT_EQ(test, have);
}

TEST(protobuf, decode) {
	struct TestMessage m = {};
	char obuf[4096];
	pb_buf_t obj = PB_INIT_BUF(obuf);

	char *buf = (char*) malloc(sizeof(test_proto) + 1);
	memcpy(buf, test_proto, sizeof(test_proto));

	EXPECT_EQ(0, pb_get_TestMessage(buf, buf + sizeof(test_proto), &obj, &m));
	pb_term_TestMessage(&m);
	check_message(&m);

	free(buf);
}
