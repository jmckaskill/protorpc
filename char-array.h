#pragma once

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

// P should point to a structure of the form
// struct {
//	int len;
//	char buf[...];
// };

// returns zero on success, non-zero on failure
#define ca_setlen(P, SZ) 		(assert(sizeof((P)->buf) != sizeof(char*)), (P)->len = (SZ), (P)->buf[(P)->len] = '\0')
#define ca_set2(P, SRC, SRCSZ) 	(assert(sizeof((P)->buf) != sizeof(char*)), ca_set2_((P)->buf, sizeof((P)->buf), &(P)->len, (SRC), (SRCSZ)))
#define ca_set(P, SRC) 			(assert(sizeof((P)->buf) != sizeof(char*)), ca_set2_((P)->buf, sizeof((P)->buf), &(P)->len, (SRC), (int) strlen(SRC)))
#define ca_setstr(P, PADD) 		(assert(sizeof((P)->buf) != sizeof(char*)), ca_set2_((P)->buf, sizeof((P)->buf), &(P)->len, (PADD)->buf, (PADD)->len))
#define ca_add2(P, SRC, SRCSZ) 	(assert(sizeof((P)->buf) != sizeof(char*)), ca_add2_((P)->buf, sizeof((P)->buf), &(P)->len, (SRC), (SRCSZ)))
#define ca_add(P, SRC) 			(assert(sizeof((P)->buf) != sizeof(char*)), ca_add2_((P)->buf, sizeof((P)->buf), &(P)->len, (SRC), (int) strlen(SRC)))
#define ca_addstr(P, PADD) 		(assert(sizeof((P)->buf) != sizeof(char*)), ca_add2_((P)->buf, sizeof((P)->buf), &(P)->len, (PADD)->buf, (PADD)->len))
#define ca_vaddf(P, FMT, AP) 	(assert(sizeof((P)->buf) != sizeof(char*)), ca_vaddf_((P)->buf, sizeof((P)->buf), &(P)->len, (FMT), (AP)))
#define ca_addf(P, ...) 		(assert(sizeof((P)->buf) != sizeof(char*)), ca_addf_((P)->buf, sizeof((P)->buf), &(P)->len, __VA_ARGS__))

// comparison functions can be used with str_t, slice_t or a char buffer like above

typedef struct {
	int len;
	char *buf;
} slice_t;

// can be used with fixed sized arrays as well as str_t
#define str_ends_with(P, TEST) 		(strlen(TEST) <= (size_t) (P)->len && !memcmp((P)->buf + (P)->len - strlen(TEST), (TEST), strlen(TEST)))
#define str_iends_with(P, TEST) 	(strlen(TEST) <= (size_t) (P)->len && !strncasecmp((P)->buf + (P)->len - strlen(TEST), (TEST), strlen(TEST)))
#define str_begins_with(P, TEST)	(strlen(TEST) <= (size_t) (P)->len && !memcmp((P)->buf, (TEST), strlen(TEST)))
#define str_ibegins_with(P, TEST)	(strlen(TEST) <= (size_t) (P)->len && !strncasecmp((P)->buf, (TEST), strlen(TEST)))

#define str_equals(P, TEST) 	(strlen(TEST) == (size_t) (P)->len && !memcmp((P)->buf, (TEST), (P)->len))
#define str_iequals(P, TEST)	(strlen(TEST) == (size_t) (P)->len && !strncasecmp((P)->buf, (TEST), (P)->len))

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

