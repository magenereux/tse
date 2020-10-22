CFLAGS= -Wall -pedantic -std=c11 -I../utils -L../lib -g
LIBS=-lutils -lcurl

%.o: %.c %.h
			gcc $(CFLAGS) -c $<

pageiotest:
				gcc $(CFLAGS) pageiotest.c $(LIBS) -o $@

run:
				./pageiotest

clean:
				rm -f *.o pageiotest

