
TESTS =                          \
        test-arraylist           \
        test-avltree             \
        test-binary-heap         \
        test-list                \
        test-slist               \
        test-queue               \
        test-compare-functions   \
        test-hash-functions      \
        test-hashtable           \
        test-set                 \
        test-trie

noinst_PROGRAMS = $(TESTS)

AM_CFLAGS = -I../src -Wall
LDADD = $(top_builddir)/src/libcalg.la

test_arraylist_SOURCES = test-arraylist.c
test_list_SOURCES = test-list.c
test_slist_SOURCES = test-slist.c
test_queue_SOURCES = test-queue.c

