
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

/* Hash table implementation */

#include <stdlib.h>
#include <string.h>

#include "hashtable.h"

typedef struct _HashtableEntry HashtableEntry;

struct _HashtableEntry {
    void *key;
    void *value;
    HashtableEntry *next;
};

struct _Hashtable {
    HashtableEntry **table;
    int table_size;
    HashtableHashFunc hash_func;
    HashtableEqualFunc equal_func;
    int entries;
    int prime_index;
};

/* Prime numbers on an escalating exponential scale, used for the table
 * size.  Each value is approximately 1.5 * the previous value, so the
 * table size increases by 50% with each enlargement */

static int hashtable_primes[] = {
    251, 383, 571, 863, 1291, 1933, 2909, 4373, 6553, 9839, 14759, 22133,
    33211, 49811, 74719, 112069, 168127, 252193, 378289, 567407, 851131,
    1276721, 1915057, 2872621, 4308937, 6463399, 9695099, 14542651,
    21813997, 32721001, 49081441, 73622251, 110433383, 165650033,
    248475107, 372712667, 559068997, 838603499, 1257905249, 1886857859,
};

static int hashtable_num_primes = sizeof(hashtable_primes) / sizeof(int);

/* Internal function used to allocate the table on hashtable creation
 * and when enlarging the table */

static void hashtable_allocate_table(Hashtable *hashtable)
{
    /* Determine the table size based on the current prime index.  
     * An attempt is made here to ensure sensible behavior if the
     * maximum prime is exceeded, but in practice other things are
     * likely to break long before that happens. */

    if (hashtable->prime_index < hashtable_num_primes)
        hashtable->table_size = hashtable_primes[hashtable->prime_index];
    else
        hashtable->table_size = hashtable->entries * 10;

    /* Allocate the table and initialise to NULL for all entries */

    hashtable->table = calloc(hashtable->table_size, 
                              sizeof(HashtableEntry *));
    memset(hashtable->table, 0, 
           hashtable->table_size * sizeof(HashtableEntry *));
}

Hashtable *hashtable_new(HashtableHashFunc hash_func, 
                         HashtableEqualFunc equal_func)
{
    Hashtable *hashtable;

    /* Allocate a new hash table structure */
    
    hashtable = (Hashtable *) malloc(sizeof(Hashtable));
    hashtable->hash_func = hash_func;
    hashtable->equal_func = equal_func;
    hashtable->entries = 0;
    hashtable->prime_index = 0;

    /* Allocate the table */

    hashtable_allocate_table(hashtable);

    return hashtable;
}

void hashtable_free(Hashtable *hashtable)
{
    HashtableEntry *rover;
    HashtableEntry *next;
    int i;
    
    /* Free all entries in all chains */

    for (i=0; i<hashtable->table_size; ++i) {
        rover = hashtable->table[i];
        while (rover != NULL) {
            next = rover->next;
            free(rover);
            rover = next;
        }
    }
    
    /* Free the table */

    free(hashtable->table);
    
    /* Free the hash table structure */

    free(hashtable);
}

static void hashtable_enlarge(Hashtable *hashtable)
{
    HashtableEntry **old_table;
    int old_table_size;
    HashtableEntry *rover;
    HashtableEntry *next;
    int index;
    int i;
    
    /* Store a copy of the old table */
    
    old_table = hashtable->table;
    old_table_size = hashtable->table_size;

    /* Allocate a new, larger table */

    ++hashtable->prime_index;
    hashtable_allocate_table(hashtable);

    /* Link all entries from all chains into the new table */

    for (i=0; i<old_table_size; ++i) {
        rover = old_table[i];

        while (rover != NULL) {
            next = rover->next;

            /* Find the index into the new table */
            
            index = hashtable->hash_func(rover->key) % hashtable->table_size;
            
            /* Link this entry into the chain */

            rover->next = hashtable->table[index];
            hashtable->table[index] = rover;
            
            /* Advance to next in the chain */

            rover = next;
        }
    }    
}

void hashtable_insert(Hashtable *hashtable, void *key, void *value) 
{
    HashtableEntry *rover;
    HashtableEntry *newentry;
    int index;
    
    /* If there are too many items in the table with respect to the table
     * size, the number of hash collisions increases and performance
     * decreases. Enlarge the table size to prevent this happening */

    if ((hashtable->entries * 3) / hashtable->table_size > 0) {
        
        /* Table is more than 1/3 full */

        hashtable_enlarge(hashtable);
    }

    /* Generate the hash of the key and hence the index into the table */

    index = hashtable->hash_func(key) % hashtable->table_size;

    /* Traverse the chain at this location and look for an existing
     * entry with the same key */

    rover = hashtable->table[index];

    while (rover != NULL) {
        if (hashtable->equal_func(rover->key, key) != 0) {

            /* Same key: overwrite with new data */

            rover->value = value;

            /* Finished */
            
            return;
        }
        rover = rover->next;
    }
    
    /* Not in the hashtable yet.  Create a new entry */

    newentry = (HashtableEntry *) malloc(sizeof(HashtableEntry));

    newentry->key = key;
    newentry->value = value;

    /* Link into the list */

    newentry->next = hashtable->table[index];
    hashtable->table[index] = newentry;

    /* Maintain the count of the number of entries */

    ++hashtable->entries;
}

void *hashtable_lookup(Hashtable *hashtable, void *key)
{
    HashtableEntry *rover;
    int index;

    /* Generate the hash of the key and hence the index into the table */
    
    index = hashtable->hash_func(key) % hashtable->table_size;

    /* Walk the chain at this index until the corresponding entry is
     * found */

    rover = hashtable->table[index];

    while (rover != NULL) {
        if (hashtable->equal_func(key, rover->key) != 0) {

            /* Found the entry.  Return the data. */

            return rover->value;
        }
        rover = rover->next;
    }

    /* Not found */

    return NULL;
}

int hashtable_remove(Hashtable *hashtable, void *key)
{
    HashtableEntry **rover;
    HashtableEntry *entry;
    int index;
    int result;

    /* Generate the hash of the key and hence the index into the table */
    
    index = hashtable->hash_func(key) % hashtable->table_size;

    /* Rover points at the pointer which points at the current entry
     * in the chain being inspected.  ie. the entry in the table, or
     * the "next" pointer of the previous entry in the chain.  This
     * allows us to unlink the entry when we find it. */

    result = 0;
    rover = &hashtable->table[index];

    while (*rover != NULL) {

        if (hashtable->equal_func(key, (*rover)->key) != 0) {

            /* This is the entry to remove */

            entry = *rover;

            /* Unlink from the list */

            *rover = entry->next;

            /* Destroy the entry structure */

            free(entry);

            /* Track count of entries */

            --hashtable->entries;

            result = 1;

            break;
        }
        
        /* Advance to the next entry */

        rover = &((*rover)->next);
    }

    return result;
}

int hashtable_num_entries(Hashtable *hashtable)
{
    return hashtable->entries;
}
