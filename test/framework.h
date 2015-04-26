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

#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file framework.h
 *
 * @brief Framework for running unit tests.
 */

/**
 * A unit test.
 */

typedef void (*UnitTestFunction)(void);

/**
 * Run a list of unit tests.  The provided array contains a list of
 * pointers to test functions to invoke; the last entry is denoted
 * by a NULL pointer.
 *
 * @param tests          List of tests to invoke.
 */

void run_tests(UnitTestFunction *tests);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef TEST_FRAMEWORK_H */

