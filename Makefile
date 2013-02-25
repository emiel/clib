CFLAGS = -Wall -Werror -g -O0

all: cbuf.o ll.o

cbuf.o: cbuf.c
	$(CC) $(CFLAGS) -c -o $@ $<

ll.o: ll.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o
