
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

/**
 * @file binary-heap.h
 *
 * @brief Binary heap.
 *
 * A binary heap is a heap data structure implemented using a 
 * binary tree.  In a heap, values are ordered by priority.
 *
 * To create a binary heap, use @ref binary_heap_new.  To destroy a 
 * binary heap, use @ref binary_heap_free.
 *
 * To insert a value into a binary heap, use @ref binary_heap_insert.
 *
 * To remove the first value from a binary heap, use @ref binary_heap_pop.
 *
 */

#ifndef ALGORITHM_BINARY_HEAP_H
#define ALGORITHM_BINARY_HEAP_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Heap type.  If a heap is a min heap, the values with the lowest 
 * priority are stored at the top of the heap and will be the first
 * returned.  If a heap is a max heap, the values with the 
 * greatest priority are stored at the top of the heap.
 */

typedef enum {
    BINARY_HEAP_TYPE_MIN,
    BINARY_HEAP_TYPE_MAX,
} BinaryHeapType;

/**
 * Type of function used to compare values in a binary heap.
 *
 * @param data1            The first value.
 * @param data2            The second value.
 * @return                 A negative number if data1 is less than data2,
 *                         a positive number if data1 is greater than data2,
 *                         zero if the two are equal.
 */

typedef int (*BinaryHeapCompareFunc)(void *data1, void *data2);

/** 
 * A binary heap data structure.
 */

typedef struct _BinaryHeap BinaryHeap;

/**
 * Create a new @ref BinaryHeap. 
 *
 * @param heap_type        The type of heap: min heap or max heap.
 * @param compare_func     Pointer to a function used to compare the priority
 *                         of values in the heap.
 * @return                 A new binary heap, or NULL if it was not possible
 *                         to allocate the memory.
 */

BinaryHeap *binary_heap_new(BinaryHeapType heap_type,
                            BinaryHeapCompareFunc compare_func);

/**
 * Destroy a binary heap.
 *
 * @param heap             The heap to destroy.
 */

void binary_heap_free(BinaryHeap *heap);

/**
 * Insert a value into a binary heap.
 *
 * @param heap             The heap to insert into.
 * @param value            The value to insert.
 * @return                 Non-zero if the entry was added, or zero if it
 *                         was not possible to allocate memory for the new
 *                         entry.
 */

int binary_heap_insert(BinaryHeap *heap, void *value);

/**
 * Remove the first value from a binary heap.
 *
 * @param heap             The heap.
 * @return                 The first value in the heap.
 */

void *binary_heap_pop(BinaryHeap *heap);

/**
 * Find the number of values stored in a binary heap.
 *
 * @param heap             The heap.
 * @return                 The number of values in the heap.
 */

int binary_heap_num_entries(BinaryHeap *heap);

#endif /* #ifndef ALGORITHM_BINARY_HEAP_H */

