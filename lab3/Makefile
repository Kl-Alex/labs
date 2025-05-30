CC = gcc
CFLAGS = -Wall -O3
LIBS = -lssl -lcrypto

all: drbg_test

drbg_test: main.c drbg.c drbg.h
	$(CC) $(CFLAGS) main.c drbg.c -o drbg_test $(LIBS)

clean:
	rm -f drbg_test random_*.bin
