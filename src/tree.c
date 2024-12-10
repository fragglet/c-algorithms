/*

Copyright (c) 2016, Stefan Cloudt

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

#include "tree.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>

/**
 * The structure of a tree node.
 */
struct _TreeNode {
	/**
	 * The parent of a node.
	 */
	TreeNode* parent;

	/**
	 * The children of the node. This is an array whose size is _alloced. All 
	 * elements in the range [0,outDegree) are defined and not null.
	 */
	TreeNode** children;

	/**
	 * The amount of children of this node.
	 */
	unsigned int outDegree;

	/**
	 * The data this node stores.
	 */
	TreeNodeValue data;

	/**
	 * The allocated size of children.
	 */
	unsigned int _alloced;

	/**
	 * The current position of this node in its parent's child array.
	 */
	unsigned int i;

	/**
	 * The height of this node.
	 */
	unsigned int h;
};

struct _TreeIterator {
	Tree* t;
	TreeNode* prev;
	TreeNode* current;
	TreeNode* next;
};

/* Updates the height values for the nodes when n is changed. */
static void tree_update_height(TreeNode* n)
{
	while (n != NULL) {
		unsigned int h = 0;
		unsigned int i = 0;
		for (i = 0; i < n->outDegree; i++) {
			h = (h > n->children[i]->h) ? h : n->children[i]->h;
		}

		n->h = h;
		n = n->parent;
	}
}

/* Grows the internal children array to size s if it is smaller. */
static int tree_children_grow_min_size(TreeNode* n, unsigned int s)
{
	TreeNode** arr;

	if (n->_alloced >= s) {
		return 1;
	}

	arr = realloc(n->children, sizeof(TreeNode*) * s);

	if (arr == NULL) {
		return 0;
	}

	n->children = arr;
	n->_alloced = s;

	return 1;
}

/* Removes node c as child from its parent child array. */
static void tree_node_remove_child(TreeNode* c)

{
	TreeNode* p = c->parent;
	unsigned int i = 0;

	/* Loop to the place where c resides. */
	for (i = 0; i < p->outDegree && p->children[i] != c; i++); 

	p->children[i] = NULL;

	/* Move all others one place back. */
	for (; i < p->outDegree - 1; i++) {
		p->children[i] = p->children[i+1];
		p->children[i]->i = i;
	}

	p->outDegree--;
}

Tree* tree_alloc(TreeNodeValue data) 
{
	Tree* t = malloc(sizeof(struct _TreeNode));

	if (t == NULL) {
		return NULL;
	}

	t->parent = NULL;
	t->children = NULL;
	t->outDegree = 0;
	t->data = data;
	t->_alloced = 0;
	t->i = 0;
	t->h = 0;

	return t;
}

void tree_free(Tree* t)
{
	if (t != NULL) {
		unsigned int i;
		
		for (i = 0; i < t->outDegree; i++) {
			tree_free(t->children[i]);
		}

		free(t->children);
		free(t);
	}
}

TreeNode* tree_root(Tree* t)
{
	return t;
}

TreeNode* tree_abs_root(TreeNode* n)
{
	TreeNode* b = n;

	while (n != NULL) {
		b = n;
		n = n->parent;
	}

	return b;
}

Tree* tree_subtree(TreeNode* n)
{
	return n;
}

TreeNodeValue tree_data(TreeNode* n)
{
	return n->data;
}

void tree_set_data(TreeNode* n, TreeNodeValue d)
{
	n->data = d;
}

TreeNode* tree_parent(Tree* t, TreeNode* n)
{
	if (n == NULL) {
		return NULL;
	}

	/* If t == n then n is root of t so no parent in t. */
	if (t == n) {
		return NULL;
	}

	return n->parent;
}

TreeNode* tree_child(TreeNode* n, unsigned int i)
{
	if (tree_out_degree(n) > i) {
		return n->children[i];
	}
	else {
		return NULL;
	}
}

TreeNode* tree_first_child(TreeNode* n)
{
	if (tree_out_degree(n) > 0) {
		return n->children[0];
	}
	else {
		return NULL;
	}
}

TreeNode* tree_last_child(TreeNode* n)
{
	unsigned int od = tree_out_degree(n);

	if (od > 0) {
		/* By def of tree node this cannot be NULL. */
		return n->children[od - 1];
	}
	else {
		return NULL;
	}
}

unsigned int tree_out_degree(TreeNode* n)
{
	return n->outDegree;
}

TreeNode** tree_children(TreeNode* n)
{
	if (tree_out_degree(n) == 0) {
		return NULL;
	}
	else {
		return n->children;
	}
}

int tree_is_leaf(TreeNode* n)
{
	return tree_out_degree(n) == 0;
}

int tree_is_descendant_of(TreeNode* d, TreeNode* n)
{
	while (d != NULL && d != n) {
		d = d->parent;
	}

	return d == n;
}

int tree_is_ancestor_of(TreeNode* a, TreeNode* n)
{
	return tree_is_descendant_of(n, a);
}

unsigned int tree_depth(Tree* t, TreeNode* n)
{
	unsigned int d = 0;

	if (!tree_is_ancestor_of(t, n)) {
		return 0;
	}

	while (n != NULL && n != t) {
		n = n->parent;
		d++;
	}

	return d;
}

unsigned int tree_level(
		Tree* t,
		TreeNode* n)
{
	if (!tree_is_ancestor_of(t, n)) {
		return 0;
	}

	return tree_depth(t, n) + 1;
}

unsigned int tree_height(Tree* t)
{
	if (t == NULL) {
		return 0;
	}

	return t->h;
}

unsigned int tree_height_node(TreeNode* n)
{
	return n->h;
}

Tree* tree_remove(TreeNode* n)
{
	if (n->parent == NULL) {
		return n;
	}

	/* Remove n from children of p. */
	tree_node_remove_child(n);

	tree_update_height(n->parent);

	n->parent = NULL;
	n->i = 0;

	return n;
}

void tree_delete(TreeNode* n)
{
	TreeNode* r = tree_remove(n);
	tree_free(r);
}

Tree* tree_add_subtree(TreeNode* n, Tree* t)
{
	if (t == NULL || t->parent != NULL) {
		return NULL;
	}

	(void) tree_remove((TreeNode*) t);

	if (!tree_children_grow_min_size(n, n->outDegree + 1)) {
		return t;
	}

	n->children[n->outDegree] = t;

	t->parent = n;
	t->i = n->outDegree;
	n->outDegree++;

	tree_update_height(n);

	return NULL;
}

TreeNode* tree_add_child(TreeNode* n, TreeNodeValue data)
{
	TreeNode* c;

	if (!tree_children_grow_min_size(n, n->outDegree + 1)) {
		return NULL;
	}
	
	c = tree_alloc(data);

	if (c == NULL) {
		return NULL;
	}

	n->children[n->outDegree] = c;

	c->parent = n;
	c->i = n->outDegree;
	n->outDegree++;

	tree_update_height(n);

	return c;
}

Tree* tree_insert_subtree(TreeNode* n, Tree* t, unsigned int i)
{
	unsigned int a;

	if (t == NULL || i > n->outDegree || t->parent != NULL) {
		return t;
	}

	if (!tree_children_grow_min_size(n, n->outDegree + 1)) {
		return t;
	}

	/* create space for t */
	for (a = n->outDegree - 1; a >= i; a--) {
		n->children[a + 1] = n->children[a];
		n->children[a + 1]->i = a + 1;
	}

	n->children[i] = t;
	n->outDegree++;

	t->parent = n;
	t->i = i;

	tree_update_height(n);

	return NULL;
}

TreeNode* tree_insert_child(TreeNode* n, TreeNodeValue data, unsigned int i)
{
	TreeNode* c;

	if (i > n->outDegree) {
		return NULL;
	}

	c = tree_alloc(data);
	c = tree_insert_subtree(n, c, i);

	if (c == NULL) {
		return c;
	}
	else {
		tree_free(c);
		return NULL;
	}
}

Tree* tree_set_subtree(TreeNode* n, Tree* t, unsigned int i)
{
	if (t == NULL || i > n->outDegree || t->parent != NULL) {
		return t;
	}

	if (!tree_children_grow_min_size(n, n->outDegree + 1)) {
		return t;
	}

	if (i == n->outDegree) {
		n->children[i] = t;
		n->outDegree++;
	}
	else {
		tree_free(n->children[i]);
		n->children[i] = t;
	}

	t->parent = n;
	t->i = i;

	tree_update_height(n);

	return NULL;
}

TreeNode* tree_set_child(TreeNode* n, TreeNodeValue data, unsigned int i)
{
	TreeNode* c;

	if (i > n->outDegree) {
		return NULL;
	}

	c = tree_alloc(data);
	c = tree_set_subtree(n, c, i);

	if (c == NULL) {
		return c;
	}
	else {
		tree_free(c);
		return NULL;
	}
}

TreeNodeValue tree_iter_data(TreeIterator* i)
{
	if (i->current == NULL) {
		return NULL;
	}

	return i->current->data;
}

int tree_iter_has_next(TreeIterator* i)
{
	return i->next != NULL;
}

int tree_iter_has_prev(TreeIterator* i)
{
	return i->prev != NULL;
}

TreeIterator* tree_iter_alloc()
{
	TreeIterator* i = malloc(sizeof(TreeIterator));

	if (i == NULL) {
		return NULL;
	}

	i->current = NULL;
	i->t = NULL;
	i->next = NULL;
	i->prev = NULL;

	return i;
}

void tree_iter_free(TreeIterator* i)
{
	free(i);
}

void tree_iter_leaves_first(TreeIterator* i, Tree* t)
{
	TreeNode* nc = t;
	TreeNode* cc = t;

	i->t = t;

	while (nc != NULL) {
		cc = nc;
		
		if (cc->outDegree > 0) {
			nc = nc->children[0];
		}
		else {
			nc = NULL;
		}
	}
	
	i->next = cc;
}

void tree_iter_leaves_last(TreeIterator* i, Tree* t)
{
	TreeNode* nc = t;
	TreeNode* cc = t;

	i->t = t;

	while (nc != NULL) {
		cc = nc;
		
		if (cc->outDegree > 0) {
			nc = nc->children[nc->outDegree - 1];
		}
		else {
			nc = NULL;
		}
	}
	
	i->prev = cc;
}

TreeNode* tree_iter_leaves_next(TreeIterator* i)
{
	TreeNode* p;

	i->prev = i->current;
	i->current = i->next;

	/* Find layer where there is a right sibling. */
	p = i->next;
	while (p != NULL && p != i->t) {
		if (p->parent != NULL && p->i < p->parent->outDegree - 1) {
			break;	
		}
		
		p = p->parent;
	}

	if (p == NULL || p == i->t) {
		i->next = NULL;
		return i->current;
	}

	/* Now move down the right branch to it's children. */
	p = p->parent->children[p->i + 1];
	while (p->outDegree > 0) {
		p = p->children[0];
	}

	i->next = p;
	
	return i->current;
}

TreeNode* tree_iter_leaves_prev(TreeIterator* i)
{
	TreeNode* p;

	i->next = i->current;
	i->current = i->prev;

	/* Find layer where there is a left sibling. */
	p = i->prev;
	while (p != NULL && p != i->t) {
		if (p->i > 0) {
			break;
		}

		p = p->parent;
	}

	if (p == NULL || p == i->t) {
		i->prev = NULL;
		return i->current;
	}

	/* Now we go down to find the previous child */
	p = p->parent->children[p->i - 1];
	while (p->outDegree > 0) {
		p = p->children[p->outDegree - 1];
	}

	i->prev = p;

	return i->current;
}

void tree_iter_parents(TreeIterator* i, Tree* t, TreeNode* n)
{
	i->t = t;
	i->prev = NULL;
	i->current = NULL;
	i->next = n;
}

TreeNode* tree_iter_parents_next(TreeIterator* i)
{
	i->prev = i->current;
	i->current = i->next;

	if (i->next != NULL && i->next != i->t) {
		i->next = i->next->parent;
	}
	else {
		i->next = NULL;
	}

	return i->current;
}

void tree_preorder_walk(TreeIterator* i, Tree* t)
{
	i->t = t;
	i->next = t;
	i->current = NULL;
	i->prev = NULL;
}

TreeNode* tree_preorder_walk_next(TreeIterator* i)
{
	i->prev = i->current;
	i->current = i->next;

	if (i->next == NULL) {
		return i->current;
	}

	if (i->next->outDegree > 0) {
		i->next = i->next->children[0];
		return i->current;
	}

	while (i->next != NULL && 
			i->next->parent != NULL && 
			i->next != i->t) {
		if (i->next->i < i->next->parent->outDegree - 1) {
			i->next = i->next->parent->children[i->next->i + 1];
			return i->current;
		}

		i->next = i->next->parent;
	}

	i->next = NULL;
	return i->current;
}

TreeNode* tree_preorder_walk_prev(TreeIterator* i)
{
	i->next = i->current;
	i->current = i->prev;

	if (i->prev == i->t || i->prev == NULL) {
		i->prev = NULL;
	}
	else if (i->prev->i > 0) {
		i->prev = i->prev->parent->children[i->prev->i - 1];

		while (i->prev->outDegree > 0) {
			i->prev = i->prev->children[i->prev->outDegree - 1];
		}
	}
	else {
		i->prev = i->prev->parent;
	}

	return i->current;
}

void tree_postorder_walk(TreeIterator* i, Tree* t)
{
	i->t = t;
	i->next = t;
	i->current = NULL;
	i->prev = NULL;

	while (i->next != NULL && i->next->outDegree > 0) {
		i->next = i->next->children[0];
	}
}

TreeNode* tree_postorder_walk_next(TreeIterator* i)
{
	i->prev = i->current;
	i->current = i->next;

	if (i->next == NULL || 
			i->next->parent == NULL || 
			i->next == i->t) {
		i->next = NULL;
	}
    else if (i->next->i < i->next->parent->outDegree - 1) {
		i->next = i->next->parent->children[i->next->i + 1];

		while (i->next->outDegree > 0) {
			i->next = i->next->children[0];
		}
	}
	else {
		i->next = i->next->parent;
	}

	return i->current;
}

TreeNode* tree_postorder_walk_prev(TreeIterator* i)
{
	i->next = i->current;
	i->current = i->prev;

	if (i->prev == NULL) {
		return i->current;
	}

	if (i->prev->outDegree > 0) {
		i->prev = i->prev->children[i->prev->outDegree - 1];
		return i->prev;
	}

	while (i->prev->parent != NULL && 
			i->prev != NULL && 
			i->prev != i->t) {
		if (i->prev ->i > 0) {
			i->prev = i->prev->parent->children[i->prev->i - 1];
			return i->prev;
		}

		i->prev = i->prev->parent;
	}

	i->prev = NULL;
	return i->current;;
}
