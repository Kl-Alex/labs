CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -lssl -lcrypto

all: kdf_test

kdf_test: main.o kdf.o
	$(CC) -o kdf_test main.o kdf.o $(LDFLAGS)

main.o: main.c kdf.h
	$(CC) $(CFLAGS) -c main.c

kdf.o: kdf.c kdf.h
	$(CC) $(CFLAGS) -c kdf.c

clean:
	rm -f *.o kdf_test
