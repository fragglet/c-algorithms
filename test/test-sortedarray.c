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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "alloc-testing.h"
#include "framework.h"

#include "compare-int.h"
#include "sortedarray.h"

#define NUM_TEST_VALUES (sizeof(test_values) / sizeof(*test_values))

static int test_values[] = {
    114812, 292972, 15252,  317887, 859422, 943227, 173673, 444396, 289730,
    60903,  706503, 412815, -13616, 464193, 921380, 411002, 118983, 908936,
    854842, 228639, 175174, 976812, 963457, 39332,  774021, 588784, 23511,
    364428, 816641, 66433,  911779, 774060, 4340,   -46542, 739951, 388501,
    710893, 817647, 582295, 994147, 741106, 813303, 187471, 147041, 933029,
    933029, 933029, /* Duplicates */
    753121, 469556, 882575, 953070, 166462, -25609, 766862, 199480, 269323,
    636875, 49809,  633426, 153528, 325532, 15949,  418818, 541376, 950242,
    824802, 67683,  583518, 91497,  832324, 591778, 296072, 96531,  867789,
    126879, 716791, 685326, 826331, 677729, 496589, -6777,  667244, 446665,
    560213, 727965, 678769, 428202, 761385, 130289, 724727, 300728, 734018,
    493283, 770024, 472722, 123696, 301295, 511707, 383382, 151978};

/* The above array, sorted into order. */
static int sorted_test_values[] = {
    -46542, -25609, -13616, -6777,  4340,   15252,  15949,  23511,  39332,
    49809,  60903,  66433,  67683,  91497,  96531,  114812, 118983, 123696,
    126879, 130289, 147041, 151978, 153528, 166462, 173673, 175174, 187471,
    199480, 228639, 269323, 289730, 292972, 296072, 300728, 301295, 317887,
    325532, 364428, 383382, 388501, 411002, 412815, 418818, 428202, 444396,
    446665, 464193, 469556, 472722, 493283, 496589, 511707, 541376, 560213,
    582295, 583518, 588784, 591778, 633426, 636875, 667244, 677729, 678769,
    685326, 706503, 710893, 716791, 724727, 727965, 734018, 739951, 741106,
    753121, 761385, 766862, 770024, 774021, 774060, 813303, 816641, 817647,
    824802, 826331, 832324, 854842, 859422, 867789, 882575, 908936, 911779,
    921380, 933029, 933029, 933029, /* Duplicates */
    943227, 950242, 953070, 963457, 976812, 994147};

void check_sorted(SortedArray *sa)
{
	unsigned int i;
	for (i = 1; i < sortedarray_length(sa); i++) {
		assert(int_compare(sortedarray_get(sa, i - 1),
		                   sortedarray_get(sa, i)) <= 0);
	}
}

SortedArray *generate_sortedarray(void)
{
	SortedArray *sa;
	unsigned int i;

	sa = sortedarray_new(0, int_compare);

	/* Populate with contents of test_values. We do not allocate any
	   integers, just take pointers into test_values */
	for (i = 0; i < NUM_TEST_VALUES; ++i) {
		sortedarray_insert(sa, &test_values[i]);
	}

	return sa;
}

void test_sortedarray_new_free(void)
{
	SortedArray *sa;

	assert(sortedarray_new(0, NULL) == NULL);

	/* test normal */
	sa = sortedarray_new(0, int_compare);
	assert(sa != NULL);
	sortedarray_free(sa);

	/* freeing null */
	sortedarray_free(NULL);

	/* low memory */
	alloc_test_set_limit(0);
	sa = sortedarray_new(0, int_compare);
	assert(sa == NULL);

	alloc_test_set_limit(-1);
}

void test_sortedarray_insert(void)
{
	SortedArray *sa = generate_sortedarray();

	assert(sortedarray_insert(NULL, NULL) == 0);

	/* generate_sortedarray above already inserted the integers. */
	check_sorted(sa);
	sortedarray_free(sa);
}

void test_sortedarray_get(void)
{
	SortedArray *sa = generate_sortedarray();
	unsigned int i;
	int *got;

	for (i = 0; i < sortedarray_length(sa); i++) {
		got = sortedarray_get(sa, i);
		assert(got != NULL);
		assert(*got == sorted_test_values[i]);
	}

	/* Invalid indexes */
	assert(sortedarray_get(NULL, 0) == NULL);
	assert(sortedarray_get(sa, sortedarray_length(sa)) == NULL);
	assert(sortedarray_get(sa, 999999) == NULL);

	sortedarray_free(sa);
}

#define REMOVE_IDX_1 23
#define REMOVE_IDX_2 57
#define REMOVE_IDX_2_LEN 7
#define REMOVE_IDX_3 95
#define REMOVE_IDX_3_LEN 10
#define REMOVE_IDX_3_REAL_LEN 5
void test_sortedarray_remove(void)
{
	SortedArray *sa = generate_sortedarray();
	unsigned int i, check_idx;
	int *got;

	/* Reverse order here so the check_idx calculations below work */
	assert(sortedarray_remove_range(sa, REMOVE_IDX_3, REMOVE_IDX_3_LEN) !=
	       0);
	assert(sortedarray_remove_range(sa, REMOVE_IDX_2, REMOVE_IDX_2_LEN) !=
	       0);
	assert(sortedarray_remove(sa, REMOVE_IDX_1) != 0);

	/* Invalid indexes */
	assert(sortedarray_remove(NULL, 0) == 0);
	assert(sortedarray_remove(sa, sortedarray_length(sa)) == 0);
	assert(sortedarray_remove_range(sa, sortedarray_length(sa), 3) == 0);
	assert(sortedarray_remove(sa, 999999) == 0);
	assert(sortedarray_remove_range(sa, 999999, 44) == 0);

	check_sorted(sa);
	assert(sortedarray_length(sa) ==
	       NUM_TEST_VALUES - 1 - REMOVE_IDX_2_LEN - REMOVE_IDX_3_REAL_LEN);

	for (i = 0; i < sortedarray_length(sa); i++) {
		check_idx = i;
		if (check_idx >= REMOVE_IDX_1) {
			++check_idx;
		}
		if (check_idx >= REMOVE_IDX_2) {
			check_idx += REMOVE_IDX_2_LEN;
		}
		got = sortedarray_get(sa, i);
		assert(*got == sorted_test_values[check_idx]);
	}

	sortedarray_free(sa);
}

void test_sortedarray_index_of(void)
{
	SortedArray *sa = generate_sortedarray();
	unsigned int i;
	int got_idx, test_index;

	assert(sortedarray_index_of(NULL, NULL) == -1);

	/* Invalid index */
	test_index = 999999;
	assert(sortedarray_index_of(sa, &test_index) == -1);

	for (i = 0; i < NUM_TEST_VALUES; i++) {
		got_idx = sortedarray_index_of(sa, &sorted_test_values[i]);
		/* We cannot just check got_idx == i as there are duplicates */
		assert(sorted_test_values[got_idx] == sorted_test_values[i]);
	}

	sortedarray_free(sa);
}

void test_sortedarray_clear(void)
{
	SortedArray *sa = generate_sortedarray();

	sortedarray_clear(sa);
	assert(sortedarray_length(sa) == 0);
	sortedarray_free(sa);
}

/* clang-format off */
static UnitTestFunction tests[] = {
	test_sortedarray_new_free,
	test_sortedarray_insert,
	test_sortedarray_get,
	test_sortedarray_remove,
	test_sortedarray_index_of,
	test_sortedarray_clear,
	NULL
};
/* clang-format on */

int main(int argc, char *argv[])
{
	run_tests(tests);

	return 0;
}
