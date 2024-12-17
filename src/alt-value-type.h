/*

Copyright (c) 2024 Simon Howard

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

/* This header file is used for testing that the *Key and *Value types can be
   replaced with different types to a void pointer. When built with
   -DTEST_ALTERNATE_VALUE_TYPES, this file will be included to define
   different types and confirm the library still builds. */

typedef struct {
	void *ptr;
	int v;
} StructType2;

typedef struct {
	int a, b;
} StructType;

#define STRUCT_TYPE_NULL                                                       \
	{                                                                      \
		0, 0                                                           \
	}
#define STRUCT_TYPE2_NULL                                                      \
	{                                                                      \
		(void *) 0, 0                                                  \
	}

typedef StructType ArrayListValue;
#define ARRAY_LIST_NULL STRUCT_TYPE_NULL

typedef StructType2 AVLTreeKey;
typedef StructType AVLTreeValue;
#define AVL_TREE_NULL STRUCT_TYPE_NULL

typedef StructType BinaryHeapValue;
#define BINARY_HEAP_NULL STRUCT_TYPE_NULL

typedef StructType BinomialHeapValue;
#define BINOMIAL_HEAP_NULL STRUCT_TYPE_NULL

typedef StructType BloomFilterValue;
#define BLOOM_FILTER_NULL STRUCT_TYPE_NULL

typedef StructType2 HashTableKey;
#define HASH_TABLE_KEY_NULL STRUCT_TYPE2_NULL
typedef StructType HashTableValue;
#define HASH_TABLE_NULL STRUCT_TYPE_NULL

typedef StructType ListValue;
#define LIST_NULL STRUCT_TYPE_NULL

typedef StructType QueueValue;
#define QUEUE_NULL STRUCT_TYPE_NULL

typedef StructType2 RBTreeKey;
typedef StructType RBTreeValue;
#define RB_TREE_NULL STRUCT_TYPE_NULL

typedef StructType SetValue;
#define SET_NULL STRUCT_TYPE_NULL

typedef StructType SListValue;
#define SLIST_NULL STRUCT_TYPE_NULL

typedef StructType SortedArrayValue;
#define SORTED_ARRAY_NULL STRUCT_TYPE_NULL

typedef StructType TrieValue;
#define TRIE_NULL STRUCT_TYPE_NULL
