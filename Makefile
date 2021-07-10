CC=gcc
CFLAGS=-O3

cfetch: cfetch.c
	$(CC) $(CFLAGS) -o cfetch cfetch.c
