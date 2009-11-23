/*
 * Copyright © 2009 Intel Corporation
 * Copyright © 1988-2004 Keith Packard and Bart Massey.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * Except as contained in this notice, the names of the authors
 * or their institutions shall not be used in advertising or
 * otherwise to promote the sale, use or other dealings in this
 * Software without prior written authorization from the
 * authors.
 *
 * Authors:
 *    Eric Anholt <eric@anholt.net>
 *    Keith Packard <keithp@keithp.com>
 */

#include <stdlib.h>

#include "hash_table.h"

/*
 * From Knuth -- a good choice for hash/rehash values is p, p-2 where
 * p and p-2 are both prime.  These tables are sized to have an extra 10%
 * free to avoid exponential performance degradation as the hash table fills
 */

uint32_t deleted_key_value;
const void *deleted_key = &deleted_key_value;

static const struct {
   uint32_t max_entries, size, rehash;
} hash_sizes[] = {
    { 2,		5,		3	  },
    { 4,		7,		5	  },
    { 8,		13,		11	  },
    { 16,		19,		17	  },
    { 32,		43,		41        },
    { 64,		73,		71        },
    { 128,		151,		149       },
    { 256,		283,		281       },
    { 512,		571,		569       },
    { 1024,		1153,		1151      },
    { 2048,		2269,		2267      },
    { 4096,		4519,		4517      },
    { 8192,		9013,		9011      },
    { 16384,		18043,		18041     },
    { 32768,		36109,		36107     },
    { 65536,		72091,		72089     },
    { 131072,		144409,		144407    },
    { 262144,		288361,		288359    },
    { 524288,		576883,		576881    },
    { 1048576,		1153459,	1153457   },
    { 2097152,		2307163,	2307161   },
    { 4194304,		4613893,	4613891   },
    { 8388608,		9227641,	9227639   },
    { 16777216,		18455029,	18455027  },
    { 33554432,		36911011,	36911009  },
    { 67108864,		73819861,	73819859  },
    { 134217728,	147639589,	147639587 },
    { 268435456,	295279081,	295279079 },
    { 536870912,	590559793,	590559791 },
    { 1073741824,	1181116273,	1181116271},
    { 2147483648ul,	2362232233ul,	2362232231ul}
};

static int
entry_is_free(struct hash_entry *entry)
{
	return entry->key == NULL;
}

static int
entry_is_present(struct hash_entry *entry)
{
	return entry->key != NULL && entry->key != deleted_key;
}

struct hash_table *
hash_table_create(uint32_t (*hash_function)(const void *key),
		  int key_equals_function(const void *a,
					  const void *b))
{
	struct hash_table *ht;

	ht = malloc(sizeof(*ht));
	if (ht == NULL)
		return NULL;

	ht->size_index = 0;
	ht->size = hash_sizes[ht->size_index].size;
	ht->rehash = hash_sizes[ht->size_index].rehash;
	ht->max_entries = hash_sizes[ht->size_index].max_entries;
	ht->hash_function = hash_function;
	ht->key_equals_function = key_equals_function;
	ht->table = calloc(ht->size, sizeof(*ht->table));

	if (ht->table == NULL) {
		free(ht);
		return NULL;
	}

	return ht;
}

/**
 * Frees the given hash table.
 *
 * If delete_function is passed, it gets called on each entry present before
 * freeing.
 */
void
hash_table_destroy(struct hash_table *ht,
		   void (*delete_function)(struct hash_entry *entry))
{
	if (!ht)
		return;

	if (delete_function) {
		struct hash_entry *entry;

		for (entry = ht->table;
		     entry != NULL;
		     entry = hash_table_next_entry(ht, entry)) {
			delete_function(entry);
		}
	}

	free(ht);
}

struct hash_entry *
hash_table_search(struct hash_table *ht, const void *key)
{
	uint32_t hash, hash_address;

	hash = ht->hash_function(key);
	hash_address = hash & ht->size;
	do {
		struct hash_entry *entry = ht->table + hash_address;

		if (entry_is_free(entry)) {
			return NULL;
		} else if (entry_is_present(entry) && hash == hash) {
			if (ht->key_equals_function(key, entry->key)) {
				return entry;
			}
		}

		hash_address = (hash_address + ht->rehash) & ht->size;
	} while (hash_address != hash % ht->size);

	return NULL;
}

static void
hash_table_expand(struct hash_table *ht)
{
	/* XXX */
}

struct hash_entry *
hash_table_insert(struct hash_table *ht, const void *key, void *data)
{
	uint32_t hash, hash_address;

	if (ht->entries + 1 > ht->max_entries) {
		hash_table_expand(ht);
	}

	hash = ht->hash_function(key);
	hash_address = hash & ht->size;
	do {
		struct hash_entry *entry = ht->table + hash_address;

		if (!entry_is_present(entry)) {
			entry->hash = hash;
			entry->key = key;
			entry->data = data;
			ht->entries++;
			return entry;
		}

		hash_address = (hash_address + ht->rehash) & ht->size;
	} while (hash_address != hash % ht->size);

	/* We could hit here if a required resize failed. An unchecked-malloc
	 * application could ignore this result.
	 */
	return NULL;
}

/**
 * This function deletes the given hash table entry.
 *
 * Note that deletion doesn't otherwise modify the table, so an iteration over
 * the table deleting entries is safe.
 */
void
hash_table_remove(struct hash_table *ht, struct hash_entry *entry)
{
	entry->key = deleted_key;
	ht->entries--;
}

/**
 * This function is an iterator over the hash table.
 *
 * Pass in NULL for the first entry, as in the start of a for loop.  Note that
 * an iteration over the table is O(table_size) not O(entries).
 */
struct hash_entry *
hash_table_next_entry(struct hash_table *ht, struct hash_entry *entry)
{
	if (entry == NULL)
		entry = ht->table;
	else
		entry = entry + 1;

	for (; entry != ht->table + ht->size; entry++) {
		if (entry_is_present(entry)) {
			return entry;
		}
	}

	return NULL;
}
