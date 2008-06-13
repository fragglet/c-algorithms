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

#include <stdlib.h>

#include "avl-tree.h"

/* malloc() / free() testing */

#ifdef ALLOC_TESTING
#include "alloc-testing.h"
#endif

/* AVL Tree (balanced binary search tree) */

struct _AVLTreeNode {
	AVLTreeNode *left_child;
	AVLTreeNode *right_child;
	AVLTreeNode *parent;
	AVLTreeKey key;
	AVLTreeValue value;
	int height;
};

struct _AVLTree {
	AVLTreeNode *root_node;
	AVLTreeCompareFunc compare_func;
	int num_nodes;
};

AVLTree *avl_tree_new(AVLTreeCompareFunc compare_func)
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

static void avl_tree_free_subtree(AVLTree *tree, AVLTreeNode *node)
{
	if (node == NULL) {
		return;
	}

	avl_tree_free_subtree(tree, node->left_child);
	avl_tree_free_subtree(tree, node->right_child);

	free(node);
}

void avl_tree_free(AVLTree *tree)
{
	/* Destroy all nodes */
	
	avl_tree_free_subtree(tree, tree->root_node);

	/* Free back the main tree data structure */

	free(tree);
}

int avl_tree_subtree_height(AVLTreeNode *node)
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

static void avl_tree_update_height(AVLTreeNode *node)
{
	int left_height, right_height;

	left_height = avl_tree_subtree_height(node->left_child);
	right_height = avl_tree_subtree_height(node->right_child);

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

static void avl_tree_rotate_left(AVLTree *tree, AVLTreeNode *node)
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

	avl_tree_update_height(new_root);
	avl_tree_update_height(node);
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

static void avl_tree_rotate_right(AVLTree *tree, AVLTreeNode *node)
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

	avl_tree_update_height(new_root);
	avl_tree_update_height(node);
}

/* Balance a particular tree node.
 *
 * Returns the root node of the new subtree which is replacing the
 * old one. */

static AVLTreeNode *avl_tree_node_balance(AVLTree *tree, AVLTreeNode *node)
{
	AVLTreeNode *new_root;
	int diff;

	/* Check the heights of the child trees.  If there is an unbalance
	 * (difference between left and right > 2), then rotate nodes
	 * around to fix it */

	diff = avl_tree_subtree_height(node->right_child)
	     - avl_tree_subtree_height(node->left_child);

	if (diff >= 2) {
		
		/* Biased toward the right side too much. */

		if (avl_tree_subtree_height(node->right_child->right_child)
		  < avl_tree_subtree_height(node->right_child->left_child)) {

			/* If the right child is biased toward the left
			 * side, it must be rotated right first (double
			 * rotation) */

			avl_tree_rotate_right(tree, node->right_child);
		}

		/* Perform a left rotation.  After this, the right child will
		 * take the place of this node.  Store a pointer to the right
		 * child so that we can continue where we left off. */

		new_root = node->right_child;
		
		avl_tree_rotate_left(tree, node);

		node = new_root;

	} else if (diff <= -2) {

		/* Biased toward the left side too much. */

		if (avl_tree_subtree_height(node->left_child->left_child)
		  < avl_tree_subtree_height(node->left_child->right_child)) {

			/* If the left child is biased toward the right
			 * side, it must be rotated right left (double
			 * rotation) */

			avl_tree_rotate_left(tree, node->left_child);
		}

		/* Perform a right rotation.  After this, the left child
		 * will take the place of this node.  Store a pointer to the
		 * left child so that we can continue where we left off. */

		new_root = node->left_child;

		avl_tree_rotate_right(tree, node);

		node = new_root;
	}

	/* Update the height of this node */

	avl_tree_update_height(node);

	return node;
}

AVLTreeNode *avl_tree_insert(AVLTree *tree, AVLTreeKey key, AVLTreeValue value)
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

		node = avl_tree_node_balance(tree, node);

		/* Go to this node's parent */

		node = node->parent;
	}

	/* Keep track of the number of entries */

	++tree->num_nodes;

	return new_node;
}

/* Remove a node from a tree */

void avl_tree_remove_node(AVLTree *tree, AVLTreeNode *node)
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

		/* Unlink this node from its parent and update tree height */

		if (node->parent != NULL) {
			if (node->parent->left_child == node) {
				node->parent->left_child = NULL;
			} else {
				node->parent->right_child = NULL;
			}

			avl_tree_update_height(node->parent);
		} else {
			/* Root node */

			tree->root_node = NULL;
		}
		
		/* "swap" stage is skipped. */

		swap_node = NULL;
	}

	/* Calculate where we will need to start rebalancing the tree.
	 * If it was a leaf node, start from the parent of the node
	 * we are removing.  Otherwise, use the old parent of the node we
	 * swapped out.  In some cases this is the node we are swapping
	 * back in again. */

	if (swap_node == NULL) {
		balance_startpoint = node->parent;
	} else if (swap_node->parent == node) {
		balance_startpoint = swap_node;
	} else {
		balance_startpoint = swap_node->parent;
	}

	/* Link the "swap" node into the tree, at the position where
	 * "node" previously was. */

	if (swap_node != NULL) {

		/* Update the subtree height for the swap node's old
		 * parent. */

		avl_tree_update_height(swap_node->parent);

		/* Copy references in the node into the swap node */

		swap_node->left_child = node->left_child;
		swap_node->right_child = node->right_child;
		swap_node->parent = node->parent;
		swap_node->height = node->height;

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
	}

	/* Destroy the node */

	free(node);

	/* Keep track of the number of nodes */

	--tree->num_nodes;

	/* Rebalance the tree */

	rover = balance_startpoint;

	while (rover != NULL) {

		/* Possibly rebalance this subtree */
		
		rover = avl_tree_node_balance(tree, rover);

		/* Go to the node's parent */

		rover = rover->parent;
	}
}

/* Remove a node by key */

int avl_tree_remove(AVLTree *tree, AVLTreeKey key)
{
	AVLTreeNode *node;

	/* Find the node to remove */

	node = avl_tree_lookup_node(tree, key);

	if (node == NULL) {
		/* Not found in tree */
		
		return 0;
	}

	/* Remove the node */

	avl_tree_remove_node(tree, node);

	return 1;
}

AVLTreeNode *avl_tree_lookup_node(AVLTree *tree, AVLTreeKey key)
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

AVLTreeValue avl_tree_lookup(AVLTree *tree, AVLTreeKey key)
{
	AVLTreeNode *node;

	/* Find the node */

	node = avl_tree_lookup_node(tree, key);

	if (node == NULL) {
		return AVL_TREE_NULL;
	} else {
		return node->value;
	}
}

AVLTreeNode *avl_tree_root_node(AVLTree *tree)
{
	return tree->root_node;
}

AVLTreeKey avl_tree_node_key(AVLTreeNode *node)
{
	return node->key;
}

AVLTreeValue avl_tree_node_value(AVLTreeNode *node)
{
	return node->value;
}

AVLTreeNode *avl_tree_node_left_child(AVLTreeNode *node)
{
	return node->left_child;
}

AVLTreeNode *avl_tree_node_right_child(AVLTreeNode *node)
{
	return node->right_child;
}

AVLTreeNode *avl_tree_node_parent(AVLTreeNode *node)
{
	return node->parent;
}

int avl_tree_num_entries(AVLTree *tree)
{
	return tree->num_nodes;
}

static void avl_tree_to_array_add_subtree(AVLTreeNode *subtree, 
                                         AVLTreeValue *array, 
                                         int *index)
{
	if (subtree == NULL) {
		return;
	}
		
	/* Add left subtree first */

	avl_tree_to_array_add_subtree(subtree->left_child, array, index);
	
	/* Add this node */
	
	array[*index] = subtree->key;
	++*index;

	/* Finally add right subtree */

	avl_tree_to_array_add_subtree(subtree->right_child, array, index);
}

AVLTreeValue *avl_tree_to_array(AVLTree *tree)
{
	AVLTreeValue *array;
	int index;

	/* Allocate the array */
	
	array = malloc(sizeof(AVLTreeValue) * tree->num_nodes);

	if (array == NULL) {
		return NULL;
	}
	
	index = 0;

	/* Add all keys */
	
	avl_tree_to_array_add_subtree(tree->root_node, array, &index);

	return array;
}

