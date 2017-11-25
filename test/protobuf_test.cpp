#include <gtest/gtest.h>
#include "test.proto.h"


TEST(protobuf, print) {
	bool rb[3] = { false, true, false };
	uint32_t ru32[3] = { 1,2,3 };
	uint64_t ru64[3] = { 3,4,5 };
	int32_t ri32[3] = { -1,0,1 };
	int64_t ri64[3] = { -2,0,2 };
	int32_t rs32[3] = { -10,0,10 };
	int64_t rs64[3] = { -20,0,20 };
	uint32_t rf32[3] = { 10,20,30 };
	uint64_t rf64[3] = { 30,40,50 };
	int32_t rsf32[3] = { -10,20,0 };
	int64_t rsf64[3] = { -100,0,100 };
	float rf[1] = { 3.5 };
	double rd[3] = { 1.1,2.2,3.3 };
	pb_bytes_t rby[2] = { {5,(uint8_t*)"defgh"}, {5, (uint8_t*)"abcde"} };
	pb_string_t rstr[2] = { {5, "ghikj"}, {5, "lmnop"} };
	enum TestEnum ren[3] = { ENUM_C, ENUM_B, ENUM_A };

	struct TestMessage msg2 = {};
	msg2.b = true;

	struct TestMessage msg3 = {};
	msg3.u64 = 10234;

	const static TestMessage *rmsg[1] = { &msg3 };

	struct TestPod pod = {};
	pod.foo_type = TESTPOD_U;
	pod.foo.u = 34;

	struct TestPod rpod[2] = {};
	rpod[0].foo_type = TESTPOD_U;
	rpod[0].foo.u = 1;
	rpod[1].foo_type = TESTPOD_I;
	rpod[1].foo.i = -1;

	struct TestMessage msg = {};
	msg.b = true;
	msg.u32 = 1;
	msg.u64 = 10234;
	msg.i32 = -23;
	msg.i64 = -34;
	msg.s32 = -1234;
	msg.s64 = -23585;
	msg.f32 = 34757;
	msg.f64 = 575859;
	msg.sf32 = -34757;
	msg.sf64 = -575859;
	msg.f = 314;
	msg.d = 3.141;
	msg.by.len = 5;
	msg.by.p = (uint8_t*) "abcde";
	msg.str.len = 5;
	msg.str.c_str = "abcde";
	msg.en = ENUM_C;
	msg.rb.len = 3;
	msg.rb.v = rb;
	msg.ru32.len = 3;
	msg.ru32.v = ru32;
	msg.ru64.len = 3;
	msg.ru64.v = ru64;
	msg.ri32.len = 3;
	msg.ri32.v = ri32;
	msg.ri64.len = 3;
	msg.ri64.v = ri64;
	msg.rs32.len = 3;
	msg.rs32.v = rs32;
	msg.rs64.len = 3;
	msg.rs64.v = rs64;
	msg.rf32.len = 3;
	msg.rf32.v = rf32;
	msg.rf64.len = 3;
	msg.rf64.v = rf64;
	msg.rsf32.len = 3;
	msg.rsf32.v = rsf32;
	msg.rsf64.len = 3;
	msg.rsf64.v = rsf64;
	msg.rf.len = 1;
	msg.rf.v = rf;
	msg.rd.len = 3;
	msg.rd.v = rd;
	msg.rby.len = 2;
	msg.rby.v = rby;
	msg.rstr.len = 2;
	msg.rstr.v = rstr;
	msg.ren.len = 3;
	msg.ren.v = ren;
	msg.msg = &msg2;
	msg.pod = pod;
	msg.rmsg.len = 1;
	msg.rmsg.v = rmsg;
	msg.rpod.len = 2;
	msg.rpod.v = rpod;

	uint8_t buf[4096], pbuf[4096];
	pb_buf_t pr = { buf, buf + sizeof(buf) };
	pb_buf_t pp = { pbuf, pbuf + sizeof(pbuf) };

	EXPECT_EQ(0, pb_print_TestMessage(&pr, &msg));
	EXPECT_EQ(0, pb_pretty_print(&pp, (char*) buf, pr.next - buf));
	ASSERT_EQ(0, pb_append(&pr, "\0", 1));
	ASSERT_EQ(0, pb_append(&pp, "\0", 1));

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
		
	EXPECT_STREQ(test_json, (char*) buf);

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
	
	EXPECT_STREQ(test_pretty, (char*) pbuf);
}

