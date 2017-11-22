#include "hash.h"
#include "khash.h"
#include "../char-array.h"

static uint32_t string_hash(slice_t key) {
    if (key.len == 0) {
        return 0;
    }

    uint32_t h = (uint8_t) *key.buf;

    for (int i = 1; i < key.len; i++) {
        h = (h << 5) - h + (uint8_t) key.buf[i];
    }

    return h;
}

static bool string_equal(slice_t a, slice_t b) {
	return a.len == b.len && !memcmp(a.buf, b.buf, a.len);
}

KHASH_INIT(string, slice_t, 1, string_hash, string_equal)
KHASH_INIT(i32, int32_t, 1, kh_int_hash_func, kh_int_hash_equal)
KHASH_INIT(i64, int64_t, 1, kh_int64_hash_func, kh_int64_hash_equal)

void map_erase_base(map_t* h, int idx) {
	kh_del_string((kh_string_t*) h, idx);
}

void map_clear_base(map_t* h) {
	kh_clear_string((kh_string_t*) h);
}

void map_free_base(map_t* h) {
	kh_destroy_string((kh_string_t*) h);
}

bool map_hasnext_base(const map_t* h, int* pidx) {
	(*pidx)++;
	while (*pidx < (int) kh_end(h)) {
		if (kh_exist(h, *pidx)) {
			return true;
		}
		(*pidx)++;
	}
	return false;
}

bool imap_32_get_base(const map_t* h, int32_t key) {
	((map_t*)h)->idx = kh_get_i32((kh_i32_t*) h, key);
	return h->idx != (int) kh_end(h);
}

bool imap_64_get_base(const map_t* h, int64_t key) {
	((map_t*) h)->idx = kh_get_i64((kh_i64_t*) h, key);
	return h->idx != (int) kh_end(h);
}

bool smap_get_base(const map_t* h, const char *key, int len) {
	slice_t skey;
	skey.buf = (char*) key;
	skey.len = len;
	((map_t*) h)->idx = kh_get_string((kh_string_t*) h, skey);
	return h->idx != (int) kh_end(h);
}

bool imap_32_add_base(map_t* h, int32_t key, size_t valsz) {
	int ret;
	((map_t*) h)->idx = kh_put_i32((kh_i32_t*) h, key, valsz, &ret);
	return ret != 0;
}

bool imap_64_add_base(map_t* h, int64_t key, size_t valsz) {
	int ret;
	((map_t*) h)->idx = kh_put_i64((kh_i64_t*) h, key, valsz, &ret);
	return ret != 0;
}

bool smap_add_base(map_t* h, const char *key, int len, size_t valsz) {
	int ret;
	slice_t skey;
	skey.buf = (char*) key;
	skey.len = len;
	((map_t*) h)->idx = kh_put_string((kh_string_t*) h, skey, valsz, &ret);
	return ret != 0;
}

