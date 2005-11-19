 
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
#include "slist.h"

int variable1 = 50, variable2, variable3, variable4;

/* Returns a list containing four entries */

SListEntry *generate_list(void)
{
    SListEntry *list = NULL;

    slist_append(&list, &variable1);
    slist_append(&list, &variable2);
    slist_append(&list, &variable3);
    slist_append(&list, &variable4);

    return list;
}

void test_slist_append(void)
{
    SListEntry *list = NULL;
    void *data;

    slist_append(&list, &variable1);
    slist_append(&list, &variable2);
    slist_append(&list, &variable3);
    slist_append(&list, &variable4);

    assert(slist_nth_data(list, 0) == &variable1);
    assert(slist_nth_data(list, 1) == &variable2);
    assert(slist_nth_data(list, 2) == &variable3);
    assert(slist_nth_data(list, 3) == &variable4);
}

void test_slist_prepend(void)
{
    SListEntry *list = NULL;
    void *data;

    slist_prepend(&list, &variable1);
    slist_prepend(&list, &variable2);
    slist_prepend(&list, &variable3);
    slist_prepend(&list, &variable4);

    assert(slist_nth_data(list, 0) == &variable4);
    assert(slist_nth_data(list, 1) == &variable3);
    assert(slist_nth_data(list, 2) == &variable2);
    assert(slist_nth_data(list, 3) == &variable1);
}

void test_slist_free(void)
{
    SListEntry *list;

    /* Create a list and free it */

    list = generate_list();

    slist_free(list);

    /* Check the empty list frees correctly */

    slist_free(NULL);
}

void test_slist_next(void)
{
    SListEntry *list;
    SListEntry *rover;

    list = generate_list();

    rover = list;
    assert(slist_data(rover) == &variable1);
    rover = slist_next(rover);
    assert(slist_data(rover) == &variable2);
    rover = slist_next(rover);
    assert(slist_data(rover) == &variable3);
    rover = slist_next(rover);
    assert(slist_data(rover) == &variable4);
    rover = slist_next(rover);
    assert(rover == NULL);
}

void test_slist_nth_entry(void)
{
    SListEntry *list;
    SListEntry *entry;

    list = generate_list();

    /* Check all values in the list */

    entry = slist_nth_entry(list, 0);
    assert(slist_data(entry) == &variable1);
    entry = slist_nth_entry(list, 1);
    assert(slist_data(entry) == &variable2);
    entry = slist_nth_entry(list, 2);
    assert(slist_data(entry) == &variable3);
    entry = slist_nth_entry(list, 3);
    assert(slist_data(entry) == &variable4);

    /* Check negative values */

    entry = slist_nth_entry(list, -1);
    assert(entry == NULL);

    /* Check out of range values */

    entry = slist_nth_entry(list, 4);
    assert(entry == NULL);
    entry = slist_nth_entry(list, 400);
    assert(entry == NULL);
}

void test_slist_nth_data(void)
{
    SListEntry *list;

    list = generate_list();

    /* Check all values in the list */

    assert(slist_nth_data(list, 0) == &variable1);
    assert(slist_nth_data(list, 1) == &variable2);
    assert(slist_nth_data(list, 2) == &variable3);
    assert(slist_nth_data(list, 3) == &variable4);

    /* Check out of range values */

    assert(slist_nth_data(list, -1) == NULL);
    assert(slist_nth_data(list, 4) == NULL);
    assert(slist_nth_data(list, 400) == NULL);
}

void test_slist_length(void)
{
    SListEntry *list;

    /* Generate a list and check that it is four entries long */

    list = generate_list();

    assert(slist_length(list) == 4);

    /* Add an entry and check that it still works properly */

    slist_prepend(&list, &variable1);

    assert(slist_length(list) == 5);

    /* Check the length of the empty list */

    assert(slist_length(NULL) == 0);
}

void test_slist_remove_entry(void)
{
    SListEntry *empty_list = NULL;
    SListEntry *list;
    SListEntry *entry;

    list = generate_list();

    /* Remove the third entry */

    entry = slist_nth_entry(list, 2);
    assert(slist_remove_entry(&list, entry) != 0);
    assert(slist_length(list) == 3);

    /* Remove the first entry */

    entry = slist_nth_entry(list, 0);
    assert(slist_remove_entry(&list, entry) != 0);
    assert(slist_length(list) == 2);
    
    /* Try some invalid removes */

    /* This was already removed: */

    assert(slist_remove_entry(&list, entry) == 0);

    /* NULL */

    assert(slist_remove_entry(&list, NULL) == 0);

    /* Removing NULL from an empty list */

    assert(slist_remove_entry(&empty_list, NULL) == 0);
}

int int_equal(int *location1, int *location2)
{
    return *location1 == *location2;
}

void test_slist_remove_data(void)
{
    int entries[] = { 89, 4, 23, 42, 4, 16, 15, 4, 8, 99, 50, 30, 4 };
    int num_entries = sizeof(entries) / sizeof(int);
    int val;
    SListEntry *list;
    int i;

    /* Generate a list containing all the entries in the array */

    list = NULL;

    for (i=0; i<num_entries; ++i) {
        slist_prepend(&list, &entries[i]);
    }

    /* Test removing invalid data */

    val = 0;
    assert(slist_remove_data(&list, (SListEqualFunc) int_equal, &val) == 0);
    val = 56;
    assert(slist_remove_data(&list, (SListEqualFunc) int_equal, &val) == 0);
    
    /* Remove the number 8 from the list */

    val = 8;
    assert(slist_remove_data(&list, (SListEqualFunc) int_equal, &val) == 1);
    assert(slist_length(list) == num_entries - 1);

    /* Remove the number 4 from the list (occurs multiple times) */

    val = 4;
    assert(slist_remove_data(&list, (SListEqualFunc) int_equal, &val) == 4);
    assert(slist_length(list) == num_entries - 5);

    /* Remove the number 89 from the list (first entry) */

    val = 89;
    assert(slist_remove_data(&list, (SListEqualFunc) int_equal, &val) == 1);
    assert(slist_length(list) == num_entries - 6);
}

int int_compare(int *location1, int *location2)
{
    if (*location1 < *location2)
        return -1;
    else if (*location1 > *location2)
        return 1;
    else
        return 0;
}

void test_slist_sort(void)
{
    SListEntry *list;
    int entries[] = { 89, 4, 23, 42, 4, 16, 15, 4, 8, 99, 50, 30, 4 };
    int sorted[]  = { 4, 4, 4, 4, 8, 15, 16, 23, 30, 42, 50, 89, 99 };
    int num_entries = sizeof(entries) / sizeof(int);
    int i;

    list = NULL;

    for (i=0; i<num_entries; ++i) {
        slist_prepend(&list, &entries[i]);
    }

    slist_sort(&list, (SListCompareFunc) int_compare);

    /* List length is unchanged */

    assert(slist_length(list) == num_entries);

    /* Check the list is sorted */

    for (i=0; i<num_entries; ++i) {
        int *value;

        value = (int *) slist_nth_data(list, i);
        assert(*value == sorted[i]);
    }

    /* Check sorting an empty list */

    list = NULL;

    slist_sort(&list, (SListCompareFunc) int_compare);

    assert(list == NULL);
}

void test_slist_find_data(void)
{
    int entries[] = { 89, 23, 42, 16, 15, 4, 8, 99, 50, 30 };
    int num_entries = sizeof(entries) / sizeof(int);
    SListEntry *list;
    SListEntry *result;
    int i;
    int val;
    int *data;

    /* Generate a list containing the entries */

    list = NULL;
    for (i=0; i<num_entries; ++i) {
        slist_append(&list, &entries[i]);
    }

    /* Check that each value can be searched for correctly */

    for (i=0; i<num_entries; ++i) {

        val = entries[i];
        
        result = slist_find_data(list, (SListEqualFunc) int_equal, &val);
                                 
        assert(result != NULL);

        data = (int *) slist_data(result);
        assert(*data == val);
    }
    
    /* Check some invalid values return NULL */

    val = 0;
    assert(slist_find_data(list, (SListEqualFunc) int_equal, &val) == NULL);
    val = 56;
    assert(slist_find_data(list, (SListEqualFunc) int_equal, &val) == NULL);
}

int main(int argc, char *argv[])
{
    test_slist_append();
    test_slist_prepend();
    test_slist_free();
    test_slist_next();
    test_slist_nth_entry();
    test_slist_nth_data();
    test_slist_length();
    test_slist_remove_entry();
    test_slist_remove_data();
    test_slist_sort();
    test_slist_find_data();
}

