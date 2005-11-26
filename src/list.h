
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
 * @file list.h
 *
 * @brief Doubly-linked list.
 *
 * A doubly-linked list stores pointers in a list, with the ability
 * to iterate in either direction.
 *
 * To create an empty list, create a new variable which is a pointer to
 * a @ref ListEntry structure, and initialise it to NULL.
 * To destroy an entire list, use @ref list_free.
 *
 * To add data to a list, use @ref list_append or @ref list_prepend.
 *
 * To remove data from a list, use @ref list_remove_entry or 
 * @ref list_remove_data.
 *
 * To iterate over entries in a list, use @ref list_next and @ref list_prev,
 * along with the @ref list_data function to access data.
 *
 * To access an entry in the list by index, use @ref list_nth_entry or
 * @ref list_nth_data.
 *
 * To sort a list, use @ref list_sort.
 *
 */

#ifndef ALGORITHM_SLIST_H
#define ALGORITHM_SLIST_H

/**
 * Represents an entry in a doubly-linked list.  The empty list is
 * represented by a NULL pointer. To initialise a new doubly linked 
 * list, simply create a variable of this type 
 * containing a pointer to NULL.
 */

typedef struct _ListEntry ListEntry;

/**
 * Callback function used for iterating over a list.
 *
 * @param data        The element being iterated over.
 * @param user_data   Extra data specified by the user.
 */

typedef void (*ListIterator)(void *data, void *user_data);

/**
 * Callback function used to compare values in a list when sorting.
 *
 * @param data1       The first value to compare.
 * @param data2       The second value to compare.
 * @return            A negative value if data1 should be sorted before 
 *                    data2, a positive value if data1 should be sorted 
 *                    after data2, zero if data1 and data2 are equal.
 */

typedef int (*ListCompareFunc)(void *data1, void *data2);

/**
 * Callback function used to determine of two values in a list are
 * equal.
 *
 * @param data1       The first value to compare.
 * @param data2       The second value to compare.
 * @return            A non-zero value if data1 and data2 are equal, zero
 *                    if they are not equal.
 */

typedef int (*ListEqualFunc)(void *data1, void *data2);

/**
 * Free an entire list.
 *
 * @param list           The list to free.
 */

void list_free(ListEntry *list);

/**
 * Prepend data to the start of a list.
 *
 * @param list      Pointer to the list to prepend to.
 * @param data      Data to prepend.
 * @return          The new entry in the list.
 */

ListEntry *list_prepend(ListEntry **list, void *data);

/**
 * Append data to the end of a list.
 *
 * @param list      Pointer to the list to append to.
 * @param data      Data to append.
 * @return          The new entry in the list.
 */

ListEntry *list_append(ListEntry **list, void *data);

/** 
 * Retrieve the previous entry in a list.
 *
 * @param listentry    Pointer to the list entry.
 * @return             The previous entry in the list, or NULL if this 
 *                     was the first entry in the list.
 */

ListEntry *list_prev(ListEntry *listentry);

/** 
 * Retrieve the next entry in a list.
 *
 * @param listentry    Pointer to the list entry.
 * @return             The next entry in the list, or NULL if this was the
 *                     last entry in the list.
 */

ListEntry *list_next(ListEntry *listentry);

/**
 * Retrieve the data at a list entry.
 *
 * @param listentry    Pointer to the list entry.
 * @return             The data at the list entry.
 */

void *list_data(ListEntry *listentry);

/** 
 * Retrieve the entry at a specified index in a list.
 *
 * @param list       The list.
 * @param n          The index into the list .
 * @return           The entry at the specified index, or NULL if out of range.
 */

ListEntry *list_nth_entry(ListEntry *list, int n);

/** 
 * Retrieve the data at a specified entry in the list.
 *
 * @param list       The list.
 * @param n          The index into the list .
 * @return           The data at the specified index, or NULL if 
 *                   unsuccessful.
 */

void *list_nth_data(ListEntry *list, int n);

/** 
 * Find the length of a list.
 *
 * @param list       The list.
 * @return           The number of entries in the list.
 */

int list_length(ListEntry *list);

/** 
 * Iterate over all entries in a list.
 *
 * @param list       The list.
 * @param callback   Callback function to invoke for each entry in the list.
 * @param user_data  Extra data to pass to the callback function.
 */

void list_foreach(ListEntry *list, ListIterator callback, void *user_data);

/**
 * Remove an entry from a list.
 *
 * @param list       Pointer to the list.
 * @param entry      The list entry to remove .
 * @return           If the entry is not found in the list, returns zero,
 *                   else returns non-zero.
 */

int list_remove_entry(ListEntry **list, ListEntry *entry);

/**
 * Remove all occurrences of a particular piece of data from a list.
 *
 * @param list       Pointer to the list.
 * @param callback   Function to invoke to compare data against the 
 *                   data to be removed.
 * @param data       The data to remove from the list.
 * @return           The number of entries removed from the list.
 */

int list_remove_data(ListEntry **list, ListEqualFunc callback, void *data);

/**
 * Sort a list.
 *
 * @param list          Pointer to the list to sort.
 * @param compare_func  Function used to compare values in the list.
 */

void list_sort(ListEntry **list, ListCompareFunc compare_func);

/**
 * Find the entry for a particular data item in a list.
 *
 * @param list           The list to search.
 * @param callback       Callback function to be invoked to determine if
 *                       values are equal to the data to search for.
 * @param data           The data to search for.
 * @return               The list entry of the item being searched for, or
 *                       NULL if not found.
 */

ListEntry *list_find_data(ListEntry *list, 
                          ListEqualFunc callback,
                          void *data);

#endif /* #ifndef ALGORITHM_SLIST_H */

