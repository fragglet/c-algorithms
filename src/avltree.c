
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
};

AVLTree *avltree_new(AVLTreeCompareFunc compare_func)
{
    AVLTree *new_tree;

    new_tree = (AVLTree *) malloc(sizeof(AVLTree));
    new_tree->root_node = NULL;
    new_tree->compare_func = compare_func;

    return new_tree;
}

static int avltree_subtree_height(AVLTreeNode *node)
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

    if (left_height > right_height)
        node->height = left_height + 1;
    else
        node->height = right_height + 1;
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

    /* Update heights of the affected nodes */

    avltree_update_height(new_root);
    avltree_update_height(node);
}

AVLTreeNode *avltree_insert(AVLTree *tree, void *key, void *value)
{
    AVLTreeNode **rover;
    AVLTreeNode *new_node;
    AVLTreeNode *previous_node;
    AVLTreeNode *node;
    AVLTreeNode *new_root;
    int diff;

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

        /* Check the heights of the child trees.  If there is an unbalance
         * (difference between left and right > 2), then rotate nodes
         * around to fix it */

        diff = avltree_subtree_height(node->right_child)
             - avltree_subtree_height(node->left_child);

        if (diff >= 2) {
            
            /* Biased toward the right side too much. */

            if (tree->compare_func(key, node->right_child->key) <= 0) {

                /* If this was added to the left of the right child,
                 * rotate the right child right first (double rotation) */

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

            if (tree->compare_func(key, node->left_child->key) >= 0) {

                /* If this was added to the right of the left child,
                 * rotate the left child left first (double rotation) */

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

        /* Go to this node's parent */

        node = node->parent;
    }

    return new_node;
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

    if (node == NULL)
        return NULL;
    else
        return node->value;
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

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/* The following all needs to be removed eventually */

static void avltree_print_tree_node(AVLTreeNode *node, int indent)
{
    int i;

    if (node == NULL)
        return;

    for (i=0; i<indent; ++i)
        putchar(' ');

    printf("node:\n");

    avltree_print_tree_node(node->left_child, indent + 2);
    avltree_print_tree_node(node->right_child, indent + 2);
}

void avltree_print_tree(AVLTree *tree)
{
    avltree_print_tree_node(tree->root_node, 0);
}

static int avltree_check_node_balanced(AVLTreeNode *node)
{
    int diff;

    if (node == NULL)
        return 1;

    avltree_check_node_balanced(node->left_child);
    avltree_check_node_balanced(node->right_child);

    avltree_update_height(node);

    diff = avltree_subtree_height(node->left_child) 
         - avltree_subtree_height(node->right_child);

    return diff > -2 && diff < 2;
}

int avltree_check_balanced(AVLTree *tree)
{
    return avltree_check_node_balanced(tree->root_node);
}

