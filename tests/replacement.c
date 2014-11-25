/*
 * Copyright © 2011 Intel Corporation
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
 * Authors:
 *    Eric Anholt <eric@anholt.net>
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "hash_table.h"
#include "fnv_hash.h"

int
main(int argc, char **argv)
{
	struct hash_table *ht;
	char *str1 = strdup("test1");
	char *str2 = strdup("test1");
	uint32_t hash_str1 = fnv1_hash_string(str1);
	struct hash_entry *entry;

	ht = hash_table_create_for_string();

	hash_table_insert_pre_hashed(ht, hash_str1, str1, str1);
	hash_table_insert(ht, str2, str2);

	entry = hash_table_search_pre_hashed(ht, hash_str1, str1);
	assert(entry);
	assert(entry->data == str2);

	entry = hash_table_search(ht, str1);
	assert(entry);
	assert(entry->data == str2);

	hash_table_remove_entry(ht, entry);

	entry = hash_table_search_pre_hashed(ht, hash_str1, str1);
	assert(!entry);

	entry = hash_table_search(ht, str1);
	assert(!entry);

	hash_table_destroy(ht, NULL);
	free(str1);
	free(str2);

	return 0;
}
