/*

Copyright (c) 2016, Stefan Cloudt

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
)
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "alloc-testing.h"
#include "framework.h"

#include "compare-int.h"
#include "sortedarray.h"

#define TEST_SIZE 20
#define TEST_ARRAY {10, 12, 12, 1, 2, 3, 6, 7, 2, 23, 13, 23, 23, 34, 31, 9,\
   	21, -2, -12, -4}
#define TEST_REMOVE_EL 15
#define TEST_REMOVE_RANGE 7
#define TEST_REMOVE_RANGE_LENGTH 4

void check_sorted_prop(SortedArray *sortedarray)
{
	unsigned int i;
	for (i = 1; i < sortedarray_length(sortedarray); i++) {
		assert(int_compare(
		                   sortedarray_get(sortedarray, i-1),
						   sortedarray_get(sortedarray, i)) <= 0);
	}
}

void free_sorted_ints(SortedArray *sortedarray)
{
	unsigned int i;
	for (i = 0; i < sortedarray_length(sortedarray); i++) {
		int *pi = (int*) sortedarray_get(sortedarray, i);
		free(pi);
	}

	sortedarray_free(sortedarray);
}

SortedArray *generate_sortedarray_equ(SortedArrayEqualFunc equ_func)
{
	/* generate a sorted array of length TEST_SIZE, filled with random 
	   numbers. */
	SortedArray *sortedarray;
	unsigned int i;

	int array[TEST_SIZE] = TEST_ARRAY;

	sortedarray = sortedarray_new(0, equ_func, int_compare);

	for (i = 0; i < TEST_SIZE; ++i) {
		int *pi = malloc(sizeof(int));
		*pi = array[i];
		sortedarray_insert(sortedarray, pi);
	}

	return sortedarray;
}

SortedArray *generate_sortedarray(void)
{
	return generate_sortedarray_equ(int_equal);
}

void test_sortedarray_new_free(void)
{
	SortedArray *sortedarray;

	/* test normal */
	sortedarray = sortedarray_new(0, int_equal, int_compare);
	assert(sortedarray != NULL);
	sortedarray_free(sortedarray);

	/* freeing null */
	sortedarray_free(NULL);

	/* low memory */
	alloc_test_set_limit(0);
	sortedarray = sortedarray_new(0, int_equal, int_compare);
	assert(sortedarray == NULL);

	alloc_test_set_limit(-1);
}

void test_sortedarray_insert(void)
{
	SortedArray *sortedarray = generate_sortedarray();
	unsigned int i;

	/* insert a few random numbers, then check if everything is sorted */
	for (i = 0; i < 20; i++) {
		int i = (int) (((float) rand())/((float) RAND_MAX) * 100);
		int *pi = malloc(sizeof(int));
		*pi = i;
		sortedarray_insert(sortedarray, pi);
	}

	check_sorted_prop(sortedarray);
	free_sorted_ints(sortedarray);
}

void test_sortedarray_remove(void)
{
	SortedArray *sortedarray = generate_sortedarray();

	/* remove index 24 */
	int *ip = (int*) sortedarray_get(sortedarray, TEST_REMOVE_EL + 1);
	int i = *ip;
	free((int*) sortedarray_get(sortedarray, TEST_REMOVE_EL));
	sortedarray_remove(sortedarray, TEST_REMOVE_EL);
	assert(*((int*) sortedarray_get(sortedarray, TEST_REMOVE_EL)) == i);

	check_sorted_prop(sortedarray);
	free_sorted_ints(sortedarray);
}

void test_sortedarray_remove_range(void)
{
	SortedArray *sortedarray = generate_sortedarray();

	/* get values in test range */
	int new[TEST_REMOVE_RANGE_LENGTH];
	unsigned int i;
	for (i = 0; i < TEST_REMOVE_RANGE_LENGTH; i++) {
		new[i] = *((int*) sortedarray_get(sortedarray, TEST_REMOVE_RANGE + 
		                                    TEST_REMOVE_RANGE_LENGTH + i));
	}
	
	/* free removed elements */
	for (i = 0; i < TEST_REMOVE_RANGE_LENGTH; i++) {
		free((int*) sortedarray_get(sortedarray, TEST_REMOVE_RANGE + i));
	}

	/* remove */
	sortedarray_remove_range(sortedarray, TEST_REMOVE_RANGE, 
			TEST_REMOVE_RANGE_LENGTH);
	
	/* assert */
	for (i = 0; i < TEST_REMOVE_RANGE_LENGTH; i++) {
		assert(*((int*) sortedarray_get(sortedarray, TEST_REMOVE_RANGE + i)) == 
		                                                                new[i]);
	}

	check_sorted_prop(sortedarray);
	free_sorted_ints(sortedarray);
}

void test_sortedarray_index_of(void) {
	SortedArray *sortedarray = generate_sortedarray();

	unsigned int i;
	for (i = 0; i < TEST_SIZE; i++) {
		int r = sortedarray_index_of(sortedarray, 
		                sortedarray_get(sortedarray, i));
		assert(r >= 0);
		assert(*((int*) sortedarray_get(sortedarray,(unsigned int) r)) == 
		        *((int*) sortedarray_get(sortedarray, i)));
	}
	
	free_sorted_ints(sortedarray);
}

static int ptr_equal(SortedArrayValue v1, SortedArrayValue v2) {
	return v1 == v2;
}

void test_sortedarray_index_of_equ_key(void)
{
	/* replace equal function by function which checks pointers */
	SortedArray *sortedarray = generate_sortedarray_equ(ptr_equal);
	unsigned int i;

	/* check if all search value return the same index */
	for (i = 0; i < TEST_SIZE; i++) {
		int r = sortedarray_index_of(sortedarray, 
		                             sortedarray_get(sortedarray, i));
		assert(r >= 0);
		assert(i == (unsigned int) r);
	}

	free_sorted_ints(sortedarray);
}

void test_sortedarray_get(void) {
	unsigned int i;

	SortedArray *arr = generate_sortedarray();

	for (i = 0; i < sortedarray_length(arr); i++) {
		assert(sortedarray_get(arr, i) == sortedarray_get(arr, i));
		assert(*((int*) sortedarray_get(arr, i)) == 
		       *((int*) sortedarray_get(arr, i)));
	}

	free_sorted_ints(arr);
}

static UnitTestFunction tests[] = {
	test_sortedarray_new_free,
	test_sortedarray_insert,
	test_sortedarray_remove,
	test_sortedarray_remove_range,
	test_sortedarray_index_of,
	test_sortedarray_index_of_equ_key,
	test_sortedarray_get,
	NULL   
};

int main(int argc, char *argv[])
{
	run_tests(tests);

	return 0;
}
