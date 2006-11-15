
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

#include "bloom-filter.h"

struct _BloomFilter {
	BloomFilterHashFunc hash_func;
	unsigned char *table;
	unsigned int table_size;
	unsigned int num_functions;
};

/* Salt values.  These salts are XORed with the output of the hash
 * function to give multiple unique hashes. */

static const int salts[] = {
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

	filter->num_functions = num_functions;
	filter->table_size = table_size;

	return filter;
}

void bloom_filter_free(BloomFilter *bloomfilter)
{
	free(bloomfilter->table);
	free(bloomfilter);
}

void bloom_filter_insert(BloomFilter *bloomfilter, void *value)
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

int bloom_filter_lookup(BloomFilter *bloomfilter, void *value)
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


