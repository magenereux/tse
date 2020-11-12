CFLAGS= -Wall -pedantic -std=c11 -I../utils -L../lib -g
LIBS=-lutils -lcurl

%.o: %.c %.h
			gcc $(CFLAGS) -c $<

testQ:
				gcc $(CFLAGS) testQ.o $(LIBS) -o $@ -lpthread

clean:
				rm -f testQ

