
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

#include <stdlib.h>
#include <string.h>

#include "arraylist.h"

/* Automatically resizing array */

ArrayList *arraylist_new(int length)
{
    ArrayList *new_arraylist;

    /* If the length is not specified, use a sensible default */
    
    if (length <= 0) {
        length = 16;
    }
    
    /* Allocate the new ArrayList and fill in the fields.  There are 
     * initially no entries. */

    new_arraylist = (ArrayList *) malloc(sizeof(ArrayList));
    new_arraylist->_alloced = length;
    new_arraylist->length = 0;

    /* Allocate the data array */

    new_arraylist->data = calloc(length, sizeof(void *));

    return new_arraylist;    
}

void arraylist_free(ArrayList *arraylist)
{
    /* Do not free if a NULL pointer is passed */

    if (arraylist != NULL) {
        free(arraylist->data);
        free(arraylist);
    }
}

static void arraylist_enlarge(ArrayList *arraylist)
{
    /* Double the allocated size */

    arraylist->_alloced *= 2;
    
    /* Reallocate the array to the new size */

    arraylist->data = realloc(arraylist->data, 
                              sizeof(void *) * arraylist->_alloced);
}

int arraylist_insert(ArrayList *arraylist, int index, void *data)
{
    /* Sanity check the index */

    if (index < 0 || index > arraylist->length)
        return 0;

    /* Increase the size if necessary */
    
    if (arraylist->length + 1 > arraylist->_alloced) {
        arraylist_enlarge(arraylist);
    }

    /* Move the contents of the array forward from the index
     * onwards */

    memmove(&arraylist->data[index + 1], 
            &arraylist->data[index],
            (arraylist->length - index) * sizeof(void *));

    /* Insert the new entry at the index */

    arraylist->data[index] = data;
    ++arraylist->length;

    return 1;
}

void arraylist_append(ArrayList *arraylist, void *data)
{
    arraylist_insert(arraylist, arraylist->length, data);
}

void arraylist_prepend(ArrayList *arraylist, void *data)
{
    arraylist_insert(arraylist, 0, data);
}

void arraylist_remove_range(ArrayList *arraylist, int index, int length)
{
    /* Check this is a valid range */

    if (index < 0 || length < 0 || index + length > arraylist->length)
        return;
    
    /* Move back the entries following the range to be removed */

    memmove(&arraylist->data[index],
            &arraylist->data[index + length],
            (arraylist->length - (index + length)) * sizeof(void *));

    /* Decrease the counter */

    arraylist->length -= length;
}

void arraylist_remove(ArrayList *arraylist, int index)
{
    arraylist_remove_range(arraylist, index, 1);
}

int arraylist_index_of(ArrayList *arraylist, 
                       ArrayListEqualFunc callback,
                       void *data)
{
    int i;

    for (i=0; i<arraylist->length; ++i) {
        if (callback(arraylist->data[i], data) != 0)
            return i;
    }

    return -1;
}

void arraylist_clear(ArrayList *arraylist)
{
    /* To clear the list, simply set the length to zero */
    
    arraylist->length = 0;
}

