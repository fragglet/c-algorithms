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

#include <stdlib.h>
#include <assert.h>

#include "alloc-testing.h"
#include "framework.h"

#include "binomial-heap.h"
#include "compare-int.h"

int test_array[1000];

void test_binomial_heap_new_free(void)
{
	BinomialHeap *heap;
	int i;

	for (i=0; i<1000; ++i) {
		heap = binomial_heap_new(BINOMIAL_HEAP_TYPE_MIN, int_compare);
		binomial_heap_free(heap);
	}
}

void test_binomial_heap_insert(void)
{
	BinomialHeap *heap;
	int i;

	heap = binomial_heap_new(BINOMIAL_HEAP_TYPE_MIN, int_compare);

	for (i=0; i<1000; ++i) {
		test_array[i] = i;
		binomial_heap_insert(heap, &test_array[i]);
	}
	assert(binomial_heap_num_entries(heap) == 1000);

	binomial_heap_free(heap);
}

void test_min_heap(void)
{
	BinomialHeap *heap;
	int *val;
	int i;

	heap = binomial_heap_new(BINOMIAL_HEAP_TYPE_MIN, int_compare);

	/* Push a load of values onto the heap */

	for (i=0; i<1000; ++i) {
		test_array[i] = i;
		binomial_heap_insert(heap, &test_array[i]);
	}

	/* Pop values off the heap and check they are in order */

	i = -1;
	while (binomial_heap_num_entries(heap) > 0) {
		val = (int *) binomial_heap_pop(heap);

		assert(*val == i + 1);
		i = *val;
	}

	binomial_heap_free(heap);
}

void test_max_heap(void)
{
	BinomialHeap *heap;
	int *val;
	int i;

	heap = binomial_heap_new(BINOMIAL_HEAP_TYPE_MAX, int_compare);

	/* Push a load of values onto the heap */

	for (i=0; i<1000; ++i) {
		test_array[i] = i;
		binomial_heap_insert(heap, &test_array[i]);
	}

	/* Pop values off the heap and check they are in order */

	i = 1000;
	while (binomial_heap_num_entries(heap) > 0) {
		val = (int *) binomial_heap_pop(heap);

		assert(*val == i - 1);
		i = *val;
	}

	binomial_heap_free(heap);
}

static UnitTestFunction tests[] = {
	test_binomial_heap_new_free,
	test_binomial_heap_insert,
	test_min_heap,
	test_max_heap,
	NULL
};

int main(int argc, char *argv[])
{
	run_tests(tests);

	return 0;
}

