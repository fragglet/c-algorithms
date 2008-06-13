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

#include "avl-tree.h"
#include "compare-int.h"

int test_array[1000];

int find_subtree_height(AVLTreeNode *node)
{
	int left_height, right_height;

	if (node == NULL) {
		return 0;
	}

	left_height = find_subtree_height(avl_tree_node_left_child(node));
	right_height = find_subtree_height(avl_tree_node_right_child(node));

	if (left_height > right_height) {
		return left_height + 1;
	} else {
		return right_height + 1;
	}
}

/* Validates a subtree, returning its height */

int counter;

int validate_subtree(AVLTreeNode *node)
{
	AVLTreeNode *left_node, *right_node;
	int left_height, right_height;
	int *key;

	if (node == NULL) {
		return 0;
	}

	left_node = avl_tree_node_left_child(node);
	right_node = avl_tree_node_right_child(node);

	/* Check the parent references of the children */

	if (left_node != NULL) {
		assert(avl_tree_node_parent(left_node) == node);
	}
	if (right_node != NULL) {
		assert(avl_tree_node_parent(right_node) == node);
	}

	/* Recursively validate the left and right subtrees,
	 * obtaining the height at the same time. */

	left_height = validate_subtree(left_node);

	/* Check that the keys are in the correct order */

	key = (int *) avl_tree_node_key(node);

	assert(*key > counter);
	counter = *key;
	
	right_height = validate_subtree(right_node);

	/* Check that the returned height value matches the 
	 * result of avl_tree_subtree_height(). */

	assert(avl_tree_subtree_height(left_node) == left_height);
	assert(avl_tree_subtree_height(right_node) == right_height);

	/* Check this node is balanced */

	assert(left_height - right_height < 2 && right_height - left_height < 2);

	/* Calculate the height of this node */

	if (left_height > right_height) {
		return left_height + 1;
	} else {
		return right_height + 1;
	}
}

void validate_tree(AVLTree *tree)
{
	AVLTreeNode *root_node;

	root_node = avl_tree_root_node(tree);

	counter = -1;
	validate_subtree(root_node);
}

void test_avl_tree_new(void)
{
	AVLTree *tree;

	tree = avl_tree_new((AVLTreeCompareFunc) int_compare);

	assert(tree != NULL);
	assert(avl_tree_root_node(tree) == NULL);
	assert(avl_tree_num_entries(tree) == 0);

	avl_tree_free(tree);
}

void test_avl_tree_insert_lookup(void)
{
	AVLTree *tree;
	AVLTreeNode *node;
	int i;

	/* Create a tree containing some values. Validate the 
	 * tree is consistent at all stages. */

	tree = avl_tree_new((AVLTreeCompareFunc) int_compare);

	for (i=0; i<1000; ++i) {
		test_array[i] = i;
		avl_tree_insert(tree, &test_array[i], NULL);

		assert(avl_tree_num_entries(tree) == i+1);
		validate_tree(tree);
	}

	assert(avl_tree_root_node(tree) != NULL);

	/* Check that all values can be read back again */

	for (i=0; i<1000; ++i) {
		node = avl_tree_lookup_node(tree, &i);
		assert(node != NULL);
	}

	/* Check that invalid nodes are not found */

	i = -1;
	assert(avl_tree_lookup_node(tree, &i) == NULL);
	i = 100000;
	assert(avl_tree_lookup_node(tree, &i) == NULL);

	avl_tree_free(tree);
}

AVLTree *create_tree(void)
{
	AVLTree *tree;
	int i;

	/* Create a tree and fill with nodes */

	tree = avl_tree_new((AVLTreeCompareFunc) int_compare);

	for (i=0; i<1000; ++i) {
		test_array[i] = i;
		avl_tree_insert(tree, &test_array[i], NULL);
	}
	
	return tree;
}

void test_avl_tree_free(void)
{
	AVLTree *tree;
	
	/* Try freeing an empty tree */

	tree = avl_tree_new((AVLTreeCompareFunc) int_compare);
	avl_tree_free(tree);

	/* Create a big tree and free it */

	tree = create_tree();
	avl_tree_free(tree);
}

void test_avl_tree_remove(void)
{
	AVLTree *tree;
	int i;

	tree = create_tree();

	/* Try removing invalid entries */

	i = 100000;
	assert(avl_tree_remove(tree, &i) == 0);
	i = -1;
	assert(avl_tree_remove(tree, &i) == 0);

	/* Delete the nodes from the tree */

	for (i=0; i<1000; ++i) {
		/* Remove this entry */

		assert(avl_tree_remove(tree, &i) != 0);

		validate_tree(tree);

		assert(avl_tree_num_entries(tree) == 1000 - (i+1));
	}

	/* All entries removed, should be empty now */

	assert(avl_tree_root_node(tree) == NULL);

	avl_tree_free(tree);
}

void test_avl_tree_to_array(void)
{
	AVLTree *tree;
	int entries[] = { 89, 23, 42, 4, 16, 15, 8, 99, 50, 30 };
	int sorted[]  = { 4, 8, 15, 16, 23, 30, 42, 50, 89, 99 };
	int num_entries = sizeof(entries) / sizeof(int);
	int i;
	int **array;

	/* Add all entries to the tree */
	
	tree = avl_tree_new((AVLTreeCompareFunc) int_compare);

	for (i=0; i<num_entries; ++i) {
		avl_tree_insert(tree, &entries[i], NULL);
	}
	
	assert(avl_tree_num_entries(tree) == num_entries);

	/* Convert to an array and check the contents */

	array = (int **) avl_tree_to_array(tree);

	for (i=0; i<num_entries; ++i) {
		assert(*array[i] == sorted[i]);
	}

	free(array);

	avl_tree_free(tree);
}

static UnitTestFunction tests[] = {
	test_avl_tree_new,
	test_avl_tree_free,
	test_avl_tree_insert_lookup,
	test_avl_tree_remove,
	test_avl_tree_to_array,
	NULL
};
	
int main(int argc, char *argv[])
{
	run_tests(tests);
	return 0;
}


