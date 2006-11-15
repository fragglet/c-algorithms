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
 * The code is licensed under the Modified BSD license, and as a result
 * may be reused in any project, whether Proprietary or Open Source.
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
 * @li @link hashtable.h Hash table @endlink: Collection of values which can be
 * addressed using a key.
 * @li @link trie.h Trie @endlink: Fast mapping using strings as keys.
 *
 * @subsection Binary_search_trees Binary search trees
 *
 * @li @link avltree.h AVL tree @endlink: Balanced binary search tree 
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


