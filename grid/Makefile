# Makefile for 'grid' module
#
# Team Big D Nuggies
# Jake Fleming, Fall 2024

OBJS = grid.o
LIBS = ../support/support.a -lm  
EXEC = gridtest 

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../support
CC = gcc
MAKE = make

# Build the object file only
all: $(OBJS)

# Compile grid.o
grid.o: grid.c grid.h ../support/log.h ../support/file.h ../support/message.h
	$(CC) $(CFLAGS) -c grid.c -o grid.o

# Compile gridtest.o
gridtest.o: gridtest.c grid.h ../support/log.h ../support/file.h ../support/message.h
	$(CC) $(CFLAGS) -c gridtest.c -o gridtest.o

# Link the test executable
gridtest: grid.o gridtest.o $(LIBS)
	$(CC) $(CFLAGS) grid.o gridtest.o $(LIBS) -o gridtest

# Ensure the support library is built before linking
../support/support.a:
	$(MAKE) -C ../support

.PHONY: clean

# Clean up generated files
clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(EXEC)