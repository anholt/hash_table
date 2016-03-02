/*
 * Copyright © 2009 Intel Corporation
 * Copyright © 2016 Broadcom Limited
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
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "hash_table.h"
#include "fnv_hash.h"

/* Return collisions, so we can test the deletion behavior for chained
 * objects.
 */
static uint32_t
badhash(const void *key)
{
	return 1;
}

int
main(int argc, char **argv)
{
	struct hash_table *ht;
	const char *str1 = "test1";
	const char *str2 = "test2";
	uint8_t data = 0;
	struct hash_entry *entry;

	ht = hash_table_create(badhash, string_key_equals);

	hash_table_insert(ht, str1, NULL);
	hash_table_insert(ht, str2, NULL);
	hash_table_remove(ht, str1);
	hash_table_insert(ht, str2, &data);

	/* Make sure that we actually replaced our str2's data, even
	 * though it was chained off of str1.
	 */
	entry = hash_table_search(ht, str2);
	assert(entry->data == &data);

	/* Make sure that if we remove str2, it wasn't just occupying
	 * str1's old slot with the old str2 still present.
	 */
	hash_table_remove(ht, str2);
	entry = hash_table_search(ht, str2);
	assert(entry == NULL);

	hash_table_destroy(ht, NULL);

	return 0;
}
