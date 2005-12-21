
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

/* AVL Tree (balanced binary search tree */

#ifndef ALGORITHM_AVLTREE_H
#define ALGORITHM_AVLTREE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _AVLTree AVLTree;
typedef struct _AVLTreeNode AVLTreeNode;

typedef int (*AVLTreeCompareFunc)(void *data1, void *data2);

/**
 * Create a new AVL tree.
 *
 * @param compare_func    Function to use when comparing keys in the tree.
 * @return                A new AVL tree.
 */

AVLTree *avltree_new(AVLTreeCompareFunc compare_func);

AVLTreeNode *avltree_insert(AVLTree *tree, void *key, void *value);

AVLTreeNode *avltree_lookup_node(AVLTree *tree, void *key);

/**
 * Search an AVL tree for a value corresponding to a particular
 * key.  This uses the tree as a mapping.
 *
 * @param tree            The AVL tree to search.
 * @param key             The key to search for.
 * @return                The value associated with the given key, or NULL
 *                        if no entry with the given key is found.
 */

void *avltree_lookup(AVLTree *tree, void *key);

/**
 * Retrieve the key for a given tree node.
 *
 * @param node            The tree node.
 * @return                The key to the given node.
 */

void *avltree_node_key(AVLTreeNode *node);

/** 
 * Retrieve the value at a given tree node.
 *
 * @param node            The tree node.
 * @return                The value at the given node.
 */

void *avltree_node_value(AVLTreeNode *node);

/**
 * Find the left child of a given tree node.
 *
 * @param node            The tree node.
 * @return                The left child of the tree node, or NULL if the
 *                        node has no left child.
 */

AVLTreeNode *avltree_node_left_child(AVLTreeNode *node);

/**
 * Find the right child of a given tree node.
 *
 * @param node            The tree node.
 * @return                The right child of the tree node, or NULL if the
 *                        node has no right child.
 */

AVLTreeNode *avltree_node_right_child(AVLTreeNode *node);

/**
 * Find the parent node of a given tree node.
 *
 * @param node            The tree node.
 * @return                The parent node of the tree node, or NULL if 
 *                        this is the root node.
 */

AVLTreeNode *avltree_node_parent(AVLTreeNode *node);


/******************** debug for removal ******************************/

void avltree_print_tree(AVLTree *tree);
int avltree_check_balanced(AVLTree *tree);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef ALGORITHM_AVLTREE_H */

