CFLAGS= -Wall -pedantic -std=c11 -I../utils -L../lib -g
LIBS=-lutils -lcurl

%.o: %.c %.h
			gcc $(CFLAGS) -c $<

all: crawler lcrawler

crawler:
				gcc $(CFLAGS) crawler.c $(LIBS) -o $@

lcrawler:
				gcc $(CFLAGS) lcrawler.c $(LIBS) -o $@ -lpthread

run:
				./crawler

clean:
				rm -f *.o crawler lcrawler

