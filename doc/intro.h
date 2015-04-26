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

/* This file is a dummy header which is used to generate the
 * introduction in Doxygen. */

/**
 * @mainpage C Algorithms Library
 *
 * @section Introduction
 *
 * The C programming language includes a very limited standard library in
 * comparison to other modern programming languages.  This is a collection
 * of common Computer Science data structures and algorithms which may be
 * used in C projects.
 *
 * The code is licensed under the ISC license (a simplified version
 * of the BSD license that is functionally identical).
 * As such, it may be reused in any project, whether Proprietary or
 * Open Source.
 *
 * @section Data_structures Data structures
 *
 * @subsection Collections
 *
 * @li @link arraylist.h ArrayList @endlink: Automatically resizing array.
 * @li @link list.h Doubly linked list @endlink: A set of values stored
 * in a list with links that point in both directions.
 * @li @link slist.h Singly linked list @endlink: A set of values stored
 * in a list with links that point in one direction.
 * @li @link queue.h Queue @endlink: Double ended queue which can be used
 * as a FIFO or a stack.
 * @li @link set.h Set @endlink: Unordered set of values.
 * @li @link bloom-filter.h Bloom Filter @endlink: Space-efficient set.
 *
 * @subsection Mappings
 *
 * @li @link hash-table.h Hash table @endlink: Collection of values which
 * can be addressed using a key.
 * @li @link trie.h Trie @endlink: Fast mapping using strings as keys.
 *
 * @subsection Binary_search_trees Binary search trees
 *
 * @li @link avl-tree.h AVL tree @endlink: Balanced binary search tree
 * with O(log n) worst case performance.
 *
 * @section Utility_functions Utility functions
 *
 * All of the above data structures operate on void pointers.  It is
 * sometimes necessary to compare values (when sorting a list, for
 * example) or generate a hash key (in a hash table or set).  This
 * is done by providing a pointer to a function which provides
 * this functionality.  The following functions provide this
 * functionality for some common data types.
 *
 * @li Integer @link compare-int.h comparison @endlink and
 * @link hash-int.h hash @endlink functions.
 * @li String @link compare-string.h comparison @endlink and
 * @link hash-string.h hash @endlink functions.
 * @li Generic (void) pointer @link compare-pointer.h comparison @endlink
 * and @link hash-pointer.h hash @endlink functions.
 *
 */


