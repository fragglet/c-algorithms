
CFLAGS = -g -Wall
OBJS = queue.o test-queue.o

test-queue : $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

%.o : %.c
	$(CC) $(CFLAGS) $^ -c -o $@

clean : 
	rm $(OBJS) test-queue

