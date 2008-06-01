
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

/* Trie: fast mapping of strings to values */

#include <stdlib.h>
#include <string.h>

#include "trie.h"

typedef struct _TrieNode TrieNode;

struct _TrieNode {
	TrieValue data;
	unsigned int use_count;
	TrieNode *next[256];
};

struct _Trie {
	TrieNode *root_node;
};

static void trie_free_node(TrieNode *node)
{
	int i;

	if (node == NULL)
		return;
	
	/* First, free all subnodes */

	for (i=0; i<256; ++i) {
		trie_free_node(node->next[i]);
	}

	/* Free this node */

	free(node);
}

Trie *trie_new(void)
{
	Trie *new_trie;

	new_trie = (Trie *) malloc(sizeof(Trie));

	if (new_trie == NULL) {
		return NULL;
	}
	
	new_trie->root_node = NULL;

	return new_trie;
}

void trie_free(Trie *trie)
{
	/* Free the subnode, and all others by implication */

	trie_free_node(trie->root_node);

	/* Free the trie */

	free(trie);
}

static TrieNode *trie_find_end(Trie *trie, char *key)
{
	TrieNode *node;
	char *p;
	int c;

	/* Search down the trie until the end of string is reached */

	node = trie->root_node;

	for (p=key; *p != '\0'; ++p) {

		if (node == NULL) {
			/* Not found in the tree. Return. */

			return NULL;
		}

		/* Jump to the next node */

		c = *p;
		node = node->next[c];
	}

	/* This key is present if the value at the last node is not NULL */

	return node;
}

/* Roll back an insert operation after a failed malloc() call. */

static void trie_insert_rollback(Trie *trie, char *key)
{
	TrieNode **rover;
	TrieNode *node;
	char *p;

	/* Follow the chain along.  We know that we will never reach the 
	 * end of the string because trie_insert never got that far.  As a
	 * result, it is not necessary to check for the end of string
	 * delimiter (NUL) */

	rover = &trie->root_node;
	p = key;

	while (*rover != NULL) {

		/* Advance to the next node in the chain.  Do this now,
		 * before we potentially free this node. */

		node = *rover;
		rover = &node->next[(int) *p];
		++p;
		
		/* Decrement the use count at this node back to what it 
		 * previously was. */

		--node->use_count;

		if (node->use_count <= 0) {
			/* This has just been allocated.  Free it back. */

			free(node);
		}
	}
}

int trie_insert(Trie *trie, char *key, TrieValue value)
{
	TrieNode **rover;
	TrieNode *node;
	char *p;
	int c;

	/* Cannot insert NULL values */

	if (value == TRIE_NULL) {
		return 0;
	}
		
	/* Search to see if this is already in the tree */

	node = trie_find_end(trie, key);

	/* Already in the tree? If so, replace the existing value and 
	 * return success. */

	if (node != NULL && node->data != TRIE_NULL) {
		node->data = value;
		return 1;
	}

	/* Search down the trie until we reach the end of string,
	 * creating nodes as necessary */

	rover = &trie->root_node;
	p = key;

	for (;;) {

		node = *rover;

		if (node == NULL) {

			/* Node does not exist, so create it */

			node = (TrieNode *) calloc(1, sizeof(TrieNode));

			if (node == NULL) {
  
				/* Allocation failed.  Go back and undo
				 * what we have done so far. */

				trie_insert_rollback(trie, key);
		
				return 0;
			}

			node->data = TRIE_NULL;

			/* Link in to the trie */

			*rover = node;
		}

		/* Increase the node use count */

		++node->use_count;

		/* Current character */

		c = *p;

		/* Reached the end of string?  If so, we're finished. */

		if (c == '\0') {

			/* Set the data at the node we have reached */

			node->data = value;

			break;
		}

		/* Advance to the next node in the chain */

		rover = &node->next[c];
		++p;
	}

	return 1;
}

int trie_remove(Trie *trie, char *key)
{
	TrieNode *node;
	TrieNode *next;
	TrieNode **last_next_ptr;
	char *p;
	int c;
	
	/* Find the end node and remove the value */

	node = trie_find_end(trie, key);

	if (node != NULL && node->data != TRIE_NULL) {
		node->data = TRIE_NULL;
	} else {
		return 0;
	}

	/* Now traverse the tree again as before, decrementing the use
	 * count of each node.  Free back nodes as necessary. */

	node = trie->root_node;
	last_next_ptr = &trie->root_node;
	p = key;

	for (;;) {

		/* Find the next node */
		
		c = *p;
		next = node->next[c];

		/* Free this node if necessary */

		--node->use_count;

		if (node->use_count <= 0) {
			free(node);

			/* Set the "next" pointer on the previous node to NULL,
			 * to unlink the freed node from the tree.  This only
			 * needs to be done once in a remove.  After the first
			 * unlink, all further nodes are also going to be
			 * free'd. */

			if (last_next_ptr != NULL) {
				*last_next_ptr = NULL;
				last_next_ptr = NULL;
			}
		}
		
		/* Go to the next character or finish */

		if (c == '\0') {
			break;
		} else {
			++p;
		}

		/* If necessary, save the location of the "next" pointer
		 * so that it may be set to NULL on the next iteration if
		 * the next node visited is freed. */

		if (last_next_ptr != NULL) {
			last_next_ptr = &node->next[c];
		}
		
		/* Jump to the next node */

		node = next;
	}

	/* Removed successfully */

	return 1;
}

TrieValue trie_lookup(Trie *trie, char *key)
{
	TrieNode *node;

	node = trie_find_end(trie, key);

	if (node != NULL) {
		return node->data;
	} else {
		return TRIE_NULL;
	}
}

int trie_num_entries(Trie *trie)
{
	/* To find the number of entries, simply look at the use count
	 * of the root node. */

	if (trie->root_node == NULL) {
		return 0;
	} else {
		return trie->root_node->use_count;
	}
}

