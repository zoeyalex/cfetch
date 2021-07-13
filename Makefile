CC=gcc
CFLAGS=-O3

all: cfetch

cfetch: cfetch.c
	$(CC) $(CFLAGS) -o cfetch cfetch.c

clean:
	rm -f cfetch

install: all
	cp -f cfetch /usr/local/bin/

uninstall:
	rm -f /usr/local/bin/cfetch

.PHONY: clean install uninstall all
