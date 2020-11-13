CFLAGS= -Wall -pedantic -std=c11 -I../utils -L../lib -g
LIBS=-lutils -lcurl

%.o: %.c %.h
			gcc $(CFLAGS) -c $<

all: testQ testH hputTest happlyTest hsearchTest

testQ:
				gcc $(CFLAGS) testQ.o $(LIBS) -o $@ -lpthread

testH:
				gcc $(CFLAGS) testH.o $(LIBS) -o $@ -lpthread

hputTest:		
				gcc $(CFLAGS) hputTest.c $(LIBS) -o $@ -lpthread

happlyTest:		
				gcc $(CFLAGS) happlyTest.c $(LIBS) -o $@ -lpthread

hsearchTest:		
				gcc $(CFLAGS) hsearchTest.c $(LIBS) -o $@ -lpthread


clean:
				rm -f testQ testH hputTest happlyTest hsearchTest

