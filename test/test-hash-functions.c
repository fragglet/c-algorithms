
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
#include <assert.h>

#include "alloc-testing.h"
#include "framework.h"

#include "hash-pointer.h"
#include "hash-int.h"
#include "hash-string.h"

void test_pointer_hash(void)
{
	int array[10];
	int i, j;

	/* Initialise the array to all zeros */
	
	for (i=0; i<10; ++i) {
		array[i] = 0;
	}
	
	/* Check hashes are never the same */

	for (i=0; i<10; ++i) {
		for (j=i+1; j<10; ++j) {
			assert(pointer_hash(&array[i]) != pointer_hash(&array[j]));
		}
	}
}

void test_int_hash(void)
{
	int array[100];
	int i, j;

	/* Initialise all entries in the array */

	for (i=0; i<100; ++i) {
		array[i] = i;
	}

	/* Check hashes are never the same */

	for (i=0; i<100; ++i) {
		for (j=i+1; j<100; ++j) {
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

