 
/*
 
Copyright (c) 2005, Simon Howard
All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions 
are met:

 * Redistributions of source code must retain the above copyright 
   notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright 
   notice, this list of conditions and the following disclaimer in 
   the documentation and/or other materials provided with the 
   distribution.
 * Neither the name of the C Algorithms project nor the names of its 
   contributors may be used to endorse or promote products derived 
   from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
POSSIBILITY OF SUCH DAMAGE.

*/

#include <stdlib.h>
#include <assert.h>

#include "binary-heap.h"
#include "compare-int.h"

void test_binary_heap_new_free(void)
{
	BinaryHeap *heap;
	int i;

	for (i=0; i<1000; ++i) {
		heap = binary_heap_new(BINARY_HEAP_TYPE_MIN, int_compare);
		binary_heap_free(heap);
	}
}

void test_binary_heap_insert(void)
{
	BinaryHeap *heap;
	int *val;
	int i;

	heap = binary_heap_new(BINARY_HEAP_TYPE_MIN, int_compare);

	for (i=0; i<1000; ++i) {
		val = malloc(sizeof(int));
		*val = i;
		binary_heap_insert(heap, val);
	}
	assert(binary_heap_num_entries(heap) == 1000);

	binary_heap_free(heap);
}

void test_min_heap(void)
{
	BinaryHeap *heap;
	int *val;
	int i;

	heap = binary_heap_new(BINARY_HEAP_TYPE_MIN, int_compare);

	/* Push a load of values onto the heap */

	for (i=0; i<1000; ++i) {
		val = malloc(sizeof(int));
		*val = i;
		binary_heap_insert(heap, val);
	}

	/* Pop values off the heap and check they are in order */

	i = -1;
	while (binary_heap_num_entries(heap) > 0) {
		val = (int *) binary_heap_pop(heap);

		assert(*val == i + 1);
		i = *val;
	}

	/* Test popping from an empty heap */

	assert(binary_heap_num_entries(heap) == 0);
	assert(binary_heap_pop(heap) == BINARY_HEAP_NULL);

	binary_heap_free(heap);
}

void test_max_heap(void)
{
	BinaryHeap *heap;
	int *val;
	int i;

	heap = binary_heap_new(BINARY_HEAP_TYPE_MAX, int_compare);

	/* Push a load of values onto the heap */

	for (i=0; i<1000; ++i) {
		val = malloc(sizeof(int));
		*val = i;
		binary_heap_insert(heap, val);
	}

	/* Pop values off the heap and check they are in order */

	i = 1000;
	while (binary_heap_num_entries(heap) > 0) {
		val = (int *) binary_heap_pop(heap);

		assert(*val == i - 1);
		i = *val;
	}

	binary_heap_free(heap);
}

int main(int argc, char *argv[])
{
	test_binary_heap_new_free();
	test_binary_heap_insert();
	test_min_heap();
	test_max_heap();

	return 0;
}

