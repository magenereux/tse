CFLAGS= -Wall -pedantic -std=c11 -I../utils -L../lib -g
LIBS=-lutils -lcurl

%.o: %.c %.h
			gcc $(CFLAGS) -c $<

indexer:
				gcc $(CFLAGS) indexer.c $(LIBS) -o $@


step2indexer:
				gcc $(CFLAGS) step2indexer.c $(LIBS) -o $@


step3indexer:
				gcc $(CFLAGS) step3indexer.c $(LIBS) -o $@


step4indexer:
				gcc $(CFLAGS) step4indexer.c $(LIBS) -o $@

run:
				./indexer

grind:
				valgrind --leak-check=full ./indexer 3

clean:
				rm -f *.o step2indexer step3indexer step4indexer indexer

