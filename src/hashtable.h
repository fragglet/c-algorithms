
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
 * To create a hash table, use @ref hashtable_new.  To destroy a 
 * hash table, use @ref hashtable_free.
 *
 * To insert a value into a hash table, use @ref hashtable_insert.
 *
 * To remove a value from a hash table, use @ref hashtable_remove.
 *
 * To look up a value by its key, use @ref hashtable_lookup.
 */

#ifndef ALGORITHM_HASHTABLE_H
#define ALGORITHM_HASHTABLE_H

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * A hash table structure.
 */

typedef struct _Hashtable Hashtable;

/**
 * Hash function used to generate hash values for keys used in a hash
 * table.
 *
 * @param data   The value to generate a hash value for.
 * @return       The hash value.
 */

typedef unsigned long (*HashtableHashFunc)(void *data);

/**
 * Function used to compare two keys for equality.
 *
 * @return   Non-zero if the two keys are equal, zero if the keys are 
 *           not equal.
 */

typedef int (*HashtableEqualFunc)(void *data1, void *data2);

/**
 * Create a new hash table.
 *
 * @param hash_func            Function used to generate hash keys for the 
 *                             keys used in the table.
 * @param equal_func           Function used to test keys used in the table 
 *                             for equality.
 * @return                     A new hash table structure.
 */

Hashtable *hashtable_new(HashtableHashFunc hash_func, 
                         HashtableEqualFunc equal_func);

/**
 * Destroy a hash table.
 *
 * @param hashtable            The hash table to destroy.
 */

void hashtable_free(Hashtable *hashtable);

/**
 * Insert a value into a hash table, overwriting any existing entry 
 * using the same key.
 *
 * @param hashtable            The hash table.
 * @param key                  The key for the new value.
 * @param value                The value to insert.
 */

void hashtable_insert(Hashtable *hashtable, void *key, void *value);

/**
 * Look up a value in a hash table by key.
 *
 * @param hashtable           The hash table.
 * @param key                 The key of the value to look up.
 * @return                    The value, or NULL if there is no value with
 *                            that key in the hash table.
 */

void *hashtable_lookup(Hashtable *hashtable, void *key);

/**
 * Remove a value from a hash table.
 *
 * @param hashtable           The hash table.
 * @param key                 The key of the value to remove.
 * @return                    Non-zero if a key was removed, or zero if the
 *                            specified key was not found in the hash table.
 */

int hashtable_remove(Hashtable *hashtable, void *key);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef ALGORITHM_HASHTABLE_H */

