#include <protorpc/flag.h>
#include <gtest/gtest.h>

static char *g_lastmsg;

static int my_exit(int code, char *msg) {
	free(g_lastmsg);
	g_lastmsg = msg;
	return code;
}

TEST(protorpc, flag) {
	bool b = false;
	int i = 34;
	const char *str = "default";

	flag_exit = &my_exit;

	// test the help
	flag_bool(&b, 'b', "bool", "bool usage");
	flag_int(&i, 0, "int", "int usage");
	flag_string(&str, 's', NULL, "string usage");

	char *args1[] = { strdup("foo"), strdup("-h"), NULL };
	int argc1 = 2;
	EXPECT_EQ(1, flag_parse(&argc1, args1, "foo [arguments]", 0));
	EXPECT_STREQ(g_lastmsg,
		"Usage: foo [arguments]\n"
		"\nMandatory arguments to long options are mandatory for short options too.\n"
		"  -b, --bool                bool usage\n"
		"      --int=34              int usage\n"
		"  -s default                string usage\n");

	// test the parsing
	flag_bool(&b, 'b', "bool", "bool usage");
	flag_int(&i, 0, "int", "int usage");
	flag_string(&str, 's', NULL, "string usage");

	char *args2[] = { strdup("foo"), strdup("-b"), strdup("--int=3"), strdup("argument"), strdup("-s"), strdup("foobar"), NULL };
	int argc2 = 6;
	EXPECT_EQ(0, flag_parse(&argc2, args2, "foo [arguments]", 0));
	EXPECT_EQ(1, argc2);
	EXPECT_STREQ("argument", args2[0]);
	EXPECT_EQ(true, b);
	EXPECT_EQ(i, 3);
	EXPECT_STREQ(str, "foobar");
}