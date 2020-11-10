CFLAGS= -Wall -pedantic -std=c11 -g
OFILES=queue.o hash.o webpage.o pageio.o indexio.o lqueue.o lputTest.o

%.o: %.c %.h
		gcc $(CFLAGS) -c $<

all:	$(OFILES)
			ar cr ../lib/libutils.a $(OFILES)

lputTest: lqueue.o lputTest.o
		gcc $(CFLAGS) lqueue.o queue.o lputTest.o -o $@ -lpthread

clean:
		rm -f *.o ../lib/libutils.a

