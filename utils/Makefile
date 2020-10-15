CFLAGS= -Wall -pedantic -std=c11 -g
OFILES=queue.o hash.o webpage.o

%.o: %.c %.h
		gcc $(CFLAGS) -c $<

all:	$(OFILES)
			ar cr ../lib/libutils.a $(OFILES)

clean:
		rm -f *.o ../lib/libutils.a

