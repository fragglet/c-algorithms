
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

#include "set.h"
#include "compare-int.h"
#include "hash-int.h"
#include "compare-pointer.h"
#include "hash-pointer.h"

Set *generate_set(void)
{
    Set *set;
    int i;
    int *value;

    set = set_new((SetHashFunc) int_hash, (SetEqualFunc) int_equal);

    /* Add 10,000 items sequentially, checking that the counter 
     * works properly */

    for (i=0; i<10000; ++i) {
        value = (int *) malloc(sizeof(int));

        *value = i;

        set_insert(set, value);

        assert(set_num_entries(set) == i + 1);
    }

    return set;
}

void test_set_new(void)
{
    Set *set;

    set = set_new((SetHashFunc) int_hash, (SetEqualFunc) int_equal);
}

void test_set_free(void)
{
    Set *set;
    int i;
    int *value;

    set = set_new((SetHashFunc) int_hash, (SetEqualFunc) int_equal);

    /* Fill the set with many values before freeing */

    for (i=0; i<10000; ++i) {
        value = (int *) malloc(sizeof(int));

        *value = i;

        set_insert(set, value);
    }

    /* Free the set */

    set_free(set);
}

void test_set_insert(void)
{
    Set *set;
    int numbers1[] = { 1, 2, 3, 4, 5, 6 };
    int numbers2[] = { 5, 6, 7, 8, 9, 10 };
    int i;

    set = generate_set();

    /* Perform a union of numbers1 and numbers2.  Cannot add the same
     * value twice. */

    set = set_new((SetHashFunc) int_hash, (SetEqualFunc) int_equal);

    for (i=0; i<6; ++i) {
        set_insert(set, &numbers1[i]);
    }
    for (i=0; i<6; ++i) {
        set_insert(set, &numbers2[i]);
    }

    assert(set_num_entries(set) == 10);
}

void test_set_query(void)
{
    Set *set;
    int i;

    set = generate_set();

    /* Test all values */
    
    for (i=0; i<10000; ++i) {
        assert(set_query(set, &i) != 0);
    }

    /* Test invalid values returning zero */

    i = -1;
    assert(set_query(set, &i) == 0);
    i = 100001;
    assert(set_query(set, &i) == 0);
}

void test_set_remove(void)
{
    Set *set;
    int i;

    set = generate_set();

    i = 5000;
    assert(set_query(set, &i) != 0);
    assert(set_num_entries(set) == 10000);

    /* Remove an entry */

    set_remove(set, &i);
    assert(set_num_entries(set) == 9999);
    assert(set_query(set, &i) == 0);

    /* Try to remove an invalid entry */

    i = 50000;
    set_remove(set, &i);
    assert(set_num_entries(set) == 9999);
}

void test_set_union(void)
{
    int numbers1[] = {1, 2, 3, 4, 5, 6, 7};
    int numbers2[] = {5, 6, 7, 8, 9, 10, 11};
    int result[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    int i;
    Set *set1;
    Set *set2;
    Set *result_set;

    /* Create the first set */

    set1 = set_new((SetHashFunc) int_hash, (SetEqualFunc) int_equal);

    for (i=0; i<7; ++i) {
        set_insert(set1, &numbers1[i]);
    }
    
    /* Create the second set */
    
    set2 = set_new((SetHashFunc) int_hash, (SetEqualFunc) int_equal);
    
    for (i=0; i<7; ++i) {
        set_insert(set2, &numbers2[i]);
    }
    
    /* Perform the union */

    result_set = set_union(set1, set2, NULL);

    assert(set_num_entries(result_set) == 11);

    for (i=0; i<11; ++i) {
        assert(set_query(result_set, &result[i]) != 0);
    }
}

void test_set_intersection(void)
{
    int numbers1[] = {1, 2, 3, 4, 5, 6, 7};
    int numbers2[] = {5, 6, 7, 8, 9, 10, 11};
    int result[] = {5, 6, 7};
    int i;
    Set *set1;
    Set *set2;
    Set *result_set;

    /* Create the first set */

    set1 = set_new((SetHashFunc) int_hash, (SetEqualFunc) int_equal);

    for (i=0; i<7; ++i) {
        set_insert(set1, &numbers1[i]);
    }
    
    /* Create the second set */
    
    set2 = set_new((SetHashFunc) int_hash, (SetEqualFunc) int_equal);
    
    for (i=0; i<7; ++i) {
        set_insert(set2, &numbers2[i]);
    }
    
    /* Perform the intersection */

    result_set = set_intersection(set1, set2, NULL);

    assert(set_num_entries(result_set) == 3);

    for (i=0; i<3; ++i) {
        assert(set_query(result_set, &result[i]) != 0);
    }
}

void test_set_to_array(void)
{
    Set *set;
    int values[100];
    int **array;
    int i;

    /* Create a set containing pointers to all entries in the "values"
     * array. */
    
    set = set_new(pointer_hash, pointer_equal);

    for (i=0; i<100; ++i) {
        values[i] = 1;
        set_insert(set, &values[i]);
    }

    array = (int **) set_to_array(set);

    /* Check the array */

    for (i=0; i<100; ++i) {
        assert(*array[i] == 1);
        *array[i] = 0;
    }
}

int main(int argc, char *argv[])
{
    test_set_new();
    test_set_free();
    test_set_insert();
    test_set_query();
    test_set_remove();
    test_set_intersection();
    test_set_union();

    return 0;
}

