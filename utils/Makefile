CFLAGS= -Wall -pedantic -std=c11 -g
OFILES=queue.o hash.o webpage.o pageio.o indexio.o lqueue.o lputTest.o lqsearchTest.o lqapplyTest.o

%.o: %.c %.h
		gcc $(CFLAGS) -c $<

all:	$(OFILES)
			ar cr ../lib/libutils.a $(OFILES)

lputTest: lqueue.o lputTest.o
		gcc $(CFLAGS) lqueue.o queue.o lputTest.o -o $@ -lpthread

lqsearchTest: lqueue.o lqsearchTest.o
		gcc $(CFLAGS) lqueue.o queue.o lqsearchTest.o -o $@ -lpthread

lqremoveTest: lqueue.o lqremoveTest.o
		gcc $(CFLAGS) lqueue.o queue.o lqremoveTest.o -o $@ -lpthread

lqgetTest: lqueue.o lqgetTest.o
		gcc $(CFLAGS) lqueue.o queue.o lqgetTest.o -o $@ -lpthread

lqapplyTest: lqueue.o lqapplyTest.o
		gcc $(CFLAGS) lqueue.o queue.o lqapplyTest.o -o $@ -lpthread

clean:
		rm -f *.o ../lib/libutils.a

