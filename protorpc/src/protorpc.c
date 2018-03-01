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

pb_string pb_strdup(pb_allocator *obj, const char *s, int sz) {
	pb_string r = { 0, NULL };
	if (obj->next + sz + 1 <= obj->end) {
		char *p = obj->next;
		memcpy(p, s, sz);
		p[sz] = 0;
		obj->end += sz + 1;
		r.c_str = p;
		r.len = sz;
	}
	return r;
}

struct list {
	int len;
	char *v;
};

void *pb_appendv_(pb_allocator *obj, void *list, size_t add, size_t objsz) {
	struct list *l = (struct list*) list;
	if (!l->len) {
		// easy case - new list
		l->len = add;
		l->v = pb_calloc(obj, add, objsz);
		return l->v;
	}
	// make sure we haven't added anything else in the interim
	char *end = l->v + (l->len * objsz);
	if (end != obj->next) {
		return NULL;
	}
	// make sure we have enough room
	char *newend = end + (add * objsz);
	if (newend > obj->next) {
		return NULL;
	}
	memset(end, 0, add * objsz);
	l->len += add;
	return end;
}

char * create_child_message(pb_allocator *a, char *parent, size_t datasz) {
	char *child = (char*)pb_calloc(a, 1, datasz);
	if (!child) {
		return NULL;
	}
	*(char**) parent = child;
	return child;
}

char *append_message_list(pb_allocator *a, char *parent, pb_message ***plast_msg, size_t datasz) {
	pb_message *child = (pb_message*) pb_calloc(a, 1, datasz);
	if (!child) {
		return NULL;
	}
	pb_msg_list *list = (pb_msg_list*)parent;
	list->len++;
	**plast_msg = child;
	*plast_msg = &child->next;
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
