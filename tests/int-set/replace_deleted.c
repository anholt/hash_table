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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "int-set.h"
#include "fnv_hash.h"

int
main(int argc, char **argv)
{
	struct int_set *set;
	struct int_set_entry *entry_1, *entry_6;

	set = int_set_create();

	/* Create a deleted entry. */
	entry_1 = int_set_add(set, 1);
	int_set_remove(set, 1);

	/* Add an entry that hits the same slot.  This assertion is
	 * just to make sure that our test is triggering the bug
	 * correctly and we're hitting the same table entry, in case
	 * implementation changes.
	 */
	entry_6 = int_set_add(set, 6);
	assert(entry_6 == entry_1);

	/* Make sure that we think that the value we just added is
	 * really present.  This catches the actual bug.
	 */
	assert(int_set_contains(set, 6));

	int_set_destroy(set);

	return 0;
}
