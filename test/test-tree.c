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
)
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "framework.h"
#include "tree.h"

int vals[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

Tree* generate_tree()
{
	Tree* t = tree_alloc(&vals[0]);
	assert(t != NULL);

	TreeNode* t1 = tree_add_child(t, &vals[1]);
	tree_add_child(t1, &vals[2]);
	TreeNode* t3 = tree_add_child(t1, &vals[3]);

	int i = 0;
	for (i = 4; i <= 6; i++) {
		tree_add_child(t3, &vals[i]);
	}

	TreeNode* t7 = tree_add_child(t, &vals[7]);
	TreeNode* t8 = tree_add_child(t7, &vals[8]);
	tree_add_child(t8, &vals[9]);
	
	return t;
}

void test_tree_abs_root_r(Tree* t, TreeNode* n) 
{
	assert(tree_abs_root(n) == t);

	unsigned int i = 0;
	for (i = 0; i < tree_out_degree(n); i++) {
		test_tree_abs_root_r(t, tree_child(n, i));
	}
}

void test_tree_abs_root(void)
{
	Tree* t = generate_tree();
	test_tree_abs_root_r(t, t);	
	tree_free(t);
}

void test_tree_is_leaf_r(TreeNode* n)
{
	assert(tree_is_leaf(n) == (tree_out_degree(n) == 0));

	unsigned int i = 0;
	for (i = 0; i < tree_out_degree(n); i++) {
		test_tree_is_leaf_r(tree_child(n, i));
	}
}

void test_tree_is_leaf(void)
{
	Tree* t = generate_tree();
	test_tree_is_leaf_r(t);
	tree_free(t);
}

void test_tree_is_descendant_of_r(Tree* t, TreeNode* n)
{
	TreeNode* p = tree_parent(t, n);	
	while (p != NULL) {
		assert(tree_is_descendant_of(n, p));
		p = tree_parent(t, p);
	}

	unsigned int i = 0;
	for (i = 0; i < tree_out_degree(n); i++) {
		test_tree_is_descendant_of_r(t, tree_child(n, i));
	}
}

void test_tree_is_descendant_of(void)
{
	Tree* t = generate_tree();
	test_tree_is_descendant_of_r(t, t);
	tree_free(t);
}

void test_tree_is_ancestor_of_r(Tree* t, TreeNode* n)
{
	TreeNode* p = tree_parent(t, n);
	while (p != NULL) {
		assert(tree_is_ancestor_of(p, n));
		p = tree_parent(t, p);
	}

	unsigned int i = 0;
	for (i = 0; i < tree_out_degree(n); i++) {
		test_tree_is_ancestor_of_r(t, tree_child(n, i));
	}
}

void test_tree_is_ancestor_of(void)
{
	Tree* t = generate_tree();
	test_tree_is_ancestor_of_r(t, t);
	tree_free(t);
}

void test_tree_depth_level_r(Tree* t, TreeNode* n, unsigned int d)
{
	assert(tree_depth(t, n) == d);
	assert(tree_level(t, n) == d + 1);

	unsigned int i;
	for (i = 0; i < tree_out_degree(n); i++) {
		test_tree_depth_level_r(t, tree_child(n, i), d + 1);
	}
}

void test_tree_depth_level(void)
{
	Tree* t = generate_tree();
	test_tree_depth_level_r(t, t, 0);
	tree_free(t);
}

unsigned int test_tree_height_r(TreeNode* n)
{
	unsigned int h = 0;
	unsigned int i;
	for (i = 0; i < tree_out_degree(n); i++) {
		unsigned int hi = test_tree_height_r(tree_child(n, i));
		h = (h > hi) ? h : hi;
	}

	assert(tree_height_node(n) == h);

	return h;
}

void test_tree_height(void)
{
	Tree* t = generate_tree();
	test_tree_height_r(t);
	tree_free(t);
}

void test_tree_remove(void)
{
	/* test removal of node 3 and 9 */
	Tree* t = generate_tree();

	/* node 3 */
	TreeNode* t1 = tree_first_child(t);
	TreeNode* t3 = tree_last_child(t1);
	unsigned int d = tree_out_degree(t1);
	t3 = tree_remove(t3);
	assert(tree_out_degree(t1) == d - 1);
	test_tree_height_r(t);
	
	/* node 9 */
	TreeNode* t7 = tree_last_child(t);
	TreeNode* t8 = tree_last_child(t7);
	TreeNode* t9 = tree_last_child(t8);
	d = tree_out_degree(t8);
	t9 = tree_remove(t9);
	assert(tree_out_degree(t8) == d - 1);
	test_tree_height_r(t);

	tree_free(t);
	tree_free(t3);
	tree_free(t9);
}

void test_tree_add_subtree(void)
{
	/* Remove t3 and add it as child of 9 */
	Tree* t = generate_tree();

	TreeNode* t1 = tree_first_child(t);
	TreeNode* t3 = tree_last_child(t1);
	t3 = tree_remove(t3);

	TreeNode* t7 = tree_last_child(t);
	TreeNode* t8 = tree_last_child(t7);
	TreeNode* t9 = tree_last_child(t8);
	tree_add_subtree(t9, t3);

	test_tree_height_r(t);
	assert(tree_first_child(t9) == t3);

	tree_free(t);
}

void test_tree_insert_subtree(void)
{
	Tree* t = generate_tree();

	TreeNode* t7 = tree_last_child(t);
	TreeNode* t1 = tree_first_child(t);
	TreeNode* t2 = tree_first_child(t1);
	TreeNode* t3 = tree_last_child(t1);

	t7 = tree_remove(t7);
	tree_insert_subtree(t1, t7, 1);

	assert(tree_child(t1, 1) == t7);
	assert(tree_child(t1, 0) == t2);
	assert(tree_child(t1, 2) == t3);
	test_tree_height_r(t);

	tree_free(t);
}

void test_tree_set_subtree(void)
{
	Tree* t = generate_tree();

	TreeNode* t7 = tree_last_child(t);
	TreeNode* t1 = tree_first_child(t);
	TreeNode* t2 = tree_first_child(t1);
	t7 = tree_remove(t7);

	TreeNode* t3 = tree_set_subtree(t1, t7, 1);

	assert(tree_child(t1, 1) == t7);
	assert(tree_child(t1, 0) == t2);
	
	test_tree_height_r(t);

	tree_free(t);
	tree_free(t3);
}

void test_tree_iter_leaves(void)
{
	Tree* t = generate_tree();

	int order[] = {2, 4, 5, 6, 9};
	unsigned i = 0;

	TreeIterator* iter = tree_iter_alloc();
	tree_iter_leaves_first(iter, t);
	while (tree_iter_has_next(iter)) {
		tree_iter_leaves_next(iter);
		assert(*((int*) tree_iter_data(iter)) == order[i++]);
	}

	tree_iter_leaves_last(iter, t);
	i = 4;
	while (tree_iter_has_prev(iter)) {
		tree_iter_leaves_prev(iter);
		assert(*((int*) tree_iter_data(iter)) == order[i--]);
	}

	tree_free(t);
}

void test_tree_iter_parents(void)
{
	Tree* t = generate_tree();

	TreeNode* t1 = tree_first_child(t);
	TreeNode* t3 = tree_last_child(t1);
	TreeNode* t4 = tree_first_child(t3);

	int order[] = {4, 3, 1, 0};
	unsigned int i = 0;
	TreeIterator* iter = tree_iter_alloc();
	tree_iter_parents(iter, t, t4);
	while (tree_iter_has_next(iter)) {
		tree_iter_parents_next(iter);
		assert(*((int*) tree_iter_data(iter)) == order[i++]);
	}

	tree_free(t);
}

void test_tree_preorder_walk(void)
{
	Tree* t = generate_tree();

	int order[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	unsigned i = 0;

	TreeIterator* iter = tree_iter_alloc();
	tree_preorder_walk(iter, t);
	while (tree_iter_has_next(iter)) {
		tree_preorder_walk_next(iter);
		assert(*((int*) tree_iter_data(iter)) == order[i++]);
	}

	i = 8;

	while (tree_iter_has_prev(iter)) {
		tree_preorder_walk_prev(iter);
		assert(*((int*) tree_iter_data(iter)) == order[i--]);
	}

	tree_free(t);
}

void test_tree_postorder_walk(void)
{
	Tree* t = generate_tree();

	int order[] = {2, 4, 5, 6, 3, 1, 9, 8, 7, 0};
	unsigned i = 0;

	TreeIterator* iter = tree_iter_alloc();
	tree_postorder_walk(iter, t);
	while (tree_iter_has_next(iter)) {
		tree_postorder_walk_next(iter);
		assert(*((int*) tree_iter_data(iter)) == order[i++]);
	}

	i = 8;

	while (tree_iter_has_prev(iter)) {
		tree_postorder_walk_prev(iter);
		assert(*((int*) tree_iter_data(iter)) == order[i--]);
	}

	tree_free(t);
}

static UnitTestFunction tests[] = {
	test_tree_abs_root,
	test_tree_is_leaf,
	test_tree_is_descendant_of,
	test_tree_is_ancestor_of,
	test_tree_depth_level,
	test_tree_height,
	test_tree_remove,
	test_tree_add_subtree,
	test_tree_insert_subtree,
	test_tree_set_subtree,
	test_tree_iter_leaves,
	test_tree_iter_parents,
	test_tree_preorder_walk,
	test_tree_postorder_walk,
	NULL
};

int main(int argc, char *argv[])
{
	run_tests(tests);

	return 0;
}
