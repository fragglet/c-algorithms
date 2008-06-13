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

#define ALLOC_TESTING_C

#include "alloc-testing.h"

/* All allocated blocks are given this magic number */

#define ALLOC_TEST_MAGIC 0x72ec82d2

/* This value is written to memory after it is freshly allocated, to ensure
 * that code under test does not rely on memory being initialised by 
 * malloc(). */

#define MALLOC_PATTERN 0xBAADF00D

/* This value is written to memory after it is freed, to ensure that code
 * does not rely on memory that has been freed. */

#define FREE_PATTERN 0xDEADBEEF

/**
 * All blocks allocated by the testing framework are preceded by a structure
 * of this type.
 */

typedef struct _BlockHeader BlockHeader;

struct _BlockHeader {
	unsigned int magic_number;
	size_t bytes;
	BlockHeader *prev, *next;
};

/* Head of a linked list of allocated blocks. */

static BlockHeader *allocated_blocks = NULL;

/* Count of the current number of allocated bytes. */

static size_t allocated_bytes = 0;

/* Allocation limit, negative value means no limit. */

signed int allocation_limit = -1;

/* Get the block header for an allocated pointer. */

static BlockHeader *alloc_test_get_header(void *ptr)
{
	BlockHeader *result;

	/* Go back from the start of the memory block to get the header. */

	result = ((BlockHeader *) ptr) - 1;

	assert(result->magic_number == ALLOC_TEST_MAGIC);
	
	return result;
}

/* Test whether a new value of allocated_bytes would be valid. */

static int alloc_test_new_size_allowed(size_t new_size)
{
	return allocation_limit < 0
	    || new_size <= allocation_limit;
}

/* Overwrite a block of memory with a repeated pattern. */

static void alloc_test_overwrite(void *ptr, size_t length, unsigned int pattern)
{
	unsigned char *byte_ptr;
	unsigned int pattern_seq;
	unsigned char pattern_byte;
	size_t i;

	byte_ptr = ptr;

	for (i=0; i<length; ++i) {
		pattern_seq = i & 3;
		pattern_byte = (pattern >> (8 * pattern_seq)) & 0xff;
		byte_ptr[i] = pattern_byte;
	}
}

/* Base malloc function used by other functions. */

BlockHeader *alloc_test_base_malloc(size_t bytes)
{
	BlockHeader *header;
	void *ptr;

	/* Allocate the requested block with enough room for the block header
	 * as well. */

	header = malloc(sizeof(BlockHeader) + bytes);

	if (header == NULL) {
		return NULL;
	}

	header->magic_number = ALLOC_TEST_MAGIC;
	header->bytes = bytes;
	
	/* Hook the new block into the linked list. */

	header->prev = NULL;
	header->next = allocated_blocks;
	allocated_blocks = header;

	if (header->next != NULL) {
		header->next->prev = header;
	}

	/* Fill memory with MALLOC_PATTERN, to ensure that code under test
	 * does not rely on memory being initialised to zero. */

	ptr = header + 1;
	alloc_test_overwrite(ptr, bytes, MALLOC_PATTERN);

	return header;
}

/* Base free function */

void alloc_test_base_free(BlockHeader *header)
{
	void *ptr;

	/* Unlink from the linked list. */

	if (header->prev != NULL) {
		header->prev->next = header->next;
	} else {
		allocated_blocks = header->next;
	}

	if (header->next != NULL) {
		header->next->prev = header->prev;
	}
	
	/* Trash the allocated block to foil any code that relies on memory 
	 * that has been freed. */

	ptr = header + 1;
	alloc_test_overwrite(ptr, header->bytes, FREE_PATTERN);

	/* Trash the magic number in the block header to stop the same block
	 * from being freed again. */

	header->magic_number = 0;

	/* Free the allocated memory. */

	free(header);
}

/* Main malloc function */

void *alloc_test_malloc(size_t bytes)
{
	BlockHeader *header;
	void *result;

	/* Check if we have reached the allocation limit. */

	if (!alloc_test_new_size_allowed(allocated_bytes + bytes)) {
		return NULL;
	}

	/* Do the allocation */

	header = alloc_test_base_malloc(bytes);

	if (header == NULL) {
		return NULL;
	}

	/* Update counter */

	allocated_bytes += bytes;

	/* Skip past the header and return the block itself */

	result = header + 1;
	return result;
}

void alloc_test_free(void *ptr)
{
	BlockHeader *header;
	size_t block_size;

	/* Must accept NULL as a valid pointer to free. */

	if (ptr == NULL) {
		return;
	}

	/* Free the block */

	header = alloc_test_get_header(ptr);
	block_size = header->bytes;
	assert(allocated_bytes >= block_size);

	alloc_test_base_free(header);

	/* Update counter */

	allocated_bytes -= block_size;
}

void *alloc_test_realloc(void *ptr, size_t bytes)
{
	BlockHeader *old_block, *new_block;
	void *new_ptr;
	size_t old_bytes;
	size_t new_allocated_bytes;
	size_t bytes_to_copy;

	/* A NULL value can be passed to ptr to make realloc behave
	 * like malloc(). */

	if (ptr != NULL) {
		old_block = alloc_test_get_header(ptr);
		old_bytes = old_block->bytes;
	} else {
		old_block = NULL;
		old_bytes = 0;
	}

	/* Sanity check the new allocated_bytes value */

	assert(allocated_bytes + bytes >= old_bytes);
	
	/* Check the new value for allocated bytes is within the 
	 * allocation limit. */

	new_allocated_bytes = allocated_bytes + bytes - old_bytes;

	if (!alloc_test_new_size_allowed(new_allocated_bytes)) {
		return NULL;
	}

	/* Always allocate a new block, to ensure that code does not rely
	 * on reallocated blocks having the same memory location. */

	new_block = alloc_test_base_malloc(bytes);

	if (new_block == NULL) {
		return NULL;
	}

	/* Work out how many bytes to copy. */

	if (bytes < old_bytes) {
		bytes_to_copy = bytes;
	} else {
		bytes_to_copy = old_bytes;
	}

	/* Copy the old block to the new block. */

	new_ptr = new_block + 1;
	memcpy(new_ptr, ptr, bytes_to_copy);

	/* Free the old block. */
	
	if (old_block != NULL) {
		alloc_test_base_free(old_block);
	}

	/* Update allocated_bytes counter. */

	allocated_bytes = new_allocated_bytes;

	return new_ptr;
}

void *alloc_test_calloc(size_t nmemb, size_t bytes)
{
	void *result;
	size_t total_bytes = nmemb * bytes;

	/* Allocate the block. */

	result = alloc_test_malloc(total_bytes);

	if (result == NULL) {
		return NULL;
	}

	/* Initialise to zero. */

	memset(result, 0, total_bytes);

	return result;
}

char *alloc_test_strdup(const char *string)
{
	char *result;
	
	result = alloc_test_malloc(strlen(string) + 1);

	if (result == NULL) {
		return NULL;
	}

	strcpy(result, string);

	return result;
}

void alloc_test_set_limit(signed int bytes)
{
	if (bytes < 0) {
		allocation_limit = -1;
	} else {
		allocation_limit = allocated_bytes + bytes;
	}
}

size_t alloc_test_get_allocated(void)
{
	return allocated_bytes;
}

