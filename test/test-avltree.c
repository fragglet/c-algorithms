 
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

#include "avltree.h"
#include "compare-int.h"

int find_subtree_height(AVLTreeNode *node)
{
	int left_height, right_height;

	if (node == NULL) {
		return 0;
        }

	left_height = find_subtree_height(avltree_node_left_child(node));
	right_height = find_subtree_height(avltree_node_right_child(node));

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

	left_node = avltree_node_left_child(node);
	right_node = avltree_node_right_child(node);

	/* Check the parent references of the children */

	if (left_node != NULL) {
		assert(avltree_node_parent(left_node) == node);
	}
	if (right_node != NULL) {
		assert(avltree_node_parent(right_node) == node);
	}

	/* Recursively validate the left and right subtrees,
	 * obtaining the height at the same time. */

	left_height = validate_subtree(left_node);

	/* Check that the keys are in the correct order */

	key = (int *) avltree_node_key(node);

	assert(*key > counter);
	counter = *key;
	
	right_height = validate_subtree(right_node);

	/* Check that the returned height value matches the 
	 * result of avltree_subtree_height(). */

	assert(avltree_subtree_height(left_node) == left_height);
	assert(avltree_subtree_height(right_node) == right_height);

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

	root_node = avltree_root_node(tree);

	counter = -1;
	validate_subtree(root_node);
}

void test_avltree_new(void)
{
	AVLTree *tree;

	tree = avltree_new((AVLTreeCompareFunc) int_compare);

	assert(tree != NULL);
	assert(avltree_root_node(tree) == NULL);
}

void test_avltree_insert_lookup(void)
{
	AVLTree *tree;
	AVLTreeNode *node;
	int i;
	int *key;

	/* Create a tree containing some values. Validate the 
	 * tree is consistent at all stages. */

	tree = avltree_new((AVLTreeCompareFunc) int_compare);

	for (i=0; i<1000; ++i) {
		key = (int *) malloc(sizeof(int));
		*key = i;

		avltree_insert(tree, key, NULL);

		validate_tree(tree);
	}

	assert(avltree_root_node(tree) != NULL);

	/* Check that all values can be read back again */

	for (i=0; i<1000; ++i) {
		node = avltree_lookup_node(tree, &i);
		assert(node != NULL);
	}

	/* Check that invalid nodes are not found */

	i = -1;
	assert(avltree_lookup_node(tree, &i) == NULL);
	i = 100000;
	assert(avltree_lookup_node(tree, &i) == NULL);
}

AVLTree *create_tree(void)
{
	AVLTree *tree;
	int *key;
	int i;

	/* Create a tree and fill with nodes */

	tree = avltree_new((AVLTreeCompareFunc) int_compare);

	for (i=0; i<1000; ++i) {
		key = (int *) malloc(sizeof(int));
		*key = i;

		avltree_insert(tree, key, NULL);
	}
	
	return tree;
}

void test_avltree_free(void)
{
	AVLTree *tree;
	
	/* Try freeing an empty tree */

	tree = avltree_new((AVLTreeCompareFunc) int_compare);
	avltree_free(tree);

	/* Create a big tree and free it */

	tree = create_tree();
	avltree_free(tree);
}

void test_avltree_remove(void)
{
	AVLTree *tree;
	int i;

	tree = create_tree();

	/* Try removing invalid entries */

	i = 100000;
	assert(avltree_remove(tree, &i) == 0);
	i = -1;
	assert(avltree_remove(tree, &i) == 0);

	/* Delete the nodes from the tree */

	for (i=0; i<1000; ++i) {
		/* Remove this entry */

		assert(avltree_remove(tree, &i) != 0);

		validate_tree(tree);
	}

	/* All entries removed, should be empty now */

	assert(avltree_root_node(tree) == NULL);
	
}

int main(int argc, char *argv[])
{
	test_avltree_new();
	test_avltree_free();
	test_avltree_insert_lookup();
	test_avltree_remove();
	
	return 0;
}


