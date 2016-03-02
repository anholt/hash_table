/*
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

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "hash_table.h"
#include "fnv_hash.h"

static bool deleted = false;

static int
key_equals(const void *a, const void *b)
{
    /* The comparison function shouldn't be called if our entry had
     * been deleted.
     */
    assert(!deleted);

    return strcmp(a, b) == 0;
}

int
main(int argc, char **argv)
{
	struct hash_table *ht;
	const char *str = "test1";
	struct hash_entry *entry;

	ht = hash_table_create((uint32_t (*)(const void *key))fnv1_hash_string,
			       key_equals);

	hash_table_insert(ht, str, NULL);
	hash_table_remove(ht, str);
	deleted = true;
	hash_table_insert(ht, str, NULL);
	assert(!entry);

	hash_table_destroy(ht, NULL);

	return 0;
}
