#include "protorpc/test.h"
#include "protorpc/flag.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>

#ifdef _WIN32
#include <windows.h>
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

void assert_true(bool a, const char *astr, const char *file, int line) {
	if (!a) {
		fprintf(stderr, "%s(%d): assertion failed\n\t%s == false\n", file, line, astr);
		exit(-1);
	}
}

void expect_int_eq(int64_t a, int64_t b, const char *astr, const char *bstr, const char *file, int line) {
	if (a != b) {
		error_count++;
		fprintf(stderr, "%s(%d): expected int ==\n\tLeft:  %"PRId64" for %s\n\tRight: %"PRId64" for %s\n",
			file, line, a, astr, b, bstr);
	}
}

void expect_str_eq(const char *a, const char *b, const char *astr, const char *bstr, const char *file, int line) {
	if (strcmp(a, b)) {
		error_count++;
		fprintf(stderr, "%s(%d): expected string ==\n\tLeft:  \"%s\" for %s\n\tRight: \"%s\" for %s\n",
			file, line, a, astr, b, bstr);
	}
}

static int difference_index(uint8_t *a, uint8_t *b, int sz) {
	for (int i = 0; i < sz; i++) {
		if (a[i] != b[i]) {
			return i;
		}
	}
	return 0;
}

void expect_bytes_eq(const void *a, int alen, const void *b, int blen, const char *astr, const char *bstr, const char *file, int line) {
	if (alen != blen) {
		error_count++;
		fprintf(stderr, "%s(%d): expected bytes ==, have different lengths\n\tLeft:  %d long for %s\n\tRight: %d long for %s\n",
			file, line, alen, astr, blen, bstr);
	} else if (memcmp(a, b, alen)) {
		uint8_t *ua = (uint8_t*)a;
		uint8_t *ub = (uint8_t*)b;
		int idx = difference_index(ua, ub, alen);
		fprintf(stderr, "%s(%d): expected bytes ==, different value in byte %d\n\tCommon:\n", file, line, idx);

		for (int i = 0; i < idx; i++) {
			fprintf(stderr, "%02X", ua[i]);
			if (i && (i % 2) == 0) {
				putc(' ', stderr);
			}
			if (i && (i & 8) == 0) {
				putc(' ', stderr);
			}
			if (i && (i & 16) == 0) {
				putc('\n', stderr);
			}
		}
		fprintf(stderr, "\n\tLeft[%d]:  %02X for %s\n", idx, ua[idx], astr);
		fprintf(stderr, "\n\tRight[%d]: %02X for %s\n", idx, ub[idx], bstr);
		error_count++;
	}
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

void expect_float_eq(double a, double b, const char *astr, const char *bstr, const char *file, int line) {
	int typea = fpclassify(a);
	int typeb = fpclassify(b);
	if (fpclassify(a) != fpclassify(b)) {
		error_count++;
		fprintf(stderr, "%s(%d): expected float ==, different types\n\tLeft:  %s for %s\n\tRight: %s for %s\n",
			file, line, float_type(typea), astr, float_type(typeb), bstr);
	} else if ((typea == FP_INFINITE && (a > 0) != (b > 0))
		|| ((typea == FP_NORMAL || typea == FP_SUBNORMAL) && fabs(a/b - 1) > 0.00001)) {
		error_count++;
		fprintf(stderr, "%s(%d): expected float ==\n\tLeft:  %g for %s\n\tRight: %g for %s\n",
			file, line, a, astr, b, bstr);
	}
}

void expect_ptr_eq(const void *a, const void *b, const char *astr, const char *bstr, const char *file, int line) {
	if (a != b) {
		error_count++;
		fprintf(stderr, "%s(%d): expected ptr ==\n\tLeft:  %p for %s\n\tRight: %p for %s\n",
			file, line, a, astr, b, bstr);
	}
}
