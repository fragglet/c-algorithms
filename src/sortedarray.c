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
#include <string.h>

#include "sortedarray.h"

#ifdef ALLOC_TESTING
#include "alloc-testing.h"
#endif

struct _SortedArray {
	SortedArrayValue *data;
	unsigned int length;
	unsigned int _alloced;
	SortedArrayCompareFunc cmp_func;
};

static const SortedArrayValue sortedarray_null_value = SORTED_ARRAY_NULL;

SortedArrayValue sortedarray_get(SortedArray *array, unsigned int i)
{
	if (array == NULL || i >= array->length) {
		return sortedarray_null_value;
	}

	return array->data[i];
}

unsigned int sortedarray_length(SortedArray *array)
{
	return array->length;
}

SortedArray *sortedarray_new(unsigned int length,
                             SortedArrayCompareFunc cmp_func)
{
	SortedArrayValue *array;
	SortedArray *sortedarray;

	if (cmp_func == NULL) {
		return NULL;
	}

	/* If length is 0, set it to a default. */
	if (length == 0) {
		length = 16;
	}

	array = malloc(sizeof(SortedArrayValue) * length);
	if (array == NULL) {
		return NULL;
	}

	sortedarray = malloc(sizeof(SortedArray));
	if (sortedarray == NULL) {
		free(array);
		return NULL;
	}

	sortedarray->data = array;
	sortedarray->length = 0;
	sortedarray->_alloced = length;
	sortedarray->cmp_func = cmp_func;

	return sortedarray;
}

void sortedarray_free(SortedArray *sortedarray)
{
	if (sortedarray != NULL) {
		free(sortedarray->data);
		free(sortedarray);
	}
}

int sortedarray_remove(SortedArray *sortedarray, unsigned int index)
{
	/* same as remove range of length 1 */
	return sortedarray_remove_range(sortedarray, index, 1);
}

int sortedarray_remove_range(SortedArray *sortedarray, unsigned int index,
                             unsigned int length)
{
	/* check if valid range */
	if (sortedarray == NULL || index >= sortedarray->length) {
		return 0;
	}
	if (index + length > sortedarray->length) {
		length = sortedarray->length - index;
	}

	/* move entries back */
	memmove(&sortedarray->data[index], &sortedarray->data[index + length],
	        (sortedarray->length - (index + length)) *
	            sizeof(SortedArrayValue));

	sortedarray->length -= length;

	return 1;
}

int sortedarray_insert(SortedArray *sortedarray, SortedArrayValue data)
{
	unsigned int left, right, index;
	int order;

	if (sortedarray == NULL) {
		return 0;
	}

	left = 0;
	right = sortedarray->length;
	index = 0;

	/* When length is 1 set right to 0 so that the loop is not entered */
	right = (right > 1) ? right : 0;

	while (left != right) {
		index = (left + right) / 2;
		order = sortedarray->cmp_func(data, sortedarray->data[index]);

		if (order < 0) {
			/* value should be left of index */
			right = index;
		} else if (order > 0) {
			/* value should be right of index */
			left = index + 1;
		} else {
			/* value should be at index */
			break;
		}
	}

	/* look whether the item should be put before or after the index */
	if (sortedarray->length > 0 &&
	    sortedarray->cmp_func(data, sortedarray->data[index]) > 0) {
		index++;
	}

	/* insert element at index */
	if (sortedarray->length + 1 > sortedarray->_alloced) {
		/* enlarge the array */
		unsigned int newsize;
		SortedArrayValue *data;

		newsize = sortedarray->_alloced * 2;
		data = realloc(sortedarray->data,
		               sizeof(SortedArrayValue) * newsize);

		if (data == NULL) {
			return 0;
		} else {
			sortedarray->data = data;
			sortedarray->_alloced = newsize;
		}
	}

	/* move all other elements */
	memmove(&sortedarray->data[index + 1], &sortedarray->data[index],
	        (sortedarray->length - index) * sizeof(SortedArrayValue));

	/* insert entry */
	sortedarray->data[index] = data;
	++sortedarray->length;

	return 1;
}

/* TODO: Refactor to eliminate duplicated binary search */
int sortedarray_index_of(SortedArray *sortedarray, SortedArrayValue data)
{
	unsigned int left, right, index;
	int order;

	if (sortedarray == NULL) {
		return -1;
	}

	left = 0;
	right = sortedarray->length;
	index = 0;

	/* safe subtract 1 of right without going negative */
	right = (right > 1) ? right : 0;

	while (left != right) {
		index = (left + right) / 2;
		order = sortedarray->cmp_func(data, sortedarray->data[index]);

		if (order < 0) {
			/* value should be left */
			right = index;
		} else if (order > 0) {
			/* value should be right */
			left = index + 1;
		} else {
			return (int) index;
		}
	}

	return -1;
}

void sortedarray_clear(SortedArray *sortedarray)
{
	/* set length to 0 */
	sortedarray->length = 0;
}
