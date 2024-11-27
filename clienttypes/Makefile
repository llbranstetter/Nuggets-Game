# Makefile for the player and spectator types
## clienttypes directory Makefile

# Libraries and compiler settings
LIBS = -lm  # Only link with the math library
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../support ../.. ../gamestatus # Compiler flags
CC = gcc
LIBDIR = ../support/support.a  # Path to the static support library

.PHONY: all clean

############# Default Rule ###########
all: player spectator playertest


############# Link player ###########
player: player.o $(LIBDIR)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@  

############# Compile player.o ###########
player.o: player.c ../support/message.h ../support/log.h ../grid.h ../gamestatus/gamestatus.h
	$(CC) $(CFLAGS) -c player.c -o $@

############# Link spectator ###########
spectator: spectator.o $(LIBDIR)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@  

############# Compile spectator.o ###########
spectator.o: spectator.c ../support/message.h ../support/log.h ../grid.h ../gamestatus/gamestatus.h
	$(CC) $(CFLAGS) -c spectator.c -o $@

############# Link playertest ###########
playertest: playertest.o $(LIBDIR)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@  

############# Compile playertest.o ###########
playertest.o: playertest.c ../support/message.h ../support/log.h ../grid.h ../gamestatus/gamestatus.h
	$(CC) $(CFLAGS) -c playertest.c -o $@

# expects a file `test.names` to exist; it can contain any text.
test: playertest
	./playertest > testing.out

############# Clean Rule ###########
clean:
	rm -f core
	rm -rf *~ *.o *.gch *.dSYM
	rm -f *.log
	rm -f player spectator
