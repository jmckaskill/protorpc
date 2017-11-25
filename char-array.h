#pragma once

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

#if !defined __GLIBC__
static inline void *memrchr(const void *s, int c, size_t n) {
	unsigned char *b = (unsigned char*) s;
	unsigned char *e = b + n - 1;
	while (e >= b) {
		if (*e == c) {
			return e;
		}
		e--;
	}
	return NULL;
}
#endif

// P should point to a structure of the form
// struct {
//	int len;
//	char buf[...];
// };

// returns zero on success, non-zero on failure
#define ca_setlen(P, SZ) 		(assert(sizeof((P)->buf) != sizeof(char*)), (P)->len = (SZ), (P)->buf[(P)->len] = '\0')
#define ca_set2(P, SRC, SRCSZ) 	(assert(sizeof((P)->buf) != sizeof(char*)), ca_set2_((P)->buf, sizeof((P)->buf), &(P)->len, (SRC), (SRCSZ)))
#define ca_set(P, SRC) 			(assert(sizeof((P)->buf) != sizeof(char*)), ca_set2_((P)->buf, sizeof((P)->buf), &(P)->len, (SRC), (int) strlen(SRC)))
#define ca_setstr(P, ADD) 		(assert(sizeof((P)->buf) != sizeof(char*)), ca_set2_((P)->buf, sizeof((P)->buf), &(P)->len, (ADD).buf, (ADD).len))
#define ca_add2(P, SRC, SRCSZ) 	(assert(sizeof((P)->buf) != sizeof(char*)), ca_add2_((P)->buf, sizeof((P)->buf), &(P)->len, (SRC), (SRCSZ)))
#define ca_add(P, SRC) 			(assert(sizeof((P)->buf) != sizeof(char*)), ca_add2_((P)->buf, sizeof((P)->buf), &(P)->len, (SRC), (int) strlen(SRC)))
#define ca_addstr(P, ADD) 		(assert(sizeof((P)->buf) != sizeof(char*)), ca_add2_((P)->buf, sizeof((P)->buf), &(P)->len, (ADD).buf, (ADD).len))
#define ca_vaddf(P, FMT, AP) 	(assert(sizeof((P)->buf) != sizeof(char*)), ca_vaddf_((P)->buf, sizeof((P)->buf), &(P)->len, (FMT), (AP)))
#define ca_addf(P, ...) 		(assert(sizeof((P)->buf) != sizeof(char*)), ca_addf_((P)->buf, sizeof((P)->buf), &(P)->len, __VA_ARGS__))

// comparison functions can be used with str_t, slice_t or a char buffer like above

typedef struct {
	int len;
	const char *buf;
} slice_t;

// can be used with fixed sized arrays as well as str_t
#define str_ends_with(P, TEST) 		(strlen(TEST) <= (size_t) (P).len && !memcmp((P).buf + (P).len - strlen(TEST), (TEST), strlen(TEST)))
#define str_iends_with(P, TEST) 	(strlen(TEST) <= (size_t) (P).len && !strncasecmp((P).buf + (P).len - strlen(TEST), (TEST), strlen(TEST)))
#define str_begins_with(P, TEST)	(strlen(TEST) <= (size_t) (P).len && !memcmp((P).buf, (TEST), strlen(TEST)))
#define str_ibegins_with(P, TEST)	(strlen(TEST) <= (size_t) (P).len && !strncasecmp((P).buf, (TEST), strlen(TEST)))

#define str_test(P, TEST) 	(strlen(TEST) == (size_t) (P).len && !memcmp((P).buf, (TEST), (P).len))
#define str_itest(P, TEST)	(strlen(TEST) == (size_t) (P).len && !strncasecmp((P).buf, (TEST), (P).len))

#define str_equals(A, B) ((A).len == (B).len && !memcmp((A).buf, (B).buf, (A).len))

#define str_find_char(P, CH)    ((const char*) memchr((P).buf, (CH), (P).len))
#define str_rfind_char(P, CH) 	((const char*) memrchr((P).buf, (CH), (P).len))

// for use with %.*s in printf style functions
#define STRF(P) (P).len, (P).buf

// the following are the implementation functions

static inline int ca_set2_(char *buf, int bufsz, int *plen, const char *a, int asz) {
	assert(*plen >= 0 && *plen < bufsz);
	if (asz < bufsz) {
		memcpy(buf, a, asz);
		*plen = asz;
		buf[asz] = '\0';
		return 0;
	} else {
		*plen = 0;
		buf[0] = '\0';
		return -1;
	}
}

static inline int ca_add2_(char *buf, int bufsz, int *plen, const char *a, int asz) {
	assert(*plen >= 0 && *plen < bufsz);
	if (*plen + asz < bufsz) {
		memcpy(buf + *plen, a, asz);
		*plen += asz;
		buf[*plen] = '\0';
		return 0;
	} else {
		return -1;
	}
}

static inline int ca_vaddf_(char *buf, int bufsz, int *plen, const char *fmt, va_list ap) {
	assert(*plen >= 0 && *plen < bufsz);
	int left = bufsz - *plen - 1;
	int ret = vsnprintf(buf + *plen, left, fmt, ap);
	if (ret < 0 || ret >= left) {
		return -1;
	}
	*plen += ret;
	buf[*plen] = '\0';
	return 0;
}

static inline int ca_addf_(char *buf, int bufsz, int *plen, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	return ca_vaddf_(buf, bufsz, plen, fmt, ap);
}


// P should point to a structure of the form
// struct {
//	char *end;
//	char buf[...];
// };

// returns zero on success, non-zero on failure
#define ca2_setend(P, END)       (assert(sizeof((P)->buf) != sizeof(char*)), (P)->end = (END), *((P)->end) = '\0')
#define ca2_set2(P, BEGIN, END)  (assert(sizeof((P)->buf) != sizeof(char*)), ca2_set2_((P)->buf, (P)->buf + sizeof((P)->buf), &(P)->end, (BEGIN), (END)))
#define ca2_set(P, SRC)          ca2_set((P), (SRC), (SRC) + strlen(SRC))
#define ca2_setstr(P, ADD)       ca2_set((P), (ADD).buf, (ADD).end)
#define ca2_add2(P, BEGIN, END)  (assert(sizeof((P)->buf) != sizeof(char*)), ca2_add2_((P)->buf, (P)->buf + sizeof((P)->buf), &(P)->end, (BEGIN), (END)))
#define ca2_add(P, SRC)          ca2_add2((P), (SRC).buf, (SRC) + strlen(SRC))
#define ca2_addstr(P, ADD)       ca2_add2((P), (SRC).buf, (SRC).end)
#define ca2_vaddf(P, FMT, AP)    (assert(sizeof((P)->buf) != sizeof(char*)), ca2_vaddf_((P)->buf, (P)->buf + sizeof((P)->buf), &(P)->end, (FMT), (AP)))
#define ca2_addf(P, ...)         (assert(sizeof((P)->buf) != sizeof(char*)), ca2_addf_((P)->buf, (P)->buf + sizeof((P)->buf), &(P)->end, __VA_ARGS__))

// comparison functions can be used with str_t, slice_t or a char buffer like above

typedef struct {
	const char *end;
	const char *buf;
} slice2_t;

// can be used with fixed sized arrays as well as str_t
#define str2_ends_with(P, TEST)     (strlen(TEST) <= ((P).end - (P).buf) && !memcmp((P).end - strlen(TEST), (TEST), strlen(TEST)))
#define str2_iends_with(P, TEST)    (strlen(TEST) <= ((P).end - (P).buf) && !strncasecmp((P).end - strlen(TEST), (TEST), strlen(TEST)))
#define str2_begins_with(P, TEST)   (strlen(TEST) <= ((P).end - (P).buf) && !memcmp((P).buf, (TEST), strlen(TEST)))
#define str2_ibegins_with(P, TEST)  (strlen(TEST) <= ((P).end - (P).buf) && !strncasecmp((P).buf, (TEST), strlen(TEST)))
#define str2_test(P, TEST)          (strlen(TEST) == ((P).end - (P).buf) && !memcmp((P).buf, (TEST), strlen(TEST)))
#define str2_itest(P, TEST)         (strlen(TEST) == ((P).end - (P).buf) && !strncasecmp((P).buf, (TEST), strlen(TEST)))
#define str2_equals(A, B)           (((A).buf) - (A).len) == ((B).end - (B).buf) && !memcmp((A).buf, (B).buf, (A).end - (A).buf))
#define str2_find_char(P, CH)       ((const char*) memchr((P).buf, (CH), (P).end - (P).buf))
#define str2_rfind_char(P, CH)      ((const char*) memrchr((P).buf, (CH), (P).end - (P).buf))

// for use with %.*s in printf style functions
#define STR2F(P) ((P).end - (P).buf), (P).buf

// the following are the implementation functions

static inline int ca2_set2_(char *buf, char *bufend, char **pend, const char *b, const char *e) {
	assert(buf <= *pend && *pend < bufend);
	if (e - b < bufend - buf) {
		memcpy(buf, b, e - b);
		*pend = buf + (e - b);
		**pend = '\0';
		return 0;
	} else {
		*pend = buf;
		**pend = '\0';
		return -1;
	}
}

static inline int ca2_add2_(char *buf, char *bufend, char **pend, const char *b, const char *e) {
	assert(buf <= *pend && *pend < bufend);
	if (*pend + (e - b) < bufend) {
		memcpy(*pend, b, e - b);
		*pend += e - b;
		**pend = '\0';
		return 0;
	} else {
		return -1;
	}
}

static inline int ca2_vaddf_(char *buf, char *bufend, char **pend, const char *fmt, va_list ap) {
	assert(buf <= *pend && *pend < bufend);
	int left = bufend - *pend - 1;
	int ret = vsnprintf(*pend, left, fmt, ap);
	if (ret < 0 || ret >= left) {
		return -1;
	}
	*pend += ret;
	**pend = '\0';
	return 0;
}

static inline int ca2_addf_(char *buf, char *bufend, char **pend, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	return ca2_vaddf_(buf, bufend, pend, fmt, ap);
}

