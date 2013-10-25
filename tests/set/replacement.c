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
#include "set.h"
#include "fnv_hash.h"

int
main(int argc, char **argv)
{
	struct set *set;
	char *str1 = strdup("test1");
	char *str2 = strdup("test1");
	uint32_t hash_str1 = fnv1_hash_string(str1);
	struct set_entry *entry;

	set = set_create(fnv1_hash_string, string_key_equals);

	set_add_pre_hashed(set, hash_str1, str1);
	set_add(set, str2);

	assert(set_contains(set, str1));
	entry = set_search_pre_hashed(set, hash_str1, str1);
	assert(entry);
	assert(entry->key == str2);

	set_remove_entry(set, entry);

	assert(!set_contains(set, str1));
	entry = set_search(set, str1);
	assert(!entry);

	set_destroy(set, NULL);
	free(str1);
	free(str2);

	return 0;
}
