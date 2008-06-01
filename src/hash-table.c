
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

/* Hash table implementation */

#include <stdlib.h>
#include <string.h>

#include "hash-table.h"

struct _HashTableEntry {
	HashTableKey key;
	HashTableValue value;
	HashTableEntry *next;
};

struct _HashTable {
	HashTableEntry **table;
	int table_size;
	HashTableHashFunc hash_func;
	HashTableEqualFunc equal_func;
	HashTableKeyFreeFunc key_free_func;
	HashTableValueFreeFunc value_free_func;
	int entries;
	int prime_index;
};

/* This is a set of good hash table prime numbers, from:
 *   http://planetmath.org/encyclopedia/GoodHashTablePrimes.html
 * Each prime is roughly double the previous value, and as far as
 * possible from the nearest powers of two. */

static const unsigned int hash_table_primes[] = {
	193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317,
	196613, 393241, 786433, 1572869, 3145739, 6291469,
	12582917, 25165843, 50331653, 100663319, 201326611,
	402653189, 805306457, 1610612741,
};

static const int hash_table_num_primes 
	= sizeof(hash_table_primes) / sizeof(int);

/* Internal function used to allocate the table on hash table creation
 * and when enlarging the table */

static int hash_table_allocate_table(HashTable *hash_table)
{
	int new_table_size;

	/* Determine the table size based on the current prime index.  
	 * An attempt is made here to ensure sensible behavior if the
	 * maximum prime is exceeded, but in practice other things are
	 * likely to break long before that happens. */

	if (hash_table->prime_index < hash_table_num_primes) {
		new_table_size = hash_table_primes[hash_table->prime_index];
	} else {
		new_table_size = hash_table->entries * 10;
	}

	hash_table->table_size = new_table_size;

	/* Allocate the table and initialise to NULL for all entries */

	hash_table->table = calloc(hash_table->table_size, 
	                           sizeof(HashTableEntry *));

	return hash_table->table != NULL;
}

/* Free an entry, calling the free functions if there are any registered */

static void hash_table_free_entry(HashTable *hash_table, HashTableEntry *entry)
{
	/* If there is a function registered for freeing keys, use it to free
	 * the key */
	
	if (hash_table->key_free_func != NULL) {
		hash_table->key_free_func(entry->key);
	}

	/* Likewise with the value */

	if (hash_table->value_free_func != NULL) {
		hash_table->value_free_func(entry->value);
	}

	/* Free the data structure */
	
	free(entry);
}

HashTable *hash_table_new(HashTableHashFunc hash_func, 
                          HashTableEqualFunc equal_func)
{
	HashTable *hash_table;

	/* Allocate a new hash table structure */
	
	hash_table = (HashTable *) malloc(sizeof(HashTable));

	if (hash_table == NULL) {
		return NULL;
	}
	
	hash_table->hash_func = hash_func;
	hash_table->equal_func = equal_func;
	hash_table->key_free_func = NULL;
	hash_table->value_free_func = NULL;
	hash_table->entries = 0;
	hash_table->prime_index = 0;

	/* Allocate the table */

	if (!hash_table_allocate_table(hash_table)) {
		free(hash_table);

		return NULL;
	}

	return hash_table;
}

void hash_table_free(HashTable *hash_table)
{
	HashTableEntry *rover;
	HashTableEntry *next;
	int i;
	
	/* Free all entries in all chains */

	for (i=0; i<hash_table->table_size; ++i) {
		rover = hash_table->table[i];
		while (rover != NULL) {
			next = rover->next;
			hash_table_free_entry(hash_table, rover);
			rover = next;
		}
	}
	
	/* Free the table */

	free(hash_table->table);
	
	/* Free the hash table structure */

	free(hash_table);
}

void hash_table_register_free_functions(HashTable *hash_table,
                                        HashTableKeyFreeFunc key_free_func,
                                        HashTableValueFreeFunc value_free_func)
{
	hash_table->key_free_func = key_free_func;
	hash_table->value_free_func = value_free_func;
}


static int hash_table_enlarge(HashTable *hash_table)
{
	HashTableEntry **old_table;
	int old_table_size;
	int old_prime_index;
	HashTableEntry *rover;
	HashTableEntry *next;
	int index;
	int i;
	
	/* Store a copy of the old table */
	
	old_table = hash_table->table;
	old_table_size = hash_table->table_size;
	old_prime_index = hash_table->prime_index;

	/* Allocate a new, larger table */

	++hash_table->prime_index;
	
	if (!hash_table_allocate_table(hash_table)) {

		/* Failed to allocate the new table */

		hash_table->table = old_table;
		hash_table->table_size = old_table_size;
		hash_table->prime_index = old_prime_index;

		return 0;
	}

	/* Link all entries from all chains into the new table */

	for (i=0; i<old_table_size; ++i) {
		rover = old_table[i];

		while (rover != NULL) {
			next = rover->next;

			/* Find the index into the new table */
			
			index = hash_table->hash_func(rover->key) % hash_table->table_size;
			
			/* Link this entry into the chain */

			rover->next = hash_table->table[index];
			hash_table->table[index] = rover;
			
			/* Advance to next in the chain */

			rover = next;
		}
	}
       
	return 1;
}

int hash_table_insert(HashTable *hash_table, HashTableKey key, HashTableValue value) 
{
	HashTableEntry *rover;
	HashTableEntry *newentry;
	int index;
	
	/* If there are too many items in the table with respect to the table
	 * size, the number of hash collisions increases and performance
	 * decreases. Enlarge the table size to prevent this happening */

	if ((hash_table->entries * 3) / hash_table->table_size > 0) {
		
		/* Table is more than 1/3 full */

		if (!hash_table_enlarge(hash_table)) {

			/* Failed to enlarge the table */

			return 0;
		}
	}

	/* Generate the hash of the key and hence the index into the table */

	index = hash_table->hash_func(key) % hash_table->table_size;

	/* Traverse the chain at this location and look for an existing
	 * entry with the same key */

	rover = hash_table->table[index];

	while (rover != NULL) {
		if (hash_table->equal_func(rover->key, key) != 0) {

			/* Same key: overwrite this entry with new data */

			/* If there is a value free function, free the old data
			 * before adding in the new data */

			if (hash_table->value_free_func != NULL) {
				hash_table->value_free_func(rover->value);
			}

			/* Same with the key: use the new key value and free 
			 * the old one */

			if (hash_table->key_free_func != NULL) {
				hash_table->key_free_func(rover->key);
			}

			rover->key = key;
			rover->value = value;

			/* Finished */
			
			return 1;
		}
		rover = rover->next;
	}
	
	/* Not in the hash table yet.  Create a new entry */

	newentry = (HashTableEntry *) malloc(sizeof(HashTableEntry));

	if (newentry == NULL) {
		return 0;
	}

	newentry->key = key;
	newentry->value = value;

	/* Link into the list */

	newentry->next = hash_table->table[index];
	hash_table->table[index] = newentry;

	/* Maintain the count of the number of entries */

	++hash_table->entries;

	/* Added successfully */

	return 1;
}

HashTableValue hash_table_lookup(HashTable *hash_table, HashTableKey key)
{
	HashTableEntry *rover;
	int index;

	/* Generate the hash of the key and hence the index into the table */
	
	index = hash_table->hash_func(key) % hash_table->table_size;

	/* Walk the chain at this index until the corresponding entry is
	 * found */

	rover = hash_table->table[index];

	while (rover != NULL) {
		if (hash_table->equal_func(key, rover->key) != 0) {

			/* Found the entry.  Return the data. */

			return rover->value;
		}
		rover = rover->next;
	}

	/* Not found */

	return HASH_TABLE_NULL;
}

int hash_table_remove(HashTable *hash_table, HashTableKey key)
{
	HashTableEntry **rover;
	HashTableEntry *entry;
	int index;
	int result;

	/* Generate the hash of the key and hence the index into the table */
	
	index = hash_table->hash_func(key) % hash_table->table_size;

	/* Rover points at the pointer which points at the current entry
	 * in the chain being inspected.  ie. the entry in the table, or
	 * the "next" pointer of the previous entry in the chain.  This
	 * allows us to unlink the entry when we find it. */

	result = 0;
	rover = &hash_table->table[index];

	while (*rover != NULL) {

		if (hash_table->equal_func(key, (*rover)->key) != 0) {

			/* This is the entry to remove */

			entry = *rover;

			/* Unlink from the list */

			*rover = entry->next;

			/* Destroy the entry structure */

			hash_table_free_entry(hash_table, entry);

			/* Track count of entries */

			--hash_table->entries;

			result = 1;

			break;
		}
		
		/* Advance to the next entry */

		rover = &((*rover)->next);
	}

	return result;
}

int hash_table_num_entries(HashTable *hash_table)
{
	return hash_table->entries;
}

void hash_table_iterate(HashTable *hash_table, HashTableIterator *iterator)
{
	int chain;
	
	iterator->hash_table = hash_table;

	/* Default value of next if no entries are found. */
	
	iterator->next_entry = NULL;
	
	/* Find the first entry */
	
	for (chain=0; chain<hash_table->table_size; ++chain) {
		
		if (hash_table->table[chain] != NULL) {
			iterator->next_entry = hash_table->table[chain];
			iterator->next_chain = chain;
			break;
		}
	}
}

int hash_table_iter_has_more(HashTableIterator *iterator)
{
	return iterator->next_entry != NULL;
}

HashTableValue hash_table_iter_next(HashTableIterator *iterator)
{
	HashTableEntry *current_entry;
	HashTable *hash_table;
	HashTableValue result;
	int chain;

	hash_table = iterator->hash_table;

	/* No more entries? */
	
	if (iterator->next_entry == NULL) {
		return HASH_TABLE_NULL;
	}
	
	/* Result is immediately available */

	current_entry = iterator->next_entry;
	result = current_entry->value;

	/* Find the next entry */

	if (current_entry->next != NULL) {
		
		/* Next entry in current chain */

		iterator->next_entry = current_entry->next;
		
	} else {
	
		/* None left in this chain, so advance to the next chain */

		chain = iterator->next_chain + 1;

		/* Default value if no next chain found */
		
		iterator->next_entry = NULL;

		while (chain < hash_table->table_size) {

			/* Is there anything in this chain? */

			if (hash_table->table[chain] != NULL) {
				iterator->next_entry = hash_table->table[chain];
				break;
			}

			/* Try the next chain */

			++chain;
		}

		iterator->next_chain = chain;
	}

	return result;
}

