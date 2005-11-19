
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

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "queue.h"

int variable1, variable2, variable3, variable4;

Queue *generate_queue(void)
{
    Queue *queue;
    int i;

    queue = queue_new();
    
    /* Add some values */

    for (i=0; i<1000; ++i) {
        queue_push_head(queue, &variable1);
        queue_push_head(queue, &variable2);
        queue_push_head(queue, &variable3);
        queue_push_head(queue, &variable4);
    }

    return queue;
}

/* Test cases for the queue */

void test_queue_new(void)
{
    Queue *queue;

    queue = queue_new();
}

void test_queue_free(void)
{
    int i;
    Queue *queue;

    /* Create and destroy a queue */
    
    queue = queue_new();

    queue_free(queue);

    /* Add lots of values and then destroy */

    queue = queue_new();

    for (i=0; i<1000; ++i) {
        queue_push_head(queue, &variable1);
    }

    queue_free(queue);
}

void test_queue_push_head(void)
{
    Queue *queue;
    int i;

    queue = queue_new();
    
    /* Add some values */

    for (i=0; i<1000; ++i) {
        queue_push_head(queue, &variable1);
        queue_push_head(queue, &variable2);
        queue_push_head(queue, &variable3);
        queue_push_head(queue, &variable4);
    }

    assert(!queue_is_empty(queue));

    /* Check values come out of the tail properly */

    assert(queue_pop_tail(queue) == &variable1);
    assert(queue_pop_tail(queue) == &variable2);
    assert(queue_pop_tail(queue) == &variable3);
    assert(queue_pop_tail(queue) == &variable4);

    /* Check values come back out of the head properly */

    assert(queue_pop_head(queue) == &variable4);
    assert(queue_pop_head(queue) == &variable3);
    assert(queue_pop_head(queue) == &variable2);
    assert(queue_pop_head(queue) == &variable1);
}

void test_queue_pop_head(void)
{
    Queue *queue;

    /* Check popping off an empty queue */

    queue = queue_new();

    assert(queue_pop_head(queue) == NULL);

    queue = generate_queue();

    /* Pop off all the values from the queue */

    while (!queue_is_empty(queue)) {
        assert(queue_pop_head(queue) == &variable4);
        assert(queue_pop_head(queue) == &variable3);
        assert(queue_pop_head(queue) == &variable2);
        assert(queue_pop_head(queue) == &variable1);
    }

    assert(queue_pop_head(queue) == NULL);
}

void test_queue_peek_head(void)
{
    Queue *queue;

    /* Check peeking into an empty queue */

    queue = queue_new();

    assert(queue_peek_head(queue) == NULL);

    queue = generate_queue();

    /* Pop off all the values from the queue, making sure that peek
     * has the correct value beforehand */

    while (!queue_is_empty(queue)) {
        assert(queue_peek_head(queue) == &variable4);
        assert(queue_pop_head(queue) == &variable4);
        assert(queue_peek_head(queue) == &variable3);
        assert(queue_pop_head(queue) == &variable3);
        assert(queue_peek_head(queue) == &variable2);
        assert(queue_pop_head(queue) == &variable2);
        assert(queue_peek_head(queue) == &variable1);
        assert(queue_pop_head(queue) == &variable1);
    }

    assert(queue_peek_head(queue) == NULL);
}

void test_queue_push_tail(void)
{
    Queue *queue;
    int i;

    queue = queue_new();
    
    /* Add some values */

    for (i=0; i<1000; ++i) {
        queue_push_tail(queue, &variable1);
        queue_push_tail(queue, &variable2);
        queue_push_tail(queue, &variable3);
        queue_push_tail(queue, &variable4);
    }

    assert(!queue_is_empty(queue));

    /* Check values come out of the head properly */

    assert(queue_pop_head(queue) == &variable1);
    assert(queue_pop_head(queue) == &variable2);
    assert(queue_pop_head(queue) == &variable3);
    assert(queue_pop_head(queue) == &variable4);

    /* Check values come back out of the tail properly */

    assert(queue_pop_tail(queue) == &variable4);
    assert(queue_pop_tail(queue) == &variable3);
    assert(queue_pop_tail(queue) == &variable2);
    assert(queue_pop_tail(queue) == &variable1);
}

void test_queue_pop_tail(void)
{
    Queue *queue;

    /* Check popping off an empty queue */

    queue = queue_new();

    assert(queue_pop_tail(queue) == NULL);

    queue = generate_queue();

    /* Pop off all the values from the queue */

    while (!queue_is_empty(queue)) {
        assert(queue_pop_tail(queue) == &variable1);
        assert(queue_pop_tail(queue) == &variable2);
        assert(queue_pop_tail(queue) == &variable3);
        assert(queue_pop_tail(queue) == &variable4);
    }

    assert(queue_pop_tail(queue) == NULL);
}

void test_queue_peek_tail(void)
{
    Queue *queue;

    /* Check peeking into an empty queue */

    queue = queue_new();

    assert(queue_peek_tail(queue) == NULL);

    queue = generate_queue();

    /* Pop off all the values from the queue, making sure that peek
     * has the correct value beforehand */

    while (!queue_is_empty(queue)) {
        assert(queue_peek_tail(queue) == &variable1);
        assert(queue_pop_tail(queue) == &variable1);
        assert(queue_peek_tail(queue) == &variable2);
        assert(queue_pop_tail(queue) == &variable2);
        assert(queue_peek_tail(queue) == &variable3);
        assert(queue_pop_tail(queue) == &variable3);
        assert(queue_peek_tail(queue) == &variable4);
        assert(queue_pop_tail(queue) == &variable4);
    }

    assert(queue_peek_tail(queue) == NULL);
}

void test_queue_is_empty(void)
{
    Queue *queue;

    queue = queue_new();

    assert(queue_is_empty(queue));

    queue_push_head(queue, &variable1);

    assert(!queue_is_empty(queue));

    queue_pop_head(queue);

    assert(queue_is_empty(queue));

    queue_push_tail(queue, &variable1);

    assert(!queue_is_empty(queue));

    queue_pop_tail(queue);

    assert(queue_is_empty(queue));
}

int main(int argc, char *argv[]) 
{
    test_queue_new();
    test_queue_free();
    test_queue_push_head();
    test_queue_pop_head();
    test_queue_peek_head();
    test_queue_push_tail();
    test_queue_pop_tail();
    test_queue_peek_tail();
    test_queue_is_empty();
}

