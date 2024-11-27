# Makefile for client
## uses ncurses library for displaying the game

LIB = -lncurses -lm
CFLAGS = -Wall -pedantic -std=gnu11 -ggdb -I../support
CC = gcc
LIBDIR = ../support/support.a

.PHONY: all clean

############# default rule ###########
all: client

client.o: client.c ../support/message.h ../support/log.h
	$(CC) $(CFLAGS) -c client.c -o $@

client: client.o $(LIBDIR)
	$(CC) $(CFLAGS) $^ $(LIB) -o $@


############# clean ###########
clean:
	rm -f core
	rm -rf *~ *.o *.gch *.dSYM
# rm -f *.log Dont remove log files automatically
	rm -f client