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

/* Trie: fast mapping of strings to values */

#include <stdlib.h>
#include <string.h>

#include "trie.h"

/* malloc() / free() testing */

#ifdef ALLOC_TESTING
#include "alloc-testing.h"
#endif

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

