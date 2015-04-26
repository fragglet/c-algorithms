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

#include "slist.h"
#include "compare-int.h"

int variable1 = 50, variable2, variable3, variable4;

/* Returns a list containing four entries */

SListEntry *generate_list(void)
{
	SListEntry *list = NULL;

	assert(slist_append(&list, &variable1) != NULL);
	assert(slist_append(&list, &variable2) != NULL);
	assert(slist_append(&list, &variable3) != NULL);
	assert(slist_append(&list, &variable4) != NULL);

	return list;
}

void test_slist_append(void)
{
	SListEntry *list = NULL;

	assert(slist_append(&list, &variable1) != NULL);
	assert(slist_append(&list, &variable2) != NULL);
	assert(slist_append(&list, &variable3) != NULL);
	assert(slist_append(&list, &variable4) != NULL);
	assert(slist_length(list) == 4);

	assert(slist_nth_data(list, 0) == &variable1);
	assert(slist_nth_data(list, 1) == &variable2);
	assert(slist_nth_data(list, 2) == &variable3);
	assert(slist_nth_data(list, 3) == &variable4);

	/* Test out of memory scenario */

	alloc_test_set_limit(0);
	assert(slist_length(list) == 4);
	assert(slist_append(&list, &variable1) == NULL);
	assert(slist_length(list) == 4);

	slist_free(list);
}

void test_slist_prepend(void)
{
	SListEntry *list = NULL;

	assert(slist_prepend(&list, &variable1) != NULL);
	assert(slist_prepend(&list, &variable2) != NULL);
	assert(slist_prepend(&list, &variable3) != NULL);
	assert(slist_prepend(&list, &variable4) != NULL);

	assert(slist_nth_data(list, 0) == &variable4);
	assert(slist_nth_data(list, 1) == &variable3);
	assert(slist_nth_data(list, 2) == &variable2);
	assert(slist_nth_data(list, 3) == &variable1);

	/* Test out of memory scenario */

	alloc_test_set_limit(0);
	assert(slist_length(list) == 4);
	assert(slist_prepend(&list, &variable1) == NULL);
	assert(slist_length(list) == 4);

	slist_free(list);
}

void test_slist_free(void)
{
	SListEntry *list;

	/* Create a list and free it */

	list = generate_list();

	slist_free(list);

	/* Check the empty list frees correctly */

	slist_free(NULL);
}

void test_slist_next(void)
{
	SListEntry *list;
	SListEntry *rover;

	list = generate_list();

	rover = list;
	assert(slist_data(rover) == &variable1);
	rover = slist_next(rover);
	assert(slist_data(rover) == &variable2);
	rover = slist_next(rover);
	assert(slist_data(rover) == &variable3);
	rover = slist_next(rover);
	assert(slist_data(rover) == &variable4);
	rover = slist_next(rover);
	assert(rover == NULL);

	slist_free(list);
}

void test_slist_nth_entry(void)
{
	SListEntry *list;
	SListEntry *entry;

	list = generate_list();

	/* Check all values in the list */

	entry = slist_nth_entry(list, 0);
	assert(slist_data(entry) == &variable1);
	entry = slist_nth_entry(list, 1);
	assert(slist_data(entry) == &variable2);
	entry = slist_nth_entry(list, 2);
	assert(slist_data(entry) == &variable3);
	entry = slist_nth_entry(list, 3);
	assert(slist_data(entry) == &variable4);

	/* Check out of range values */

	entry = slist_nth_entry(list, 4);
	assert(entry == NULL);
	entry = slist_nth_entry(list, 400);
	assert(entry == NULL);

	slist_free(list);
}

void test_slist_nth_data(void)
{
	SListEntry *list;

	list = generate_list();

	/* Check all values in the list */

	assert(slist_nth_data(list, 0) == &variable1);
	assert(slist_nth_data(list, 1) == &variable2);
	assert(slist_nth_data(list, 2) == &variable3);
	assert(slist_nth_data(list, 3) == &variable4);

	/* Check out of range values */

	assert(slist_nth_data(list, 4) == NULL);
	assert(slist_nth_data(list, 400) == NULL);

	slist_free(list);
}

void test_slist_length(void)
{
	SListEntry *list;

	/* Generate a list and check that it is four entries long */

	list = generate_list();

	assert(slist_length(list) == 4);

	/* Add an entry and check that it still works properly */

	slist_prepend(&list, &variable1);

	assert(slist_length(list) == 5);

	/* Check the length of the empty list */

	assert(slist_length(NULL) == 0);

	slist_free(list);
}

void test_slist_remove_entry(void)
{
	SListEntry *empty_list = NULL;
	SListEntry *list;
	SListEntry *entry;

	list = generate_list();

	/* Remove the third entry */

	entry = slist_nth_entry(list, 2);
	assert(slist_remove_entry(&list, entry) != 0);
	assert(slist_length(list) == 3);

	/* Remove the first entry */

	entry = slist_nth_entry(list, 0);
	assert(slist_remove_entry(&list, entry) != 0);
	assert(slist_length(list) == 2);

	/* Try some invalid removes */

	/* This was already removed: */

	assert(slist_remove_entry(&list, entry) == 0);

	/* NULL */

	assert(slist_remove_entry(&list, NULL) == 0);

	/* Removing NULL from an empty list */

	assert(slist_remove_entry(&empty_list, NULL) == 0);

	slist_free(list);
}

void test_slist_remove_data(void)
{
	int entries[] = { 89, 4, 23, 42, 4, 16, 15, 4, 8, 99, 50, 30, 4 };
	unsigned int num_entries = sizeof(entries) / sizeof(int);
	int val;
	SListEntry *list;
	unsigned int i;

	/* Generate a list containing all the entries in the array */

	list = NULL;

	for (i=0; i<num_entries; ++i) {
		slist_prepend(&list, &entries[i]);
	}

	/* Test removing invalid data */

	val = 0;
	assert(slist_remove_data(&list, int_equal, &val) == 0);
	val = 56;
	assert(slist_remove_data(&list, int_equal, &val) == 0);

	/* Remove the number 8 from the list */

	val = 8;
	assert(slist_remove_data(&list, int_equal, &val) == 1);
	assert(slist_length(list) == num_entries - 1);

	/* Remove the number 4 from the list (occurs multiple times) */

	val = 4;
	assert(slist_remove_data(&list, int_equal, &val) == 4);
	assert(slist_length(list) == num_entries - 5);

	/* Remove the number 89 from the list (first entry) */

	val = 89;
	assert(slist_remove_data(&list, int_equal, &val) == 1);
	assert(slist_length(list) == num_entries - 6);

	slist_free(list);
}

void test_slist_sort(void)
{
	SListEntry *list;
	int entries[] = { 89, 4, 23, 42, 4, 16, 15, 4, 8, 99, 50, 30, 4 };
	int sorted[]  = { 4, 4, 4, 4, 8, 15, 16, 23, 30, 42, 50, 89, 99 };
	unsigned int num_entries = sizeof(entries) / sizeof(int);
	unsigned int i;

	list = NULL;

	for (i=0; i<num_entries; ++i) {
		slist_prepend(&list, &entries[i]);
	}

	slist_sort(&list, int_compare);

	/* List length is unchanged */

	assert(slist_length(list) == num_entries);

	/* Check the list is sorted */

	for (i=0; i<num_entries; ++i) {
		int *value;

		value = (int *) slist_nth_data(list, i);
		assert(*value == sorted[i]);
	}

	slist_free(list);

	/* Check sorting an empty list */

	list = NULL;

	slist_sort(&list, int_compare);

	assert(list == NULL);
}

void test_slist_find_data(void)
{
	int entries[] = { 89, 23, 42, 16, 15, 4, 8, 99, 50, 30 };
	int num_entries = sizeof(entries) / sizeof(int);
	SListEntry *list;
	SListEntry *result;
	int i;
	int val;
	int *data;

	/* Generate a list containing the entries */

	list = NULL;
	for (i=0; i<num_entries; ++i) {
		slist_append(&list, &entries[i]);
	}

	/* Check that each value can be searched for correctly */

	for (i=0; i<num_entries; ++i) {

		val = entries[i];

		result = slist_find_data(list, int_equal, &val);

		assert(result != NULL);

		data = (int *) slist_data(result);
		assert(*data == val);
	}

	/* Check some invalid values return NULL */

	val = 0;
	assert(slist_find_data(list, int_equal, &val) == NULL);
	val = 56;
	assert(slist_find_data(list, int_equal, &val) == NULL);

	slist_free(list);
}

void test_slist_to_array(void)
{
	SListEntry *list;
	void **array;

	list = generate_list();

	array = slist_to_array(list);

	assert(array[0] == &variable1);
	assert(array[1] == &variable2);
	assert(array[2] == &variable3);
	assert(array[3] == &variable4);

	free(array);

	/* Test out of memory scenario */

	alloc_test_set_limit(0);

	array = slist_to_array(list);
	assert(array == NULL);

	slist_free(list);
}

void test_slist_iterate(void)
{
	SListEntry *list;
	SListIterator iter;
	int *data;
	int a;
	int i;
	int counter;

	/* Create a list with 50 entries */

	list = NULL;

	for (i=0; i<50; ++i) {
		slist_prepend(&list, &a);
	}

	/* Iterate over the list and count the number of entries visited */

	counter = 0;

	slist_iterate(&list, &iter);

	/* Test remove before slist_iter_next has been called */

	slist_iter_remove(&iter);

	/* Iterate over the list */

	while (slist_iter_has_more(&iter)) {

		data = (int *) slist_iter_next(&iter);

		++counter;

		/* Remove half the entries from the list */

		if ((counter % 2) == 0) {
			slist_iter_remove(&iter);

			/* Test double remove */

			slist_iter_remove(&iter);
		}
	}

	/* Test iter_next after iteration has completed. */

	assert(slist_iter_next(&iter) == SLIST_NULL);

	/* Test remove at the end of a list */

	slist_iter_remove(&iter);

	assert(counter == 50);
	assert(slist_length(list) == 25);

	slist_free(list);

	/* Test iterating over an empty list */

	list = NULL;
	counter = 0;

	slist_iterate(&list, &iter);

	while (slist_iter_has_more(&iter)) {

		data = (int *) slist_iter_next(&iter);

		++counter;

		/* Remove half the entries from the list */

		if ((counter % 2) == 0) {
			slist_iter_remove(&iter);
		}
	}

	assert(counter == 0);
}

/* Test that the iterator functions can survive removal of the current
 * value using the normal remove functions. */

void test_slist_iterate_bad_remove(void)
{
	SListEntry *list;
	SListIterator iter;
	int values[49];
	int i;
	int *val;

	/* Create a list with 49 entries */

	list = NULL;

	for (i=0; i<49; ++i) {
		values[i] = i;
		slist_prepend(&list, &values[i]);
	}

	/* Iterate over the list, removing each element in turn.  We
	 * use an odd number of list elements so that the first and
	 * last entries are removed. */

	slist_iterate(&list, &iter);

	while (slist_iter_has_more(&iter)) {
		val = slist_iter_next(&iter);

		/* Remove all the even numbers. Check that slist_iter_remove
		 * can cope with the fact that the current element has
		 * already been removed. */

		if ((*val % 2) == 0) {
			assert(slist_remove_data(&list, int_equal, val) != 0);
			slist_iter_remove(&iter);
		}
	}

	slist_free(list);
}

static UnitTestFunction tests[] = {
	test_slist_append,
	test_slist_prepend,
	test_slist_free,
	test_slist_next,
	test_slist_nth_entry,
	test_slist_nth_data,
	test_slist_length,
	test_slist_remove_entry,
	test_slist_remove_data,
	test_slist_sort,
	test_slist_find_data,
	test_slist_to_array,
	test_slist_iterate,
	test_slist_iterate_bad_remove,
	NULL
};

int main(int argc, char *argv[])
{
	run_tests(tests);

	return 0;
}

