
/*

Copyright (c) 2008, Simon Howard

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

#include "rb-tree.h"

/* malloc() / free() testing */

#ifdef ALLOC_TESTING
#include "alloc-testing.h"
#endif

struct _RBTreeNode {
	RBTreeNodeColor color;
	RBTreeKey key;
	RBTreeValue value;
	RBTreeNode *parent;
	RBTreeNode *children[2];
};

struct _RBTree {
	RBTreeNode *root_node;
	RBTreeCompareFunc compare_func;
	int num_nodes;
};

static RBTreeNodeSide rb_tree_node_side(RBTreeNode *node)
{
	if (node->parent->children[RB_TREE_NODE_LEFT] == node) {
		return RB_TREE_NODE_LEFT;
	} else {
		return RB_TREE_NODE_RIGHT;
	}
}

static RBTreeNode *rb_tree_node_sibling(RBTreeNode *node)
{
	RBTreeNodeSide side;

	side = rb_tree_node_side(node);

	return node->parent->children[1 - side];
}

RBTreeNode *rb_tree_node_uncle(RBTreeNode *node)
{
	return rb_tree_node_sibling(node->parent);
}

/* Replace node1 with node2 at its parent. */

static void rb_tree_node_replace(RBTree *tree, RBTreeNode *node1,
                                 RBTreeNode *node2)
{
	int side;

	/* Set the node's parent pointer. */

	if (node2 != NULL) {
		node2->parent = node1->parent;
	}

	/* The root node? */

	if (node1->parent == NULL) {
		tree->root_node = node2;
	} else {
		side = rb_tree_node_side(node1);
		node1->parent->children[side] = node2;
	}
}

/* Rotate a section of the tree.  'node' is the node at the top
 * of the section to be rotated.  'direction' is the direction in
 * which to rotate the tree: left or right, as shown in the following
 * diagram:
 *
 * Left rotation:              Right rotation:
 *
 *      B                             D
 *     / \                           / \
 *    A   D                         B   E
 *       / \                       / \
 *      C   E                     A   C

 * is rotated to:              is rotated to:
 *
 *        D                           B
 *       / \                         / \
 *      B   E                       A   D
 *     / \                             / \
 *    A   C                           C   E
 */

static RBTreeNode *rb_tree_rotate(RBTree *tree, RBTreeNode *node,
                                  RBTreeNodeSide direction)
{
	RBTreeNode *new_root;

	/* The child of this node will take its place:
	   for a left rotation, it is the right child, and vice versa. */

	new_root = node->children[1-direction];

	/* Make new_root the root, update parent pointers. */

	rb_tree_node_replace(tree, node, new_root);

	/* Rearrange pointers */

	node->children[1-direction] = new_root->children[direction];
	new_root->children[direction] = node;

	/* Update parent references */

	node->parent = new_root;

	if (node->children[1-direction] != NULL) {
		node->children[1-direction]->parent = node;
	}

	return new_root;
}


RBTree *rb_tree_new(RBTreeCompareFunc compare_func)
{
	RBTree *new_tree;

	new_tree = malloc(sizeof(RBTree));

	if (new_tree == NULL) {
		return NULL;
	}

	new_tree->root_node = NULL;
	new_tree->num_nodes = 0;
	new_tree->compare_func = compare_func;

	return new_tree;
}

static void rb_tree_free_subtree(RBTreeNode *node)
{
	if (node != NULL) {
		/* Recurse to subnodes */

		rb_tree_free_subtree(node->children[RB_TREE_NODE_LEFT]);
		rb_tree_free_subtree(node->children[RB_TREE_NODE_RIGHT]);

		/* Free this node */

		free(node);
	}
}

void rb_tree_free(RBTree *tree)
{
	/* Free all nodes in the tree */

	rb_tree_free_subtree(tree->root_node);

	/* Free back the main tree structure */

	free(tree);
}

static void rb_tree_insert_case1(RBTree *tree, RBTreeNode *node);
static void rb_tree_insert_case2(RBTree *tree, RBTreeNode *node);
static void rb_tree_insert_case3(RBTree *tree, RBTreeNode *node);
static void rb_tree_insert_case4(RBTree *tree, RBTreeNode *node);
static void rb_tree_insert_case5(RBTree *tree, RBTreeNode *node);

/* Insert case 1: If the new node is at the root of the tree, it must
 * be recolored black, as the root is always black. */

static void rb_tree_insert_case1(RBTree *tree, RBTreeNode *node)
{
	if (node->parent == NULL) {

		/* The root node is black */

		node->color = RB_TREE_NODE_BLACK;

	} else {

		/* Not root */

		rb_tree_insert_case2(tree, node);
	}
}

/* Insert case 2: If the parent of the new node is red, this
 * conflicts with the red-black tree conditions, as both children
 * of every red node are black. */

static void rb_tree_insert_case2(RBTree *tree, RBTreeNode *node)
{
	/* Note that if this function is being called, we already know
	 * the node has a parent, as it is not the root node. */

	if (node->parent->color != RB_TREE_NODE_BLACK) {
		rb_tree_insert_case3(tree, node);
	}
}

/* Insert case 3: If the parent and uncle are both red, repaint them
 * both black and repaint the grandparent red.  */

static void rb_tree_insert_case3(RBTree *tree, RBTreeNode *node)
{
	RBTreeNode *grandparent;
	RBTreeNode *uncle;

	/* Note that the node must have a grandparent, as the parent
	 * is red, and the root node is always black. */

	grandparent = node->parent->parent;
	uncle = rb_tree_node_uncle(node);

	if (uncle != NULL && uncle->color == RB_TREE_NODE_RED) {

		node->parent->color = RB_TREE_NODE_BLACK;
		uncle->color = RB_TREE_NODE_BLACK;
		grandparent->color = RB_TREE_NODE_RED;

		/* Recurse to grandparent */

		rb_tree_insert_case1(tree, grandparent);

	} else {
		rb_tree_insert_case4(tree, node);
	}
}

/* Case 4: If the parent is red, but the uncle is black, we need to do
 * some rotations to keep the tree balanced and complying with the tree
 * conditions.  If the node is on the opposite side relative to its parent
 * as the parent is relative to its grandparent, rotate around the
 * parent.  Either way, we will continue to case 5.
 *
 * eg.
 *
 *         B                              B
 *        / \                            / \
 *       R   B          ->     node ->  R   B
 *        \                            /
 *         R  <- node                 R
 *
 */

void rb_tree_insert_case4(RBTree *tree, RBTreeNode *node)
{
	RBTreeNode *next_node;
	RBTreeNodeSide side;

	/* Note that at this point, by implication from case 3, we know
	 * that the parent is red, but the uncle is black.  We therefore
	 * only need to examine what side the node is on relative
	 * to its parent, and the side the parent is on relative to
	 * the grandparent. */

	side = rb_tree_node_side(node);

	if (side != rb_tree_node_side(node->parent)) {

		/* After the rotation, we will continue to case 5, but
		 * the parent node will be at the bottom. */

		next_node = node->parent;

		/* Rotate around the parent in the opposite direction
		 * to side. */

		rb_tree_rotate(tree, node->parent, 1-side);
	} else {
		next_node = node;
	}

	rb_tree_insert_case5(tree, next_node);
}

/* Case 5: The node is on the same side relative to its parent as the
 * parent is relative to its grandparent.  The node and its parent are
 * red, but the uncle is black.
 *
 * Now, rotate at the grandparent and recolor the parent and grandparent
 * to black and red respectively.
 *
 *               G/B                 P/B
 *              /   \               /   \
 *           P/R     U/B    ->   N/R     G/R
 *          /   \                       /   \
 *       N/R      ?                   ?      U/B
 *
 */

void rb_tree_insert_case5(RBTree *tree, RBTreeNode *node)
{
	RBTreeNode *parent;
	RBTreeNode *grandparent;
	RBTreeNodeSide side;

	parent = node->parent;
	grandparent = parent->parent;

	/* What side are we, relative to the parent?  This will determine
	 * the direction that we rotate. */

	side = rb_tree_node_side(node);

	/* Rotate at the grandparent, in the opposite direction to side. */

	rb_tree_rotate(tree, grandparent, 1-side);

	/* Recolor the (old) parent and grandparent. */

	parent->color = RB_TREE_NODE_BLACK;
	grandparent->color = RB_TREE_NODE_RED;
}

RBTreeNode *rb_tree_insert(RBTree *tree, RBTreeKey key, RBTreeValue value)
{
	RBTreeNode *node;
	RBTreeNode **rover;
	RBTreeNode *parent;
	RBTreeNodeSide side;

	/* Allocate a new node */

	node = malloc(sizeof(RBTreeNode));

	if (node == NULL) {
		return NULL;
	}

	/* Set up structure.  Initially, the node is red. */

	node->key = key;
	node->value = value;
	node->color = RB_TREE_NODE_RED;
	node->children[RB_TREE_NODE_LEFT] = NULL;
	node->children[RB_TREE_NODE_RIGHT] = NULL;

	/* First, perform a normal binary tree-style insert. */

	parent = NULL;
	rover = &tree->root_node;

	while (*rover != NULL) {

		/* Update parent */

		parent = *rover;

		/* Choose which path to go down, left or right child */

		if (tree->compare_func(value, (*rover)->value) < 0) {
			side = RB_TREE_NODE_LEFT;
		} else {
			side = RB_TREE_NODE_RIGHT;
		}

		rover = &(*rover)->children[side];
	}

	/* Insert at the position we have reached */

	*rover = node;
	node->parent = parent;

	/* Possibly reorder the tree. */

	rb_tree_insert_case1(tree, node);

	/* Update the node count */

	++tree->num_nodes;

	return node;
}

RBTreeNode *rb_tree_lookup_node(RBTree *tree, RBTreeKey key)
{
	RBTreeNode *node;
	RBTreeNodeSide side;
	int diff;

	node = tree->root_node;

	/* Search down the tree. */

	while (node != NULL) {
		diff = tree->compare_func(key, node->key);

		if (diff == 0) {
			return node;
		} else if (diff < 0) {
			side = RB_TREE_NODE_LEFT;
		} else {
			side = RB_TREE_NODE_RIGHT;
		}

		node = node->children[side];
	}

	/* Not found. */

	return NULL;
}

RBTreeValue rb_tree_lookup(RBTree *tree, RBTreeKey key)
{
	RBTreeNode *node;

	/* Find the node for this key. */

	node = rb_tree_lookup_node(tree, key);

	if (node == NULL) {
		return RB_TREE_NULL;
	} else {
		return node->value;
	}
}

void rb_tree_remove_node(RBTree *tree, RBTreeNode *node)
{
	/* TODO */
}

int rb_tree_remove(RBTree *tree, RBTreeKey key)
{
	RBTreeNode *node;

	/* Find the node to remove. */

	node = rb_tree_lookup_node(tree, key);

	if (node == NULL) {
		return 0;
	}

	rb_tree_remove_node(tree, node);

	return 1;
}

RBTreeNode *rb_tree_root_node(RBTree *tree)
{
	return tree->root_node;
}

RBTreeKey rb_tree_node_key(RBTreeNode *node)
{
	return node->key;
}

RBTreeValue rb_tree_node_value(RBTreeNode *node)
{
	return node->value;
}

RBTreeNode *rb_tree_node_child(RBTreeNode *node, RBTreeNodeSide side)
{
	if (side == RB_TREE_NODE_LEFT || side == RB_TREE_NODE_RIGHT) {
		return node->children[side];
	} else {
		return NULL;
	}
}

RBTreeNode *rb_tree_node_parent(RBTreeNode *node)
{
	return node->parent;
}

RBTreeValue *rb_tree_to_array(RBTree *tree)
{
	/* TODO */
	return NULL;
}

int rb_tree_num_entries(RBTree *tree)
{
	return tree->num_nodes;
}

