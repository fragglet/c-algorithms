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
 * @file sortedarray.h
 *
 * @brief Automatically sorted and resizing array
 *
 * An SortedArray is an automatically resizing sorted array. Most operations
 * run O(n) worst case running time. Some operations run in O(log n).
 *
 * To retrieve a value use the sortedarray structure by accessing the data
 * field.
 *
 * To create a SortedArray, use @ref sortedarray_new
 * To destroy a SortedArray, use @ref sortedarray_free
 *
 * To add a value to a SortedArray, use @ref sortedarray_prepend, 
 * @ref sortedarray_append, or @ref sortedarray_insert.
 *
 * To remove a value from a SortedArray, use @ref sortedarray_remove
 * or @ref sortedarray_remove_range.
 */

#ifndef ALGORITHM_SORTEDARRAY_H
#define ALGORITHM_SORTEDARRAY_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A value to store in @ref SortedArray.
 */
typedef void *SortedArrayValue;

/**
 * A SortedArray structure. Use @ref sortedarray_new to create one.
 *
 * The SortedArray is an automatically resizing array which stores its 
 * elements in sorted order. Userdefined functions determine the sorting order.
 * All operations on a SortedArray maintain the sorted property. Most 
 * operations are done in O(n) time, but searching can be done in O(log n)
 * worst case.
 *
 * @see sortedarray_new
 */
typedef struct _SortedArray SortedArray;

/**
 * Compare two values in a SortedArray to determine if they are equal.
 *
 * @param value1	The first value to compare.
 * @param value2	The second value to compare.
 * @return		Non-zero if value1 equals value2, zero if they do not
 *			equal.
 *
 */
typedef int (*SortedArrayEqualFunc)(SortedArrayValue value1,
                                    SortedArrayValue value2);

/**
 * Compare two values in a SortedArray to determine their order.
 *
 * @param value1	The first value to compare.
 * @param value2	The second value to compare.
 * @return		Less than zero if value1 is compared smaller than 
 * 			value2, zero if they compare equal, or greater than
 * 			zero if value1 compares greate than value2.
 */
typedef int (*SortedArrayCompareFunc)(SortedArrayValue value1,
                                      SortedArrayValue value2);

/**
 * @brief Function to retrieve element at index i from array
 *
 * @param array			The pointer to the sortedarray to retrieve the element from.
 * @param i				The index of the element to retrieve.
 * @return				The i-th element of the array, or NULL if array was NULL.
 */
SortedArrayValue *sortedarray_get(SortedArray *array, unsigned int i);

/**
 * @brief Function to retrieve the length of the SortedArray array.
 *
 * @param array			The array to retrieve the length from.
 * @return				The lenght of the SortedArray.
 */
unsigned int sortedarray_length(SortedArray *array);

/**
 * Allocate a new SortedArray for use.
 *
 * @param length        Indication to the amount of memory that should be 
 *                      allocated. If 0 is given, then a default is used.
 * @param equ_func      The function used to determine if two values in the
 *                      SortedArray equal. This may not be NULL.
 * @param cmp_func      The function used to determine the relative order of
 *                      two values in the SortedArray. This may not be NULL.
 *
 * @return              A new SortedArray or NULL if it was not possible to
 *                      allocate one.
 */
SortedArray *sortedarray_new(unsigned int length, 
                             SortedArrayEqualFunc equ_func, 
                             SortedArrayCompareFunc cmp_func);

/**
 * Frees a SortedArray from memory.
 *
 * @param sortedarray   The SortedArray to free.
 */
void sortedarray_free(SortedArray *sortedarray);

/**
 * Remove a value from a SortedArray at a specified index while maintaining the
 * sorted property.
 *
 * @param sortedarray   The SortedArray to remove a value from.
 * @param index         The index to remove from the array.
 */
void sortedarray_remove(SortedArray *sortedarray, unsigned int index);

/**
 * Remove a range of entities from a SortedArray while maintaining the sorted 
 * property.
 *
 * @param sortedarray   The SortedArray to remove the range of values from.
 * @param index         The starting index of the range to remove.
 * @param length        The length of the range to remove.
 */
void sortedarray_remove_range(SortedArray *sortedarray, unsigned int index,
                              unsigned int length);

/**
 * Insert a value into a SortedArray while maintaining the sorted property.
 *
 * @param sortedarray   The SortedArray to insert into.
 * @param data          The data to insert.
 *
 * @return              Zero on failure, or a non-zero value if successfull.
 */
int sortedarray_insert(SortedArray *sortedarray, SortedArrayValue data);

/**
 * Find the index of a value in a SortedArray.
 *
 * @param sortedarray   The SortedArray to find in.
 * @param data          The value to find.
 * @return              The index of the value or -1 if the value is not found.
 */
int sortedarray_index_of(SortedArray *sortedarray, SortedArrayValue data);

/**
 * Remove all values from a SortedArray.
 *
 * @param sortedarray   The SortedArray to clear.
 */
void sortedarray_clear(SortedArray *sortedarray);

#ifdef __cplusplus
}
#endif

#endif // #ifndef ALGORITHM_SORTEDARRAY_H
