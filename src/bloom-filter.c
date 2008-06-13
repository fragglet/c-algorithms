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

#include <stdlib.h>
#include <string.h>

#include "bloom-filter.h"

/* malloc() / free() testing */

#ifdef ALLOC_TESTING
#include "alloc-testing.h"
#endif

struct _BloomFilter {
	BloomFilterHashFunc hash_func;
	unsigned char *table;
	unsigned int table_size;
	unsigned int num_functions;
};

/* Salt values.  These salts are XORed with the output of the hash
 * function to give multiple unique hashes. */

static const unsigned int salts[] = {
	0x5cee4612, 0xb5587b1c, 0xa250f2b0, 0xa3bf6d2a, 
	0x7a81bd1a, 0x92888d7f, 0x1dc977c7, 0xedc96624, 
	0x920c85d9, 0xf16066b3, 0xc6f0d4b3, 0x2b76eb86, 
	0xcacb3893, 0x493d81c5, 0xf5a133ac, 0x039740bf, 
	0x162b8224, 0xf841de90, 0xc3e5090d, 0x3bce93a7, 
	0xf1860334, 0xe832b5f1, 0xf5b6535b, 0xe4cf4fa6, 
	0x8357b769, 0x1442b07a, 0x21c5863d, 0xabc0d846, 
	0x6dc0d77a, 0x23a3992c, 0xe12179ba, 0xd81d1e23, 
	0xcff4727b, 0xe957ecfb, 0xee8f391a, 0x426efa23, 
	0x3a34ff2c, 0x8b875d94, 0x34fd0f63, 0xf159daae, 
	0xaabab8b3, 0xa83a07ba, 0x4e54fb33, 0xfb82fab8, 
	0x2ae2888f, 0xd1a307a8, 0xbe33322d, 0x87c73f86, 
	0x7270fa7e, 0x68673c55, 0x2c8026d0, 0xead8e422, 
	0xa3ee5132, 0xecb67767, 0x1c3b1ae5, 0x47adf5b6, 
	0xf4518d30, 0x46e62797, 0x9889aa76, 0x1405aadf, 
	0xf62f9124, 0x5c435ac5, 0x35b8dfe3, 0x651c08c5, 
};

BloomFilter *bloom_filter_new(unsigned int table_size, 
                              BloomFilterHashFunc hash_func,
                              unsigned int num_functions)
{
	BloomFilter *filter;

	/* There is a limit on the number of functions which can be 
	 * applied, due to the table size */

	if (num_functions > sizeof(salts) / sizeof(*salts)) {
		return NULL;
	}

	/* Allocate bloom filter structure */

	filter = malloc(sizeof(BloomFilter));

	if (filter == NULL) {
		return NULL;
	}

	/* Allocate table, each entry is one bit; these are packed into
	 * bytes.  When allocating we must round the length up to the nearest
	 * byte. */

	filter->table = calloc((table_size + 7) / 8, 1);

	if (filter->table == NULL) {
		free(filter);
		return NULL;
	}

	filter->hash_func = hash_func;
	filter->num_functions = num_functions;
	filter->table_size = table_size;

	return filter;
}

void bloom_filter_free(BloomFilter *bloomfilter)
{
	free(bloomfilter->table);
	free(bloomfilter);
}

void bloom_filter_insert(BloomFilter *bloomfilter, BloomFilterValue value)
{
	unsigned long hash;
	unsigned long subhash;
	unsigned int index;
	unsigned int i;

	/* Generate hash of the value to insert */

	hash = bloomfilter->hash_func(value);

	/* Generate multiple unique hashes by XORing with values in the
	 * salt table. */

	for (i=0; i<bloomfilter->num_functions; ++i) {

		/* Generate a unique hash */

		subhash = hash ^ salts[i];

		/* Find the index into the table */

		index = subhash % bloomfilter->table_size;

		/* Insert into the table.  
		 * index / 8 finds the byte index of the table,
		 * index % 8 gives the bit index within that byte to set. */

		bloomfilter->table[index / 8] |= 1 << (index % 8);
	}
}

int bloom_filter_query(BloomFilter *bloomfilter, BloomFilterValue value)
{
	unsigned long hash;
	unsigned long subhash;
	unsigned int index;
	unsigned int i;
	unsigned char b;
	int bit;

	/* Generate hash of the value to lookup */

	hash = bloomfilter->hash_func(value);

	/* Generate multiple unique hashes by XORing with values in the
	 * salt table. */

	for (i=0; i<bloomfilter->num_functions; ++i) {

		/* Generate a unique hash */

		subhash = hash ^ salts[i];

		/* Find the index into the table to test */

		index = subhash % bloomfilter->table_size;

		/* The byte at index / 8 holds the value to test */

		b = bloomfilter->table[index / 8];
		bit = 1 << (index % 8);

		/* Test if the particular bit is set; if it is not set,
		 * this value can not have been inserted. */

		if ((b & bit) == 0) {
			return 0;
		}
	}

	/* All necessary bits were set.  This may indicate that the value
	 * was inserted, or the values could have been set through other
	 * insertions. */

	return 1;
}

void bloom_filter_read(BloomFilter *bloomfilter, unsigned char *array)
{
	unsigned int array_size;

	/* The table is an array of bits, packed into bytes.  Round up
	 * to the nearest byte. */

	array_size = (bloomfilter->table_size + 7) / 8;

	/* Copy into the buffer of the calling routine. */

	memcpy(array, bloomfilter->table, array_size);
}

void bloom_filter_load(BloomFilter *bloomfilter, unsigned char *array)
{
	unsigned int array_size;

	/* The table is an array of bits, packed into bytes.  Round up
	 * to the nearest byte. */

	array_size = (bloomfilter->table_size + 7) / 8;

	/* Copy from the buffer of the calling routine. */

	memcpy(bloomfilter->table, array, array_size);
}

BloomFilter *bloom_filter_union(BloomFilter *filter1, BloomFilter *filter2)
{
	BloomFilter *result;
	unsigned int i;
	unsigned int array_size;

	/* To perform this operation, both filters must be created with
	 * the same values. */

	if (filter1->table_size != filter2->table_size
	 || filter1->num_functions != filter2->num_functions
	 || filter1->hash_func != filter2->hash_func) {
		return NULL;
	}

	/* Create a new bloom filter for the result */

	result = bloom_filter_new(filter1->table_size, 
	                          filter1->hash_func, 
	                          filter1->num_functions);

	if (result == NULL) {
		return NULL;
	}

	/* The table is an array of bits, packed into bytes.  Round up
	 * to the nearest byte. */

	array_size = (filter1->table_size + 7) / 8;

	/* Populate the table of the new filter */

	for (i=0; i<array_size; ++i) {
		result->table[i] = filter1->table[i] | filter2->table[i];
	}

	return result;
}

BloomFilter *bloom_filter_intersection(BloomFilter *filter1, 
                                       BloomFilter *filter2)
{
	BloomFilter *result;
	unsigned int i;
	unsigned int array_size;

	/* To perform this operation, both filters must be created with
	 * the same values. */

	if (filter1->table_size != filter2->table_size
	 || filter1->num_functions != filter2->num_functions
	 || filter1->hash_func != filter2->hash_func) {
		return NULL;
	}

	/* Create a new bloom filter for the result */

	result = bloom_filter_new(filter1->table_size, 
	                          filter1->hash_func, 
	                          filter1->num_functions);

	if (result == NULL) {
		return NULL;
	}

	/* The table is an array of bits, packed into bytes.  Round up
	 * to the nearest byte. */

	array_size = (filter1->table_size + 7) / 8;

	/* Populate the table of the new filter */

	for (i=0; i<array_size; ++i) {
		result->table[i] = filter1->table[i] & filter2->table[i];
	}

	return result;
}

