
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

#include "binary-heap.h"

struct _BinaryHeap {
	BinaryHeapType heap_type;
	void **values;
	int num_values;
	int alloced_size;
	BinaryHeapCompareFunc compare_func;
};

static int binary_heap_cmp(BinaryHeap *heap, void *data1, void *data2)
{
	if (heap->heap_type == BINARY_HEAP_TYPE_MIN) {
		return heap->compare_func(data1, data2);
	} else {
		return -heap->compare_func(data1, data2);
	}
}

BinaryHeap *binary_heap_new(BinaryHeapType heap_type,
                            BinaryHeapCompareFunc compare_func)
{
	BinaryHeap *heap;

	heap = malloc(sizeof(BinaryHeap));

	if (heap == NULL) {
		return NULL;
	}
	
	heap->heap_type = heap_type;
	heap->num_values = 0;
	heap->compare_func = compare_func;
	
	/* Initial size of 16 elements */

	heap->alloced_size = 16;
	heap->values = malloc(sizeof(void *) * heap->alloced_size);

	if (heap->values == NULL) {
		free(heap);
		return NULL;
	}
	
	return heap;
}

void binary_heap_free(BinaryHeap *heap)
{
	free(heap->values);
	free(heap);
}

int binary_heap_insert(BinaryHeap *heap, void *value)
{
	void **new_values;
	int index;
	int newsize;
	int parent;

	/* Possibly realloc the heap to a larger size */

	if (heap->num_values >= heap->alloced_size) {

		/* Double the table size */
		
		newsize = heap->alloced_size * 2;
		new_values = realloc(heap->values, sizeof(void *) * newsize);

		if (new_values == NULL) {
			return 0;
		}
		
		heap->alloced_size = newsize;
		heap->values = new_values;
	}

	/* Add to the bottom of the heap and start from there */

	index = heap->num_values;
	++heap->num_values;

	/* Percolate the value up to the top of the heap */

	while (index > 0) {

		/* The parent index is found by halving the node index */

		parent = (index - 1) / 2;

		/* Compare the node with its parent */

		if (binary_heap_cmp(heap, heap->values[parent], value) < 0) {
			
			/* Ordered correctly - insertion is complete */

			break;

		} else {

			/* Need to swap this node with its parent */

			heap->values[index] = heap->values[parent];

			/* Advance up to the parent */

			index = parent;
		}
	}

	/* Save the new value in the final location */

	heap->values[index] = value;

	return 1;
}

void *binary_heap_pop(BinaryHeap *heap)
{
	void *result;
	void *new_value;
	int index;
	int next_index;
	int child1, child2;

	/* Empty heap? */

	if (heap->num_values == 0) {
		return NULL;
	}

	/* Take the value from the top of the heap */

	result = heap->values[0];

	/* Remove the last value from the heap; we will percolate this down
	 * from the top. */

	new_value = heap->values[heap->num_values - 1];
	--heap->num_values;

	/* Percolate the new top value down */

	index = 0;

	for (;;) {

		/* Calculate the array indexes of the children of this node */
		
		child1 = index * 2 + 1;
		child2 = index * 2 + 2;

		if (child1 < heap->num_values
		 && binary_heap_cmp(heap, 
		                    new_value, 
		                    heap->values[child1]) > 0) {

			/* Left child is less than the node.  We need to swap
			 * with one of the children, whichever is less. */

			if (child2 < heap->num_values
			 && binary_heap_cmp(heap,
			                    heap->values[child1],
			                    heap->values[child2]) > 0) {
				next_index = child2;
			} else {
				next_index = child1;
			}
			
		} else if (child2 < heap->num_values
		        && binary_heap_cmp(heap, 
		                           new_value, 
		                           heap->values[child2]) > 0) {

			/* Right child is less than the node.  Swap with the 
			 * right child. */

			next_index = child2;

		} else {
			/* Node is less than both its children.  The heap condition
			 * is satisfied.  We can stop percolating down. */

			heap->values[index] = new_value;
			break;
		}

		/* Swap the current node with the least of the child nodes. */

		heap->values[index] = heap->values[next_index];

		/* Advance to the child we chose */

		index = next_index;
	}

	return result;
}

int binary_heap_num_entries(BinaryHeap *heap)
{
	return heap->num_values;
}

