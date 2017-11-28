#include "perfect-hash.h"
#include <stdio.h>
#include <stdlib.h>

static uint32_t compute_hash(const char *s, uint32_t mul) {
	uint32_t hash = 0;
	while (*s) {
		hash = (hash * mul) + (uint8_t)*s;
		s++;
	}
	return hash;
}

// slow as algorithm, but who cares
void calc_hash_values(struct hash_entry* values, int num, uint32_t* hashmul, uint32_t* hashsz) {
	int tries_left = 1000;
	*hashsz = num + 1;
	*hashmul = 0;

try_next:
	*hashmul = *hashmul * 3 + 1;
	(*hashsz)++;
	if (!tries_left--) {
		fprintf(stderr, "failed to compute perfect hash\n");
		exit(3);
	}

	for (int i = 0; i < num; i++) {
		uint32_t off = compute_hash(values[i].str, *hashmul) % (*hashsz);
		if (!off) {
			// collides with the null hash
			goto try_next;
		}
		for (int j = 0; j < i; j++) {
			if (values[j].off == off) {
				// we have a collision
				goto try_next;
			}
		}
		values[i].off = off;
	}
	// no collisions, can return
}

