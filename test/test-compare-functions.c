 
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

/* Test cases for compare functions */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "compare-int.h"
#include "compare-pointer.h"
#include "compare-string.h"

void test_int_compare(void)
{
    int a = 4;
    int b = 8;
    int c = 4;

    /* If first is less than second, result is negative) */

    assert(int_compare(&a, &b) < 0);

    /* If first is more than second, result is positive) */

    assert(int_compare(&b, &a) > 0);

    /* If both are equal, result is zero */

    assert(int_compare(&a, &c) == 0);
}

void test_int_equal(void)
{
    int a = 4;
    int b = 8;
    int c = 4;

    /* Returns non-zero (true) if both are equal */

    assert(int_equal(&a, &c) != 0);

    /* Returns zero (false) if not equal) */

    assert(int_equal(&a, &b) == 0);
}

void test_pointer_compare(void)
{
    int array[5];

    /* Negative if first argument is a lower memory address than
     * the second */

    assert(pointer_compare(&array[0], &array[4]) < 0);

    /* Positive if the first argument is a higher memory address
     * than the second */

    assert(pointer_compare(&array[3], &array[2]) > 0);

    /* Zero if the two arguments are equal */

    assert(pointer_compare(&array[4], &array[4]) == 0);
}

void test_pointer_equal(void)
{
    int a, b;
    
    /* Non-zero (true) if the two pointers are equal */
    
    assert(pointer_equal(&a, &a) != 0);

    /* Zero (false) if the two pointers are not equal */

    assert(pointer_equal(&a, &b) == 0);
}

void test_string_compare(void)
{
    char test1[] = "Apple";
    char test2[] = "Orange";
    char test3[] = "Apple";

    /* Negative if first argument should be sorted before the second */

    assert(string_compare(test1, test2) < 0);

    /* Positive if the second argument should be sorted before the first */

    assert(string_compare(test2, test1) > 0);

    /* Zero if the two arguments are equal */

    assert(string_compare(test1, test3) == 0);
}

void test_string_equal(void)
{
    char test1[] = "this is a test string";
    char test2[] = "this is a test string ";
    char test3[] = "this is a test strin";
    char test4[] = "this is a test strinG";
    char test5[] = "this is a test string";

    /* Non-zero (true) if the two strings are equal */

    assert(string_equal(test1, test5) != 0);

    /* Zero (false) if the two strings are different */

    /* Check that length affects the result */
    assert(string_equal(test1, test2) == 0);
    assert(string_equal(test1, test3) == 0);
    
    /* Case sensitive */
    assert(string_equal(test1, test4) == 0);
}

int main(int argc, char *argv[])
{
    test_int_compare();
    test_int_equal();
    test_pointer_compare();
    test_pointer_equal();
    test_string_compare();
    test_string_equal();

    return 0;
}

