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

/* ArrayList test cases */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "alloc-testing.h"
#include "framework.h"

#include "arraylist.h"
#include "compare-int.h"

int variable1, variable2, variable3, variable4;

ArrayList *generate_arraylist(void)
{
	ArrayList *arraylist;
	int i;

	arraylist = arraylist_new(0);

	for (i=0; i<4; ++i) {
		arraylist_append(arraylist, &variable1);
		arraylist_append(arraylist, &variable2);
		arraylist_append(arraylist, &variable3);
		arraylist_append(arraylist, &variable4);
	}

	return arraylist;
}

void test_arraylist_new_free(void)
{
	ArrayList *arraylist;

	/* Use a default size when given zero */

	arraylist = arraylist_new(0);
	assert(arraylist != NULL);
	arraylist_free(arraylist);

	/* Normal allocated */

	arraylist = arraylist_new(10);
	assert(arraylist != NULL);
	arraylist_free(arraylist);

	/* Freeing a null arraylist works */

	arraylist_free(NULL);

	/* Test low memory scenarios (failed malloc) */

	alloc_test_set_limit(0);
	arraylist = arraylist_new(0);
	assert(arraylist == NULL);

	alloc_test_set_limit(1);
	arraylist = arraylist_new(100);
	assert(arraylist == NULL);
}

void test_arraylist_append(void)
{
	ArrayList *arraylist;
	int i;

	arraylist = arraylist_new(0);

	assert(arraylist->length == 0);

	/* Append some entries */

	assert(arraylist_append(arraylist, &variable1) != 0);
	assert(arraylist->length == 1);

	assert(arraylist_append(arraylist, &variable2) != 0);
	assert(arraylist->length == 2);

	assert(arraylist_append(arraylist, &variable3) != 0);
	assert(arraylist->length == 3);

	assert(arraylist_append(arraylist, &variable4) != 0);
	assert(arraylist->length == 4);

	assert(arraylist->data[0] == &variable1);
	assert(arraylist->data[1] == &variable2);
	assert(arraylist->data[2] == &variable3);
	assert(arraylist->data[3] == &variable4);

	/* Test appending many entries */

	for (i=0; i<10000; ++i) {
		assert(arraylist_append(arraylist, NULL) != 0);
	}

	arraylist_free(arraylist);

	/* Test low memory scenario */

	arraylist = arraylist_new(100);

	alloc_test_set_limit(0);

	for (i=0; i<100; ++i) {
		assert(arraylist_append(arraylist, NULL) != 0);
	}

	assert(arraylist->length == 100);
	assert(arraylist_append(arraylist, NULL) == 0);
	assert(arraylist->length == 100);

	arraylist_free(arraylist);
}


void test_arraylist_prepend(void)
{
	ArrayList *arraylist;
	int i;

	arraylist = arraylist_new(0);

	assert(arraylist->length == 0);

	/* Append some entries */

	assert(arraylist_prepend(arraylist, &variable1) != 0);
	assert(arraylist->length == 1);

	assert(arraylist_prepend(arraylist, &variable2) != 0);
	assert(arraylist->length == 2);

	assert(arraylist_prepend(arraylist, &variable3) != 0);
	assert(arraylist->length == 3);

	assert(arraylist_prepend(arraylist, &variable4) != 0);
	assert(arraylist->length == 4);

	assert(arraylist->data[0] == &variable4);
	assert(arraylist->data[1] == &variable3);
	assert(arraylist->data[2] == &variable2);
	assert(arraylist->data[3] == &variable1);

	/* Test prepending many entries */

	for (i=0; i<10000; ++i) {
		assert(arraylist_prepend(arraylist, NULL) != 0);
	}

	arraylist_free(arraylist);

	/* Test low memory scenario */

	arraylist = arraylist_new(100);

	alloc_test_set_limit(0);

	for (i=0; i<100; ++i) {
		assert(arraylist_prepend(arraylist, NULL) != 0);
	}

	assert(arraylist->length == 100);
	assert(arraylist_prepend(arraylist, NULL) == 0);
	assert(arraylist->length == 100);

	arraylist_free(arraylist);
}

void test_arraylist_insert(void)
{
	ArrayList *arraylist;
	int i;

	arraylist = generate_arraylist();

	/* Check for out of range insert */

	assert(arraylist->length == 16);
	assert(arraylist_insert(arraylist, 17, &variable1) == 0);
	assert(arraylist->length == 16);

	/* Insert a new entry at index 5 */

	assert(arraylist->length == 16);
	assert(arraylist->data[4] == &variable1);
	assert(arraylist->data[5] == &variable2);
	assert(arraylist->data[6] == &variable3);

	assert(arraylist_insert(arraylist, 5, &variable4) != 0);

	assert(arraylist->length == 17);
	assert(arraylist->data[4] == &variable1);
	assert(arraylist->data[5] == &variable4);
	assert(arraylist->data[6] == &variable2);
	assert(arraylist->data[7] == &variable3);

	/* Inserting at the start */

	assert(arraylist->data[0] == &variable1);
	assert(arraylist->data[1] == &variable2);
	assert(arraylist->data[2] == &variable3);

	assert(arraylist_insert(arraylist, 0, &variable4) != 0);

	assert(arraylist->length == 18);
	assert(arraylist->data[0] == &variable4);
	assert(arraylist->data[1] == &variable1);
	assert(arraylist->data[2] == &variable2);
	assert(arraylist->data[3] == &variable3);

	/* Inserting at the end */

	assert(arraylist->data[15] == &variable2);
	assert(arraylist->data[16] == &variable3);
	assert(arraylist->data[17] == &variable4);

	assert(arraylist_insert(arraylist, 18, &variable1) != 0);

	assert(arraylist->length == 19);
	assert(arraylist->data[15] == &variable2);
	assert(arraylist->data[16] == &variable3);
	assert(arraylist->data[17] == &variable4);
	assert(arraylist->data[18] == &variable1);

	/* Test inserting many entries */

	for (i=0; i<10000; ++i) {
		arraylist_insert(arraylist, 10, &variable1);
	}

	arraylist_free(arraylist);
}

void test_arraylist_remove_range(void)
{
	ArrayList *arraylist;

	arraylist = generate_arraylist();

	assert(arraylist->length == 16);
	assert(arraylist->data[3] == &variable4);
	assert(arraylist->data[4] == &variable1);
	assert(arraylist->data[5] == &variable2);
	assert(arraylist->data[6] == &variable3);

	arraylist_remove_range(arraylist, 4, 3);

	assert(arraylist->length == 13);
	assert(arraylist->data[3] == &variable4);
	assert(arraylist->data[4] == &variable4);
	assert(arraylist->data[5] == &variable1);
	assert(arraylist->data[6] == &variable2);

	/* Try some invalid ones and check they don't do anything */

	arraylist_remove_range(arraylist, 10, 10);
	arraylist_remove_range(arraylist, 0, 16);

	assert(arraylist->length == 13);

	arraylist_free(arraylist);
}

void test_arraylist_remove(void)
{
	ArrayList *arraylist;

	arraylist = generate_arraylist();

	assert(arraylist->length == 16);
	assert(arraylist->data[3] == &variable4);
	assert(arraylist->data[4] == &variable1);
	assert(arraylist->data[5] == &variable2);
	assert(arraylist->data[6] == &variable3);

	arraylist_remove(arraylist, 4);

	assert(arraylist->length == 15);
	assert(arraylist->data[3] == &variable4);
	assert(arraylist->data[4] == &variable2);
	assert(arraylist->data[5] == &variable3);
	assert(arraylist->data[6] == &variable4);

	/* Try some invalid removes */

	arraylist_remove(arraylist, 15);

	assert(arraylist->length == 15);

	arraylist_free(arraylist);
}

void test_arraylist_index_of(void)
{
	int entries[] = { 89, 4, 23, 42, 16, 15, 8, 99, 50, 30 };
	int num_entries;
	ArrayList *arraylist;
	int i;
	int index;
	int val;

	/* Generate an arraylist containing the entries in the array */

	num_entries = sizeof(entries) / sizeof(int);
	arraylist = arraylist_new(0);

	for (i=0; i<num_entries; ++i) {
		arraylist_append(arraylist, &entries[i]);
	}

	/* Check all values get found correctly */

	for (i=0; i<num_entries; ++i) {

		val = entries[i];

		index = arraylist_index_of(arraylist, int_equal, &val);

		assert(index == i);
	}

	/* Check invalid values */

	val = 0;
	assert(arraylist_index_of(arraylist, int_equal, &val) < 0);
	val = 57;
	assert(arraylist_index_of(arraylist, int_equal, &val) < 0);

	arraylist_free(arraylist);
}

void test_arraylist_clear(void)
{
	ArrayList *arraylist;

	arraylist = arraylist_new(0);

	/* Emptying an already-empty arraylist */

	arraylist_clear(arraylist);
	assert(arraylist->length == 0);

	/* Add some items and then empty it */

	arraylist_append(arraylist, &variable1);
	arraylist_append(arraylist, &variable2);
	arraylist_append(arraylist, &variable3);
	arraylist_append(arraylist, &variable4);

	arraylist_clear(arraylist);

	assert(arraylist->length == 0);

	arraylist_free(arraylist);
}

void test_arraylist_sort(void)
{
	ArrayList *arraylist;
	int entries[] = { 89, 4, 23, 42, 4, 16, 15, 4, 8, 99, 50, 30, 4 };
	int sorted[]  = { 4, 4, 4, 4, 8, 15, 16, 23, 30, 42, 50, 89, 99 };
	unsigned int num_entries = sizeof(entries) / sizeof(int);
	unsigned int i;

	arraylist = arraylist_new(10);

	for (i=0; i<num_entries; ++i) {
		arraylist_prepend(arraylist, &entries[i]);
	}

	arraylist_sort(arraylist, int_compare);

	/* List length is unchanged */

	assert(arraylist->length == num_entries);

	/* Check the list is sorted */

	for (i=0; i<num_entries; ++i) {
		int *value;

		value = (int *) arraylist->data[i];
		assert(*value == sorted[i]);
	}

	arraylist_free(arraylist);

	/* Check sorting an empty list */

	arraylist = arraylist_new(5);

	arraylist_sort(arraylist, int_compare);

	assert(arraylist->length == 0);

	arraylist_free(arraylist);

	/* Check sorting a list with 1 entry */

	arraylist = arraylist_new(5);

	arraylist_prepend(arraylist, &entries[0]);
	arraylist_sort(arraylist, int_compare);

	assert(arraylist->length == 1);
	assert(arraylist->data[0] == &entries[0]);

	arraylist_free(arraylist);
}

static UnitTestFunction tests[] = {
	test_arraylist_new_free,
	test_arraylist_append,
	test_arraylist_prepend,
	test_arraylist_insert,
	test_arraylist_remove,
	test_arraylist_remove_range,
	test_arraylist_index_of,
	test_arraylist_clear,
	test_arraylist_sort,
	NULL
};

int main(int argc, char *argv[])
{
	run_tests(tests);

	return 0;
}

