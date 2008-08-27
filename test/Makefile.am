
if USE_VALGRIND
TESTS_ENVIRONMENT=./valgrind-wrapper
endif

TESTS =                          \
        test-alloc-testing       \
        test-arraylist           \
        test-avl-tree            \
        test-binary-heap         \
        test-binomial-heap       \
        test-bloom-filter        \
        test-cpp                 \
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
alloc-testing.c      alloc-testing.h        \
framework.c          framework.h

AM_CFLAGS = $(COV_FLAGS) -I../src -Wall
LDADD = $(top_builddir)/src/libcalgtest.a libtestframework.a

EXTRA_DIST=valgrind-wrapper

test-cpp: test-cpp.cpp
	$(CXX) $(AM_CFLAGS) $^ -o $@ $(LDADD)

