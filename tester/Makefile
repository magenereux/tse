CFLAGS= -Wall -pedantic -std=c11 -I../utils -L../lib -g
LIBS=-lutils -lcurl

%.o: %.c %.h
			gcc $(CFLAGS) -c $<

indexiotest:
			gcc $(CFLAGS) indexiotest.c $(LIBS) -o $@

pageiotest:
			gcc $(CFLAGS) pageiotest.c $(LIBS) -o $@


run:
				./indexiotest

grind:
				valgrind --leak-check=full ./indexiotest

clean:
				rm -f *.o pageiotest indexiotest

