#pragma once
#include <protorpc/protorpc.h>

#define MAX_DEPTH 8
#define OBJ_ALIGN 8
#define WIRE_VARINT 0
#define WIRE_FIXED_64 1
#define WIRE_VARIABLE 2
#define WIRE_FIXED_32 5

// lists are of the form
// struct {int len; <type> const *v;} field;
typedef struct {
	int len;
	union {
		union pb_msg **v;
		union pb_msg *last;
	} u;
} pb_message_list;

typedef struct {
	int len;
	char *data;
} pb_pod_list;

static inline char *align(char *p, size_t align) {
	return (char*)(((uintptr_t)p + (align - 1)) &~(align - 1));
}

char *append_message_list(pb_allocator *a, char *parent, size_t datasz);
int create_message_list(pb_allocator *a, char *parent);

char *create_child_message(pb_allocator *a, char *parent, size_t datasz);

char *append_pod_list(pb_allocator *a, char *parent, size_t datasz);

// binary_search does a binary search on the field names
// the field names are ordered by length and then bitwise
const pb_string *binary_search(const pb_string **names, int num, pb_string want);
