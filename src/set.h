
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
 * @file set.h
 *
 * @brief Set of values.
 *
 * A set stores a collection of values.  Each value can only exist once in 
 * the set.
 *
 * To create a new set, use @ref set_new.  To destroy a set, use 
 * @ref set_free.
 *
 * To add a value to a set, use @ref set_insert.  To remove a value
 * from a set, use @ref set_remove.
 *
 * To find the number of entries in a set, use @ref set_num_entries.
 *
 * To query if a particular value is in a set, use @ref set_query.
 *
 * To iterate over all values in a set, use @ref set_foreach.
 */

#ifndef ALGORITHM_SET_H
#define ALGORITHM_SET_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Represents a set of data.  Created using the set_new function and destroyed
 * using the set_free function.
 *
 * @see set_new
 * @see set_free
 */

typedef struct _Set Set;

/** 
 * Hash function.  Generates a hash key for data to be stored in a set.
 */

typedef unsigned long (*SetHashFunc)(void *data);

/** 
 * Equality function.  Compares two values to determine if they are
 * equivalent.
 */

typedef int (*SetEqualFunc)(void *data1, void *data2);

/**
 * Copy function.  Given a pointer to some data, return a
 * copy of it. Used by @ref set_intersection and @ref set_union.
 */

typedef void *(*SetCopyFunc)(void *data);

/**
 * Set iterator.  Callback function used to iterate over values in a set.
 */

typedef void (*SetIterator)(void *data, void *user_data);

/**
 * Function used to free values stored in a set.  See
 * @ref set_register_free_function.
 */

typedef void (*SetFreeFunc)(void *data);

/**
 * Create a new set.
 *
 * @param hash_func     Hash function used on data in the set .
 * @param equal_func    Compares two values in the set to determine
 *                      if they are equal.
 * @return              A new set.
 */

Set *set_new(SetHashFunc hash_func, SetEqualFunc equal_func);

/**
 * Destroy a set.
 *
 * @param set           The set to destroy.
 */

void set_free(Set *set);

/**
 * Register a function to be called when values are removed from 
 * the set.
 *
 * @param set           The set.
 * @param free_func     Function to call when values are removed from the
 *                      set.
 */

void set_register_free_function(Set *set, SetFreeFunc free_func);

/**
 * Add a value to a set.
 *
 * @param set           The set.
 * @param data          The data to add to the set .
 * @return              Non-zero (true) if the value was added to the set,
 *                      zero (false) if it already exists in the set.
 */

int set_insert(Set *set, void *data);

/**
 * Remove a value from a set.
 *
 * @param set           The set.
 * @param data          The data to remove from the set.
 * @return              Non-zero (true) if the data was found and removed
 *                      from the set, zero (false) if the data was not
 *                      found in the set.
 */

int set_remove(Set *set, void *data);

/** 
 * Query if a particular value is in a set.
 *
 * @param set           The set.
 * @param data          The data to query for.
 * @return              Zero if the data is not in the set, non-zero if the
 *                      data is in the set.
 */

int set_query(Set *set, void *data);

/**
 * Iterate over all values in a set.
 *
 * @param set           The set.
 * @param callback      Callback function to be invoked for each value.
 * @param user_data     Extra data to be passed to the callback function.
 */

void set_foreach(Set *set, SetIterator callback, void *user_data);

/**
 * Retrieve the number of entries in a set
 *
 * @param set           The set.
 * @return              A count of the number of entries in the set.
 */

int set_num_entries(Set *set);

/**
 * Perform a union of two sets.  
 *
 * @param set1             The first set.
 * @param set2             The second set.
 * @param copy_func        Pointer to a function to use to copy data.
 *                         When values are inserted into the new set, they
 *                         are first copied using the copy function.
 *                         If NULL is passed, no copying is performed and
 *                         the reference from the first set is added.
 * @return                 A new set containing all values which are in the 
 *                         first or second sets. 
 */

Set *set_union(Set *set1, Set *set2, SetCopyFunc copy_func);

/**
 * Perform an intersection of two sets.
 *
 * @param set1             The first set.
 * @param set2             The second set.
 * @param copy_func        Pointer to a function to use to copy data.
 *                         When values are inserted into the new set, they
 *                         are first copied using the copy function.
 *                         If NULL is passed, no copying is performed and
 *                         the reference from the first set is added.
 * @return                 A new set containing all values which are in both
 *                         sets.
 */

Set *set_intersection(Set *set1, Set *set2, 
                      SetCopyFunc copy_func);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef ALGORITHM_SET_H */

