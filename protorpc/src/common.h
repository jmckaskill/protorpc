#pragma once
#include <protorpc/protorpc.h>

#define MAX_DEPTH 8
#define OBJ_ALIGN 8
#define WIRE_VARINT 0
#define WIRE_FIXED_64 1
#define WIRE_VARIABLE 2
#define WIRE_FIXED_32 5

typedef struct pb_message pb_message;
typedef struct pb_msg_list pb_msg_list;
typedef struct pb_pod_list pb_pod_list;

// Messages are of the form
// struct my_message {
//   my_message *_next;
//   int _encsz;
//   fields ...
// }

struct pb_message {
	pb_message *next;
	int encoded_size;
};

struct pb_msg_list {
	int len;
	pb_message *first;
};

struct pb_pod_list {
	int len;
	char *data;
};

static inline char *align(char *p, size_t align) {
	return (char*)(((uintptr_t)p + (align - 1)) &~(align - 1));
}

char *append_pod_list(pb_allocator *a, char *parent, size_t datasz);
char *append_message_list(pb_allocator *a, char *parent, pb_message ***plast_msg, size_t datasz);
char *create_child_message(pb_allocator *a, char *parent, size_t datasz);

// binary_search does a binary search on the field names
// the field names are ordered by length and then bitwise
const pb_string *binary_search(const pb_string **names, int num, pb_string want);
