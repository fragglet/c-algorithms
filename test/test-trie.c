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
unsigned char bin_key[] = { 'a', 'b', 'c', 0, 1, 2, 0xff };
unsigned char bin_key2[] = { 'a', 'b', 'c', 0, 1, 2, 0xff, 0 };
unsigned char bin_key3[] = { 'a', 'b', 'c' };
unsigned char bin_key4[] = { 'z', 0, 'z', 'z' };

Trie *generate_trie(void)
{
	Trie *trie;
	int i;
	unsigned int entries;

	/* Create a trie and fill it with a large number of values */

	trie = trie_new();
	entries = 0;

	for (i=0; i<NUM_TEST_VALUES; ++i) {

		/* Create a string containing a text version of i, and use
		 * it as a key for the value */

		test_array[i] = i;
		sprintf(test_strings[i], "%i", i);

		assert(trie_insert(trie, test_strings[i],
		                   &test_array[i]) != 0);

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
	unsigned int entries;
	size_t allocated;

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
	unsigned int entries;

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

/* Test the use of the trie when characters in the keys used are negative
 * (top bit set in the character; alternative, c >= 128). */

static void test_trie_negative_keys(void)
{
	char my_key[] = { 'a', 'b', 'c', -50, -20, '\0' };
	Trie *trie;
	void *value;

	trie = trie_new();

	assert(trie_insert(trie, my_key, "hello world") != 0);

	value = trie_lookup(trie, my_key);

	assert(!strcmp(value, "hello world"));

	assert(trie_remove(trie, my_key) != 0);
	assert(trie_remove(trie, my_key) == 0);
	assert(trie_lookup(trie, my_key) == NULL);

	trie_free(trie);
}

Trie *generate_binary_trie(void)
{
	Trie *trie;

	trie = trie_new();

	/* Insert some values */

	assert(trie_insert_binary(trie,
	                          bin_key2, sizeof(bin_key2),
	                          "goodbye world") != 0);
	assert(trie_insert_binary(trie,
	                          bin_key, sizeof(bin_key),
	                          "hello world") != 0);

	return trie;
}

void test_trie_insert_binary(void)
{
	Trie *trie;
	char *value;

	trie = generate_binary_trie();

	/* Overwrite a value */

	assert(trie_insert_binary(trie,
	                          bin_key, sizeof(bin_key),
	                          "hi world") != 0);

	/* Insert NULL value doesn't work */

	assert(trie_insert_binary(trie, bin_key3,
	                          sizeof(bin_key3), NULL) == 0);

	/* Read them back */

	value = trie_lookup_binary(trie, bin_key, sizeof(bin_key));
	assert(!strcmp(value, "hi world"));

	value = trie_lookup_binary(trie, bin_key2, sizeof(bin_key2));
	assert(!strcmp(value, "goodbye world"));

	trie_free(trie);
}

void test_trie_insert_out_of_memory(void)
{
	Trie *trie;

	trie = generate_binary_trie();

	alloc_test_set_limit(3);

	assert(trie_insert_binary(trie,
	                          bin_key4, sizeof(bin_key4),
	                          "test value") == 0);

	assert(trie_lookup_binary(trie, bin_key4, sizeof(bin_key4)) == NULL);
	assert(trie_num_entries(trie) == 2);

	trie_free(trie);
}

void test_trie_remove_binary(void)
{
	Trie *trie;
	void *value;

	trie = generate_binary_trie();

	/* Test look up and remove of invalid values */

	value = trie_lookup_binary(trie, bin_key3, sizeof(bin_key3));
	assert(value == NULL);

	assert(trie_remove_binary(trie, bin_key3, sizeof(bin_key3)) == 0);

	assert(trie_lookup_binary(trie, bin_key4, sizeof(bin_key4)) == 0);
	assert(trie_remove_binary(trie, bin_key4, sizeof(bin_key4)) == 0);

	/* Remove the two values */

	assert(trie_remove_binary(trie, bin_key2, sizeof(bin_key2)) != 0);
	assert(trie_lookup_binary(trie, bin_key2, sizeof(bin_key2)) == NULL);
	assert(trie_lookup_binary(trie, bin_key, sizeof(bin_key)) != NULL);

	assert(trie_remove_binary(trie, bin_key, sizeof(bin_key)) != 0);
	assert(trie_lookup_binary(trie, bin_key, sizeof(bin_key)) == NULL);

	trie_free(trie);
}

static UnitTestFunction tests[] = {
	test_trie_new_free,
	test_trie_insert,
	test_trie_lookup,
	test_trie_remove,
	test_trie_replace,
	test_trie_insert_empty,
	test_trie_free_long,
	test_trie_negative_keys,
	test_trie_insert_binary,
	test_trie_insert_out_of_memory,
	test_trie_remove_binary,
	NULL
};

int main(int argc, char *argv[])
{
	run_tests(tests);

	return 0;
}

