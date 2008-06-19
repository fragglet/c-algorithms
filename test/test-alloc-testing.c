
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

/* Tests for the allocation testing framework. */

#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "alloc-testing.h"
#include "framework.h"

static void test_malloc_free(void)
{
	void *block;
	unsigned char *ptr;
	int i;

	/* Allocate a block and check that the counters increase */

	assert(alloc_test_get_allocated() == 0);

	block = malloc(1024);

	assert(block != NULL);
	assert(alloc_test_get_allocated() == 1024);

	/* Check that the block is initialised with garbage */

	ptr = block;

	for (i=0; i<1024; ++i) {
		assert(ptr[i] != 0);
	}

	/* Free the block back and check the counters decrease */

	free(block);

	assert(alloc_test_get_allocated() == 0);

	/* Try setting a limit */

	alloc_test_set_limit(1024);

	block = malloc(1025);
	assert(block == NULL);
	block = malloc(1024);
	assert(block != NULL);
	free(block);
}

static void test_realloc(void)
{
	void *block;
	void *block2;

	/* This block will be allocated while the other tests are run */

	block2 = malloc(1024);

	/* Allocate a block */

	block = malloc(1024);

	assert(block != NULL);
	assert(alloc_test_get_allocated() == 1024 + 1024);

	/* Reallocate the block larger */

	block = realloc(block, 2048);
	assert(block != NULL);

	assert(alloc_test_get_allocated() == 2048 + 1024);

	/* Reallocate the block smaller */

	block = realloc(block, 1500);
	assert(block != NULL);

	assert(alloc_test_get_allocated() == 1500 + 1024);

	free(block);

	assert(alloc_test_get_allocated() == 0 + 1024);

	/* Test passing a NULL pointer to make realloc behave as malloc() */

	block = realloc(NULL, 1024);

	assert(block != NULL);

	assert(alloc_test_get_allocated() == 1024 + 1024);

	free(block);
	free(block2);

	assert(alloc_test_get_allocated() == 0);

	/* Test realloc with a limit set */

	alloc_test_set_limit(1024);

	block = malloc(512);
	assert(block != NULL);
	assert(alloc_test_get_allocated() == 512);

	block = realloc(block, 1024);
	assert(block != NULL);
	assert(alloc_test_get_allocated() == 1024);

	assert(realloc(block, 1025) == NULL);
	assert(alloc_test_get_allocated() == 1024);

	free(block);
	assert(alloc_test_get_allocated() == 0);

	/* Test NULL realloc with limit */

	block = realloc(NULL, 1025);
	assert(block == NULL);
	assert(alloc_test_get_allocated() == 0);

	block = realloc(NULL, 1024);
	assert(block != NULL);
	assert(alloc_test_get_allocated() == 1024);

	free(block);
}

static void test_calloc(void)
{
	unsigned char *block;
	int i;

	assert(alloc_test_get_allocated() == 0);

	/* Allocate a block */

	block = calloc(16, 64);

	assert(alloc_test_get_allocated() == 1024);

	assert(block != NULL);

	/* Check the block contents are initialised to zero */

	for (i=0; i<1024; ++i) {
		assert(block[i] == 0);
	}

	free(block);

	assert(alloc_test_get_allocated() == 0);

	/* Test calloc with limit */

	alloc_test_set_limit(1024);

	block = calloc(1025, 1);
	assert(block == NULL);
	assert(alloc_test_get_allocated() == 0);

	block = calloc(1024, 1);
	assert(block != NULL);
	assert(alloc_test_get_allocated() == 1024);

	free(block);
}

static void test_strdup(void)
{
	char *str;

	assert(alloc_test_get_allocated() == 0);

	/* Test strdup */

	str = strdup("hello world");

	assert(str != NULL);
	assert(strcmp(str, "hello world") == 0);

	assert(alloc_test_get_allocated() == 12);

	free(str);

	assert(alloc_test_get_allocated() == 0);

	/* Test strdup with limit */

	alloc_test_set_limit(12);

	str = strdup("hello world1");
	assert(str == NULL);
	assert(alloc_test_get_allocated() == 0);

	str = strdup("hello world");
	assert(str != NULL);
	assert(alloc_test_get_allocated() == 12);

	free(str);
}

static void test_limits(void)
{
	void *block;
	void *block2;

	/* Test normal malloc */

	block = malloc(2048);
	assert(block != NULL);
	free(block);

	/* Test malloc with limit */

	alloc_test_set_limit(1024);
	assert(malloc(2048) == NULL);
	block = malloc(1024);
	assert(block != NULL);
	free(block);

	/* Check that it is possible to remove the limit */

	alloc_test_set_limit(-1);
	block = malloc(2048);
	assert(block != NULL);
	free(block);

	/* Check that limits are set _relative_ to the currently allocated
	   amount of memory */

	block2 = malloc(1024);
	assert(alloc_test_get_allocated() == 1024);
	alloc_test_set_limit(2048);
	block = malloc(2048);
	assert(block != NULL);
	free(block);
	free(block2);
}

static UnitTestFunction tests[] = {
	test_malloc_free,
	test_realloc,
	test_calloc,
	test_strdup,
	test_limits,
	NULL
};

int main(int argc, char *argv[])
{
	run_tests(tests);

	return 0;
}

