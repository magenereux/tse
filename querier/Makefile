CFLAGS= -Wall -pedantic -std=c11 -I../utils -L../lib -g
LIBS=-lutils -lcurl

%.o: %.c %.h
			gcc $(CFLAGS) -c $<

query:
				gcc $(CFLAGS) query.c $(LIBS) -o $@

step1query:
				gcc $(CFLAGS) step1query.c $(LIBS) -o $@

step2query:
				gcc $(CFLAGS) step2query.c $(LIBS) -o $@

step3query:
				gcc $(CFLAGS) step3query.c $(LIBS) -o $@

grind:
				valgrind --leak-check=full --show-leak-kinds=all ./query

clean:
				rm -f *.o query step1query step2query step3query
