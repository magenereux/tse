CFLAGS= -Wall -pedantic -std=c11 -I../utils -L../lib -g
LIBS=-lutils -lcurl

%.o: %.c %.h
			gcc $(CFLAGS) -c $<

query:
				gcc $(CFLAGS) query.c $(LIBS) -o $@


grind:
				valgrind --leak-check=full --show-leak-kinds=all ./query indexnm

clean:
				rm -f *.o query
