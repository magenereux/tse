CFLAGS= -Wall -pedantic -std=c11 -I../utils -L../lib -g
LIBS=-lutils -lcurl

%.o: %.c %.h
			gcc $(CFLAGS) -c $<

crawler:
				gcc $(CFLAGS) crawler.c $(LIBS) -o $@

run:
				./crawler

clean:
				rm -f *.o crawler

