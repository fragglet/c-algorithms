
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
#include <string.h>
#include <assert.h>

#include "alloc-testing.h"

#include "trie.h"

int test_array[100000];
char test_strings[100000][10];

Trie *generate_trie(void)
{
	Trie *trie;
	int i;
	int entries;

	/* Create a trie and fill it with a large number of values */

	trie = trie_new();
	entries = 0;

	for (i=0; i<100000; ++i) {

		/* Create a string containing a text version of i, and use
		 * it as a key for the value */
		
		test_array[i] = i;
		sprintf(test_strings[i], "%i", i);
		
		trie_insert(trie, test_strings[i], &test_array[i]);

		++entries;

		assert(trie_num_entries(trie) == entries);
	}

	return trie;
}

void test_trie_new_free(void)
{
	Trie *trie;
	
	/* Allocate and free an empty trie */

	trie = trie_new();

	assert(trie != NULL);

	trie_free(trie);

	/* Add some values before freeing */

	trie = trie_new();

	trie_insert(trie, "hello", "there");
	trie_insert(trie, "hell", "testing");
	trie_insert(trie, "testing", "testing");
	trie_insert(trie, "", "asfasf");

	trie_free(trie);

	/* Add a value, remove it and then free */

	trie = trie_new();

	trie_insert(trie, "hello", "there");
	assert(trie_remove(trie, "hello") == 1);
	
	trie_free(trie);
}

void test_lookup(void)
{
	Trie *trie;
	char buf[10];
	int *val;
	int i;

	trie = generate_trie();

	/* Test lookup for non-existent values */

	assert(trie_lookup(trie, "000000000000000") == TRIE_NULL);
	assert(trie_lookup(trie, "") == TRIE_NULL);

	/* Look up all values */

	for (i=0; i<100000; ++i) {

		sprintf(buf, "%i", i);

		val = (int *) trie_lookup(trie, buf);
		
		assert(*val == i);
	}

	trie_free(trie);
}

void test_remove(void)
{
	Trie *trie;
	char buf[10];
	int i;
	int entries;

	trie = generate_trie();

	/* Test remove on non-existent values. */

	assert(trie_remove(trie, "000000000000000") == 0);
	assert(trie_remove(trie, "") == 0);

	entries = trie_num_entries(trie);

	assert(entries == 100000);

	/* Remove all values */

	for (i=0; i<100000; ++i) {

		sprintf(buf, "%i", i);

		/* Remove value and check counter */

		assert(trie_remove(trie, buf) != 0);
		--entries;
		assert(trie_num_entries(trie) == entries);
	}

	trie_free(trie);
}

void test_replace(void)
{
	Trie *trie;
	int *val;

	trie = generate_trie();

	/* Test replacing values */

	val = malloc(sizeof(int));
	*val = 999;
	trie_insert(trie, "999", val);
	assert(trie_num_entries(trie) == 100000);

	assert(trie_lookup(trie, "999") == val);
	free(val);
	trie_free(trie);
}

void test_insert_empty(void)
{
	Trie *trie;
	char buf[10];

	trie = trie_new();

	/* Test insert on empty string */

	trie_insert(trie, "", buf);
	assert(trie_num_entries(trie) == 1);
	assert(trie_lookup(trie, "") == buf);
	assert(trie_remove(trie, "") != 0);

	assert(trie_num_entries(trie) == 0);

	trie_free(trie);
}

int main(int argc, char *argv[])
{
	test_trie_new_free();
	test_lookup();
	test_remove();
	test_replace();
	test_insert_empty();

	return 0;
}

