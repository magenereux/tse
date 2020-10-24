CFLAGS= -Wall -pedantic -std=c11 -I../utils -L../lib -g
LIBS=-lutils -lcurl

%.o: %.c %.h
			gcc $(CFLAGS) -c $<

indexer:
				gcc $(CFLAGS) indexer.c $(LIBS) -o $@

run:
				./indexer

grind:
				valgrind --leak-check=full ./indexer

clean:
				rm -f *.o indexer

