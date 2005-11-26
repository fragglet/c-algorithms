
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
    int prime_index;
    SetHashFunc hash_func;
    SetEqualFunc equal_func;
};

/* Prime numbers on an escalating exponential scale, used for the table
 * size.  Each value is approximately 1.5 * the previous value, so the
 * table size increases by 50% with each enlargement */

static int set_primes[] = {
    251, 383, 571, 863, 1291, 1933, 2909, 4373, 6553, 9839, 14759, 22133,
    33211, 49811, 74719, 112069, 168127, 252193, 378289, 567407, 851131,
    1276721, 1915057, 2872621, 4308937, 6463399, 9695099, 14542651,
    21813997, 32721001, 49081441, 73622251, 110433383, 165650033,
    248475107, 372712667, 559068997, 838603499, 1257905249, 1886857859,
};

static int set_num_primes = sizeof(set_primes) / sizeof(int);

static void set_allocate_table(Set *set)
{
    /* Determine the table size based on the current prime index.  
     * An attempt is made here to ensure sensible behavior if the
     * maximum prime is exceeded, but in practice other things are
     * likely to break long before that happens. */

    if (set->prime_index < set_num_primes) {
        set->table_size = set_primes[set->prime_index];
    } else {
        set->table_size = set->entries * 10;
    }

    /* Allocate the table and initialise to NULL */

    set->table = calloc(set->table_size, sizeof(SetEntry *));
    memset(set->table, 0, set->table_size * sizeof(SetEntry *));
}

Set *set_new(SetHashFunc hash_func, SetEqualFunc equal_func)
{
    Set *new_set;

    /* Allocate a new set and fill in the fields */

    new_set = (Set *) malloc(sizeof(Set));
    new_set->hash_func = hash_func;
    new_set->equal_func = equal_func;
    new_set->entries = 0;
    new_set->prime_index = 0;
    
    /* Allocate the table */
    
    set_allocate_table(new_set);

    return new_set;
}

void set_free(Set *set)
{
    SetEntry *rover;
    SetEntry *next;
    int i;
    
    /* Free all entries in all chains */
    
    for (i=0; i<set->table_size; ++i) {
        rover = set->table[i];

        while (rover != NULL) {
            next = rover->next;

            /* Free this entry */

            free(rover);

            /* Advance to the next entry in the chain */
            
            rover = next;
        }
    }
    
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

    /* Use the next table size from the prime number array */

    ++set->prime_index;

    /* Allocate the new table */

    set_allocate_table(set);

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

void set_insert(Set *set, void *data)
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

void set_remove(Set *set, void *data)
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

int set_query(Set *set, void *data)
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

        /* Advance to the next entry in the chain */

        rover = rover->next;
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

static void set_union_foreach(void *value, void *user_data)
{
    Set *new_set = (Set *) user_data;

    set_insert(new_set, value);
}

Set *set_union(Set *set1, Set *set2)
{
    Set *new_set;

    new_set = set_new(set1->hash_func, set1->equal_func);

    /* Add all values from the first set */
    
    set_foreach(set1, set_union_foreach, new_set);

    /* Add all values from the second set */
    
    set_foreach(set2, set_union_foreach, new_set);

    return new_set;
}

struct set_intersection_data {
    Set *new_set;
    Set *set2;
};

static void set_intersection_foreach(void *value, void *user_data)
{
    struct set_intersection_data *params;

    params = (struct set_intersection_data *) user_data;

    /* Is this value in set 2 as well?  If so, it should be in the 
     * new set. */

    if (set_query(params->set2, value) != 0) {
        set_insert(params->new_set, value);
    }
}

Set *set_intersection(Set *set1, Set *set2)
{
    struct set_intersection_data user_data;
    Set *new_set;

    new_set = set_new(set1->hash_func, set2->equal_func);

    /* Iterate over all values in set 1. */

    user_data.new_set = new_set;
    user_data.set2 = set2;

    set_foreach(set1, set_intersection_foreach, &user_data);
    
    return new_set;
}
