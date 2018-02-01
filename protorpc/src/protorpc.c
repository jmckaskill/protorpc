#include "common.h"
#include <string.h>

void *pb_calloc(pb_allocator *b, size_t num, size_t sz) {
	sz *= num;
	char *p = align(b->next, OBJ_ALIGN);
	char *n = p + sz;
	if (n > b->end) {
		return NULL;
	}
	memset(p, 0, sz);
	b->next = n;
	return p;
}

int create_message_list(pb_allocator *a, char *parent) {
	pb_message_list *list = (pb_message_list*)parent;
	union pb_msg **v = (union pb_msg**) pb_calloc(a, list->len, sizeof(union pb_msg*));
	if (!v) {
		return -1;
	}
	union pb_msg *iter = list->u.last;
	for (int i = list->len - 1; i >= 0; i--) {
		v[i] = iter;
		iter = iter->previous;
	}
	list->u.v = v;
	return 0;
}

char * create_child_message(pb_allocator *a, char *parent, size_t datasz) {
	char *child = (char*)pb_calloc(a, 1, datasz);
	if (!child) {
		return NULL;
	}
	*(char**) parent = child;
	return child;
}

char *append_message_list(pb_allocator *a, char *parent, size_t datasz) {
	pb_message_list *list = (pb_message_list*)parent;
	union pb_msg *child = (union pb_msg*) pb_calloc(a, 1, datasz);
	if (!child) {
		return NULL;
	}
	list->len++;
	child->previous = list->u.last;
	list->u.last = child;
	return (char*)child;
}

char *append_pod_list(pb_allocator *a, char *parent, size_t datasz) {
	pb_pod_list *list = (pb_pod_list*)parent;
	char *ret = a->next;
	if (!list->data) {
		ret = list->data = a->next = align(a->next, OBJ_ALIGN);
	}
	if (a->next != list->data + (list->len * datasz)) {
		return NULL;
	}
	if (ret + datasz > a->end) {
		return NULL;
	}
	memset(ret, 0, datasz);
	list->len++;
	a->next = ret + datasz;
	return ret;
}

const pb_string *binary_search(const pb_string **names, int num, pb_string want) {
	int left = 0;
	int right = num - 1;

	while (left <= right) {
		int mid = left + (right - left) / 2;
		const pb_string *mids = names[mid];

		int diff = want.len - mids->len;
		if (!diff) {
			diff = memcmp(want.c_str, mids->c_str, want.len);
			if (!diff) {
				return mids;
			}
		}

		if (diff > 0) {
			// name is larger than midpoint
			left = mid + 1;
		} else {
			// name is smaller than midpoint
			right = mid - 1;
		}
	}

	return NULL;
}
