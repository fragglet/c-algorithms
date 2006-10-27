
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

#include <stdlib.h>

#include "avltree.h"

/* AVL Tree (balanced binary search tree) */

struct _AVLTreeNode {
	AVLTreeNode *left_child;
	AVLTreeNode *right_child;
	AVLTreeNode *parent;
	void *key;
	void *value;
	int height;
};

struct _AVLTree {
	AVLTreeNode *root_node;
	AVLTreeCompareFunc compare_func;
	int num_nodes;
};

AVLTree *avltree_new(AVLTreeCompareFunc compare_func)
{
	AVLTree *new_tree;

	new_tree = (AVLTree *) malloc(sizeof(AVLTree));

        if (new_tree == NULL) {
                return NULL; 
        }
        
	new_tree->root_node = NULL;
	new_tree->compare_func = compare_func;
	new_tree->num_nodes = 0;

	return new_tree;
}

static void avltree_free_subtree(AVLTree *tree, AVLTreeNode *node)
{
	if (node == NULL) {
		return;
	}

	avltree_free_subtree(tree, node->left_child);
	avltree_free_subtree(tree, node->right_child);

	free(node);
}

void avltree_free(AVLTree *tree)
{
	/* Destroy all nodes */
	
	avltree_free_subtree(tree, tree->root_node);

	/* Free back the main tree data structure */

	free(tree);
}

int avltree_subtree_height(AVLTreeNode *node)
{
	if (node == NULL) {
		return 0;
	} else {
		return node->height;
	}
}

/* Update the "height" variable of a node, from the heights of its
 * children.  This does not update the height variable of any parent
 * nodes. */

static void avltree_update_height(AVLTreeNode *node)
{
	int left_height, right_height;

	left_height = avltree_subtree_height(node->left_child);
	right_height = avltree_subtree_height(node->right_child);

	if (left_height > right_height) {
		node->height = left_height + 1;
	} else {
		node->height = right_height + 1;
	}
}

/* Rotate a section of the tree left.  'node' is the node at the top
 * of the section to be rotated. 
 * 
 *      B
 *     / \
 *    A   D
 *       / \ 
 *      C   E 
 *
 * is rotated to:
 *
 *        D
 *       / \
 *      B   E
 *     / \
 *    A   C
 */

static void avltree_rotate_left(AVLTree *tree, AVLTreeNode *node)
{
	AVLTreeNode *parent;
	AVLTreeNode *new_root;

	parent = node->parent;

	/* The right child will take the place of this node */

	new_root = node->right_child;
	
	/* Make new_root the root */
	
	new_root->parent = parent;

	if (parent == NULL) {

		/* No parent means this is the root of the entire tree */

		tree->root_node = new_root;

	} else {

		/* Either the left or right child pointer points into this
		 * section */

		if (parent->left_child == node) {
			parent->left_child = new_root;
		} else {
			parent->right_child = new_root;
		}
	}

	/* Rearrange pointers */

	node->right_child = new_root->left_child;
	new_root->left_child = node;

	/* Update parent references */

	node->parent = new_root;

	if (node->right_child != NULL) {
		node->right_child->parent = node;
	}

	/* Update heights of the affected nodes */

	avltree_update_height(new_root);
	avltree_update_height(node);
}

/* Rotate a section of the tree right.  'node' is the node at the top
 * of the section to be rotated. 
 * 
 *        D
 *       / \
 *      B   E
 *     / \
 *    A   C
 *
 * is rotated to:
 *
 *      B
 *     /  \
 *    A    D
 *        / \ 
 *       C   E 
 */

static void avltree_rotate_right(AVLTree *tree, AVLTreeNode *node)
{
	AVLTreeNode *parent;
	AVLTreeNode *new_root;

	parent = node->parent;

	/* The left child will now take the place of this node */

	new_root = node->left_child;
	
	/* Make new_root the root */
	
	new_root->parent = parent;

	if (parent == NULL) {

		/* No parent means this is the root of the entire tree */

		tree->root_node = new_root;

	} else {

		/* Either the left or right child pointer points into this
		 * section */

		if (parent->left_child == node) {
			parent->left_child = new_root;
		} else {
			parent->right_child = new_root;
		}
	}

	/* Rearrange pointers */

	node->left_child = new_root->right_child;
	new_root->right_child = node;

	/* Update parent references */

	node->parent = new_root;

	if (node->left_child != NULL) {
		node->left_child->parent = node;
	}

	/* Update heights of the affected nodes */

	avltree_update_height(new_root);
	avltree_update_height(node);
}

/* Balance a particular tree node.
 *
 * Returns the root node of the new subtree which is replacing the
 * old one. */

static AVLTreeNode *avltree_node_balance(AVLTree *tree, AVLTreeNode *node)
{
	AVLTreeNode *new_root;
	int diff;

	/* Check the heights of the child trees.  If there is an unbalance
	 * (difference between left and right > 2), then rotate nodes
	 * around to fix it */

	diff = avltree_subtree_height(node->right_child)
		 - avltree_subtree_height(node->left_child);

	if (diff >= 2) {
		
		/* Biased toward the right side too much. */

		if (avltree_subtree_height(node->right_child->right_child)
		 <= avltree_subtree_height(node->right_child->left_child)) {

			/* If the right child is biased toward the left
			 * side, it must be rotated right first (double
			 * rotation) */

			avltree_rotate_right(tree, node->right_child);
		}

		/* Perform a left rotation.  After this, the right child will
		 * take the place of this node.  Store a pointer to the right
		 * child so that we can continue where we left off. */

		new_root = node->right_child;
		
		avltree_rotate_left(tree, node);

		node = new_root;

	} else if (diff <= -2) {

		/* Biased toward the left side too much. */

		if (avltree_subtree_height(node->left_child->left_child)
		 <= avltree_subtree_height(node->left_child->right_child)) {

			/* If the left child is biased toward the right
			 * side, it must be rotated right left (double
			 * rotation) */

			avltree_rotate_left(tree, node->left_child);
		}

		/* Perform a right rotation.  After this, the left child
		 * will take the place of this node.  Store a pointer to the
		 * left child so that we can continue where we left off. */

		new_root = node->left_child;

		avltree_rotate_right(tree, node);

		node = new_root;
	}

	/* Update the height of this node */

	avltree_update_height(node);

	return node;
}

AVLTreeNode *avltree_insert(AVLTree *tree, void *key, void *value)
{
	AVLTreeNode **rover;
	AVLTreeNode *new_node;
	AVLTreeNode *previous_node;
	AVLTreeNode *node;

	/* Walk down the tree until we reach a NULL pointer */

	rover = &tree->root_node;
	previous_node = NULL;

	while (*rover != NULL) {
		previous_node = *rover;
		if (tree->compare_func(key, (*rover)->key) < 0) {
			rover = &((*rover)->left_child);
		} else {
			rover = &((*rover)->right_child);
		}
	}

	/* Create a new node.  Use the last node visited as the parent link. */

	new_node = (AVLTreeNode *) malloc(sizeof(AVLTreeNode));

        if (new_node == NULL) {
                return NULL;
        }
        
	new_node->left_child = NULL;
	new_node->right_child = NULL;
	new_node->parent = previous_node;
	new_node->key = key;
	new_node->value = value;
	new_node->height = 1;

	/* Insert at the NULL pointer that was reached */

	*rover = new_node;

	/* Walk up the tree performing any needed rotations */

	node = previous_node;

	while (node != NULL) {

		/* Balance this node if necessary */

		node = avltree_node_balance(tree, node);

		/* Go to this node's parent */

		node = node->parent;
	}

	/* Keep track of the number of entries */

	++tree->num_nodes;

	return new_node;
}

/* Remove a node from a tree */

void avltree_remove_node(AVLTree *tree, AVLTreeNode *node)
{
	AVLTreeNode *swap_node;
	AVLTreeNode *rover;
	AVLTreeNode *balance_startpoint;

	/* The node to be removed must be swapped with an "adjacent"
	 * node, ie. one which has the closest key to this one. Find
	 * a node to swap with. */

	if (node->left_child != NULL) {

		/* Search for the right-most node in the left subtree, ie.
		 * the greatest value. */

		swap_node = node->left_child;

		while (swap_node->right_child != NULL) {
			swap_node = swap_node->right_child;
		}

		/* Unlink the swap node, move the swap node's left 
		 * child tree in to replace it */

		if (swap_node->parent->right_child == swap_node) {
			swap_node->parent->right_child 
				= swap_node->left_child;
		} else {
			swap_node->parent->left_child
				= swap_node->left_child;
		}

		if (swap_node->left_child != NULL) {
			swap_node->left_child->parent = swap_node->parent;
		}

	} else if (node->right_child != NULL) {

		/* Search for the left-most node in the right subtree, ie.
		 * the least value. */

		swap_node = node->right_child;

		while (swap_node->left_child != NULL) {
			swap_node = swap_node->left_child;
		}

		/* Unlink the swap node, move the swap node's right 
		 * child tree in to replace it */

		if (swap_node->parent->left_child == swap_node) {
			swap_node->parent->left_child
				= swap_node->right_child;
		} else {
			swap_node->parent->right_child
				= swap_node->right_child;
		}

		if (swap_node->right_child != NULL) {
			swap_node->right_child->parent = swap_node->parent;
		}

	} else {
		/* This is a leaf node and has no children, therefore
		 * it can be immediately removed. */

		/* Unlink this node from its parent */

		if (node->parent != NULL) {
			if (node->parent->left_child == node) {
				node->parent->left_child = NULL;
			} else {
				node->parent->right_child = NULL;
			}
		} else {
			/* Root node */

			tree->root_node = NULL;
		}
		
		/* "swap" stage is skipped. */

		swap_node = NULL;

		/* Start balancing from the node's parent */

		balance_startpoint = node->parent;
	}

	/* Link the "swap" node into the tree, at the position where
	 * "node" previously was. */

	if (swap_node != NULL) {
		
		/* Copy references in the node into the swap node */

		swap_node->left_child = node->left_child;
		swap_node->right_child = node->right_child;
		swap_node->parent = node->parent;

		/* Link the parent's reference to this node */

		if (node->parent != NULL) {
			if (node->parent->left_child == node) {
				node->parent->left_child = swap_node;
			} else {
				node->parent->right_child = swap_node;
			}
		} else {
			/* Root of the tree */

			tree->root_node = swap_node;
		}

		/* Fix the "parent" references of child nodes */

		if (node->left_child != NULL) {
			node->left_child->parent = swap_node;
		}
		
		if (node->right_child != NULL) {
			node->right_child->parent = swap_node;
		}

		/* Start balancing from the swap node's former parent */

		balance_startpoint = swap_node->parent;
	}

	/* Destroy the node */

	free(node);

	/* Keep track of the number of nodes */

	--tree->num_nodes;

	/* Rebalance the tree */

	rover = balance_startpoint;

	while (rover != NULL) {

		/* Possibly rebalance this subtree */
		
		rover = avltree_node_balance(tree, rover);

		/* Go to the node's parent */

		rover = rover->parent;
	}
}

/* Remove a node by key */

int avltree_remove(AVLTree *tree, void *key)
{
	AVLTreeNode *node;

	/* Find the node to remove */

	node = avltree_lookup_node(tree, key);

	if (node == NULL) {
		/* Not found in tree */
		
		return 0;
	}

	/* Remove the node */

	avltree_remove_node(tree, node);

	return 1;
}

AVLTreeNode *avltree_lookup_node(AVLTree *tree, void *key)
{
	AVLTreeNode *node;
	int diff;
	
	/* Search down the tree and attempt to find the node which 
	 * has the specified key */

	node = tree->root_node;

	while (node != NULL) {
		
		diff = tree->compare_func(key, node->key);

		if (diff == 0) {

			/* Keys are equal: return this node */
			
			return node;
			
		} else if (diff < 0) {
			node = node->left_child;
		} else {
			node = node->right_child;
		}
	}

	/* Not found */

	return NULL;
}

void *avltree_lookup(AVLTree *tree, void *key)
{
	AVLTreeNode *node;

	/* Find the node */

	node = avltree_lookup_node(tree, key);

	if (node == NULL) {
		return NULL;
	} else {
		return node->value;
	}
}

AVLTreeNode *avltree_root_node(AVLTree *tree)
{
	return tree->root_node;
}


void *avltree_node_key(AVLTreeNode *node)
{
	return node->key;
}

void *avltree_node_value(AVLTreeNode *node)
{
	return node->value;
}

AVLTreeNode *avltree_node_left_child(AVLTreeNode *node)
{
	return node->left_child;
}

AVLTreeNode *avltree_node_right_child(AVLTreeNode *node)
{
	return node->right_child;
}

AVLTreeNode *avltree_node_parent(AVLTreeNode *node)
{
	return node->parent;
}

int avltree_num_entries(AVLTree *tree)
{
	return tree->num_nodes;
}

static void avltree_to_array_add_subtree(AVLTreeNode *subtree, 
                                         void **array, 
                                         int *index)
{
	if (subtree == NULL) {
		return;
	}
		
	/* Add left subtree first */

	avltree_to_array_add_subtree(subtree->left_child, array, index);
	
	/* Add this node */
	
	array[*index] = subtree->key;
	++*index;

	/* Finally add right subtree */

	avltree_to_array_add_subtree(subtree->right_child, array, index);
}

void **avltree_to_array(AVLTree *tree)
{
	void **array;
	int index;

	/* Allocate the array */
	
	array = malloc(sizeof(void *) * tree->num_nodes);

        if (array == NULL) {
                return NULL;
        }
        
	index = 0;

	/* Add all keys */
	
	avltree_to_array_add_subtree(tree->root_node, array, &index);

	return array;
}

