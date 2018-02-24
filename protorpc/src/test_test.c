#include "protorpc/test.h"
#include "protorpc/flag.h"
#include "protorpc/str.h"
#include <math.h>
#include <stdlib.h>

static str_t last_msg = STR_INIT;

static int failure_cb(const char *file, int line, const char *msg) {
	str_set(&last_msg, msg);
	return 0;
}

static void check_msg(const char *fmt, ...) {
	char buf[256];
	va_list ap;
	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);
	if (strcmp(last_msg.c_str, buf)) {
		fprintf(stderr, "expected %s\ngot %s\n", buf, last_msg.c_str);
		exit(2);
	}
	str_clear(&last_msg);
}

int main(int argc, char *argv[]) {
	const char *output_fn;
	flag_string(&output_fn, 'o', "output", "output filename for success");
	flag_parse(&argc, argv, "[arguments]", 0);

	test_failed = &failure_cb;

	EXPECT_EQ(0, 0);
	check_msg("");
	EXPECT_EQ(0, 1);
	check_msg("expected int ==\n\tLeft:  0 for 0\n\tRight: 1 for 1\n");

	EXPECT_FLOAT_EQ(0.2, 1.0);
	check_msg("expected float ==\n\tLeft:  0.2 for 0.2\n\tRight: 1 for 1.0\n");
	EXPECT_FLOAT_EQ(NAN, 0);
	check_msg("expected float ==, different types\n\tLeft:  nan for NAN\n\tRight: zero for 0\n");
	EXPECT_FLOAT_EQ(NAN, NAN);
	check_msg("");
	EXPECT_FLOAT_EQ(INFINITY, INFINITY);
	check_msg("");
	EXPECT_FLOAT_EQ(INFINITY, -INFINITY);
	check_msg("expected float ==\n\tLeft:  %g for INFINITY\n\tRight: %g for -INFINITY\n", INFINITY, -INFINITY);

	char a, b;
	EXPECT_PTREQ(&a, &a);
	check_msg("");
	EXPECT_PTREQ(&a, &b);
	check_msg("expected ptr ==\n\tLeft:  %p for &a\n\tRight: %p for &b\n", &a, &b);

	EXPECT_STREQ("a", "a");
	check_msg("");
	EXPECT_STREQ("a", "ab");
	check_msg("expected string ==\n\tLeft:  \"a\" for \"a\"\n\tRight: \"ab\" for \"ab\"\n");

	EXPECT_GT(1, 0);
	check_msg("");
	EXPECT_GT(0, 0);
	check_msg("expected int >\n\tLeft:  0 for 0\n\tRight: 0 for 0\n");
	EXPECT_GT(-1, 0);
	check_msg("expected int >\n\tLeft:  -1 for -1\n\tRight: 0 for 0\n");

	EXPECT_GE(1, 0);
	check_msg("");
	EXPECT_GE(0, 0);
	check_msg("");
	EXPECT_GE(-1, 0);
	check_msg("expected int >=\n\tLeft:  -1 for -1\n\tRight: 0 for 0\n");

	char ab[] = "ab";
	char bc[] = "bc";
	char abd[] = "abd";
	EXPECT_BYTES_EQ(ab, 2, abd, 2);
	check_msg("");
	EXPECT_BYTES_EQ(ab, 2, bc, 2);
	check_msg("expected bytes ==, different value in byte 0\n\tLeft[0]:  x61 for ab\n\tRight[0]: x62 for bc\n");
	EXPECT_BYTES_EQ(bc, 2, abd+1, 2);
	check_msg("expected bytes ==, different value in byte 1\n\tCommon: x62\n\tLeft[1]:  x63 for bc\n\tRight[1]: x64 for abd+1\n");
	EXPECT_BYTES_EQ(ab, 2, abd, 3);
	check_msg("expected bytes ==, have different lengths\n\tLeft:  2 long for ab\n\tRight: 3 long for abd\n");

	if (output_fn) {
		FILE *f = fopen(output_fn, "wb");
		if (f) {
			fclose(f);
		}
	}
	return 0;
}

