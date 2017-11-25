/* vim: ts=4 sw=4 sts=4 et tw=78
 *
 * Copyright (c) 2009 James R. McKaskill
 *
 * This software is licensed under the stock MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------
 */

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ------------------------------------------------------------------------- */

typedef struct {
    uint32_t n_buckets, size, n_occupied, upper_bound;
    uint32_t *flags;
    int idx;
} map_t;

// to create a map, create your own structure that has map_t as the first member
// followed by an array of keys and then an array of values
// use pb_string_t from char-array.h
// struct { map_t h; pb_string_t *keys; struct my_struct *values;} my_table;

/* ------------------------------------------------------------------------- */

void map_erase_base(map_t* h, int idx);
void map_clear_base(map_t* h);
void map_free_base(map_t* h);
bool map_hasnext_base(const map_t* h, int* pidx);

/* ------------------------------------------------------------------------- */

bool imap_32_get_base(const map_t* h, int32_t key);
bool imap_64_get_base(const map_t* h, int64_t key);

bool imap_32_add_base(map_t* h, int32_t key, size_t valsz);
bool imap_64_add_base(map_t* h, int64_t key, size_t valsz);

/* ------------------------------------------------------------------------- */

bool smap_get_base(const map_t* h, const char *key, int len);
bool smap_add_base(map_t* h, const char *key, int len, size_t valsz);

/* ------------------------------------------------------------------------- */

/* Key type agnostic functions */
#define map_size(H)             ((H)->h.size)
#define map_clear(H)             map_clear_base(&(H)->h)
#define map_free(H)              map_free_base(&(H)->h)
#define map_erase(H, IDX)        map_erase_base(&(H)->h, IDX)
/* To iterate over the elements in a map:
 * int idx = -1;
 * while (map_hasnext(&table, &idx)) {
 *  // key is table.keys[idx]
 *  // value is table.values[idx]
 * }
 */
#define map_hasnext(H, PIDX)     map_hasnext_base(&(H)->h, PIDX)

/* imap_* are the integer key versions.
 * smap_* are the string key versions.
 *
 * [is]map_get: Looks up the key in h and stores the value in *(pval). Returns
 * true if the lookup was successful. only sets pval if the lookup was successful
 *
 * [is]map_find: Looks up the key in h and stores the index in *(pidx). Returns
 * true if the lookup was successful.
 *
 * [is]map_remove: Removes h[key] from the table. Returns true if it removed a
 * value.
 *
 * [is]map_add: Adds a slot for the value and returns the index in *pidx.
 * Returns true if a slot was added.
 *
 * [is]map_set: Sets h[key] = val. This will overwrite the existing value.
 */

#define imap_get_base(H, KEY)    ((sizeof((H)->keys[0]) == sizeof(int64_t)) ? imap_64_get_base(&(H)->h, (int64_t) (uintptr_t) (KEY)) : imap_32_get_base(&(H)->h, (int32_t) (uintptr_t) (KEY)))
#define imap_add_base(H, KEY)    ((sizeof((H)->keys[0]) == sizeof(int64_t)) ? imap_64_add_base(&(H)->h, (int64_t) (uintptr_t) (KEY), sizeof((H)->values[0])) : imap_32_add_base(&(H)->h, (int32_t) (uintptr_t) (KEY), sizeof((H)->values[0])))

#define imap_get(H, KEY, PVAL)   (imap_get_base(H, KEY) && (*(PVAL) = (H)->values[(H)->h.idx], true))
#define imap_find(H, KEY, PIDX)  (imap_get_base(H, KEY) && (*(PIDX) = (H)->h.idx, true))
#define imap_remove(H, KEY)      (imap_get_base(H, KEY) && (map_erase_base(&(H)->h, (H)->h.idx), true))
#define imap_add(H, KEY, PIDX)   (imap_add_base(H, KEY) ? ((*(PIDX) = (H)->h.idx), true) : ((*(PIDX) = (H)->h.idx), false))
#define imap_set(H, KEY, VAL)    (imap_add_base(H, KEY), ((H)->values[(H)->h.idx] = (VAL)))

#define smap_get(H, KEY, LEN, PVAL)   (smap_get_base(&(H)->h, KEY, LEN) && (*(PVAL) = (H)->values[(H)->h.idx], true))
#define smap_find(H, KEY, LEN, PIDX)  (smap_get_base(&(H)->h, KEY, LEN) && (*(PIDX) = (H)->h.idx, true))
#define smap_remove(H, KEY, LEN)      (smap_get_base(&(H)->h, KEY, LEN) && (map_erase_base(&(H)->h, (H)->h.idx), true))
#define smap_add(H, KEY, LEN, PIDX)   (smap_add_base(&(H)->h, KEY, LEN, sizeof((H)->values[0])) ? ((*(PIDX) = (H)->h.idx), true) : ((*(PIDX) = (H)->h.idx), false))
#define smap_set(H, KEY, LEN, VAL)    (smap_add_base(&(H)->h, KEY, LEN, sizeof((H)->values[0])), ((H)->values[(H)->h.idx] = (VAL)))

