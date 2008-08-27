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

/* This file tests that the library can be used from C++ programs. */

#include <stdlib.h>
#include <assert.h>

#include <compare-int.h>
#include <compare-pointer.h>
#include <compare-string.h>

#include <hash-int.h>
#include <hash-pointer.h>
#include <hash-string.h>

#include <arraylist.h>
#include <avl-tree.h>
#include <binary-heap.h>
#include <binomial-heap.h>
#include <bloom-filter.h>
#include <hash-table.h>
#include <list.h>
#include <queue.h>
#include <set.h>
#include <slist.h>
#include <trie.h>

#include "framework.h"

static void test_compare_int(void)
{
	int a, b;

	a = 1; b = 2;
	assert(!int_equal(&a, &b));
	a = 2; b = 2;
	assert(int_equal(&a, &b));
}

static void test_compare_pointer(void)
{
	int a, b;
	void *p1, *p2;

	p1 = &a; p2 = &b;
	assert(!pointer_equal(p1, p2));
	p1 = &a; p2 = &a;
	assert(pointer_equal(p1, p2));
}

static void test_compare_string(void)
{
	char s1[] = "hello";
	char s2[] = "hello";
	char s3[] = "world";

	assert(!string_equal(s1, s3));
	assert(string_equal(s1, s2));
}

static void test_hash_int(void)
{
	int a, b;

	a = 1; b = 2;
	assert(int_hash(&a) != int_hash(&b));
	a = 2; b = 2;
	assert(int_hash(&a) == int_hash(&b));
}

static void test_hash_pointer(void)
{
	int a, b;
	void *p1, *p2;

	p1 = &a; p2 = &b;
	assert(pointer_hash(p1) != pointer_hash(p2));
	p1 = &a; p2 = &a;
	assert(pointer_hash(p1) == pointer_hash(p2));
}

static void test_hash_string(void)
{
	char s1[] = "hello";
	char s2[] = "hello";
	char s3[] = "world";

	assert(string_hash(s1) != string_hash(s3));
	assert(string_hash(s1) == string_hash(s2));
}

static void test_arraylist(void)
{
	ArrayList *arraylist;

	arraylist = arraylist_new(0);
	arraylist_free(arraylist);
}

static void test_avl_tree(void)
{
	AVLTree *avl_tree;

	avl_tree = avl_tree_new(string_compare);
	avl_tree_free(avl_tree);
}

static void test_binary_heap(void)
{
	BinaryHeap *heap;

	heap = binary_heap_new(BINARY_HEAP_TYPE_MAX, string_compare);
	binary_heap_free(heap);
}

static void test_binomial_heap(void)
{
	BinomialHeap *heap;

	heap = binomial_heap_new(BINOMIAL_HEAP_TYPE_MAX, string_compare);
	binomial_heap_free(heap);
}

static void test_bloom_filter(void)
{
	BloomFilter *filter;

	filter = bloom_filter_new(16, string_hash, 10);
	bloom_filter_free(filter);
}

static void test_hash_table(void)
{
	HashTable *hash_table;

	hash_table = hash_table_new(string_hash, string_equal);
	hash_table_free(hash_table);
}

static void test_list(void)
{
	ListEntry *list = NULL;
	int a, b, c;

	list_prepend(&list, &a);
	list_prepend(&list, &b);
	list_prepend(&list, &c);

	list_free(list);
}

static void test_queue(void)
{
	Queue *queue;

	queue = queue_new();
	queue_free(queue);
}

static void test_set(void)
{
	Set *set;

	set = set_new(string_hash, string_equal);
	set_free(set);
}

static void test_slist(void)
{
	SListEntry *list = NULL;
	int a, b, c;

	slist_prepend(&list, &a);
	slist_prepend(&list, &b);
	slist_prepend(&list, &c);

	slist_free(list);
}

static void test_trie(void)
{
	Trie *trie;

	trie = trie_new();
	trie_free(trie);
}

static UnitTestFunction tests[] = {
	test_compare_int,
	test_compare_pointer,
	test_compare_string,
	test_hash_int,
	test_hash_pointer,
	test_hash_string,
	test_arraylist,
	test_avl_tree,
	test_binary_heap, 
	test_binomial_heap,
	test_bloom_filter,
	test_hash_table,
	test_list,
	test_queue,
	test_set,
	test_slist,
	test_trie,
	NULL
};

int main(int argc, char *argv[])
{
	run_tests(tests);
}

