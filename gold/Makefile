# Makefile for gold module
# Group Big D Nuggets

OBJS = goldtest.o gold.o

CFLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc
MAKE = make

VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

goldtest: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

goldtest.o: gold.h
gold.o: gold.h

test: goldtest
	./goldtest

valgrind: goldtest
	$(VALGRIND) ./goldtest

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f goldtest
	rm -f core