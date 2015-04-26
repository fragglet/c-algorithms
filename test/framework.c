/*

Copyright (c) 2005-2008, Simon Howard

Permission to use, copy, modify, and/or distribute this software
for any purpose with or without fee is hereby granted, provided
that the above copyright notice and this permission notice appear
in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

 */

#include <stdio.h>
#include <stdlib.h>

#include <assert.h>

#include "alloc-testing.h"
#include "framework.h"

/* Run a single test. */

static void run_test(UnitTestFunction test)
{
	/* Turn off any allocation limits that may have been set
	 * by a previous test. */

	alloc_test_set_limit(-1);

	/* Run the test */

	test();

	/* Check that all memory was correctly freed back during
	 * the test. */

	assert(alloc_test_get_allocated() == 0);
}

void run_tests(UnitTestFunction *tests)
{
	int i;

	for (i=0; tests[i] != NULL; ++i) {
		run_test(tests[i]);
	}
}

