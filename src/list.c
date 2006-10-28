
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

#include "list.h"

/* A doubly-linked list */

struct _ListEntry {
	void *data;
	ListEntry *prev;
	ListEntry *next;
};

/* Iterator for iterating over a doubly-linked list. */

struct _ListIterator {
	ListEntry **list;
	ListEntry **prev_next;
	ListEntry *current;
};

void list_free(ListEntry *list)
{
	ListEntry *entry;

	/* Iterate over each entry, freeing each list entry, until the
	 * end is reached */

	entry = list;

	while (entry != NULL) {
		ListEntry *next;
	   
		next = entry->next;

		free(entry);

		entry = next;
	}
}

ListEntry *list_prepend(ListEntry **list, void *data)
{
	ListEntry *newentry;

	/* Create new entry */

	newentry = malloc(sizeof(ListEntry));

	if (newentry == NULL) {
		return NULL;
	}
	
	newentry->data = data;

	/* Hook into the list start */

	if (*list != NULL) {
		(*list)->prev = newentry;
	}
	newentry->prev = NULL;
	newentry->next = *list;
	*list = newentry;

	return newentry;
}

ListEntry *list_append(ListEntry **list, void *data)
{
	ListEntry *rover;
	ListEntry *newentry;

	/* Create new list entry */

	newentry = malloc(sizeof(ListEntry));

	if (newentry == NULL) {
		return NULL;
	}
	
	newentry->data = data;
	newentry->next = NULL;
	
	/* Hooking into the list is different if the list is empty */

	if (*list == NULL) {

		/* Create the start of the list */

		*list = newentry;
		newentry->prev = NULL;

	} else {

		/* Find the end of list */

		for (rover=*list; rover->next != NULL; rover = rover->next);

		/* Add to the end of list */

		newentry->prev = rover;
		rover->next = newentry;
	}

	return newentry;
}

void *list_data(ListEntry *listentry)
{
	return listentry->data;
}

ListEntry *list_prev(ListEntry *listentry)
{
	return listentry->prev;
}

ListEntry *list_next(ListEntry *listentry)
{
	return listentry->next;
}

ListEntry *list_nth_entry(ListEntry *list, int n)
{
	ListEntry *entry;
	int i;

	/* Negative values are always out of range */

	if (n < 0) {
		return NULL;
	}

	/* Iterate through n list entries to reach the desired entry.
	 * Make sure we do not reach the end of the list. */

	entry = list;

	for (i=0; i<n; ++i) {

		if (entry == NULL) {
			return NULL;
		}
		entry = entry->next;
	}

	return entry;
}

void *list_nth_data(ListEntry *list, int n)
{
	ListEntry *entry;

	/* Find the specified entry */

	entry = list_nth_entry(list, n);

	/* If out of range, return NULL, otherwise return the data */

	if (entry == NULL) {
		return NULL;
	} else {
		return entry->data;
	}
}

int list_length(ListEntry *list)
{
	ListEntry *entry;
	int length;

	length = 0;
	entry = list;

	while (entry != NULL) {
		
		/* Count the number of entries */
		
		++length;

		entry = entry->next;
	}

	return length;
}

void **list_to_array(ListEntry *list)
{
	ListEntry *rover;
	int listlen;
	void **array;
	int i;

	/* Allocate an array equal in size to the list length */
	
	listlen = list_length(list);

	array = malloc(sizeof(void *) * listlen);

	if (array == NULL) {
		return NULL;
	}
	
	/* Add all entries to the array */
	
	rover = list;
	
	for (i=0; i<listlen; ++i) {

		/* Add this node's data */

		array[i] = rover->data;
		
		/* Jump to the next list node */

		rover = rover->next;
	}

	return array;
}

int list_remove_entry(ListEntry **list, ListEntry *entry)
{
	/* If the list is empty, or entry is NULL, always fail */

	if (*list == NULL || entry == NULL) {
		return 0;
	}
	
	/* Action to take is different if the entry is the first in the list */

	if (entry->prev == NULL) {

		/* Unlink the first entry and update the starting pointer */

		*list = entry->next;

		/* Update the second entry's prev pointer, if there is a second
		 * entry */

		if (entry->next != NULL) {
			entry->next->prev = NULL;
		}

	} else {

		/* This is not the first in the list, so we must have a 
		 * previous entry.  Update its 'next' pointer to the new 
		 * value */

		entry->prev->next = entry->next;

		/* If there is an entry following this one, update its 'prev'
		 * pointer to the new value */

		if (entry->next != NULL) {
			entry->next->prev = entry->prev;
		}
	}

	/* Free the list entry */

	free(entry);

	/* Operation successful */

	return 1;
}

int list_remove_data(ListEntry **list, ListEqualFunc callback, void *data)
{
	int entries_removed;
	ListEntry *rover;
	ListEntry *next;
	
	entries_removed = 0;

	/* Iterate over the entries in the list */
	
	rover = *list;
	
	while (rover != NULL) {

		next = rover->next;

		if (callback(rover->data, data)) {

			/* This data needs to be removed.  Unlink this entry 
			 * from the list. */

			if (rover->prev == NULL) {
				
				/* This is the first entry in the list */

				*list = rover->next;
			} else {

				/* Point the previous entry at its new 
				 * location */
				
				rover->prev->next = rover->next;
			}

			if (rover->next != NULL) {
				rover->next->prev = rover->prev;
			}

			/* Free the entry */

			free(rover);

			++entries_removed;
		}
		
		/* Advance to the next list entry */

		rover = next;
	}

	return entries_removed;
}

/* Function used internally for sorting.  Returns the last entry in the
 * new sorted list */

static ListEntry *list_sort_internal(ListEntry **list, 
                                     ListCompareFunc compare_func)
{
	ListEntry *pivot;
	ListEntry *rover;
	ListEntry *less_list, *more_list;
	ListEntry *less_list_end, *more_list_end;
	
	/* If there are less than two entries in this list, it is
	 * already sorted */

	if (*list == NULL || (*list)->next == NULL) {
		return *list;
	}

	/* The first entry is the pivot */

	pivot = *list;

	/* Iterate over the list, starting from the second entry.  Sort
	 * all entries into the less and more lists based on comparisons
	 * with the pivot */

	less_list = NULL;
	more_list = NULL;
	rover = (*list)->next;

	while (rover != NULL) {
		ListEntry *next = rover->next;

		if (compare_func(rover->data, pivot->data) < 0) {

			/* Place this in the less list */

			rover->prev = NULL;
			rover->next = less_list;
			if (less_list != NULL) {
				less_list->prev = rover;
			}
			less_list = rover;

		} else {

			/* Place this in the more list */

			rover->prev = NULL;
			rover->next = more_list;
			if (more_list != NULL) {
				more_list->prev = rover;
			}
			more_list = rover;
		}

		rover = next;
	}

	/* Sort the sublists recursively */

	less_list_end = list_sort_internal(&less_list, compare_func);
	more_list_end = list_sort_internal(&more_list, compare_func);

	/* Create the new list starting from the less list */

	*list = less_list;

	/* Append the pivot to the end of the less list.  If the less list
	 * was empty, start from the pivot */

	if (less_list == NULL) {
		pivot->prev = NULL;
		*list = pivot;
	} else {
		pivot->prev = less_list_end;
		less_list_end->next = pivot;
	}

	/* Append the more list after the pivot */

	pivot->next = more_list;
	if (more_list != NULL) {
		more_list->prev = pivot;
	}

	/* Work out what the last entry in the list is.  If the more list was 
	 * empty, the pivot was the last entry.  Otherwise, the end of the 
	 * more list is the end of the total list. */

	if (more_list == NULL) {
		return pivot;
	} else {
		return more_list_end;
	}
}

void list_sort(ListEntry **list, ListCompareFunc compare_func)
{
	list_sort_internal(list, compare_func);
}

ListEntry *list_find_data(ListEntry *list,
                          ListEqualFunc callback,
                          void *data)
{
	ListEntry *rover;

	/* Iterate over entries in the list until the data is found */

	for (rover=list; rover != NULL; rover=rover->next) {
		if (callback(rover->data, data) != 0) {
			return rover;
		}
	}
	
	/* Not found */

	return NULL;
}

ListIterator *list_iterate(ListEntry **list)
{
	ListIterator *iter;

	/* Create a new iterator */

	iter = malloc(sizeof(ListIterator));

	if (iter == NULL) {
		return NULL;
	}

	/* Save pointer to the list */
	
	iter->list = list;

	/* These are NULL until the first call to list_iter_next: */

	iter->prev_next = NULL;
	iter->current = NULL;

	return iter;
}

int list_iter_has_more(ListIterator *iter)
{
	if (iter->prev_next == NULL) {
	       
		/* The list has just been created.  list_iter_next
		 * has not been called yet.  There are more entries 
		 * if the list is not empty. */
		
		return *iter->list != NULL;

	} else if (*iter->prev_next != iter->current) {

                /* The entry last returned by list_iter_next has been
                 * deleted.  The next entry is indincated by prev_next. */

		return *iter->prev_next != NULL;

	} else {
		/* The current entry as not been deleted since the last 
		 * call to list_iter_next: there is a next entry if 
		 * current->next is not NULL */

		return iter->current->next != NULL;
	}
}

void *list_iter_next(ListIterator *iter)
{
	if (iter->prev_next == NULL) {
		/* First call to list_iter_next.  Initialise. */

		/* Initial previous next link is the pointer to the list 
		 * start variable */

		iter->prev_next = iter->list;

		/* Start at the first element */

		iter->current = *iter->list;

	} else if (*iter->prev_next != iter->current) {

		/* The value last returned by list_iter_next was deleted.
                 * Use prev_next to find the next entry. */

		iter->current = *iter->prev_next;

	} else {

                /* Last value returned from list_iter_next was not deleted.
                 * Advance to the next entry. */

		if (iter->current != NULL) {
			iter->prev_next = &iter->current->next;
			iter->current = iter->current->next;
		}
	}
	
	/* Return data from the current entry */

        if (iter->current == NULL) {
                return NULL;
        } else {
	        return iter->current->data;
        }
}

void list_iter_remove(ListIterator *iter)
{
        if (iter->prev_next == NULL) {

                /* list_iter_next has not been called yet. */

        } else if (*iter->prev_next != iter->current) {

                /* Current entry was already deleted. */

        } else {
                
                /* Remove the current entry */

                if (iter->current != NULL) {
                	list_remove_entry(iter->list, iter->current);
                }
        }
}

void list_iter_free(ListIterator *iter)
{
	free(iter);
}

