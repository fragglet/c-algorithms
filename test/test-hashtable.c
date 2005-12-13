 
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

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "hashtable.h"
#include "hash-int.h"
#include "compare-int.h"

int value1 = 1, value2 = 2, value3 = 3, value4 = 4;

/* Generates a hashtable for use in tests containing 10,000 entries */

HashTable *generate_hashtable(void)
{
    HashTable *hashtable;
    int *value;
    int i;
    
    /* Allocate a new hash table */

    hashtable = hash_table_new((HashTableHashFunc) int_hash, 
                               (HashTableEqualFunc) int_equal);
    
    /* Insert lots of values */
    
    for (i=0; i<10000; ++i) {
        value = (int *) malloc(sizeof(int));

        *value = i;

        hash_table_insert(hashtable, value, value);
    }
    
    return hashtable;
}

void test_hash_table_new(void)
{
    HashTable *hashtable;

    hashtable = hash_table_new((HashTableHashFunc) int_hash, 
                               (HashTableEqualFunc) int_equal);
    
    assert(hashtable != NULL);
}

void test_hash_table_free(void)
{
    HashTable *hashtable;

    hashtable = hash_table_new((HashTableHashFunc) int_hash, 
                               (HashTableEqualFunc) int_equal);

    /* Add some values */

    hash_table_insert(hashtable, &value1, &value1);
    hash_table_insert(hashtable, &value2, &value2);
    hash_table_insert(hashtable, &value3, &value3);
    hash_table_insert(hashtable, &value4, &value4);

    /* Free the hash table */

    hash_table_free(hashtable);
}

/* Test insert and lookup functions */

void test_hash_table_insert_lookup(void)
{
    HashTable *hashtable;
    int *value;
    int i;

    /* Generate a hash table */

    hashtable = generate_hashtable();

    assert(hash_table_num_entries(hashtable) == 10000);

    /* Check all values */

    for (i=0; i<10000; ++i) {
        value = (int *) hash_table_lookup(hashtable, &i);

        assert(*value == i);
    }

    /* Lookup on invalid values returns NULL */

    i = -1;
    assert(hash_table_lookup(hashtable, &i) == NULL);
    i = 10000;
    assert(hash_table_lookup(hashtable, &i) == NULL);

    /* Insert overwrites existing entries with the same key */

    value = (int *) malloc(sizeof(int));
    *value = 12345;
    i = 5000;
    hash_table_insert(hashtable, &i, value);
    value = (int *) hash_table_lookup(hashtable, &i);
    assert(*value == 12345);
}

void test_hash_table_remove(void)
{
    HashTable *hashtable;
    int i;

    hashtable = generate_hashtable();

    assert(hash_table_num_entries(hashtable) == 10000);
    i = 5000;
    assert(hash_table_lookup(hashtable, &i) != NULL);

    /* Remove an entry */

    hash_table_remove(hashtable, &i);

    /* Check entry counter */

    assert(hash_table_num_entries(hashtable) == 9999);

    /* Check that NULL is returned now */

    assert(hash_table_lookup(hashtable, &i) == NULL);

    /* Try removing a non-existent entry */

    i = -1;
    hash_table_remove(hashtable, &i);

    assert(hash_table_num_entries(hashtable) == 9999);
}

int hash_table_foreach_count;

void hash_table_foreach_callback(void *key, void *value, void *user_data)
{
    ++hash_table_foreach_count;
}

void test_hash_table_foreach(void)
{
    HashTable *hashtable;

    hashtable = generate_hashtable();

    /* Iterate over all values in the table */

    hash_table_foreach_count = 0;

    hash_table_foreach(hashtable, hash_table_foreach_callback, NULL);

    assert(hash_table_foreach_count == 10000);

    /* Test iterating over an empty table */

    hashtable = hash_table_new((HashTableHashFunc) int_hash, 
                               (HashTableEqualFunc) int_equal);
    
    hash_table_foreach_count = 0;

    hash_table_foreach(hashtable, hash_table_foreach_callback, NULL);

    assert(hash_table_foreach_count == 0);
}

int main(int argc, char *argv[])
{
    test_hash_table_new();
    test_hash_table_free();
    test_hash_table_insert_lookup();
    test_hash_table_remove();
    test_hash_table_foreach();
    
    return 0;
}

