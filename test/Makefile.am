
if USE_VALGRIND
TESTS_ENVIRONMENT=./valgrind-wrapper
endif

TESTS =                          \
        test-arraylist           \
        test-avl-tree            \
        test-binary-heap         \
        test-binomial-heap       \
        test-bloom-filter        \
        test-list                \
        test-slist               \
        test-queue               \
        test-compare-functions   \
        test-hash-functions      \
        test-hash-table          \
        test-set                 \
        test-trie

check_PROGRAMS = $(TESTS)
check_LIBRARIES = libtestframework.a

libtestframework_a_SOURCES=\
        alloc-testing.c              alloc-testing.h

AM_CFLAGS = -I../src -Wall
LDADD = $(top_builddir)/src/libcalgtest.a libtestframework.a

EXTRA_DIST=valgrind-wrapper

