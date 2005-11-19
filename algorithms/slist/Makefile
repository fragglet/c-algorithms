
CFLAGS = -g -Wall
OBJS = slist.o test-slist.o

test-slist : $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

%.o : %.c
	$(CC) $(CFLAGS) $^ -c -o $@

clean : 
	rm -f $(OBJS) test-slist

