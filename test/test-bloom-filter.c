 
/*
 
Copyright (c) 2006, Simon Howard
All rights reserved.

Redistribution and use in source and binomial forms, with or without 
modification, are permitted provided that the following conditions 
are met:

 * Redistributions of source code must retain the above copyright 
   notice, this list of conditions and the following disclaimer.
 * Redistributions in binomial form must reproduce the above copyright 
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

#include "alloc-testing.h"
#include "framework.h"

#include "bloom-filter.h"
#include "hash-string.h"

void test_bloom_filter_new_free(void)
{
	BloomFilter *filter;

	filter = bloom_filter_new(128, string_hash, 1);

	assert(filter != NULL);

	bloom_filter_free(filter);

	filter = bloom_filter_new(128, string_hash, 64);

	assert(filter != NULL);

	bloom_filter_free(filter);
}

void test_bloom_filter_insert_query(void)
{
	BloomFilter *filter;

	/* Create a filter */

	filter = bloom_filter_new(128, string_hash, 4);

	/* Check values are not present at the start */

	assert(bloom_filter_query(filter, "test 1") == 0);
	assert(bloom_filter_query(filter, "test 2") == 0);

	/* Insert some values */

	bloom_filter_insert(filter, "test 1");
	bloom_filter_insert(filter, "test 2");

	/* Check they are set */

	assert(bloom_filter_query(filter, "test 1") != 0);
	assert(bloom_filter_query(filter, "test 2") != 0);

	bloom_filter_free(filter);
}

void test_bloom_filter_read_load(void)
{
	BloomFilter *filter1;
	BloomFilter *filter2;
	unsigned char state[16];

	/* Create a filter with some values set */

	filter1 = bloom_filter_new(128, string_hash, 4);

	bloom_filter_insert(filter1, "test 1");
	bloom_filter_insert(filter1, "test 2");

	/* Read the current state into an array */

	bloom_filter_read(filter1, state);

	bloom_filter_free(filter1);

	/* Create a new filter and load the state */

	filter2 = bloom_filter_new(128, string_hash, 4);

	bloom_filter_load(filter2, state);

	/* Check the values are set in the new filter */

	assert(bloom_filter_query(filter2, "test 1") != 0);
	assert(bloom_filter_query(filter2, "test 2") != 0);

	bloom_filter_free(filter2);
}

void test_bloom_filter_intersection(void)
{
	BloomFilter *filter1;
	BloomFilter *filter2;
	BloomFilter *result;

	/* Create one filter with both values set */

	filter1 = bloom_filter_new(128, string_hash, 4);

	bloom_filter_insert(filter1, "test 1");
	bloom_filter_insert(filter1, "test 2");

	/* Create second filter with only one value set */

	filter2 = bloom_filter_new(128, string_hash, 4);

	bloom_filter_insert(filter2, "test 1");

	/* For this test, we need this to be definitely not present. 
	 * Note that this could theoretically return non-zero here,
	 * depending on the hash function. */

	assert(bloom_filter_query(filter2, "test 2") == 0);
	
	/* Intersection */

	result = bloom_filter_intersection(filter1, filter2);

	/* test 1 should be set, as it is in both
	 * test 2 should not be set, as it is not present in both */

	assert(bloom_filter_query(result, "test 1") != 0);
	assert(bloom_filter_query(result, "test 2") == 0);

	bloom_filter_free(filter1);
	bloom_filter_free(filter2);
	bloom_filter_free(result);
}

void test_bloom_filter_union(void)
{
	BloomFilter *filter1;
	BloomFilter *filter2;
	BloomFilter *result;

	/* Create one filter with both values set */

	filter1 = bloom_filter_new(128, string_hash, 4);

	bloom_filter_insert(filter1, "test 1");

	/* Create second filter with only one value set */

	filter2 = bloom_filter_new(128, string_hash, 4);

	bloom_filter_insert(filter2, "test 2");

	/* Union */

	result = bloom_filter_union(filter1, filter2);

	/* Both should be present */

	assert(bloom_filter_query(result, "test 1") != 0);
	assert(bloom_filter_query(result, "test 2") != 0);

	bloom_filter_free(filter1);
	bloom_filter_free(filter2);
	bloom_filter_free(result);
}

static UnitTestFunction tests[] = {
	test_bloom_filter_new_free,
	test_bloom_filter_insert_query,
	test_bloom_filter_read_load,
	test_bloom_filter_intersection,
	test_bloom_filter_union,
	NULL
};

int main(int argc, char *argv[])
{
	run_tests(tests);

	return 0;
}

