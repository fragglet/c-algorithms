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

/**
 * @file trie.h
 *
 * @brief Fast string lookups
 *
 * A trie is a data structure which provides fast mappings from strings
 * to values.
 *
 * To create a new trie, use @ref trie_new.  To destroy a trie,
 * use @ref trie_free.
 *
 * To insert a value into a trie, use @ref trie_insert. To remove a value
 * from a trie, use @ref trie_remove.
 *
 * To look up a value from its key, use @ref trie_lookup.
 *
 * To find the number of entries in a trie, use @ref trie_num_entries.
 */

#ifndef ALGORITHM_TRIE_H
#define ALGORITHM_TRIE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A trie structure.
 */

typedef struct _Trie Trie;

/**
 * Value stored in a @ref Trie.
 */

typedef void *TrieValue;

/**
 * A null @ref TrieValue.
 */

#define TRIE_NULL ((void *) 0)

/**
 * Create a new trie.
 *
 * @return                   Pointer to a new trie structure, or NULL if it
 *                           was not possible to allocate memory for the
 *                           new trie.
 */

Trie *trie_new(void);

/**
 * Destroy a trie.
 *
 * @param trie               The trie to destroy.
 */

void trie_free(Trie *trie);

/**
 * Insert a new key-value pair into a trie.  The key is a NUL-terminated
 * string.  For binary strings, use @ref trie_insert_binary.
 *
 * @param trie               The trie.
 * @param key                The key to access the new value.
 * @param value              The value.
 * @return                   Non-zero if the value was inserted successfully,
 *                           or zero if it was not possible to allocate
 *                           memory for the new entry.
 */

int trie_insert(Trie *trie, char *key, TrieValue value);

/**
 * Insert a new key-value pair into a trie. The key is a sequence of bytes.
 * For a key that is a NUL-terminated text string, use @ref trie_insert.
 *
 * @param trie               The trie.
 * @param key                The key to access the new value.
 * @param key_length         The key length in bytes.
 * @param value              The value.
 * @return                   Non-zero if the value was inserted successfully,
 *                           or zero if it was not possible to allocate
 *                           memory for the new entry.
 */

int trie_insert_binary(Trie *trie, unsigned char *key,
                       int key_length, TrieValue value);

/**
 * Look up a value from its key in a trie.
 * The key is a NUL-terminated string; for binary strings, use
 * @ref trie_lookup_binary.
 *
 * @param trie               The trie.
 * @param key                The key.
 * @return                   The value associated with the key, or
 *                           @ref TRIE_NULL if not found in the trie.
 */

TrieValue trie_lookup(Trie *trie, char *key);

/**
 * Look up a value from its key in a trie.
 * The key is a sequence of bytes; for a key that is a NUL-terminated
 * text string, use @ref trie_lookup.
 *
 * @param trie               The trie.
 * @param key                The key.
 * @param key_length         The key length in bytes.
 * @return                   The value associated with the key, or
 *                           @ref TRIE_NULL if not found in the trie.
 */

TrieValue trie_lookup_binary(Trie *trie, unsigned char *key, int key_length);

/**
 * Remove an entry from a trie.
 * The key is a NUL-terminated string; for binary strings, use
 * @ref trie_lookup_binary.
 *
 * @param trie               The trie.
 * @param key                The key of the entry to remove.
 * @return                   Non-zero if the key was removed successfully,
 *                           or zero if it is not present in the trie.
 */

int trie_remove(Trie *trie, char *key);

/**
 * Remove an entry from a trie.
 * The key is a sequence of bytes; for a key that is a NUL-terminated
 * text string, use @ref trie_lookup.
 *
 * @param trie               The trie.
 * @param key                The key of the entry to remove.
 * @param key_length         The key length in bytes.
 * @return                   Non-zero if the key was removed successfully,
 *                           or zero if it is not present in the trie.
 */

int trie_remove_binary(Trie *trie, unsigned char *key, int key_length);

/**
 * Find the number of entries in a trie.
 *
 * @param trie               The trie.
 * @return                   Count of the number of entries in the trie.
 */

unsigned int trie_num_entries(Trie *trie);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef ALGORITHM_TRIE_H */

