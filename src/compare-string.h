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
 * @file compare-string.h
 *
 * Comparison functions for strings.
 *
 * To find the difference between two strings, use @ref string_compare.
 *
 * To find if two strings are equal, use @ref string_equal.
 *
 * For case insensitive versions, see @ref string_nocase_compare and
 * @ref string_nocase_equal.
 */

#ifndef ALGORITHM_COMPARE_STRING_H
#define ALGORITHM_COMPARE_STRING_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Compare two strings to determine if they are equal.
 *
 * @param string1         The first string.
 * @param string2         The second string.
 * @return                Non-zero if the strings are equal, zero if they are
 *                        not equal.
 */

int string_equal(void *string1, void *string2);

/**
 * Compare two strings.
 *
 * @param string1         The first string.
 * @param string2         The second string.
 * @return                A negative value if the first string should be
 *                        sorted before the second, a positive value if the
 *                        first string should be sorted after the second,
 *                        zero if the two strings are equal.
 */

int string_compare(void *string1, void *string2);

/**
 * Compare two strings to determine if they are equal, ignoring the
 * case of letters.
 *
 * @param string1         The first string.
 * @param string2         The second string.
 * @return                Non-zero if the strings are equal, zero if they are
 *                        not equal.
 */

int string_nocase_equal(void *string1, void *string2);

/**
 * Compare two strings, ignoring the case of letters.
 *
 * @param string1         The first string.
 * @param string2         The second string.
 * @return                A negative value if the first string should be
 *                        sorted before the second, a positive value if the
 *                        first string should be sorted after the second,
 *                        zero if the two strings are equal.
 */

int string_nocase_compare(void *string1, void *string2);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef ALGORITHM_COMPARE_STRING_H */

