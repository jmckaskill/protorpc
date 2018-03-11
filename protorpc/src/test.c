#include "protorpc/test.h"
#include "protorpc/flag.h"
#include "protorpc/str.h"
#include "protorpc/log.h"
#include "protorpc/protorpc.h"
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
#include <time.h>
#endif

static const char *output_fn;
static str_t slog = STR_INIT;
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
	LARGE_INTEGER end_time, freq;
	QueryPerformanceCounter(&end_time);
	QueryPerformanceFrequency(&freq);
	uint64_t span = end_time.QuadPart - start_time.QuadPart;
	float sec = (float)span / (float)freq.QuadPart;
	return sec * 1000.0f;
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

static int do_log(log_t *l, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	int sz = slog.len;

	const char *bar = strchr(fmt, '|');

	if (bar) {
		str_addf(&slog, "{\n\t\"msg\": \"%.*s\",", (int)(bar - fmt), fmt);
		str_grow(&slog, slog.len + 256);
		int ret = pb_vprint(slog.c_str + slog.len, slog.cap - slog.len, bar+1, ap, 1);
		if (ret > 0) {
			str_setlen(&slog, slog.len + ret);
		}
		if (str_ends_with(slog, ",")) {
			slog.len--;
		}
		str_add(&slog, "\n}\n");
	} else {
		str_addf(&slog, "{\n\t\"msg\": \"%s\"\n}\n", fmt);
	}

#ifdef _WIN32
	if (IsDebuggerPresent()) {
		OutputDebugStringA(slog.c_str + sz);
	}
#endif

	if (error_count) {
		fwrite(slog.c_str + sz, 1, slog.len - sz, stderr);
	}

	return slog.len - sz;
}

static log_t test_logger = {
	&do_log,
};

#ifdef _WIN32
static DWORD WINAPI timeout_thread(void *udata) {
	Sleep(1000);
	OutputDebugStringA("test timed out\n");
	fprintf(stderr, "test timed out\n");
	ExitProcess(3);
	return 0;
}
#endif

log_t *start_test(int *argc, char *argv[]) {
	test_name = argv[0];
	flag_string(&output_fn, 'o', "output", "FILE", "test status output file");
	flag_parse(argc, argv, "[options]", 0);
#ifdef _WIN32
	if (output_fn) {
		DeleteFileA(output_fn);
		HANDLE h = CreateThread(NULL, 0, &timeout_thread, NULL, 0, NULL);
		CloseHandle(h);
	}
#else
	if (output_fn) {
		unlink(output_fn);
		alarm(1);
	}
#endif
	record_start_time();
	str_clear(&slog);
	return &test_logger;
}

int finish_test() {
	float span = calc_time_span_ms();
	LOG(&test_logger, "test finished|test:%s|timeMillis:%g", test_name, span);
	if (output_fn) {
		FILE *f = fopen(output_fn, "wb");
		if (f) {
			fwrite(slog.c_str, 1, slog.len, f);
			fclose(f);
		}
	}
	if (!output_fn && !error_count) {
		fwrite(slog.c_str, 1, slog.len, stderr);
	}
	return error_count;
}

static int do_test_failed(const char *file, int line, const char *msg) {
	if (!error_count) {
		fwrite(slog.c_str, 1, slog.len, stderr);
	}
	error_count++;
	fprintf(stderr, "%s(%d,1): error: %s", file, line, msg);
#if defined _MSC_VER && !defined NDEBUG
	if (IsDebuggerPresent()) {
		return _CrtDbgReport(_CRT_ASSERT, file, line, NULL, "%s", msg);
	}
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

static void print_int_argument(str_t *s, int64_t a, const char *astr) {
	char *end;
	if (strtoll(astr, &end, 0) != a || *end) {
		str_addf(s, " (%s)", astr);
	}
}

static void print_float_argument(str_t *s, double a, const char *astr) {
	char *end;
	if (strtod(astr, &end) != a || *end) {
		str_addf(s, " (%s)", astr);
	}
}

int expect_int_gt(int64_t a, int64_t b, const char *astr, const char *bstr, const char *file, int line) {
	int ret = 0;
	if (!(a > b)) {
		str_t s = STR_INIT;
		str_addf(&s, "expected %"PRId64, a);
		print_int_argument(&s, a, astr);
		str_addf(&s, " > %"PRId64, b);
		print_int_argument(&s, b, bstr);
		str_addch(&s, '\n');
		ret = test_failed(file, line, s.c_str);
		str_destroy(&s);
	}
	return ret;
}

int expect_int_ge(int64_t a, int64_t b, const char *astr, const char *bstr, const char *file, int line) {
	int ret = 0;
	if (!(a >= b)) {
		str_t s = STR_INIT;
		str_addf(&s, "expected %"PRId64, a);
		print_int_argument(&s, a, astr);
		str_addf(&s, " >= %"PRId64, b);
		print_int_argument(&s, b, bstr);
		str_addch(&s, '\n');
		ret = test_failed(file, line, s.c_str);
		str_destroy(&s);
	}
	return ret;
}

int expect_int_eq(int64_t a, int64_t b, const char *astr, const char *bstr, const char *file, int line) {
	int ret = 0;
	if (a != b) {
		str_t s = STR_INIT;
		str_addf(&s, "value of %s\n\t  Actual: %"PRId64"\n\tExpected: %"PRId64, bstr, b, a);
		print_int_argument(&s, a, astr);
		str_addch(&s, '\n');
		ret = test_failed(file, line, s.c_str);
		str_destroy(&s);
	}
	return ret;
}

int expect_str_eq(const char *a, const char *b, const char *astr, const char *bstr, const char *file, int line) {
	int ret = 0;
	if (strcmp(a, b)) {
		str_t s = STR_INIT;
		str_addf(&s, "value of %s\n\t  Actual: \"%s\"\n\tExpected: \"%s\" (%s)\n", bstr, b, a, astr);
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

int expect_bytes_eq(const void *a, int alen, const void *b, int blen, const char *astr, const char *bstr, const char *file, int line) {
	int ret = 0;
	if (alen != blen || memcmp(a, b, alen)) {
		str_t s = STR_INIT;
		str_addf(&s, "value of %s\n\t  Actual: ", bstr);
		print_test_data(&s, b, blen);
		str_add(&s, "\n\tExpected: ");
		print_test_data(&s, a, alen);
		str_addf(&s, " (%s)\n", astr);
		ret = test_failed(file, line, s.c_str);
		str_destroy(&s);
	}
	return ret;
}

int expect_ptr_eq(const void *a, const void *b, const char *astr, const char *bstr, const char *file, int line) {
	int ret = 0;
	if (a != b) {
		str_t s = STR_INIT;
		str_addf(&s, "value of %s\n\t  Actual: %p\n\tExpected: %p (%s)\n", bstr, b, a, astr);
		ret = test_failed(file, line, s.c_str);
		str_destroy(&s);
	}
	return ret;
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
		str_addf(&s, "value of %s\n\t  Actual: %g\n\tExpected: %g", bstr, b, a);
		print_float_argument(&s, a, astr);
		str_addch(&s, '\n');
		ret = test_failed(file, line, s.c_str);
		str_destroy(&s);
	}
	return ret;
}

int expect_near(double a, double b, double delta, const char *astr, const char *bstr, const char *file, int line) {
	int ret = 0;
	if (fabs(a-b) > delta) {
		str_t s = STR_INIT;
		str_addf(&s, "value of %s\n\t  Actual: %g\n\tExpected: %g", bstr, b, a);
		print_float_argument(&s, a, astr);
		str_addf(&s, "\n\t  Within: %g\n", delta);
		ret = test_failed(file, line, s.c_str);
		str_destroy(&s);
	}
	return ret;
}
