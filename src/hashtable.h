
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
 * @file hashtable.h
 *
 * @brief Hash table.
 *
 * A hash table stores a set of values which can be addressed by a 
 * key.  Given the key, the corresponding value can be looked up
 * quickly.
 *
 * To create a hash table, use @ref hash_table_new.  To destroy a 
 * hash table, use @ref hash_table_free.
 *
 * To insert a value into a hash table, use @ref hash_table_insert.
 *
 * To remove a value from a hash table, use @ref hash_table_remove.
 *
 * To look up a value by its key, use @ref hash_table_lookup.
 */

#ifndef ALGORITHM_HASHTABLE_H
#define ALGORITHM_HASHTABLE_H

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * A hash table structure.
 */

typedef struct _HashTable HashTable;

/**
 * Structure used to iterate over a hash table.
 */

typedef struct _HashTableIterator HashTableIterator;

/**
 * Hash function used to generate hash values for keys used in a hash
 * table.
 *
 * @param data   The value to generate a hash value for.
 * @return       The hash value.
 */

typedef unsigned long (*HashTableHashFunc)(void *data);

/**
 * Function used to compare two keys for equality.
 *
 * @return   Non-zero if the two keys are equal, zero if the keys are 
 *           not equal.
 */

typedef int (*HashTableEqualFunc)(void *data1, void *data2);

/**
 * Type of function used to free keys and values when entries are
 * removed from a hash table.
 */

typedef void (*HashTableFreeFunc)(void *data);

/**
 * Create a new hash table.
 *
 * @param hash_func            Function used to generate hash keys for the 
 *                             keys used in the table.
 * @param equal_func           Function used to test keys used in the table 
 *                             for equality.
 * @return                     A new hash table structure, or NULL if it 
 *                             was not possible to allocate the new hash
 *                             table.
 */

HashTable *hash_table_new(HashTableHashFunc hash_func, 
                          HashTableEqualFunc equal_func);

/**
 * Destroy a hash table.
 *
 * @param hashtable            The hash table to destroy.
 */

void hash_table_free(HashTable *hashtable);

/**
 * Register functions used to free the key and value when an entry is
 * removed from a hash table.
 *
 * @param hashtable            The hash table.
 * @param key_free_func        Function used to free keys.
 * @param value_free_func      Function used to free values.
 */

void hash_table_register_free_functions(HashTable *hashtable,
                                        HashTableFreeFunc key_free_func,
                                        HashTableFreeFunc value_free_func);

/**
 * Insert a value into a hash table, overwriting any existing entry 
 * using the same key.
 *
 * @param hashtable            The hash table.
 * @param key                  The key for the new value.
 * @param value                The value to insert.
 * @return                     Non-zero if the value was added successfully,
 *                             or zero if it was not possible to allocate
 *                             memory for the new entry.
 */

int hash_table_insert(HashTable *hashtable, void *key, void *value);

/**
 * Look up a value in a hash table by key.
 *
 * @param hashtable           The hash table.
 * @param key                 The key of the value to look up.
 * @return                    The value, or NULL if there is no value with
 *                            that key in the hash table.
 */

void *hash_table_lookup(HashTable *hashtable, void *key);

/**
 * Remove a value from a hash table.
 *
 * @param hashtable           The hash table.
 * @param key                 The key of the value to remove.
 * @return                    Non-zero if a key was removed, or zero if the
 *                            specified key was not found in the hash table.
 */

int hash_table_remove(HashTable *hashtable, void *key);

/** 
 * Retrieve the number of entries in a hash table.
 *
 * @param hashtable           The hash table.
 * @return                    The number of entries in the hash table.
 */

int hash_table_num_entries(HashTable *hashtable);

/**
 * Create a new @ref HashTableIterator to iterate over a hash table.
 * Note: iterators should be freed back with 
 * @ref hash_table_iter_free once iterating has completed.
 *
 * @param hashtable           The hash table.
 * @return                    A pointer to a new @ref HashTableIterator 
 *                            to iterate over the hash table, or NULL
 *                            if it was not possible to allocate the
 *                            memory.
 */

HashTableIterator *hash_table_iterate(HashTable *hashtable);

/**
 * Determine if there are more keys in the hash table to iterate
 * over. 
 *
 * @param iterator            The hash table iterator.
 * @return                    Zero if there are no more values to iterate
 *                            over, non-zero if there are more values to 
 *                            iterate over.
 */

int hash_table_iter_has_more(HashTableIterator *iterator);

/**
 * Using a hash table iterator, retrieve the next key.
 *
 * @param iterator            The hash table iterator.
 * @return                    The next key from the hash table, or NULL
 *                            if there are no more keys to iterate over.
 */

void *hash_table_iter_next(HashTableIterator *iterator);

/**
 * Free back a hash table iterator object.  This must be done once
 * iterating has completed.
 * 
 * @param iterator            The hash table iterator.
 */

void hash_table_iter_free(HashTableIterator *iterator);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef ALGORITHM_HASHTABLE_H */

