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

#include "list.h"
#include "compare-int.h"

int variable1 = 50, variable2, variable3, variable4;

/* Returns a list containing four entries */

ListEntry *generate_list(void)
{
	ListEntry *list = NULL;

	assert(list_append(&list, &variable1) != NULL);
	assert(list_append(&list, &variable2) != NULL);
	assert(list_append(&list, &variable3) != NULL);
	assert(list_append(&list, &variable4) != NULL);

	return list;
}

void check_list_integrity(ListEntry *list)
{
	ListEntry *prev;
	ListEntry *rover;

	prev = NULL;
	rover = list;

	while (rover != NULL) {
		assert(list_prev(rover) == prev);
		prev = rover;
		rover = list_next(rover);
	}
}

void test_list_append(void)
{
	ListEntry *list = NULL;

	assert(list_append(&list, &variable1) != NULL);
	check_list_integrity(list);
	assert(list_append(&list, &variable2) != NULL);
	check_list_integrity(list);
	assert(list_append(&list, &variable3) != NULL);
	check_list_integrity(list);
	assert(list_append(&list, &variable4) != NULL);
	check_list_integrity(list);

	assert(list_length(list) == 4);

	assert(list_nth_data(list, 0) == &variable1);
	assert(list_nth_data(list, 1) == &variable2);
	assert(list_nth_data(list, 2) == &variable3);
	assert(list_nth_data(list, 3) == &variable4);

	/* Test out of memory scenario */

	alloc_test_set_limit(0);
	assert(list_length(list) == 4);
	assert(list_append(&list, &variable1) == NULL);
	assert(list_length(list) == 4);
	check_list_integrity(list);

	list_free(list);
}

void test_list_prepend(void)
{
	ListEntry *list = NULL;

	assert(list_prepend(&list, &variable1) != NULL);
	check_list_integrity(list);
	assert(list_prepend(&list, &variable2) != NULL);
	check_list_integrity(list);
	assert(list_prepend(&list, &variable3) != NULL);
	check_list_integrity(list);
	assert(list_prepend(&list, &variable4) != NULL);
	check_list_integrity(list);

	assert(list_nth_data(list, 0) == &variable4);
	assert(list_nth_data(list, 1) == &variable3);
	assert(list_nth_data(list, 2) == &variable2);
	assert(list_nth_data(list, 3) == &variable1);

	/* Test out of memory scenario */

	alloc_test_set_limit(0);
	assert(list_length(list) == 4);
	assert(list_prepend(&list, &variable1) == NULL);
	assert(list_length(list) == 4);
	check_list_integrity(list);

	list_free(list);
}

void test_list_free(void)
{
	ListEntry *list;

	/* Create a list and free it */

	list = generate_list();

	list_free(list);

	/* Check the empty list frees correctly */

	list_free(NULL);
}

void test_list_next(void)
{
	ListEntry *list;
	ListEntry *rover;

	list = generate_list();

	rover = list;
	assert(list_data(rover) == &variable1);
	rover = list_next(rover);
	assert(list_data(rover) == &variable2);
	rover = list_next(rover);
	assert(list_data(rover) == &variable3);
	rover = list_next(rover);
	assert(list_data(rover) == &variable4);
	rover = list_next(rover);
	assert(rover == NULL);

	list_free(list);
}

void test_list_nth_entry(void)
{
	ListEntry *list;
	ListEntry *entry;

	list = generate_list();

	/* Check all values in the list */

	entry = list_nth_entry(list, 0);
	assert(list_data(entry) == &variable1);
	entry = list_nth_entry(list, 1);
	assert(list_data(entry) == &variable2);
	entry = list_nth_entry(list, 2);
	assert(list_data(entry) == &variable3);
	entry = list_nth_entry(list, 3);
	assert(list_data(entry) == &variable4);

	/* Check out of range values */

	entry = list_nth_entry(list, 4);
	assert(entry == NULL);
	entry = list_nth_entry(list, 400);
	assert(entry == NULL);

	list_free(list);
}

void test_list_nth_data(void)
{
	ListEntry *list;

	list = generate_list();

	/* Check all values in the list */

	assert(list_nth_data(list, 0) == &variable1);
	assert(list_nth_data(list, 1) == &variable2);
	assert(list_nth_data(list, 2) == &variable3);
	assert(list_nth_data(list, 3) == &variable4);

	/* Check out of range values */

	assert(list_nth_data(list, 4) == NULL);
	assert(list_nth_data(list, 400) == NULL);

	list_free(list);
}

void test_list_length(void)
{
	ListEntry *list;

	/* Generate a list and check that it is four entries long */

	list = generate_list();

	assert(list_length(list) == 4);

	/* Add an entry and check that it still works properly */

	assert(list_prepend(&list, &variable1) != NULL);

	assert(list_length(list) == 5);

	list_free(list);

	/* Check the length of the empty list */

	assert(list_length(NULL) == 0);
}

void test_list_remove_entry(void)
{
	ListEntry *empty_list = NULL;
	ListEntry *list;
	ListEntry *entry;

	list = generate_list();

	/* Remove the third entry */

	entry = list_nth_entry(list, 2);
	assert(list_remove_entry(&list, entry) != 0);
	assert(list_length(list) == 3);
	check_list_integrity(list);

	/* Remove the first entry */

	entry = list_nth_entry(list, 0);
	assert(list_remove_entry(&list, entry) != 0);
	assert(list_length(list) == 2);
	check_list_integrity(list);

	/* Try some invalid removes */

	/* NULL */

	assert(list_remove_entry(&list, NULL) == 0);

	/* Removing NULL from an empty list */

	assert(list_remove_entry(&empty_list, NULL) == 0);

	list_free(list);

	/* Test removing an entry when it is the only entry. */

	list = NULL;
	assert(list_append(&list, &variable1) != NULL);
	assert(list != NULL);
	assert(list_remove_entry(&list, list) != 0);
	assert(list == NULL);

	/* Test removing the last entry */

	list = generate_list();
	entry = list_nth_entry(list, 3);
	assert(list_remove_entry(&list, entry) != 0);
	check_list_integrity(list);
	list_free(list);
}

void test_list_remove_data(void)
{
	int entries[] = { 89, 4, 23, 42, 4, 16, 15, 4, 8, 99, 50, 30, 4 };
	unsigned int num_entries = sizeof(entries) / sizeof(int);
	int val;
	ListEntry *list;
	unsigned int i;

	/* Generate a list containing all the entries in the array */

	list = NULL;

	for (i=0; i<num_entries; ++i) {
		assert(list_prepend(&list, &entries[i]) != NULL);
	}

	/* Test removing invalid data */

	val = 0;
	assert(list_remove_data(&list, int_equal, &val) == 0);
	val = 56;
	assert(list_remove_data(&list, int_equal, &val) == 0);
	check_list_integrity(list);

	/* Remove the number 8 from the list */

	val = 8;
	assert(list_remove_data(&list, int_equal, &val) == 1);
	assert(list_length(list) == num_entries - 1);
	check_list_integrity(list);

	/* Remove the number 4 from the list (occurs multiple times) */

	val = 4;
	assert(list_remove_data(&list, int_equal, &val) == 4);
	assert(list_length(list) == num_entries - 5);
	check_list_integrity(list);

	/* Remove the number 89 from the list (first entry) */

	val = 89;
	assert(list_remove_data(&list, int_equal, &val) == 1);
	assert(list_length(list) == num_entries - 6);
	check_list_integrity(list);

	list_free(list);
}

void test_list_sort(void)
{
	ListEntry *list;
	int entries[] = { 89, 4, 23, 42, 4, 16, 15, 4, 8, 99, 50, 30, 4 };
	int sorted[]  = { 4, 4, 4, 4, 8, 15, 16, 23, 30, 42, 50, 89, 99 };
	unsigned int num_entries = sizeof(entries) / sizeof(int);
	unsigned int i;

	list = NULL;

	for (i=0; i<num_entries; ++i) {
		assert(list_prepend(&list, &entries[i]) != NULL);
	}

	list_sort(&list, int_compare);

	/* List length is unchanged */

	assert(list_length(list) == num_entries);

	/* Check the list is sorted */

	for (i=0; i<num_entries; ++i) {
		int *value;

		value = (int *) list_nth_data(list, i);
		assert(*value == sorted[i]);
	}

	list_free(list);

	/* Check sorting an empty list */

	list = NULL;

	list_sort(&list, int_compare);

	assert(list == NULL);
}

void test_list_find_data(void)
{
	int entries[] = { 89, 23, 42, 16, 15, 4, 8, 99, 50, 30 };
	int num_entries = sizeof(entries) / sizeof(int);
	ListEntry *list;
	ListEntry *result;
	int i;
	int val;
	int *data;

	/* Generate a list containing the entries */

	list = NULL;
	for (i=0; i<num_entries; ++i) {
		assert(list_append(&list, &entries[i]) != NULL);
	}

	/* Check that each value can be searched for correctly */

	for (i=0; i<num_entries; ++i) {

		val = entries[i];

		result = list_find_data(list, int_equal, &val);

		assert(result != NULL);

		data = (int *) list_data(result);
		assert(*data == val);
	}

	/* Check some invalid values return NULL */

	val = 0;
	assert(list_find_data(list, int_equal, &val) == NULL);
	val = 56;
	assert(list_find_data(list, int_equal, &val) == NULL);

	list_free(list);
}

void test_list_to_array(void)
{
	ListEntry *list;
	void **array;

	list = generate_list();

	array = list_to_array(list);

	assert(array[0] == &variable1);
	assert(array[1] == &variable2);
	assert(array[2] == &variable3);
	assert(array[3] == &variable4);

	free(array);

	/* Test out of memory scenario */

	alloc_test_set_limit(0);

	array = list_to_array(list);
	assert(array == NULL);

	list_free(list);
}

void test_list_iterate(void)
{
	ListEntry *list;
	ListIterator iter;
	int i;
	int a;
	int counter;
	int *data;

	/* Create a list with 50 entries */

	list = NULL;

	for (i=0; i<50; ++i) {
		assert(list_prepend(&list, &a) != NULL);
	}

	/* Iterate over the list and count the number of entries visited */

	counter = 0;

	list_iterate(&list, &iter);

	/* Test remove before list_iter_next has been called */

	list_iter_remove(&iter);

	/* Iterate over the list */

	while (list_iter_has_more(&iter)) {
		data = (int *) list_iter_next(&iter);
		++counter;

		if ((counter % 2) == 0) {
			/* Delete half the entries in the list.  */

			list_iter_remove(&iter);

			/* Test double remove */

			list_iter_remove(&iter);
		}
	}

	/* Test iter_next after iteration has completed. */

	assert(list_iter_next(&iter) == NULL);

	/* Test remove at the end of a list */

	list_iter_remove(&iter);

	assert(counter == 50);
	assert(list_length(list) == 25);

	list_free(list);

	/* Test iterating over an empty list */

	list = NULL;
	counter = 0;

	list_iterate(&list, &iter);

	while (list_iter_has_more(&iter)) {
		data = (int *) list_iter_next(&iter);
		++counter;
	}

	assert(counter == 0);
}

/* Test that the iterator functions can survive removal of the current
 * value using the normal remove functions. */

void test_list_iterate_bad_remove(void)
{
	ListEntry *list;
	ListIterator iter;
	int values[49];
	int i;
	int *val;

	/* Create a list with 49 entries */

	list = NULL;

	for (i=0; i<49; ++i) {
		values[i] = i;
		assert(list_prepend(&list, &values[i]) != NULL);
	}

	/* Iterate over the list, removing each element in turn.  We
	 * use an odd number of list elements so that the first and
	 * last entries are removed. */

	list_iterate(&list, &iter);

	while (list_iter_has_more(&iter)) {
		val = list_iter_next(&iter);

		/* Remove all the even numbers. Check that list_iter_remove
		 * can cope with the fact that the current element has
		 * already been removed. */

		if ((*val % 2) == 0) {
			assert(list_remove_data(&list, int_equal, val) != 0);
			list_iter_remove(&iter);
		}
	}

	list_free(list);
}

static UnitTestFunction tests[] = {
	test_list_append,
	test_list_prepend,
	test_list_free,
	test_list_next,
	test_list_nth_entry,
	test_list_nth_data,
	test_list_length,
	test_list_remove_entry,
	test_list_remove_data,
	test_list_sort,
	test_list_find_data,
	test_list_to_array,
	test_list_iterate,
	test_list_iterate_bad_remove,
	NULL
};

int main(int argc, char *argv[])
{
	run_tests(tests);

	return 0;
}

