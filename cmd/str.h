#pragma once

#include "../protobuf.h"
#include "../char-array.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

// see char-array.h for string comparison functions

typedef struct {
	int cap;
	int len;
	char *c_str;
} str_t;

extern char str_initbuf[];

#define STR_INIT {0,0,str_initbuf}
#define STR_STATIC(str) {0,sizeof(str)-1,(char*)str}

static inline void str_init(str_t *s) {
	s->cap = s->len = 0;
	s->c_str = str_initbuf;
}
static inline void str_setlen(str_t *s, int len) {
	assert(0 <= len && len <= s->cap);
	s->len = len;
	s->c_str[len] = '\0';
}
static inline void str_setend(str_t *s, char *end) {
	assert(s->c_str <= end && end <= s->c_str + s->cap);
	*end = '\0';
	s->len = (int) (end - s->c_str);
}
static inline void str_clear(str_t *s) {
	s->len = 0;
	s->c_str[0] = 0;
}
static inline char *str_release(str_t *s) {
	char *p = s->c_str;
	s->cap = 0;
	s->len = 0;
	s->c_str = NULL;
	return p;
}

void str_destroy(str_t *s);
void str_read_file(str_t *s, const char *fn);
void str_fread_all(str_t *s, FILE *f);
void str_grow(str_t *s, int cap);
void str_add2(str_t *s, const char *a, int len);

#ifdef __GNUC__
__attribute__((format (printf,2,3)))
#endif
int str_addf(str_t *s, const char *fmt, ...);

#ifdef __GNUC__
__attribute__((format (printf,2,0)))
#endif
int str_vaddf(str_t *s, const char *fmt, va_list ap);

// defined as a macro to also support ca_* char arrays
#define str_addstr(P, STR) str_add2(P, (STR).c_str, (STR).len);

static inline void str_add(str_t *s, const char *a) {
	str_add2(s, a, (int) strlen(a));
}
static inline void str_set2(str_t *s, const char *a, int len) {
	str_clear(s);
	str_add2(s, a, len);
}
static inline void str_set(str_t *s, const char *a) {
	str_set2(s, a, (int) strlen(a));
}

#define str_setstr(P, STR) str_set2(P, (STR).c_str, (STR).len)

static inline void str_addch(str_t *s, char ch) {
	if (s->len == s->cap) {
		str_grow(s, s->cap+1);
	}
	s->c_str[s->len++] = ch;
	s->c_str[s->len] = 0;
}

static inline void str_swap(str_t *a, str_t *b) {
	str_t c = *a;
	*a = *b;
	*b = c;
}
