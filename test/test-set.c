
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
#include "framework.h"

#include "set.h"
#include "compare-int.h"
#include "hash-int.h"
#include "compare-pointer.h"
#include "hash-pointer.h"
#include "compare-string.h"
#include "hash-string.h"

int allocated_values;

Set *generate_set(void)
{
	Set *set;
	char buf[10];
	int i;
	char *value;

	set = set_new(string_hash, string_equal);

	/* Add 10,000 items sequentially, checking that the counter 
	 * works properly */

	for (i=0; i<10000; ++i) {
		sprintf(buf, "%i", i);
		value = strdup(buf);

		set_insert(set, value);

		assert(set_num_entries(set) == i + 1);
	}

	set_register_free_function(set, free);

	return set;
}

void test_set_new_free(void)
{
	Set *set;
	int i;
	int *value;

	set = set_new(int_hash, int_equal);

	set_register_free_function(set, free);

	assert(set != NULL);

	/* Fill the set with many values before freeing */

	for (i=0; i<10000; ++i) {
		value = (int *) malloc(sizeof(int));

		*value = i;

		set_insert(set, value);
	}

	/* Free the set */

	set_free(set);
}

void test_set_insert(void)
{
	Set *set;
	int numbers1[] = { 1, 2, 3, 4, 5, 6 };
	int numbers2[] = { 5, 6, 7, 8, 9, 10 };
	int i;

	/* Perform a union of numbers1 and numbers2.  Cannot add the same
	 * value twice. */

	set = set_new(int_hash, int_equal);

	for (i=0; i<6; ++i) {
		set_insert(set, &numbers1[i]);
	}
	for (i=0; i<6; ++i) {
		set_insert(set, &numbers2[i]);
	}

	assert(set_num_entries(set) == 10);

	set_free(set);
}

void test_set_query(void)
{
	Set *set;
	char buf[10];
	int i;

	set = generate_set();

	/* Test all values */
	
	for (i=0; i<10000; ++i) {
		sprintf(buf, "%i", i);
		assert(set_query(set, buf) != 0);
	}

	/* Test invalid values returning zero */

	assert(set_query(set, "-1") == 0);
	assert(set_query(set, "100001") == 0);

	set_free(set);
}

void test_set_remove(void)
{
	Set *set;
	char buf[10];
	int i;
	int num_entries;

	set = generate_set();

	num_entries = set_num_entries(set);
	assert(num_entries == 10000);

	/* Remove some entries */

	for (i=4000; i<6000; ++i) {

		sprintf(buf, "%i", i);

		/* Check this is in the set */

		assert(set_query(set, buf) != 0);

		/* Remove it */

		assert(set_remove(set, buf) != 0);

		/* Check the number of entries decreases */

		assert(set_num_entries(set) == num_entries - 1);

		/* Check it is no longer in the set */

		assert(set_query(set, buf) == 0);

		--num_entries;
	}

	/* Try to remove some invalid entries */

	for (i=-1000; i<-500; ++i) {
		sprintf(buf, "%i", i);

		assert(set_remove(set, buf) == 0);
		assert(set_num_entries(set) == num_entries);
	}

	for (i=50000; i<51000; ++i) {
		sprintf(buf, "%i", i);

		assert(set_remove(set, buf) == 0);
		assert(set_num_entries(set) == num_entries);
	}

	set_free(set);
}

void test_set_union(void)
{
	int numbers1[] = {1, 2, 3, 4, 5, 6, 7};
	int numbers2[] = {5, 6, 7, 8, 9, 10, 11};
	int result[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
	int i;
	Set *set1;
	Set *set2;
	Set *result_set;

	/* Create the first set */

	set1 = set_new(int_hash, int_equal);

	for (i=0; i<7; ++i) {
		set_insert(set1, &numbers1[i]);
	}
	
	/* Create the second set */
	
	set2 = set_new(int_hash, int_equal);
	
	for (i=0; i<7; ++i) {
		set_insert(set2, &numbers2[i]);
	}
	
	/* Perform the union */

	result_set = set_union(set1, set2);

	assert(set_num_entries(result_set) == 11);

	for (i=0; i<11; ++i) {
		assert(set_query(result_set, &result[i]) != 0);
	}

	set_free(set1);
	set_free(set2);
	set_free(result_set);
}

void test_set_intersection(void)
{
	int numbers1[] = {1, 2, 3, 4, 5, 6, 7};
	int numbers2[] = {5, 6, 7, 8, 9, 10, 11};
	int result[] = {5, 6, 7};
	int i;
	Set *set1;
	Set *set2;
	Set *result_set;

	/* Create the first set */

	set1 = set_new(int_hash, int_equal);

	for (i=0; i<7; ++i) {
		set_insert(set1, &numbers1[i]);
	}
	
	/* Create the second set */
	
	set2 = set_new(int_hash, int_equal);
	
	for (i=0; i<7; ++i) {
		set_insert(set2, &numbers2[i]);
	}
	
	/* Perform the intersection */

	result_set = set_intersection(set1, set2);

	assert(set_num_entries(result_set) == 3);

	for (i=0; i<3; ++i) {
		assert(set_query(result_set, &result[i]) != 0);
	}

	set_free(set1);
	set_free(set2);
	set_free(result_set);
}

void test_set_to_array(void)
{
	Set *set;
	int values[100];
	int **array;
	int i;

	/* Create a set containing pointers to all entries in the "values"
	 * array. */
	
	set = set_new(pointer_hash, pointer_equal);

	for (i=0; i<100; ++i) {
		values[i] = 1;
		set_insert(set, &values[i]);
	}

	array = (int **) set_to_array(set);

	/* Check the array */

	for (i=0; i<100; ++i) {
		assert(*array[i] == 1);
		*array[i] = 0;
	}

	free(array);
	set_free(set);
}

void test_set_iterating(void)
{
	Set *set;
	SetIterator iterator;
	int count;

	set = generate_set();

	/* Iterate over all values in the set */

	count = 0;
	set_iterate(set, &iterator);

	while (set_iter_has_more(&iterator)) {

		set_iter_next(&iterator);

		++count;
	}

	/* Test iter_next after iteration has completed. */

	assert(set_iter_next(&iterator) == NULL);

	/* Check final count */

	assert(count == 10000);

	set_free(set);
	
	/* Test iterating over an empty set */

	set = set_new(int_hash, int_equal);

	set_iterate(set, &iterator);

	assert(set_iter_has_more(&iterator) == 0);
	
	set_free(set);
}

/* Test the ability to remove the current value while iterating over 
 * a set.  ie. the act of removing the current value should not affect
 * the iterator. */

void test_set_iterating_remove(void)
{
	Set *set;
	SetIterator iterator;
	int count;
	int removed;
	char *value;

	set = generate_set();

	count = 0;
	removed = 0;

	/* Iterate over all values in the set */

	set_iterate(set, &iterator);

	while (set_iter_has_more(&iterator)) {

		value = set_iter_next(&iterator);

		if ((atoi(value) % 100) == 0) {

			/* Remove this value */

			set_remove(set, value);

			++removed;
		}

		++count;
	}

	/* Check final counts */

	assert(count == 10000);
	assert(removed == 100);
	assert(set_num_entries(set) == 10000 - removed);

	set_free(set);
}

int *new_value(int value)
{
	int *result;

	result = malloc(sizeof(int));
	*result = value;

	++allocated_values;

	return result;
}

void free_value(void *value)
{
	free(value);

	--allocated_values;
}

void test_set_free_function(void)
{
	Set *set;
	int i;
	int *value;

	/* Create a set and fill it with 1000 values */

	set = set_new(int_hash, int_equal);

	set_register_free_function(set, free_value);

	allocated_values = 0;

	for (i=0; i<1000; ++i) {
		value = new_value(i);

		set_insert(set, value);
	}

	assert(allocated_values == 1000);

	/* Test removing a value */

	i = 500;
	set_remove(set, &i);

	assert(allocated_values == 999);

	/* Test freeing the set */

	set_free(set);

	assert(allocated_values == 0);
}

static UnitTestFunction tests[] = {
	test_set_new_free,
	test_set_insert,
	test_set_query,
	test_set_remove,
	test_set_intersection,
	test_set_union,
	test_set_iterating,
	test_set_iterating_remove,
	test_set_to_array,
	test_set_free_function,
	NULL
};

int main(int argc, char *argv[])
{
	run_tests(tests);

	return 0;
}

