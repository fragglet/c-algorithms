
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
 * @file bloom-filter.h
 *
 * @brief Bloom filter
 *
 * A bloom filter is a space efficient data structure that can be
 * used to test whether a given element is part of a set.  Lookups
 * will occasionally generate false positives, but never false 
 * negatives. 
 *
 * To create a bloom filter, use @ref bloom_filter_new.  To destroy a 
 * bloom filter, use @ref bloom_filter_free.
 *
 * To insert a value into a bloom filter, use @ref bloom_filter_insert.
 *
 * To query whether a value is part of the set, use 
 * @ref bloom_filter_lookup.
 */

#ifndef ALGORITHM_BLOOM_FILTER_H
#define ALGORITHM_BLOOM_FILTER_H

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * A bloom filter structure.
 */

typedef struct _BloomFilter BloomFilter;

/**
 * Hash function used to generate hash values for values inserted into a 
 * bloom filter.
 *
 * @param data   The value to generate a hash value for.
 * @return       The hash value.
 */

typedef unsigned long (*BloomFilterHashFunc)(void *data);

/**
 * Create a new bloom filter.
 *
 * @param table_size       The size of the bloom filter.  The greater
 *                         the table size, the more elements can be
 *                         stored, and the lesser the chance of false
 *                         positives.
 * @param num_functions    Number of hash functions to apply to each
 *                         element on insertion.  This running time for
 *                         insertion and lookups is proportional to this
 *                         value.  The more functions applied, the lesser
 *                         the chance of false positives.  The maximum
 *                         number of functions is 64.
 * @return                 A new hash table structure, or NULL if it 
 *                         was not possible to allocate the new bloom
 *                         filter.
 */

BloomFilter *bloom_filter_new(unsigned int table_size, 
                              BloomFilterHashFunc hash_func,
                              unsigned int num_functions);

/**
 * Destroy a bloom filter.
 *
 * @param bloomfilter      The bloom filter to destroy.
 */

void bloom_filter_free(BloomFilter *bloomfilter);

/**
 * Insert a value into a bloom filter.
 *
 * @param bloomfilter          The bloom filter.
 * @param value                The value to insert.
 */

void bloom_filter_insert(BloomFilter *bloomfilter, void *value);

/**
 * Query a bloom filter for a particular value.
 *
 * @param bloomfilter          The bloom filter.
 * @param value                The value to look up.
 * @return                     Zero if the value was definitely not 
 *                             inserted into the filter.  Non-zero
 *                             indicates that it either may or may not
 *                             have been inserted.
 */

int bloom_filter_lookup(BloomFilter *bloomfilter, void *value);

/**
 * Read the contents of a bloom filter into an array.
 *
 * @param bloomfilter          The bloom filter.
 * @param array                Pointer to the array to read into.  This
 *                             should be (table_size + 7) / 8 bytes in 
 *                             length.
 */

void bloom_filter_read(BloomFilter *bloomfilter, unsigned char *array);

/**
 * Load the contents of a bloom filter from an array.
 * The data loaded should be the output read from @ref bloom_filter_read,
 * from a bloom filter created using the same arguments used to create
 * the original filter.
 *
 * @param bloomfilter          The bloom filter.
 * @param array                Pointer to the array to load from.  This 
 *                             should be (table_size + 7) / 8 bytes in 
 *                             length.
 */

void bloom_filter_load(BloomFilter *bloomfilter, unsigned char *array);

/** 
 * Find the union of two bloom filters.  Values are present in the 
 * resulting filter if they are present in either of the original 
 * filters.
 *
 * Both of the original filters must have been created using the 
 * same parameters to @ref bloom_filter_new.
 *
 * @param filter1              The first filter.
 * @param filter2              The second filter.
 * @return                     A new filter which is an intersection of the
 *                             two filters, or NULL if it was not possible
 *                             to allocate memory for the new filter, or
 *                             if the two filters specified were created
 *                             with different parameters. 
 */

BloomFilter *bloom_filter_union(BloomFilter *filter1, 
                                BloomFilter *filter2);

/** 
 * Find the intersection of two bloom filters.  Values are only ever 
 * present in the resulting filter if they are present in both of the
 * original filters.
 *
 * Both of the original filters must have been created using the 
 * same parameters to @ref bloom_filter_new.
 *
 * @param filter1              The first filter.
 * @param filter2              The second filter.
 * @return                     A new filter which is an intersection of the
 *                             two filters, or NULL if it was not possible
 *                             to allocate memory for the new filter, or
 *                             if the two filters specified were created
 *                             with different parameters. 
 */

BloomFilter *bloom_filter_intersection(BloomFilter *filter1, 
                                       BloomFilter *filter2);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef ALGORITHM_BLOOM_FILTER_H */

