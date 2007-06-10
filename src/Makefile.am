
AM_CFLAGS = -Wall

lib_LTLIBRARIES=libcalg.la

MAIN_HEADERFILES = libcalg.h

CALG_HEADERFILES=\
arraylist.h  compare-int.h      hash-int.h      hashtable.h  set.h         \
avl-tree.h   compare-pointer.h  hash-pointer.h  list.h       slist.h       \
queue.h      compare-string.h   hash-string.h   trie.h       binary-heap.h \
bloom-filter.h binomial-heap.h

SRC=\
arraylist.c    compare-pointer.c  hash-pointer.c  list.c   slist.c       \
avl-tree.c     compare-string.c   hash-string.c   queue.c  trie.c        \
compare-int.c  hash-int.c         hashtable.c     set.c    binary-heap.c \
bloom-filter.c binomial-heap.c

libcalg_la_SOURCES=$(SRC) $(MAIN_HEADERFILES) $(CALG_HEADERFILES)

headerfilesdir=$(includedir)/libcalg-1.0
headerfiles_HEADERS=$(MAIN_HEADERFILES)

calgheaderfilesdir=$(headerfilesdir)/libcalg
calgheaderfiles_HEADERS=$(CALG_HEADERFILES)

