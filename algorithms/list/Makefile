
CFLAGS = -g -Wall
OBJS = list.o test-list.o

test-list : $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

%.o : %.c
	$(CC) $(CFLAGS) $^ -c -o $@

clean : 
	rm $(OBJS) test-list

