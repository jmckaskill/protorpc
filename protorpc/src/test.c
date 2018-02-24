#include "protorpc/test.h"
#include "protorpc/flag.h"
#include "protorpc/str.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>

#ifdef _WIN32
#include <windows.h>
#include <crtdbg.h>
#else
#include <unistd.h>
#endif

static const char *output_fn;
static int error_count;

void start_test(int *argc, char *argv[]) {
	flag_string(&output_fn, 'o', "output", "test status output file");
	flag_parse(argc, argv, "test.exe [arguments]", 0);
	if (output_fn) {
#ifdef _WIN32
		DeleteFileA(output_fn);
#else
		unlink(output_fn);
#endif
	}
}

int finish_test() {
	// if we got this far then the test has passed
	// really just touch the file to indicate that we've passed
	if (!error_count) {
		if (output_fn) {
			FILE *f = fopen(output_fn, "wb");
			if (f) {
				fclose(f);
			}
		} else {
			fprintf(stderr, "test passed\n");
		}
	}
	return error_count;
}

static int do_test_failed(const char *file, int line, const char *msg) {
	error_count++;
#ifdef _MSC_VER
	fprintf(stderr, "%s(%d): error : %s", file, line, msg);
#ifndef NDEBUG
	if (IsDebuggerPresent()) {
		return _CrtDbgReport(_CRT_ASSERT, file, line, NULL, "%s", msg);
	}
#endif
#else
	fprintf(stderr, "%s:%d: %s", file, line, msg);
#endif
	return 0;
}

test_failure_fn test_failed = &do_test_failed;

int assert_true(bool a, const char *astr, const char *file, int line) {
	int ret = 0;
	if (!a) {
		str_t s = STR_INIT;
		str_addf(&s, "assertion failed\n\t%s == false\n", astr);
		ret = test_failed(file, line, s.c_str);
		if (!ret) {
			exit(-1);
		}
	}
	return ret;
}

int expect_int_eq(int64_t a, int64_t b, const char *astr, const char *bstr, const char *file, int line) {
	int ret = 0;
	if (a != b) {
		str_t s = STR_INIT;
		str_addf(&s, "expected int ==\n\tLeft:  %"PRId64" for %s\n\tRight: %"PRId64" for %s\n",
			a, astr, b, bstr);
		ret = test_failed(file, line, s.c_str);
		str_destroy(&s);
	}
	return ret;
}

int expect_str_eq(const char *a, const char *b, const char *astr, const char *bstr, const char *file, int line) {
	int ret = 0;
	if (strcmp(a, b)) {
		str_t s = STR_INIT;
		str_addf(&s, "expected string ==\n\tLeft:  \"%s\" for %s\n\tRight: \"%s\" for %s\n",
			a, astr, b, bstr);
		ret = test_failed(file, line, s.c_str);
		str_destroy(&s);
	}
	return ret;
}

static int difference_index(uint8_t *a, uint8_t *b, int sz) {
	for (int i = 0; i < sz; i++) {
		if (a[i] != b[i]) {
			return i;
		}
	}
	return 0;
}

int expect_bytes_eq(const void *a, int alen, const void *b, int blen, const char *astr, const char *bstr, const char *file, int line) {
	int ret = 0;
	if (alen != blen) {
		str_t s = STR_INIT;
		str_addf(&s, "expected bytes ==, have different lengths\n\tLeft:  %d long for %s\n\tRight: %d long for %s\n",
			alen, astr, blen, bstr);
		ret = test_failed(file, line, s.c_str);
		str_destroy(&s);
	} else if (memcmp(a, b, alen)) {
		uint8_t *ua = (uint8_t*)a;
		uint8_t *ub = (uint8_t*)b;
		int idx = difference_index(ua, ub, alen);
		str_t s = STR_INIT;
		str_addf(&s, "expected bytes ==, different value in byte %d\n\tCommon:\n", idx);

		for (int i = 0; i < idx; i++) {
			str_addf(&s, "%02X", ua[i]);
			if (i && (i % 2) == 0) {
				str_addch(&s, ' ');
			}
			if (i && (i & 8) == 0) {
				str_addch(&s, ' ');
			}
			if (i && (i & 16) == 0) {
				str_addch(&s, '\n');
			}
		}
		str_addf(&s, "\n\tLeft[%d]:  %02X for %s\n", idx, ua[idx], astr);
		str_addf(&s, "\n\tRight[%d]: %02X for %s\n", idx, ub[idx], bstr);
		ret = test_failed(file, line, s.c_str);
		str_destroy(&s);
	}
	return ret;
}

static const char *float_type(int type) {
	switch (type) {
	case FP_ZERO:
		return "zero";
	case FP_NORMAL:
		return "normal";
	case FP_SUBNORMAL:
		return "subnormal";
	case FP_INFINITE:
		return "infinite";
	case FP_NAN:
		return "nan";
	default:
		return "unknown";
	}
}

int expect_float_eq(double a, double b, const char *astr, const char *bstr, const char *file, int line) {
	int typea = fpclassify(a);
	int typeb = fpclassify(b);
	int ret = 0;
	if (fpclassify(a) != fpclassify(b)) {
		str_t s = STR_INIT;
		str_addf(&s, "expected float ==, different types\n\tLeft:  %s for %s\n\tRight: %s for %s\n",
			float_type(typea), astr, float_type(typeb), bstr);
		ret = test_failed(file, line, s.c_str);
		str_destroy(&s);
	} else if ((typea == FP_INFINITE && (a > 0) != (b > 0))
		|| ((typea == FP_NORMAL || typea == FP_SUBNORMAL) && fabs(a/b - 1) > 0.00001)) {
		str_t s = STR_INIT;
		str_addf(&s, "expected float ==\n\tLeft:  %g for %s\n\tRight: %g for %s\n",
			a, astr, b, bstr);
		ret = test_failed(file, line, s.c_str);
		str_destroy(&s);
	}
	return ret;
}

int expect_ptr_eq(const void *a, const void *b, const char *astr, const char *bstr, const char *file, int line) {
	int ret = 0;
	if (a != b) {
		str_t s = STR_INIT;
		str_addf(&s, "expected ptr ==\n\tLeft:  %p for %s\n\tRight: %p for %s\n",
			a, astr, b, bstr);
		ret = test_failed(file, line, s.c_str);
		str_destroy(&s);
	}
	return ret;
}
