
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
#include "set.h"

/* A set */

typedef struct _SetEntry SetEntry;

struct _SetEntry {
    void *data;
    SetEntry *next;
};

struct _Set {
    SetEntry **table;
    int entries;
    int table_size;
    SetHashFunc hash_func;
    SetEqualFunc equal_func;
};

Set *set_new(SetHashFunc hash_func, SetEqualFunc equal_func)
{
    Set *new_set;

    /* Allocate a new set and fill in the fields */

    new_set = (Set *) malloc(sizeof(Set));
    new_set->table_size = 16;
    new_set->hash_func = hash_func;
    new_set->equal_func = equal_func;
    new_set->entries = 0;
    
    /* Allocate the table */

    new_set->table = calloc(new_set->table_size, sizeof(SetEntry *));
    memset(new_set->table, 0, new_set->table_size * sizeof(SetEntry *));
    
    return new_set;
}

void set_free(Set *set)
{
    /* Free the table */

    free(set->table);

    /* Free the set structure */

    free(set);
}

static void set_enlarge(Set *set)
{
    SetEntry *rover;
    SetEntry *next;
    SetEntry **old_table;
    int old_table_size;
    int index;
    int i;

    /* Store the old table */
    
    old_table = set->table;
    old_table_size = set->table_size;

    /* Increase table size and allocate a new one */

    set->table_size *= 2;
    set->table = calloc(set->table_size, sizeof(void *));
    memset(set->table, 0, set->table_size * sizeof(void *));
    
    /* Iterate through all entries in the old table and add them
     * to the new one */

    for (i=0; i<old_table_size; ++i) {

        /* Walk along this chain */

        rover = old_table[i];

        while (rover != NULL) {

            next = rover->next;

            /* Hook this entry into the new table */
            
            index = set->hash_func(rover->data) % set->table_size;
            rover->next = set->table[index];
            set->table[index] = rover;

            /* Advance to the next entry in the chain */
            
            rover = next;
        }
    }

    /* Free back the old table */

    free(old_table);
}

void set_add_data(Set *set, void *data)
{
    SetEntry *newentry;
    SetEntry *rover;
    int index;

    /* The hash table becomes less efficient as the number of entries
     * increases. Check if the percentage used becomes large. */
    
    if ((set->entries * 3) / set->table_size > 0) {
        
        /* The table is more than 1/3 full and must be increased in size */

        set_enlarge(set);
    }

    /* Use the hash of the data to determine an index to insert into the 
     * table at. */

    index = set->hash_func(data) % set->table_size;

    /* Walk along this chain and attempt to determine if this data has
     * already been added to the table */

    rover = set->table[index];

    while (rover != NULL) {

        if (set->equal_func(data, rover->data) != 0) {
            
            /* This data is already in the set */

            return;
        }
        
        rover = rover->next;
    }

    /* Not in the set.  We must add a new entry. */

    /* Make a new entry for this data */

    newentry = (SetEntry *) malloc(sizeof(SetEntry));
    newentry->data = data;
    
    /* Link into chain */

    newentry->next = set->table[index];
    set->table[index] = newentry;

    /* Keep track of the number of entries in the set */

    ++set->entries;
}

void set_remove_data(Set *set, void *data)
{
    SetEntry **rover;
    SetEntry *entry;
    int index;

    /* Look up the data by its hash key */

    index = set->hash_func(data) % set->table_size;

    /* Search this chain, until the corresponding entry is found */

    rover = &set->table[index];

    while (*rover != NULL) {
        if (set->equal_func(data, (*rover)->data) != 0) {
            
            /* Found the entry */

            entry = *rover;

            /* Unlink from the linked list */

            *rover = entry->next;

            /* Update counter */

            --set->entries;

            /* Free the entry and return */

            free(entry);

            return;
        }
    }

    /* Not found in set */
}

int set_query_data(Set *set, void *data)
{
    SetEntry *rover;
    int index;

    /* Look up the data by its hash key */

    index = set->hash_func(data) % set->table_size;

    /* Search this chain, until the corresponding entry is found */

    rover = set->table[index];

    while (rover != NULL) {
        if (set->equal_func(data, rover->data) != 0) {
            
            /* Found the entry */

            return 1;
        }
    }

    /* Not found */

    return 0;
}

void set_foreach(Set *set, SetIterator callback, void *user_data)
{
    SetEntry *rover;
    int i;

    /* Iterate over all chains and all values in the chains */

    for (i=0; i<set->table_size; ++i) {
        rover = set->table[i];

        while (rover != NULL) {

            /* Invoke callback for this data */

            callback(rover->data, user_data);

            /* Advance to the next entry */

            rover = rover->next;
        }
    }
}

int set_num_entries(Set *set)
{
    return set->entries;
}

