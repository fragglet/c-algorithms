
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

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "compare-string.h"

/* Comparison functions for strings */

int string_equal(void *string1, void *string2)
{
	return strcmp((char *) string1, (char *) string2) == 0;
}

int string_compare(void *string1, void *string2)
{
	int result;

	result = strcmp((char *) string1, (char *) string2);
	
	if (result < 0) {
		return -1;
	} else if (result > 0) {
		return 1;
	} else {
		return 0;
	}
}

/* Comparison functions for strings, which ignore the case of letters. */

int string_nocase_equal(void *string1, void *string2)
{
	return string_nocase_compare((char *) string1, (char *) string2) == 0;
}

/* On many systems, strcasecmp or stricmp will give the same functionality
 * as this function.  However, it is non-standard and cannot be relied
 * on to be present. */

int string_nocase_compare(void *string1, void *string2)
{
	char *p1;
	char *p2;
	int c1, c2;

	/* Iterate over each character in the strings */

	p1 = (char *) string1;
	p2 = (char *) string2;

	for (;;) {

		c1 = tolower(*p1);
		c2 = tolower(*p2);

		if (c1 != c2) {

			/* Strings are different */

			if (c1 < c2) {
				return -1;
			} else {
				return 1;
			}
		}

		/* End of string */

		if (c1 == '\0')
			break;

		/* Advance to the next character */

		++p1;
		++p2;
	}

	/* Reached the end of string and no difference found */

	return 0;
}


