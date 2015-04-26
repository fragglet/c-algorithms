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
#include <string.h>

#include "arraylist.h"

/* malloc() / free() testing */

#ifdef ALLOC_TESTING
#include "alloc-testing.h"
#endif

/* Automatically resizing array */

ArrayList *arraylist_new(unsigned int length)
{
	ArrayList *new_arraylist;

	/* If the length is not specified, use a sensible default */

	if (length <= 0) {
		length = 16;
	}

	/* Allocate the new ArrayList and fill in the fields.  There are
	 * initially no entries. */

	new_arraylist = (ArrayList *) malloc(sizeof(ArrayList));

	if (new_arraylist == NULL) {
		return NULL;
	}

	new_arraylist->_alloced = length;
	new_arraylist->length = 0;

	/* Allocate the data array */

	new_arraylist->data = malloc(length * sizeof(ArrayListValue));

	if (new_arraylist->data == NULL) {
		free(new_arraylist);
		return NULL;
	}

	return new_arraylist;
}

void arraylist_free(ArrayList *arraylist)
{
	/* Do not free if a NULL pointer is passed */

	if (arraylist != NULL) {
		free(arraylist->data);
		free(arraylist);
	}
}

static int arraylist_enlarge(ArrayList *arraylist)
{
	ArrayListValue *data;
	unsigned int newsize;

	/* Double the allocated size */

	newsize = arraylist->_alloced * 2;

	/* Reallocate the array to the new size */

	data = realloc(arraylist->data, sizeof(ArrayListValue) * newsize);

	if (data == NULL) {
		return 0;
	} else {
		arraylist->data = data;
		arraylist->_alloced = newsize;

		return 1;
	}
}

int arraylist_insert(ArrayList *arraylist, unsigned int index,
                     ArrayListValue data)
{
	/* Sanity check the index */

	if (index > arraylist->length) {
		return 0;
	}

	/* Increase the size if necessary */

	if (arraylist->length + 1 > arraylist->_alloced) {
		if (!arraylist_enlarge(arraylist)) {
			return 0;
		}
	}

	/* Move the contents of the array forward from the index
	 * onwards */

	memmove(&arraylist->data[index + 1],
	        &arraylist->data[index],
	        (arraylist->length - index) * sizeof(ArrayListValue));

	/* Insert the new entry at the index */

	arraylist->data[index] = data;
	++arraylist->length;

	return 1;
}

int arraylist_append(ArrayList *arraylist, ArrayListValue data)
{
	return arraylist_insert(arraylist, arraylist->length, data);
}

int arraylist_prepend(ArrayList *arraylist, ArrayListValue data)
{
	return arraylist_insert(arraylist, 0, data);
}

void arraylist_remove_range(ArrayList *arraylist, unsigned int index,
                            unsigned int length)
{
	/* Check this is a valid range */

	if (index > arraylist->length || index + length > arraylist->length) {
		return;
	}

	/* Move back the entries following the range to be removed */

	memmove(&arraylist->data[index],
	        &arraylist->data[index + length],
	        (arraylist->length - (index + length))
	            * sizeof(ArrayListValue));

	/* Decrease the counter */

	arraylist->length -= length;
}

void arraylist_remove(ArrayList *arraylist, unsigned int index)
{
	arraylist_remove_range(arraylist, index, 1);
}

int arraylist_index_of(ArrayList *arraylist,
                       ArrayListEqualFunc callback,
                       ArrayListValue data)
{
	unsigned int i;

	for (i=0; i<arraylist->length; ++i) {
		if (callback(arraylist->data[i], data) != 0)
			return (int) i;
	}

	return -1;
}

void arraylist_clear(ArrayList *arraylist)
{
	/* To clear the list, simply set the length to zero */

	arraylist->length = 0;
}

static void arraylist_sort_internal(ArrayListValue *list_data,
                                    unsigned int list_length,
                                    ArrayListCompareFunc compare_func)
{
	ArrayListValue pivot;
	ArrayListValue tmp;
	unsigned int i;
	unsigned int list1_length;
	unsigned int list2_length;

	/* If less than two items, it is always sorted. */

	if (list_length <= 1) {
		return;
	}

	/* Take the last item as the pivot. */

	pivot = list_data[list_length-1];

	/* Divide the list into two lists:
	 *
	 * List 1 contains data less than the pivot.
	 * List 2 contains data more than the pivot.
	 *
	 * As the lists are build up, they are stored sequentially after
	 * each other, ie. list_data[list1_length-1] is the last item
	 * in list 1, list_data[list1_length] is the first item in
	 * list 2.
	 */

	list1_length = 0;

	for (i=0; i<list_length-1; ++i) {

		if (compare_func(list_data[i], pivot) < 0) {

			/* This should be in list 1.  Therefore it is in the
			 * wrong position. Swap the data immediately following
			 * the last item in list 1 with this data. */

			tmp = list_data[i];
			list_data[i] = list_data[list1_length];
			list_data[list1_length] = tmp;

			++list1_length;

		} else {
			/* This should be in list 2.  This is already in the
			 * right position. */
		}
	}

	/* The length of list 2 can be calculated. */

	list2_length = list_length - list1_length - 1;

	/* list_data[0..list1_length-1] now contains all items which are
	 * before the pivot.
	 * list_data[list1_length..list_length-2] contains all items after
	 * or equal to the pivot. */

	/* Move the pivot into place, by swapping it with the item
	 * immediately following the end of list 1.  */

	list_data[list_length-1] = list_data[list1_length];
	list_data[list1_length] = pivot;

	/* Recursively sort the sublists. */

	arraylist_sort_internal(list_data, list1_length, compare_func);

	arraylist_sort_internal(&list_data[list1_length + 1], list2_length,
	                        compare_func);
}

void arraylist_sort(ArrayList *arraylist, ArrayListCompareFunc compare_func)
{
	/* Perform the recursive sort */

	arraylist_sort_internal(arraylist->data, arraylist->length,
	                        compare_func);
}

