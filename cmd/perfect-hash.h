#include <stdint.h>

struct hash_entry {
    const char *str;
	uint32_t off;
};

void calc_hash_values(struct hash_entry *entries, int num, uint32_t *hashmul, uint32_t *hashsz);

