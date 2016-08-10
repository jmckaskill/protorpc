#define BUILDING_PROTORPC
#include <protorpc/protorpc.h>

struct pb_page {
	struct pb_page *next;
	struct pb_page *prev;
	size_t sz, used;
};

// allow up to 16 byte alignment
#define HEADERSZ ((sizeof(struct pb_page) + 15) &~ 15U) 

#ifdef WIN32
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
static void *alloc_page(size_t sz) {
	return VirtualAlloc(NULL, sz, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
}
static void free_page(void *p) {
	VirtualFree(p, 0, MEM_RELEASE);
}
#else
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
static void *alloc_page(size_t sz) {
	void *p;
	int ret = posix_memalign(&p, PB_PGSZ, sz);
	if (ret) {
		fprintf(stderr, "alloc failed %d\n", ret);
		exit(1);
	}
	return p;
}
static void free_page(struct pb_page *p) {
	free(p);
}
#endif

static char *doalloc(struct pb_page **page, char *data, size_t minsz) {
	char *ret;
	struct pb_page *next, *cur = *page;
	// round up to the nearest page size
	size_t allocsz = (minsz + 8 + HEADERSZ + PB_PGSZ - 1) &~(PB_PGSZ - 1);
	if (cur && cur->next && cur->next->sz >= allocsz) {
		next = cur->next;
	} else {
		next = (struct pb_page*) alloc_page(allocsz);
		next->prev = cur;
		next->next = cur ? cur->next : NULL;
		next->sz = allocsz;
		if (next->next) {
			next->next->prev = next;
		}
		if (cur) {
			cur->next = next;
			cur->used = data - (char*)cur - HEADERSZ;
		}
	}
	*page = next;
	ret = (char*)next + HEADERSZ;
#ifndef NDEBUG
	memset(ret, 0xEE, allocsz - HEADERSZ);
#endif
	return ret;
}

static struct pb_page* first_page(struct pb_page *b) {
	while (b && b->prev) {
		b = b->prev;
	}
	return b;
}

void pb_alloc_clear(pb_alloc_t *a) {
	a->page = first_page(a->page);
	a->next = (char*)a->page + (a->page ? HEADERSZ : -1);
}

void pb_alloc_destroy(pb_alloc_t *a) {
	struct pb_page *b = first_page(a->page);
	while (b) {
		struct pb_page* n = b->next;
		free_page(b);
		b = n;
	}
}

// pb_calloc allocates zeroed objects 
void *pb_calloc(pb_alloc_t *alloc, int n, size_t objsz) {
	size_t need = PB_ALIGN8(objsz*n);
	if (PB_PAGE(alloc->next) != PB_PAGE(alloc->next + need)) {
		alloc->next = doalloc(&alloc->page, alloc->next, need);
	}
	void *ret = alloc->next;
	memset(ret, 0, need);
	alloc->next += need;
	return ret;
}

// pb_reserve reserves room for a growing vector of objects at the end of the memory pool
void *pb_reserve(pb_alloc_t *alloc, int have, size_t objsz) {
	size_t need = PB_ALIGN8(objsz*(have+1));
	if (PB_PAGE(alloc->next) != PB_PAGE(alloc->next + need)) {
		char *pg = doalloc(&alloc->page, alloc->next, need);
		memcpy(pg, alloc->next, have*objsz);
		alloc->next = pg;
	}
	void *ret = alloc->next;
	memset(alloc->next + (have*objsz), 0, need - (have*objsz));
	return ret;
}

// pb_commit commits the vector that has been growing using pb_reserve
void pb_commit(pb_alloc_t *alloc, int n, size_t objsz) {
	alloc->next += PB_ALIGN8(n*objsz);
}
