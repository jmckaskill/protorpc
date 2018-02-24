#include "protorpc/test.h"
#include "protorpc/flag.h"
#include "protorpc/str.h"
#include "protorpc/log.h"
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
static str_t log_text = STR_INIT;
static int error_count;
static const char *test_name;

#if defined _WIN32
static LARGE_INTEGER start_time;
#elif defined __MACH__
#include <mach/mach_time.h>
static uint64_t start_time;
#else
#include <sys/time.h>
static struct timespec start_time;
#endif

static void record_start_time() {
#ifdef _WIN32
	QueryPerformanceCounter(&start_time);
#elif defined __MACH__
	start_time = mach_absolute_time();
#else
	clock_gettime(CLOCK_MONOTONIC, &start_time);
#endif
}

static float calc_time_span_ms() {
#ifdef _WIN32
#elif defined __MACH__
	uint64_t end_time = mach_absolute_time();
	uint64_t span = end_time - start_time;
	mach_timebase_info_data_t base;
	mach_timebase_info(&base);
	float ns = ((float)span * base.numer) / base.denom;
	return ns / 1000000.0f;
#else
	struct timespec end_time;
	clock_gettime(CLOCK_MONOTONIC, &end_time);
	end_time.tv_sec -= start_time.tv_sec;
	end_time.tv_nsec -= start_time.tv_nsec;
	return (float) end_time.tv_sec + (end_time.tv_nsec / 1000000.0f);
#endif
}

static int do_log(log_t *log, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	int sz = log_text.len;
	int ret = str_vaddf(&log_text, fmt, ap);
	if (!str_ends_with(log_text, "\n")) {
		str_addch(&log_text, '\n');
	}
#ifdef _WIN32
	if (IsDebuggerPresent()) {
		OutputDebugStringA(log_test.c_str + sz);
	}
#endif
	if (error_count) {
		fwrite(log_text.c_str + sz, 1, log_text.len - sz, stderr);
	}
	return ret;
}

static log_t test_logger = {
	&do_log,
}; 

log_t *start_test(int *argc, char *argv[]) {
	test_name = argv[0];
	flag_string(&output_fn, 'o', "output", "test status output file");
	flag_parse(argc, argv, "test.exe [arguments]", 0);
	if (output_fn) {
#ifdef _WIN32
		DeleteFileA(output_fn);
#else
		unlink(output_fn);
#endif
	}
	record_start_time();
	str_clear(&log_text);
	return &test_logger;
}

int finish_test() {
	float span = calc_time_span_ms();
	LOG(&test_logger, "finished %s in %f ms\n", test_name, span);
	if (output_fn) {
		FILE *f = fopen(output_fn, "wb");
		if (f) {
			fwrite(log_text.c_str, 1, log_text.len, f);
			fclose(f);
		}
	}
	if (!output_fn && !error_count) {
		fwrite(log_text.c_str, 1, log_text.len, stderr);
	}
	return error_count;
}

static int do_test_failed(const char *file, int line, const char *msg) {
	if (!error_count) {
		fwrite(log_text.c_str, 1, log_text.len, stderr);
	}
	error_count++;
#ifdef _MSC_VER
	fprintf(stderr, "%s(%d): error : %s", file, line, msg);
#ifndef NDEBUG
	if (IsDebuggerPresent()) {
		return _CrtDbgReport(_CRT_ASSERT, file, line, NULL, "%s", msg);
	}
#endif
#else
	fprintf(stderr, "%s:%d:1: error: %s", file, line, msg);
#endif
	return 0;
}

test_failure_fn test_failed = &do_test_failed;

int assert_true(int a, const char *astr, const char *file, int line) {
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

int expect_int_gt(int64_t a, int64_t b, const char *astr, const char *bstr, const char *file, int line) {
	int ret = 0;
	if (a <= b) {
		str_t s = STR_INIT;
		str_addf(&s, "expected int >\n\tLeft:  %"PRId64" for %s\n\tRight: %"PRId64" for %s\n",
			a, astr, b, bstr);
		ret = test_failed(file, line, s.c_str);
		str_destroy(&s);
	}
	return ret;
}

int expect_int_ge(int64_t a, int64_t b, const char *astr, const char *bstr, const char *file, int line) {
	int ret = 0;
	if (a < b) {
		str_t s = STR_INIT;
		str_addf(&s, "expected int >=\n\tLeft:  %"PRId64" for %s\n\tRight: %"PRId64" for %s\n",
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

void print_test_data(str_t *s, const uint8_t *p, int sz) {
	str_addch(s, '[');
	for (int i = 0; i < sz; i++) {
		if (i) {
			str_addch(s, ',');
		}
		str_addf(s, "x%02X", p[i]);
	}
	str_addch(s, ']');
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
		str_addf(&s, "expected bytes ==, different value in byte %d\n", idx);

		if (idx) {
			str_add(&s, "\tCommon: x");
			for (int i = 0; i < idx; i++) {
				str_addf(&s, "%02X", ua[i]);
				if (i && (i % 2) == 0) {
					str_addch(&s, ' ');
				}
				if (i && (i % 8) == 0) {
					str_addch(&s, ' ');
				}
				if (i && (i % 16) == 0) {
					str_addch(&s, '\n');
				}
			}
			if ((idx % 16) != 0) {
				str_addch(&s, '\n');
			}
		}
		str_addf(&s, "\tLeft[%d]:  x%02X for %s\n", idx, ua[idx], astr);
		str_addf(&s, "\tRight[%d]: x%02X for %s\n", idx, ub[idx], bstr);
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

int is_equiv_float(double a, double b) {
	if (isfinite(a) && isfinite(b)) {
		return fabs(a-b) < 0.000001;
	} else if (isinf(a) && isinf(b)) {
		return (a > 0) == (b > 0);
	} else {
		return fpclassify(a) == fpclassify(b);
	}
}

int expect_float_eq(double a, double b, const char *astr, const char *bstr, const char *file, int line) {
	int ret = 0;
	if (!is_equiv_float(a, b)) {
		str_t s = STR_INIT;
		str_addf(&s, "expected float ==\n\tLeft:  %g for %s\n\tRight: %g for %s\n",
			a, astr, b, bstr);
		ret = test_failed(file, line, s.c_str);
		str_destroy(&s);
	}
	return ret;
}

int expect_near(double a, double b, double delta, const char *astr, const char *bstr, const char *file, int line) {
	int ret = 0;
	if (fabs(a-b) > delta) {
		str_t s = STR_INIT;
		str_addf(&s, "expected float within %g\n\tLeft:  %g for %s\n\tRight: %g for %s\n",
			delta, a, astr, b, bstr);
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
