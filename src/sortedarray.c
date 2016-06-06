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

/**
 * @file sortedarray.c
 * 
 * @brief File containing the implementation of sortedarray.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sortedarray.h"

#ifdef ALLOC_TESTING
#include "alloc-testing.h"
#endif

/**
 * Definition of a @ref SortedArray
 */
struct _SortedArray {
	/**
	 * This field contains the actual array. The array always has a length
	 * of value of field length.
	 */
	SortedArrayValue *data;

	/**
	 * The length of the sorted array.
	 */
	unsigned int length;

	/**
	 * Field for internal usage only indicating how much memory already has
	 * been allocated for *data.
	 */
	unsigned int _alloced;

	/**
	 * The callback used to determine if two values equal.
	 */
	SortedArrayEqualFunc equ_func;

	/**
	 * The callback use to determine the order of two values.
	 */
	SortedArrayCompareFunc cmp_func;
};

/* Function for finding first index of range which equals data. An equal value
   must be present. */
static unsigned int sortedarray_first_index(SortedArray *sortedarray,
                                   SortedArrayValue data, unsigned int left,
                                   unsigned int right)
{
	unsigned int index = left;

	while (left < right) {
		index = (left + right) / 2;

		int order = sortedarray->cmp_func(data, 
		                                  sortedarray->data[index]);
		if (order > 0) {
			left = index + 1;
		} else {
			right = index;
		}
	}

	return index;
}

/* Function for finding last index of range which equals data. An equal value
   must be present. */
static unsigned int sortedarray_last_index(SortedArray *sortedarray, 
                                  SortedArrayValue data, unsigned int left, 
                                  unsigned int right)
{
	unsigned int index = right;

	while (left < right) {
		index = (left + right) / 2;

		int order = sortedarray->cmp_func(data, 
		                                  sortedarray->data[index]);
		if (order <= 0) {
			left = index + 1;
		} else {
			right = index;
		}
	}

	return index;
}

SortedArrayValue *sortedarray_get(SortedArray *array, unsigned int i)
{
	//check if array is NULL
	if (array == NULL) {
		return NULL;
	}

	//otherwise just return the element
	return array->data[i];	
}

unsigned int sortedarray_length(SortedArray *array)
{
	return array->length;
}

SortedArray *sortedarray_new(unsigned int length,
                             SortedArrayEqualFunc equ_func,
                             SortedArrayCompareFunc cmp_func)
{
	/* check input requirements */
	if (equ_func == NULL || cmp_func == NULL) {
		return NULL;
	}

	/* If length is 0, set it to a default. */
	if (length == 0) {
		length = 16;
	}

	SortedArrayValue *array = malloc(sizeof(SortedArrayValue) * length);

	/* on failure, return null */
	if (array == NULL) {
		return NULL;
	}

	SortedArray *sortedarray = malloc(sizeof(SortedArray));    

	/* check for failure */
	if (sortedarray == NULL) {
		free(array);
		return NULL;
	}
    
	/* init */
	sortedarray->data = array;
	sortedarray->length = 0;
	sortedarray->_alloced = length;
	sortedarray->equ_func = equ_func;
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

void sortedarray_remove(SortedArray *sortedarray, unsigned int index)
{
	/* same as remove range of length 1 */
	sortedarray_remove_range(sortedarray, index, 1);
}

void sortedarray_remove_range(SortedArray *sortedarray, unsigned int index,
                              unsigned int length)
{
	/* removal does not violate sorted property */

	/* check if valid range */
	if (index > sortedarray->length || index + length > sortedarray->length) {
		return;
	}

	/* move entries back */
	memmove(&sortedarray->data[index],
	        &sortedarray->data[index + length],
	        (sortedarray->length - (index + length)) 
	              * sizeof(SortedArrayValue));

	sortedarray->length -= length;
}

int sortedarray_insert(SortedArray *sortedarray, SortedArrayValue data)
{
	/* do a binary search like loop to find right position */
	unsigned int left  = 0;
	unsigned int right = sortedarray->length;
	unsigned int index = 0;

	/* When length is 1 set right to 0 so that the loop is not entered */	
	right = (right > 1) ? right : 0;

	while (left != right) {
		index = (left + right) / 2;

		int order = sortedarray->cmp_func(data, 
		                                  sortedarray->data[index]);
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
	if (sortedarray->length > 0 && sortedarray->cmp_func(data, 
	                       sortedarray->data[index]) > 0) {
		index++;
	}

	/* insert element at index */
	if (sortedarray->length + 1 > sortedarray->_alloced) {
		/* enlarge the array */
		unsigned int newsize;
		SortedArrayValue *data;

		newsize = sortedarray->_alloced * 2;
		data = realloc(sortedarray->data, sizeof(SortedArrayValue) * newsize);

		if (data == NULL) {
			return 0;
		} else {
			sortedarray->data = data;
			sortedarray->_alloced = newsize;
		}
	}

	/* move all other elements */
	memmove(&sortedarray->data[index + 1],
	        &sortedarray->data[index],
	        (sortedarray->length - index) * sizeof(SortedArrayValue));

	/* insert entry */
	sortedarray->data[index] = data;
	++(sortedarray->length);

	return 1;
}

int sortedarray_index_of(SortedArray *sortedarray, SortedArrayValue data)
{
	if (sortedarray == NULL) {
		return -1;
	}
	
	/* do a binary search */
	unsigned int left = 0;
	unsigned int right = sortedarray->length;
	unsigned int index = 0;

	/* safe subtract 1 of right without going negative */
	right = (right > 1) ? right : 0;

	while (left != right) {
		index = (left + right) / 2;

		int order = sortedarray->cmp_func(data, 
		                                  sortedarray->data[index]);
		if (order < 0) {
			/* value should be left */
			right = index;
		} else if (order > 0) {
			/* value should be right */
			left = index + 1;
		} else {
			/* no binary search can be done anymore, 
			   search linear now */
			left = sortedarray_first_index(sortedarray, data, left,
			                               index);
			right = sortedarray_last_index(sortedarray, data, 
			                               index, right);

			for (index = left; index <= right; index++) {
				if (sortedarray->equ_func(data, 
				                sortedarray->data[index])) {
					return (int) index;
				}
			}

			/* nothing is found */
			return -1;
		}
	}

	return -1;
}

void sortedarray_clear(SortedArray *sortedarray)
{
	/* set length to 0 */
	sortedarray->length = 0;
}
