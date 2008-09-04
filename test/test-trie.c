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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "alloc-testing.h"
#include "framework.h"

#include "trie.h"

#define NUM_TEST_VALUES 10000

int test_array[NUM_TEST_VALUES];
char test_strings[NUM_TEST_VALUES][10];

Trie *generate_trie(void)
{
	Trie *trie;
	int i;
	int entries;

	/* Create a trie and fill it with a large number of values */

	trie = trie_new();
	entries = 0;

	for (i=0; i<NUM_TEST_VALUES; ++i) {

		/* Create a string containing a text version of i, and use
		 * it as a key for the value */
		
		test_array[i] = i;
		sprintf(test_strings[i], "%i", i);
		
		assert(trie_insert(trie, test_strings[i], &test_array[i]) != 0);

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

	assert(trie_insert(trie, "hello", "there") != 0);
	assert(trie_insert(trie, "hell", "testing") != 0);
	assert(trie_insert(trie, "testing", "testing") != 0);
	assert(trie_insert(trie, "", "asfasf") != 0);

	trie_free(trie);

	/* Add a value, remove it and then free */

	trie = trie_new();

	assert(trie_insert(trie, "hello", "there") != 0);
	assert(trie_remove(trie, "hello") != 0);
	
	trie_free(trie);

	/* Test out of memory scenario */

	alloc_test_set_limit(0);
	trie = trie_new();
	assert(trie == NULL);
}

void test_trie_insert(void)
{
	Trie *trie;
	int entries;
	int allocated;

	trie = generate_trie();

	/* Test insert of NULL value has no effect */

	entries = trie_num_entries(trie);
	assert(trie_insert(trie, "hello world", NULL) == 0);
	assert(trie_num_entries(trie) == entries);
	
	/* Test out of memory scenario */

	allocated = alloc_test_get_allocated();
	alloc_test_set_limit(0);
	assert(trie_insert(trie, "a", "test value") == 0);
	assert(trie_num_entries(trie) == entries);

	/* Test rollback */

	alloc_test_set_limit(5);
	assert(trie_insert(trie, "hello world", "test value") == 0);
	assert(alloc_test_get_allocated() == allocated);
	assert(trie_num_entries(trie) == entries);

	trie_free(trie);
}

void test_trie_lookup(void)
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

	for (i=0; i<NUM_TEST_VALUES; ++i) {

		sprintf(buf, "%i", i);

		val = (int *) trie_lookup(trie, buf);
		
		assert(*val == i);
	}

	trie_free(trie);
}

void test_trie_remove(void)
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

	assert(entries == NUM_TEST_VALUES);

	/* Remove all values */

	for (i=0; i<NUM_TEST_VALUES; ++i) {

		sprintf(buf, "%i", i);

		/* Remove value and check counter */

		assert(trie_remove(trie, buf) != 0);
		--entries;
		assert(trie_num_entries(trie) == entries);
	}

	trie_free(trie);
}

void test_trie_replace(void)
{
	Trie *trie;
	int *val;

	trie = generate_trie();

	/* Test replacing values */

	val = malloc(sizeof(int));
	*val = 999;
	assert(trie_insert(trie, "999", val) != 0);
	assert(trie_num_entries(trie) == NUM_TEST_VALUES);

	assert(trie_lookup(trie, "999") == val);
	free(val);
	trie_free(trie);
}

void test_trie_insert_empty(void)
{
	Trie *trie;
	char buf[10];

	trie = trie_new();

	/* Test insert on empty string */

	assert(trie_insert(trie, "", buf) != 0);
	assert(trie_num_entries(trie) != 0);
	assert(trie_lookup(trie, "") == buf);
	assert(trie_remove(trie, "") != 0);

	assert(trie_num_entries(trie) == 0);

	trie_free(trie);
}

#define LONG_STRING_LEN 4096
static void test_trie_free_long(void)
{
	char *long_string;
	Trie *trie;

	/* Generate a long string */

	long_string = malloc(LONG_STRING_LEN);
	memset(long_string, 'A', LONG_STRING_LEN);
	long_string[LONG_STRING_LEN - 1] = '\0';

	/* Create a trie and add the string */

	trie = trie_new();
	trie_insert(trie, long_string, long_string);

	trie_free(trie);

	free(long_string);
}

static UnitTestFunction tests[] = {
	test_trie_new_free,
	test_trie_insert,
	test_trie_lookup,
	test_trie_remove,
	test_trie_replace,
	test_trie_insert_empty,
	test_trie_free_long,
	NULL
};

int main(int argc, char *argv[])
{
	run_tests(tests);

	return 0;
}

