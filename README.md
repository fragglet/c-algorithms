
# C Algorithms

## Background

The C programming language includes a very limited standard library in
comparison to other modern programming languages.  This is a collection of
common Computer Science algorithms which may be used in C projects.

The code is licensed under the ISC license (a simplified version of the BSD
license that is functionally identical).  As such, it may legitimately be
reused in any project, whether Proprietary or Open Source.

## Example code 

### Array Lists

```C
// This Arraylist is an array of void pointers that resizes.

// Create a new array list of length 20
uint8_t num = 10;

// Add value to the end of the list
ArrayListValue value = (ArrayListValue)(&num);
ArrayList list = arraylist_new(20);

arraylist_append(list, value);

// Access the newly inserted value
list[20];

// Destory the list
arraylist_free(list);

// Functions for inserting, clearing, sorting, and indexing can be found in src/arraylist.h
```

### Doubly Linked Lists
```C

// Create an empty linked list.
ListEntry* head = NULL;

// Add a value to the end of the list
int num;
ListValue value = (ListValue)(&num);

// NOTE: returns the new entry to the list or NULL if appending was not possible
list_append(&head, value); 

// Destory the list
list_free(head);

// Functions for iterating, inserting, settting values, and can be found in src/list.h
```

### Sets
```C
// Create an empty set which assumes the existence of a SetHashFunc type function, hash
// , and a SetEqualFunc type function equals
// these can either be made by the user or using one that already exists such as in src/hash-pointer.h
Set set = set(hash, equals);

// Inserting a value into the set
int num = 10;
SetValue value = (SetValue)(&num);

set_insert(&set, value);

// Removing a value from a set
set_remove(&set, value);

// Destorying a set
set_free(&set);

// Functions for union, intersection, and more can be foudn in src/set.h
```

### Other Datastructures and features
Some other datastrutures and features that are included in this library are
-  Queues.
-  Red-black trees.
-  Singly linked lists.
-  Sorted arrays.
-  Tries.
-  Hash maps.
-  AVL trees.
-  Binary heaps.
-  Binomial heaps.
-  Hashing functions

More information can be found in the doc section and through the doxygen.
