
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

/** 
 * @file arraylist.h
 *
 * @brief Automatically resizing array 
 *
 * ArrayLists are arrays of pointers which automatically increase in 
 * size.
 *
 * To create an ArrayList, use @ref arraylist_new.
 * To destroy an ArrayList, use @ref arraylist_free.
 *
 * To add data an ArrayList, use @ref arraylist_prepend, 
 * @ref arraylist_append, or @ref arraylist_insert.
 *
 * To remove data from an ArrayList, use @ref arraylist_remove
 * or @ref arraylist_remove_range.
 */

#ifndef ALGORITHM_ARRAYLIST_H
#define ALGORITHM_ARRAYLIST_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * An ArrayList structure.  New ArrayLists can be created using the 
 * arraylist_new function.
 *
 * @see arraylist_new 
 */

typedef struct _ArrayList {

	/** Entries in the array */
	
	void **data;

	/** Length of the array */
	
	int length;

	/** Private data and should not be accessed */

	int _alloced;
} ArrayList;

/**
 * Compare two values in an arraylist to determine if they are equal.
 *
 * @return Non-zero if the values are not equal, zero if they are equal.
 */

typedef int (*ArrayListEqualFunc)(void *data1, void *data2);

/**
 * Compare two values in an arraylist.  Used by @ref arraylist_sort
 * when sorting values.
 *
 * @param data1               The first value.
 * @param data2               The second value.
 * @return                    A negative number if data1 should be sorted
 *                            before data2, a positive number if data2 should
 *                            be sorted before data1, zero if the two values
 *                            are equal.
 */

typedef int (*ArrayListCompareFunc)(void *data1, void *data2);

/**
 * Allocate a new ArrayList for use.
 *
 * @param length         Hint to the initialise function as to the amount
 *                       of memory to allocate initially to the ArrayList.
 *
 * @see arraylist_free
 */

ArrayList *arraylist_new(int length);

/**
 * Destroy an ArrayList and free back the memory it uses.
 *
 * @param arraylist      The ArrayList to free.
 */

void arraylist_free(ArrayList *arraylist);

/**
 * Append data to the end of an ArrayList.
 *
 * @param arraylist      The ArrayList.
 * @param data           The data to append.
 */

void arraylist_append(ArrayList *arraylist, void *data);

/** 
 * Prepend data to the beginning of an ArrayList.
 *
 * @param arraylist      The ArrayList.
 * @param data           The data to prepend.
 */

void arraylist_prepend(ArrayList *arraylist, void *data);

/**
 * Remove the entry at the specified location in an ArrayList.
 *
 * @param arraylist      The ArrayList.
 * @param index          The index of the entry to remove.
 */

void arraylist_remove(ArrayList *arraylist, int index);

/**
 * Remove a range of entries at the specified location in an ArrayList.
 *
 * @param arraylist      The ArrayList.
 * @param index          The index of the start of the range to remove.
 * @param length         The length of the range to remove.
 */

void arraylist_remove_range(ArrayList *arraylist, int index, int length);

/**
 * Insert new data at the specified index in an ArrayList.
 * The index where new data can be inserted is limited by the 
 * size of the ArrayList.
 *
 * @param arraylist      The ArrayList.
 * @param index          The index at which to insert the data.
 * @param data           The data.
 * @return               Returns zero if unsuccessful, else non-zero 
 *                       if successful.
 */

int arraylist_insert(ArrayList *arraylist, int index, void *data);

/**
 * Find the index of a particular pointer in an ArrayList.
 *
 * @param arraylist      The ArrayList to search.
 * @param callback       Callback function to be invoked to determine if 
 *                       values are equal to the data to search for.
 * @param data           The data to search for.
 * @return               The index of the data if found, or -1 if not found.
 */

int arraylist_index_of(ArrayList *arraylist, 
                       ArrayListEqualFunc callback, 
                       void *data);

/** 
 * Remove all entries from an ArrayList.
 *
 * @param arraylist      The ArrayList.
 */

void arraylist_clear(ArrayList *arraylist);

/** 
 * Sort the values in an ArrayList.
 *
 * @param arraylist      The ArrayList.
 * @param compare_func   Function used to compare values in sorting.
 */

void arraylist_sort(ArrayList *arraylist, ArrayListCompareFunc compare_func);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef ALGORITHM_ARRAYLIST_H */

