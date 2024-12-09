
AM_CFLAGS = $(TEST_CFLAGS) -I$(top_srcdir)/src -g
AM_CXXFLAGS = $(AM_CFLAGS)
LDADD = $(top_builddir)/src/libcalgtest.a libtestframework.a

if USE_VALGRIND
LOG_COMPILER=./valgrind-wrapper
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
        test-rb-tree             \
        test-set                 \
        test-trie		 \
	test-sortedarray

check_PROGRAMS = $(TESTS)
check_LIBRARIES = libtestframework.a

libtestframework_a_SOURCES=\
alloc-testing.c      alloc-testing.h        \
framework.c          framework.h

test_cpp_SOURCES = test-cpp.cpp

EXTRA_DIST=valgrind-wrapper gencov

if BUILD_COVERAGE

# Delete .gcda files before running tests.
check-am: check-prerun

check-prerun:
	@rm -f $(top_builddir)/lib/*.gcda   \
               $(top_builddir)/src/*.gcda   \
               $(top_builddir)/test/*.gcda

# Display coverage summary after running tests.
check: report-coverage

report-coverage: check-am
	@cd $(top_builddir); ./test/gencov src/*.c

endif

