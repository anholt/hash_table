/*
 * Copyright © 2009,2013 Intel Corporation
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
 *    Carl Worth <cworth@cworth.org>
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "int-set.h"

int
main(int argc, char **argv)
{
	struct int_set *set;

	/* Use two values with the lowest bits in common so they will
	 * hash to the same initial entry, so we can test the deletion
	 * behavior for chained objects. */
	uint32_t value1 = 0x00000123;
	uint32_t value2 = 0x10000123;
	struct int_set_entry *entry;

	set = int_set_create();

	int_set_add(set, value1);
	int_set_add(set, value2);

	entry = int_set_search(set, value1);
	assert(entry->value == value1);

	entry = int_set_search(set, value2);
	assert(entry->value == value2);

	int_set_remove(set, entry);

	entry = int_set_search(set, value2);
	assert(entry == NULL);

	entry = int_set_search(set, value1);
	assert(entry->value == value1);

	int_set_destroy(set);

	return 0;
}
