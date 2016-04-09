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

 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "alloc-testing.h"
#include "framework.h"

#include "sortedarray.h"

#define TEST_SIZE 100

static int cmp_int(SortedArrayValue v1, SortedArrayValue v2)
{
	int *i1 = (int*) v1;
	int *i2 = (int*) v2;

	if (i1 == i2) {
		return 0;
	}

	if (i1 == NULL || i2 == NULL) {
		return (i1 == NULL) ? 1 : -1;
	}

	/* discard last 2 digits, for testing difference between elements with
	   same order but which are not equal. See index_of */
	int a = (*i1)/10;
	int b = (*i2)/10;

	int r = 0;

	if (a == b) {
		r = 0;
	}
	else {
		r = (a > b) ? 1 : -1;
	}

	return r;
}

static int equ_int(SortedArrayValue v1, SortedArrayValue v2)
{
	int *i1 = (int*) v1;
	int *i2 = (int*) v2;

	if (i1 == i2) {
		return 1;
	}

	if (i1 == NULL || i2 == NULL) {
		return 0;
	}

	return *i1 == *i2;
}

void check_sorted_prop(SortedArray *sortedarray)
{
	unsigned int i;
	for (i = 1; i < sortedarray->length; i++) {
		assert(cmp_int(
					sortedarray->data[i - 1],
					sortedarray->data[i]) <= 0);
	}
}

void free_sorted_ints(SortedArray *sortedarray)
{
	unsigned int i;
	for (i = 0; i < sortedarray->length; i++) {
		int *pi = sortedarray->data[i];
		free(pi);
	}

	sortedarray_free(sortedarray);
}

SortedArray *generate_sortedarray(void)
{
	/* generate a sorted array of length TEST_SIZE, filled with random 
	   numbers. */
	SortedArray *sortedarray;
	unsigned int i;

	sortedarray = sortedarray_new(0, equ_int, cmp_int);

	/* set random seed */
	srand((unsigned int) time(NULL));

	/* fill with random numbers */
	for (i = 0; i < TEST_SIZE; ++i) {
		int r = rand();
		r = (int) (((float) r)/((float) RAND_MAX) * 100);
		int *pi = malloc(sizeof(int));
		*pi = r;
		sortedarray_insert(sortedarray, pi);
	}

	return sortedarray;
}

void test_sortedarray_new_free(void)
{
	SortedArray *sortedarray;

	/* test normal */
	sortedarray = sortedarray_new(0, equ_int, cmp_int);
	assert(sortedarray != NULL);
	sortedarray_free(sortedarray);

	/* freeing null */
	sortedarray_free(NULL);

	/* low memory */
	alloc_test_set_limit(0);
	sortedarray = sortedarray_new(0, equ_int, cmp_int);
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
	int *ip25 = sortedarray->data[25];
	int i25 = *ip25;
	free(sortedarray->data[24]);
	sortedarray_remove(sortedarray, 24);
	assert(*((int*) sortedarray->data[24]) == i25);

	check_sorted_prop(sortedarray);
	free_sorted_ints(sortedarray);
}

void test_sortedarray_remove_range(void)
{
	SortedArray *sortedarray = generate_sortedarray();

	/* remove 66 -> 70 */
	int i70 = *((int*) sortedarray->data[70]);
	int i71 = *((int*) sortedarray->data[71]);
	int i72 = *((int*) sortedarray->data[72]);
	int i73 = *((int*) sortedarray->data[73]);
	free(sortedarray->data[66]);
	free(sortedarray->data[67]);
	free(sortedarray->data[68]);
	free(sortedarray->data[69]);
	sortedarray_remove_range(sortedarray, 66, 4);
	assert(*((int*) sortedarray->data[66]) == i70);
	assert(*((int*) sortedarray->data[67]) == i71);
	assert(*((int*) sortedarray->data[68]) == i72);
	assert(*((int*) sortedarray->data[69]) == i73);

	check_sorted_prop(sortedarray);
	free_sorted_ints(sortedarray);
}

void test_sortedarray_index_of(void)
{
	SortedArray *sortedarray = generate_sortedarray();
	unsigned int i;

	/* insert 10 values with different fractional part */
	int base = (int) ((((float) rand())/((float) RAND_MAX)) * 100);
	for (i = 0; i < 10; i++) {
		int i = base + (int) (((float) rand())/((float) RAND_MAX));
		int *ip = malloc(sizeof(int));
		*ip = i;
		sortedarray_insert(sortedarray, ip);
	}

	/* find starting index */
	unsigned int start = 0;
	for (i = 0; i < sortedarray->length; i++) {
		if ((int) *((int*) sortedarray->data[i]) == base) {
			start = i;
			break;
		}
	}

	/* search for 5th number */
	int index = sortedarray_index_of(sortedarray, sortedarray->data[start + 5]);
	int number = *((int*) sortedarray->data[index]);
	int searched = *((int*) sortedarray->data[start + 5]);
	assert(index == (int) start + 5 || number == searched);

	free_sorted_ints(sortedarray);
}

static UnitTestFunction tests[] = {
	test_sortedarray_new_free,
	test_sortedarray_insert,
	test_sortedarray_remove,
	test_sortedarray_remove_range,
	test_sortedarray_index_of,
	NULL   
};

int main(int argc, char *argv[])
{
	run_tests(tests);

	return 0;
}
