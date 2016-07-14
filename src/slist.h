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

/**
 * @file slist.h
 *
 * Singly-linked list.
 *
 * A singly-linked list stores a collection of values.  Each
 * entry in the list (represented by a pointer to a @ref SListEntry
 * structure) contains a link to the next entry.  It is only
 * possible to iterate over entries in a singly linked list in one
 * direction.
 *
 * To create a new singly-linked list, create a variable which is
 * a pointer to a @ref SListEntry, and initialise it to NULL.
 *
 * To destroy a singly linked list, use @ref slist_free.
 *
 * To add a new value at the start of a list, use @ref slist_prepend.
 * To add a new value at the end of a list, use @ref slist_append.
 *
 * To find the length of a list, use @ref slist_length.
 *
 * To access a value in a list by its index in the list, use
 * @ref slist_nth_data.
 *
 * To search a list for a value, use @ref slist_find_data.
 *
 * To sort a list into an order, use @ref slist_sort.
 *
 * To find a particular entry in a list by its index, use
 * @ref slist_nth_entry.
 *
 * To iterate over each value in a list, use @ref slist_iterate to
 * initialise a @ref SListIterator structure, with @ref slist_iter_next
 * and @ref slist_iter_has_more to retrieve each value in turn.
 * @ref slist_iter_remove can be used to efficiently remove the
 * current entry from the list.
 *
 * Given a particular entry in a list (@ref SListEntry):
 *
 * @li To find the next entry, use @ref slist_next.
 * @li To access the value stored at the entry, use @ref slist_data.
 * @li To set the value stored at the entry, use @ref slist_set_data.
 * @li To remove the entry, use @ref slist_remove_entry.
 *
 */

#ifndef ALGORITHM_SLIST_H
#define ALGORITHM_SLIST_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Represents an entry in a singly-linked list.  The empty list is
 * represented by a NULL pointer. To initialise a new singly linked
 * list, simply create a variable of this type
 * containing a pointer to NULL.
 */

typedef struct _SListEntry SListEntry;

/**
 * Structure used to iterate over a list.
 */

typedef struct _SListIterator SListIterator;

/**
 * Value stored in a list.
 */

typedef void *SListValue;

/**
 * Definition of a @ref SListIterator.
 */

struct _SListIterator {
	SListEntry **prev_next;
	SListEntry *current;
};

/**
 * A null @ref SListValue.
 */

#define SLIST_NULL ((void *) 0)

/**
 * Callback function used to compare values in a list when sorting.
 *
 * @return   A negative value if value1 should be sorted before value2,
 *           a positive value if value1 should be sorted after value2,
 *           zero if value1 and value2 are equal.
 */

typedef int (*SListCompareFunc)(SListValue value1, SListValue value2);

/**
 * Callback function used to determine of two values in a list are
 * equal.
 *
 * @return   A non-zero value if value1 and value2 are equal, zero if they
 *           are not equal.
 */

typedef int (*SListEqualFunc)(SListValue value1, SListValue value2);

/**
 * Free an entire list.
 *
 * @param list           The list to free.
 */

void slist_free(SListEntry *list);

/**
 * Prepend a value to the start of a list.
 *
 * @param list      Pointer to the list to prepend to.
 * @param data      The value to prepend.
 * @return          The new entry in the list, or NULL if it was not possible
 *                  to allocate a new entry.
 */

SListEntry *slist_prepend(SListEntry **list, SListValue data);

/**
 * Append a value to the end of a list.
 *
 * @param list      Pointer to the list to append to.
 * @param data      The value to append.
 * @return          The new entry in the list, or NULL if it was not possible
 *                  to allocate a new entry.
 */

SListEntry *slist_append(SListEntry **list, SListValue data);

/**
 * Retrieve the next entry in a list.
 *
 * @param listentry    Pointer to the list entry.
 * @return             The next entry in the list.
 */

SListEntry *slist_next(SListEntry *listentry);

/**
 * Retrieve the value stored at a list entry.
 *
 * @param listentry    Pointer to the list entry.
 * @return             The value at the list entry.
 */

SListValue slist_data(SListEntry *listentry);

/**
 * Set the value at a list entry. The value provided will be written to the 
 * given listentry. If listentry is NULL nothing is done.
 *
 * @param listentry 	Pointer to the list entry.
 * @param value			The value to set.
 */
void slist_set_data(SListEntry *listentry, SListValue value);

/**
 * Retrieve the entry at a specified index in a list.
 *
 * @param list       The list.
 * @param n          The index into the list .
 * @return           The entry at the specified index, or NULL if out of range.
 */

SListEntry *slist_nth_entry(SListEntry *list, unsigned int n);

/**
 * Retrieve the value stored at a specified index in the list.
 *
 * @param list       The list.
 * @param n          The index into the list.
 * @return           The value stored at the specified index, or
 *                   @ref SLIST_NULL if unsuccessful.
 */

SListValue slist_nth_data(SListEntry *list, unsigned int n);

/**
 * Find the length of a list.
 *
 * @param list       The list.
 * @return           The number of entries in the list.
 */

unsigned int slist_length(SListEntry *list);

/**
 * Create a C array containing the contents of a list.
 *
 * @param list       The list.
 * @return           A newly-allocated C array containing all values in the
 *                   list, or NULL if it was not possible to allocate the
 *                   memory for the array.  The length of the array is
 *                   equal to the length of the list (see @ref slist_length).
 */

SListValue *slist_to_array(SListEntry *list);

/**
 * Remove an entry from a list.
 *
 * @param list       Pointer to the list.
 * @param entry      The list entry to remove.
 * @return           If the entry is not found in the list, returns zero,
 *                   else returns non-zero.
 */

int slist_remove_entry(SListEntry **list, SListEntry *entry);

/**
 * Remove all occurrences of a particular value from a list.
 *
 * @param list       Pointer to the list.
 * @param callback   Callback function to invoke to compare values in the
 *                   list with the value to remove.
 * @param data       The value to remove from the list.
 * @return           The number of entries removed from the list.
 */

unsigned int slist_remove_data(SListEntry **list,
                               SListEqualFunc callback,
                               SListValue data);

/**
 * Sort a list.
 *
 * @param list          Pointer to the list to sort.
 * @param compare_func  Function used to compare values in the list.
 */

void slist_sort(SListEntry **list, SListCompareFunc compare_func);

/**
 * Find the entry for a particular value in a list.
 *
 * @param list           The list to search.
 * @param callback       Callback function to be invoked to determine if
 *                       values in the list are equal to the value to be
 *                       searched for.
 * @param data           The value to search for.
 * @return               The list entry of the value being searched for, or
 *                       NULL if not found.
 */

SListEntry *slist_find_data(SListEntry *list,
                            SListEqualFunc callback,
                            SListValue data);

/**
 * Initialise a @ref SListIterator structure to iterate over a list.
 *
 * @param list           Pointer to the list to iterate over.
 * @param iter           Pointer to a @ref SListIterator structure to
 *                       initialise.
 */

void slist_iterate(SListEntry **list, SListIterator *iter);

/**
 * Determine if there are more values in the list to iterate over.
 *
 * @param iterator       The list iterator.
 * @return               Zero if there are no more values in the list to
 *                       iterate over, non-zero if there are more values to
 *                       read.
 */

int slist_iter_has_more(SListIterator *iterator);

/**
 * Using a list iterator, retrieve the next value from the list.
 *
 * @param iterator       The list iterator.
 * @return               The next value from the list, or SLIST_NULL if
 *                       there are no more values in the list.
 */

SListValue slist_iter_next(SListIterator *iterator);

/**
 * Delete the current entry in the list (the value last returned from
 * @ref slist_iter_next)
 *
 * @param iterator       The list iterator.
 */

void slist_iter_remove(SListIterator *iterator);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef ALGORITHM_SLIST_H */

