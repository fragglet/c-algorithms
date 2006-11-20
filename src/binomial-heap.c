
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
#include <string.h>

#include "binomial-heap.h"

typedef struct _BinomialTree BinomialTree;

struct _BinomialTree
{
	void *value;
	unsigned short order;
	unsigned short refcount;
	BinomialTree **subtrees;
};

struct _BinomialHeap
{
	BinomialHeapType heap_type;
	BinomialHeapCompareFunc compare_func;
	int num_values;
	BinomialTree **roots;
	int roots_length;
};

static int binomial_heap_cmp(BinomialHeap *heap, void *data1, void *data2)
{
	if (heap->heap_type == BINOMIAL_HEAP_TYPE_MIN) {
		return heap->compare_func(data1, data2);
	} else {
		return -(heap->compare_func(data1, data2));
	}
}

static void binomial_tree_ref(BinomialTree *tree)
{
	if (tree != NULL) {
		++tree->refcount;
	}
}

static void binomial_tree_unref(BinomialTree *tree)
{
	int i;

	if (tree == NULL) {
		return;
	}
	
	/* Subtract a reference */

	--tree->refcount;

	/* If this removed the last reference, unreference all subtrees
	 * and free. */

	if (tree->refcount == 0) {

		for (i=0; i<tree->order; ++i) {
			binomial_tree_unref(tree->subtrees[i]);
		}
		
		free(tree->subtrees);
		free(tree);
	}
}

static BinomialTree *binomial_tree_merge(BinomialHeap *heap,
                                         BinomialTree *tree1,
                                         BinomialTree *tree2)
{
	BinomialTree *new_tree;
	BinomialTree *tmp;
	int i;

	/* Order tree1 and tree2 so that tree1 is the tree with the
	 * smallest root */

	if (binomial_heap_cmp(heap, tree1->value, tree2->value) > 0) {

		/* Swap tree1 and tree2 */

		tmp = tree1;
		tree1 = tree2;
		tree2 = tmp;
	}
	
	/* Allocate a new tree */

	new_tree = malloc(sizeof(BinomialTree));

	if (new_tree == NULL) {
		return NULL;
	}
	
	new_tree->order = tree1->order + 1;

	/* Take the smallest value of the two trees */

	new_tree->value = tree1->value;

	/* Copy subtrees of the smallest tree.  The last entry in the
	 * array is the larger tree */

	new_tree->subtrees = malloc(sizeof(BinomialTree *) * new_tree->order);
	memcpy(new_tree->subtrees, tree1->subtrees, 
	       sizeof(BinomialTree *) * tree1->order);
	new_tree->subtrees[new_tree->order - 1] = tree2;

	/* Add a reference to each of the subtrees we have referenced */

	for (i=0; i<new_tree->order; ++i) {
		binomial_tree_ref(new_tree->subtrees[i]);
	}
	
	return new_tree;
}

/* Used to perform an "undo" when an error occurs during 
 * binomial_heap_merge.  Go through the list of roots so far and remove
 * references that have been added. */

static void binomial_heap_merge_undo(BinomialTree **new_roots, int count)
{
	int i;

	for (i=0; i<=count; ++i) {
		binomial_tree_unref(new_roots[i]);
	}

	free(new_roots);
}

/* Merge the data in the 'other' heap into the 'heap' heap. 
 * Returns non-zero if successful. */

static int binomial_heap_merge(BinomialHeap *heap, BinomialHeap *other)
{
	BinomialTree **new_roots;
	int new_roots_length;
	BinomialTree *vals[3];
	int num_vals;
	BinomialTree *carry;
	BinomialTree *new_carry;
	int i;
	int max;

	/* Find the maximum length of the two heaps.  Add one because
	 * after merging we may have one more value to carry over. */

	if (heap->roots_length > other->roots_length) {
		max = heap->roots_length + 1;
	} else {
		max = other->roots_length + 1;
	}

	/* Allocate an array for the new roots */

	new_roots = malloc(sizeof(BinomialTree *) * max);

	if (new_roots == NULL) {
		return 0;
	}

	/* Go through one entry at a time.  This works kind of like a
	 * ripple-carry adder. */

	new_roots_length = 0;
	carry = NULL;

	for (i=0; i<max; ++i) {
	
		/* Build up 'vals' as a list of all the values we must
		 * merge at this step. */

		num_vals = 0;

		/* If there is a value in 'heap', add it */

		if (i < heap->roots_length && heap->roots[i] != NULL) {
			vals[num_vals] = heap->roots[i];
			++num_vals;
		}

		/* If there is a value in 'other', add it */

		if (i < other->roots_length && other->roots[i] != NULL) {
			vals[num_vals] = other->roots[i];
			++num_vals;
		}

		/* If there is a carried value from the previous iteration,
		 * add it */

		if (carry != NULL) {
			vals[num_vals] = carry;
			++num_vals;
		}

		/* When num_vals == 1 or 3, we store a value. */

		if ((num_vals & 1) != 0) {

			/* Save the last value into new_roots. */

			new_roots[i] = vals[num_vals - 1];
			binomial_tree_ref(new_roots[i]);
			new_roots_length = i + 1;

		} else {

			/* No value to store at this iteration */

			new_roots[i] = NULL;
		}

		/* When num_vals == 2 or 3, we must carry over to the
		 * next iteration */

		if ((num_vals & 2) != 0) {

			/* Merge the first two values and carry to the
			 * next iteration */

			new_carry = binomial_tree_merge(heap,
			                                vals[0],
			                                vals[1]);

			if (new_carry == NULL) {

				/* Remove references that we have added 
				 * (freeing any BinomialTree structures 
				 * that were created in the process) */

				binomial_heap_merge_undo(new_roots, i);

				/* Unreference the carry variable */

				binomial_tree_unref(carry);

				return 0;
			}

		} else {

			/* Nothing to carry */

			new_carry = NULL;
		}

		/* Unreference previous carried value */

		binomial_tree_unref(carry);
		
		/* Assign the new value of carry, and add a reference */

		carry = new_carry;

		binomial_tree_ref(carry);
	}

	/* Unreference all values in the old 'roots' array, freeing unused
	 * BinomialTree structures as necessary. */

	for (i=0; i<heap->roots_length; ++i) {
		if (heap->roots[i] != NULL) {
			binomial_tree_unref(heap->roots[i]);
		}
	}

	/* Free the old roots array and use the new one */
	
	free(heap->roots);
	heap->roots = new_roots;
	heap->roots_length = new_roots_length;

	/* Merged successfully */

	return 1;
}

BinomialHeap *binomial_heap_new(BinomialHeapType heap_type,
                                BinomialHeapCompareFunc compare_func)
{
	BinomialHeap *new_heap;

	/* Allocate a new heap */
	
	new_heap = calloc(1, sizeof(BinomialHeap));

	if (new_heap == NULL) {
		return NULL;
	}

	/* Initialise and return */

	new_heap->heap_type = heap_type;
	new_heap->compare_func = compare_func;

	return new_heap;
}

void binomial_heap_free(BinomialHeap *heap)
{
	int i;

	/* Unreference all trees in the heap.  This should free
	 * back all subtrees. */

	for (i=0; i<heap->roots_length; ++i) {
		binomial_tree_unref(heap->roots[i]);
	}
	
	/* Free the heap itself */

	free(heap->roots);
	free(heap);
}

int binomial_heap_insert(BinomialHeap *heap, void *value)
{
	BinomialHeap fake_heap;
	BinomialTree *new_tree;
	int result;

	/* Allocate an order 0 tree for storing the new value */

	new_tree = malloc(sizeof(BinomialTree));

	if (new_tree == NULL) {
		return 0;
	}

	/* Fill in values.  This has an initial reference count of 1 that
	 * the "fake" heap holds; this will be removed at the end of 
	 * this function. */
	
	new_tree->value = value;
	new_tree->order = 0;
	new_tree->refcount = 1;
	new_tree->subtrees = NULL;

	/* Build a fake heap structure for merging */

	fake_heap.heap_type = heap->heap_type;
	fake_heap.compare_func = heap->compare_func;
	fake_heap.num_values = 1;
	fake_heap.roots = &new_tree;
	fake_heap.roots_length = 1;

	/* Perform the merge */

	result = binomial_heap_merge(heap, &fake_heap);

	if (result != 0) {
		++heap->num_values;
	}

	/* Remove reference to the new tree. */

	binomial_tree_unref(new_tree);

	return result;
}

void *binomial_heap_pop(BinomialHeap *heap)
{
	BinomialTree *least_tree;
	BinomialHeap fake_heap;
	void *result;
	int least;
	int i;

	if (heap->num_values == 0) {
		return NULL;
	}
	
	/* Find the tree with the lowest root value */
	
	least = -1;

	for (i=0; i<heap->roots_length; ++i) {

		if (heap->roots[i] == NULL) {
			continue;
		}
		
		if (least < 0 
		 || binomial_heap_cmp(heap, 
		                      heap->roots[i]->value, 
		                      heap->roots[least]->value) < 0) {
			least = i;
		}
	}

	/* Remove the least_tree from the heap. */

	least_tree = heap->roots[least];
	heap->roots[least] = NULL;

	/* Construct a fake heap containing the data in the least tree */

	fake_heap.heap_type = heap->heap_type;
	fake_heap.compare_func = heap->compare_func;
	fake_heap.roots = least_tree->subtrees;
	fake_heap.roots_length = least_tree->order;

	/* Merge subtrees of least tree back into the heap */

	if (binomial_heap_merge(heap, &fake_heap)) {

		/* Merge successful */

		/* Remove reference to least tree */

		result = least_tree->value;
		binomial_tree_unref(least_tree);

		/* Update the number of values */

		--heap->num_values;

		return result;
		
	} else {

		/* Add the least tree back */

		heap->roots[least] = least_tree;

		/* Pop failed */

		return NULL;
	}
}

int binomial_heap_num_entries(BinomialHeap *heap)
{
	return heap->num_values;
}

