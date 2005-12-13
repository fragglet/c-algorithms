
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

#include "trie.h"

void test_trie_new()
{
	Trie *trie;

	trie = trie_new();

	assert(trie != NULL);
}

void test_trie_free()
{
	Trie *trie;
	
	/* Allocate and free an empty trie */

	trie = trie_new();

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
	trie_remove(trie, "hello");
	
	trie_free(trie);
}

void test_trie_insert_lookup_remove()
{
	Trie *trie;
	char buf[10];
	int *val;
	int i;
	int entries;

	/* Create a trie and fill it with a large number of values */

	trie = trie_new();
	entries = 0;

	for (i=0; i<100000; ++i) {

		/* Create a string containing a text version of i, and use
		 * it as a key for the value */
		
		sprintf(buf, "%i", i);
		val = (int *) malloc(sizeof(int));
		*val = i;
		
		trie_insert(trie, strdup(buf), val);

		++entries;

		assert(trie_num_entries(trie) == entries);
	}

	/* Look up all values */

	for (i=0; i<100000; ++i) {

		sprintf(buf, "%i", i);

		val = (int *) trie_lookup(trie, buf);
		
		assert(*val == i);

		/* Remove value and check counter */

		trie_remove(trie, buf);
		--entries;
		assert(trie_num_entries(trie) == entries);
	}

	/* Test insert on empty string */

	trie_insert(trie, "", buf);
	assert(trie_num_entries(trie) == 1);
	assert(trie_lookup(trie, "") == buf);
	trie_remove(trie, "");

	assert(trie_num_entries(trie) == 0);
}

int main(int argc, char *argv[])
{
	test_trie_new();
	test_trie_free();
	test_trie_insert_lookup_remove();

	return 0;
}

