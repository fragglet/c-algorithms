
CFLAGS = -g
OBJS = arraylist.o test-arraylist.o

test-arraylist : $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

%.o : %.c
	$(CC) $(CFLAGS) $^ -c -o $@

clean : 
	rm $(OBJS) test-arraylist

