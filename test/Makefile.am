
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

noinst_PROGRAMS = $(TESTS)

AM_CFLAGS = -I../src -Wall -fprofile-arcs -ftest-coverage
LDADD = $(top_builddir)/src/libcalg.la

test_arraylist_SOURCES = test-arraylist.c
test_list_SOURCES = test-list.c
test_slist_SOURCES = test-slist.c
test_queue_SOURCES = test-queue.c

