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
#include <assert.h>

#include "alloc-testing.h"
#include "framework.h"

#include "hash-pointer.h"
#include "hash-int.h"
#include "hash-string.h"

#define NUM_TEST_VALUES 200

void test_pointer_hash(void)
{
	int array[NUM_TEST_VALUES];
	int i, j;

	/* Initialise the array to all zeros */

	for (i=0; i<NUM_TEST_VALUES; ++i) {
		array[i] = 0;
	}

	/* Check hashes are never the same */

	for (i=0; i<NUM_TEST_VALUES; ++i) {
		for (j=i+1; j<NUM_TEST_VALUES; ++j) {
			assert(pointer_hash(&array[i])
			       != pointer_hash(&array[j]));
		}
	}
}

void test_int_hash(void)
{
	int array[NUM_TEST_VALUES];
	int i, j;

	/* Initialise all entries in the array */

	for (i=0; i<NUM_TEST_VALUES; ++i) {
		array[i] = i;
	}

	/* Check hashes are never the same */

	for (i=0; i<NUM_TEST_VALUES; ++i) {
		for (j=i+1; j<NUM_TEST_VALUES; ++j) {
			assert(int_hash(&array[i]) != int_hash(&array[j]));
		}
	}

	/* Hashes of two variables containing the same value are the same */

	i = 5000;
	j = 5000;

	assert(int_hash(&i) == int_hash(&j));
}

void test_string_hash(void)
{
	char test1[] = "this is a test";
	char test2[] = "this is a tesu";
	char test3[] = "this is a test ";
	char test4[] = "this is a test";
	char test5[] = "This is a test";

	/* Contents affect the hash */

	assert(string_hash(test1) != string_hash(test2));

	/* Length affects the hash */

	assert(string_hash(test1) != string_hash(test3));

	/* Case sensitive */

	assert(string_hash(test1) != string_hash(test5));

	/* The same strings give the same hash */

	assert(string_hash(test1) == string_hash(test4));
}

void test_string_nocase_hash(void)
{
	char test1[] = "this is a test";
	char test2[] = "this is a tesu";
	char test3[] = "this is a test ";
	char test4[] = "this is a test";
	char test5[] = "This is a test";

	/* Contents affect the hash */

	assert(string_nocase_hash(test1) != string_nocase_hash(test2));

	/* Length affects the hash */

	assert(string_nocase_hash(test1) != string_nocase_hash(test3));

	/* Case insensitive */

	assert(string_nocase_hash(test1) == string_nocase_hash(test5));

	/* The same strings give the same hash */

	assert(string_nocase_hash(test1) == string_nocase_hash(test4));
}

static UnitTestFunction tests[] = {
	test_pointer_hash,
	test_int_hash,
	test_string_hash,
	test_string_nocase_hash,
	NULL
};

int main(int argc, char *argv[])
{
	run_tests(tests);

	return 0;
}

