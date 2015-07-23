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

#include "hash-table.h"
#include "hash-int.h"
#include "compare-int.h"
#include "hash-string.h"
#include "compare-string.h"

#define NUM_TEST_VALUES 10000

int value1 = 1, value2 = 2, value3 = 3, value4 = 4;
int allocated_keys = 0;
int allocated_values = 0;

/* Generates a hash table for use in tests containing 10,000 entries */

HashTable *generate_hash_table(void)
{
	HashTable *hash_table;
	char buf[10];
	char *value;
	int i;

	/* Allocate a new hash table.  We use a hash table with keys that are
	 * string versions of the integer values 0..9999 to ensure that there
	 * will be collisions within the hash table (using integer values
	 * with int_hash causes no collisions) */

	hash_table = hash_table_new(string_hash, string_equal);

	/* Insert lots of values */

	for (i=0; i<NUM_TEST_VALUES; ++i) {
		sprintf(buf, "%i", i);

		value = strdup(buf);

		hash_table_insert(hash_table, value, value);
	}

	/* Automatically free all the values with the hash table */

	hash_table_register_free_functions(hash_table, NULL, free);

	return hash_table;
}

/* Basic allocate and free */

void test_hash_table_new_free(void)
{
	HashTable *hash_table;

	hash_table = hash_table_new(int_hash, int_equal);

	assert(hash_table != NULL);

	/* Add some values */

	hash_table_insert(hash_table, &value1, &value1);
	hash_table_insert(hash_table, &value2, &value2);
	hash_table_insert(hash_table, &value3, &value3);
	hash_table_insert(hash_table, &value4, &value4);

	/* Free the hash table */

	hash_table_free(hash_table);

	/* Test out of memory scenario */

	alloc_test_set_limit(0);
	hash_table = hash_table_new(int_hash, int_equal);
	assert(hash_table == NULL);
	assert(alloc_test_get_allocated() == 0);

	alloc_test_set_limit(1);
	hash_table = hash_table_new(int_hash, int_equal);
	assert(hash_table == NULL);
	assert(alloc_test_get_allocated() == 0);
}

/* Test insert and lookup functions */

void test_hash_table_insert_lookup(void)
{
	HashTable *hash_table;
	char buf[10];
	char *value;
	int i;

	/* Generate a hash table */

	hash_table = generate_hash_table();

	assert(hash_table_num_entries(hash_table) == NUM_TEST_VALUES);

	/* Check all values */

	for (i=0; i<NUM_TEST_VALUES; ++i) {
		sprintf(buf, "%i", i);
		value = hash_table_lookup(hash_table, buf);

		assert(strcmp(value, buf) == 0);
	}

	/* Lookup on invalid values returns NULL */

	sprintf(buf, "%i", -1);
	assert(hash_table_lookup(hash_table, buf) == NULL);
	sprintf(buf, "%i", NUM_TEST_VALUES);
	assert(hash_table_lookup(hash_table, buf) == NULL);

	/* Insert overwrites existing entries with the same key */

	sprintf(buf, "%i", 12345);
	hash_table_insert(hash_table, buf, strdup("hello world"));
	value = hash_table_lookup(hash_table, buf);
	assert(strcmp(value, "hello world") == 0);

	hash_table_free(hash_table);
}

void test_hash_table_remove(void)
{
	HashTable *hash_table;
	char buf[10];

	hash_table = generate_hash_table();

	assert(hash_table_num_entries(hash_table) == NUM_TEST_VALUES);
	sprintf(buf, "%i", 5000);
	assert(hash_table_lookup(hash_table, buf) != NULL);

	/* Remove an entry */

	hash_table_remove(hash_table, buf);

	/* Check entry counter */

	assert(hash_table_num_entries(hash_table) == 9999);

	/* Check that NULL is returned now */

	assert(hash_table_lookup(hash_table, buf) == NULL);

	/* Try removing a non-existent entry */

	sprintf(buf, "%i", -1);
	hash_table_remove(hash_table, buf);

	assert(hash_table_num_entries(hash_table) == 9999);

	hash_table_free(hash_table);
}

void test_hash_table_iterating(void)
{
	HashTable *hash_table;
	HashTableIterator iterator;
	int count;

	hash_table = generate_hash_table();

	/* Iterate over all values in the table */

	count = 0;

	hash_table_iterate(hash_table, &iterator);

	while (hash_table_iter_has_more(&iterator)) {
		hash_table_iter_next(&iterator);

		++count;
	}

	assert(count == NUM_TEST_VALUES);

	/* Test iter_next after iteration has completed. */

	HashTablePair pair = hash_table_iter_next(&iterator);
	assert(pair.value == HASH_TABLE_NULL);

	hash_table_free(hash_table);

	/* Test iterating over an empty table */

	hash_table = hash_table_new(int_hash, int_equal);

	hash_table_iterate(hash_table, &iterator);

	assert(hash_table_iter_has_more(&iterator) == 0);

	hash_table_free(hash_table);
}

/* Demonstrates the ability to iteratively remove objects from
 * a hash table: ie. removing the current key being iterated over
 * does not break the iterator. */

void test_hash_table_iterating_remove(void)
{
	HashTable *hash_table;
	HashTableIterator iterator;
	char buf[10];
	char *val;
	HashTablePair pair;
	int count;
	unsigned int removed;
	int i;

	hash_table = generate_hash_table();

	/* Iterate over all values in the table */

	count = 0;
	removed = 0;

	hash_table_iterate(hash_table, &iterator);

	while (hash_table_iter_has_more(&iterator)) {

		/* Read the next value */

		pair = hash_table_iter_next(&iterator);
		val = pair.value;

		/* Remove every hundredth entry */

		if ((atoi(val) % 100) == 0) {
			hash_table_remove(hash_table, val);
			++removed;
		}

		++count;
	}

	/* Check counts */

	assert(removed == 100);
	assert(count == NUM_TEST_VALUES);

	assert(hash_table_num_entries(hash_table)
	       == NUM_TEST_VALUES - removed);

	/* Check all entries divisible by 100 were really removed */

	for (i=0; i<NUM_TEST_VALUES; ++i) {
		sprintf(buf, "%i", i);

		if (i % 100 == 0) {
			assert(hash_table_lookup(hash_table, buf) == NULL);
		} else {
			assert(hash_table_lookup(hash_table, buf) != NULL);
		}
	}

	hash_table_free(hash_table);
}

/* Create a new key */

int *new_key(int value)
{
	int *result;

	result = malloc(sizeof(int));
	*result = value;

	++allocated_keys;

	return result;
}

/* Callback function invoked when a key is freed */

void free_key(void *key)
{
	free(key);

	--allocated_keys;
}

/* Create a new value */

int *new_value(int value)
{
	int *result;

	result = malloc(sizeof(int));
	*result = value;

	++allocated_values;

	return result;
}

/* Callback function invoked when a value is freed */

void free_value(void *value)
{
	free(value);

	--allocated_values;
}

/* Test the use of functions to free keys / values when they are removed. */

void test_hash_table_free_functions(void)
{
	HashTable *hash_table;
	int *key;
	int *value;
	int i;

	/* Create a hash table, fill it with values */

	hash_table = hash_table_new(int_hash, int_equal);

	hash_table_register_free_functions(hash_table, free_key, free_value);

	allocated_values = 0;

	for (i=0; i<NUM_TEST_VALUES; ++i) {
		key = new_key(i);
		value = new_value(99);

		hash_table_insert(hash_table, key, value);
	}

	assert(allocated_keys == NUM_TEST_VALUES);
	assert(allocated_values == NUM_TEST_VALUES);

	/* Check that removing a key works */

	i = NUM_TEST_VALUES / 2;
	hash_table_remove(hash_table, &i);

	assert(allocated_keys == NUM_TEST_VALUES - 1);
	assert(allocated_values == NUM_TEST_VALUES - 1);

	/* Check that replacing an existing key works */

	key = new_key(NUM_TEST_VALUES / 3);
	value = new_value(999);

	assert(allocated_keys == NUM_TEST_VALUES);
	assert(allocated_values == NUM_TEST_VALUES);

	hash_table_insert(hash_table, key, value);

	assert(allocated_keys == NUM_TEST_VALUES - 1);
	assert(allocated_values == NUM_TEST_VALUES - 1);

	/* A free of the hash table should free all of the keys and values */

	hash_table_free(hash_table);

	assert(allocated_keys == 0);
	assert(allocated_values == 0);
}

/* Test for out of memory scenario */

void test_hash_table_out_of_memory(void)
{
	HashTable *hash_table;
	int values[66];
	unsigned int i;

	hash_table = hash_table_new(int_hash, int_equal);

	/* Test normal failure */

	alloc_test_set_limit(0);
	values[0] = 0;
	assert(hash_table_insert(hash_table, &values[0], &values[0]) == 0);
	assert(hash_table_num_entries(hash_table) == 0);

	alloc_test_set_limit(-1);

	/* Test failure when increasing table size.
	 * The initial table size is 193 entries.  The table increases in
	 * size when 1/3 full, so the 66th entry should cause the insert
	 * to fail.
	 */

	for (i=0; i<65; ++i) {
		values[i] = (int) i;

		assert(hash_table_insert(hash_table,
		                         &values[i], &values[i]) != 0);
		assert(hash_table_num_entries(hash_table) == i + 1);
	}

	assert(hash_table_num_entries(hash_table) == 65);

	/* Test the 66th insert */

	alloc_test_set_limit(0);

	values[65] = 65;

	assert(hash_table_insert(hash_table, &values[65], &values[65]) == 0);
	assert(hash_table_num_entries(hash_table) == 65);

	hash_table_free(hash_table);
}

void test_hash_iterator_key_pair()
{
	HashTable *hash_table;
	HashTableIterator iterator;
	HashTablePair pair;
	hash_table = hash_table_new(int_hash, int_equal);

	/* Add some values */

	hash_table_insert(hash_table, &value1, &value1);
	hash_table_insert(hash_table, &value2, &value2);

	hash_table_iterate(hash_table, &iterator);

	while (hash_table_iter_has_more(&iterator)) {

		/* Retrieve both Key and Value */

		pair = hash_table_iter_next(&iterator);

		int *key = (int*) pair.key;
		int *val = (int*) pair.value;

		assert(*key == *val);
	}

	hash_table_free(hash_table);
}

static UnitTestFunction tests[] = {
	test_hash_table_new_free,
	test_hash_table_insert_lookup,
	test_hash_table_remove,
	test_hash_table_iterating,
	test_hash_table_iterating_remove,
	test_hash_table_free_functions,
	test_hash_table_out_of_memory,
	test_hash_iterator_key_pair,
	NULL
};

int main(int argc, char *argv[])
{
	run_tests(tests);

	return 0;
}

