
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

/* A singly-linked list */

#ifndef ALGORITHM_SLIST_H
#define ALGORITHM_SLIST_H

/**
 * Represents an entry in a singly-linked list.  The empty list is
 * represented by a NULL pointer. To initialise a new singly linked 
 * list, simply create a variable of this type 
 * containing a pointer to NULL.
 */

typedef struct _SListEntry SListEntry;

/**
 * Callback function used for iterating over a list
 */

typedef void (*SListIterator)(void *data, void *user_data);

/**
 * Callback function used to compare values in a list when sorting
 *
 * @return   A negative value if data1 should be sorted before data2, 
 *           a positive value if data1 should be sorted after data2, 
 *           zero if data1 and data2 are equal.
 */

typedef int (*SListCompareFunc)(void *data1, void *data2);

/**
 * Callback function used to determine of two values in a list are
 * equal
 *
 * @return   A non-zero value if data1 and data2 are equal, zero if they
 *           are not equal.
 */

typedef int (*SListEqualFunc)(void *data1, void *data2);

/**
 * Free an entire list
 *
 * @param list           The list to free
 */

void slist_free(SListEntry *list);

/**
 * Prepend some new data to the end of a list.
 *
 * @param list      Pointer to the list to prepend to
 * @param data      Data to prepend
 * @return          The new entry in the list
 */

SListEntry *slist_prepend(SListEntry **list, void *data);

/**
 * Append some new data to the end of a list.
 *
 * @param list      Pointer to the list to append to
 * @param data      Data to append
 * @return          The new entry in the list
 */

SListEntry *slist_append(SListEntry **list, void *data);

/** 
 * Retrieve the next entry in a list.
 *
 * @param listentry    Pointer to the list entry
 * @return             The next entry in the list
 */

SListEntry *slist_next(SListEntry *listentry);

/**
 * Retrieve the data at a list entry
 *
 * @param listentry    Pointer to the list entry
 * @return             The data at the list entry
 */

void *slist_data(SListEntry *listentry);

/** 
 * Retrieve the entry at a specified index in a list
 *
 * @param list       The list
 * @param n          The index into the list 
 * @return           The entry at the specified index, or NULL if out of range
 */

SListEntry *slist_nth_entry(SListEntry *list, int n);

/** 
 * Retrieve the data at a specified entry in the list
 *
 * @param list       The list
 * @param n          The index into the list 
 * @return           The data at the specified index, or NULL if 
 *                   unsuccessful
 */

void *slist_nth_data(SListEntry *list, int n);

/** 
 * Find the length of a list
 *
 * @param list       The list
 * @return           The number of entries in the list 
 */

int slist_length(SListEntry *list);

/** 
 * Iterate over all entries in a list
 *
 * @param list       The list
 * @param callback   Callback function to invoke for each entry in the list
 * @param user_data  Extra data to pass to the callback function
 */

void slist_foreach(SListEntry *list, SListIterator callback, void *user_data);

/**
 * Remove an entry from a list 
 *
 * @param list       Pointer to the list
 * @param entry      The list entry to remove 
 * @return           If the entry is not found in the list, returns zero,
 *                   else returns non-zero
 */

int slist_remove_entry(SListEntry **list, SListEntry *entry);

/**
 * Remove all occurrences of a particular piece of data from a list
 *
 * @param list       Pointer to the list
 * @param callback   Callback function to invoke to compare data in the 
 *                   list with the data to remove
 * @param data       The data to remove from the list
 * @return           The number of entries removed from the list
 */

int slist_remove_data(SListEntry **list, SListEqualFunc callback, void *data);

/**
 * Sort a list
 *
 * @param list          Pointer to the list to sort
 * @param compare_func  Function used to compare values in the list
 */

void slist_sort(SListEntry **list, SListCompareFunc compare_func);

/**
 * Finds the entry for a particular data item in a list.
 *
 * @param list           The list to search
 * @param callback       Callback function to be invoked to determine if
 *                       values are equal to the data to search for
 * @param data           The data to search for
 * @return               The list entry of the item being searched for, or
 *                       NULL if not found.
 */

SListEntry *slist_find_data(SListEntry *list, 
                            SListEqualFunc callback,
                            void *data);

#endif /* #ifndef ALGORITHM_SLIST_H */

