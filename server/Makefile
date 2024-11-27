# Makefile for Server 
# Builds and compiles the server and its utility functions.
#
# Rana Moeez Hassan, Fall 2024

# Directories for the library and common modules
LIBS = -lm ../support/support.a 
SUPPORT_DIRECTORY = ../support
CLIENTTYPES_DIRECTORY = ../clienttypes
GAMESTATUS_DIRECTORY = ../gamestatus
GRID_DIRECTORY = ../grid
GOLD_DIRECTORY = ../gold

# Uncomment to enable debugging prints for testing
# TESTING = -DDEBUGPRINT

# Object files to compile
OBJS = server.o \
       $(SUPPORT_DIRECTORY)/file.o \
       $(SUPPORT_DIRECTORY)/log.o \
       $(SUPPORT_DIRECTORY)/message.o \
       $(CLIENTTYPES_DIRECTORY)/player.o \
       $(CLIENTTYPES_DIRECTORY)/spectator.o \
       $(GRID_DIRECTORY)/grid.o \
       $(GOLD_DIRECTORY)/gold.o \
       $(GAMESTATUS_DIRECTORY)/gamestatus.o
               
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I$(SUPPORT_DIRECTORY) \
         -I$(CLIENTTYPES_DIRECTORY) -I$(GAMESTATUS_DIRECTORY) -I$(GRID_DIRECTORY) -I$(GOLD_DIRECTORY)
CC = gcc

# Default target
all: server

# Target to build the server executable
server:$(OBJS)
	$(CC) $(CFLAGS) $^ -o ./server $(LIBS)

# Dependencies for server object file
server.o: server.c $(SUPPORT_DIRECTORY)/file.h \
          $(SUPPORT_DIRECTORY)/log.h $(SUPPORT_DIRECTORY)/message.h \
          $(CLIENTTYPES_DIRECTORY)/player.h $(CLIENTTYPES_DIRECTORY)/spectator.h \
          $(GAMESTATUS_DIRECTORY)/gamestatus.h $(GRID_DIRECTORY)/grid.h \
          $(GOLD_DIRECTORY)/gold.h

$(SUPPORT_DIRECTORY)/file.o: $(SUPPORT_DIRECTORY)/file.h
$(SUPPORT_DIRECTORY)/log.o: $(SUPPORT_DIRECTORY)/log.h
$(SUPPORT_DIRECTORY)/message.o: $(SUPPORT_DIRECTORY)/message.h
$(CLIENTTYPES_DIRECTORY)/player.o: $(CLIENTTYPES_DIRECTORY)/player.h
$(CLIENTTYPES_DIRECTORY)/spectator.o: $(CLIENTTYPES_DIRECTORY)/spectator.h
$(GRID_DIRECTORY)/grid.o: $(GRID_DIRECTORY)/grid.h $(SUPPORT_DIRECTORY)/file.h $(SUPPORT_DIRECTORY)/log.h
$(GOLD_DIRECTORY)/gold.o: $(GOLD_DIRECTORY)/gold.h
$(GAMESTATUS_DIRECTORY)/gamestatus.o: $(GAMESTATUS_DIRECTORY)/gamestatus.h

# Testing target
gridtest:
	make gridtest -C $(GRID_DIRECTORY)

# Example target for cleaning up object files and executables
clean:
	rm -f core
	rm -f *~ *.o
# rm -f *.log Dont remove log files automatically
	rm -f $(GRID_DIRECTORY)/*~
	rm -f $(GRID_DIRECTORY)/*.o
	rm -f $(GOLD_DIRECTORY)/*~
	rm -f $(GOLD_DIRECTORY)/*.o
	rm -f $(GAMESTATUS_DIRECTORY)/*~
	rm -f $(GAMESTATUS_DIRECTORY)/*.o
	rm -f $(CLIENTTYPES_DIRECTORY)/*~
	rm -f $(CLIENTTYPES_DIRECTORY)/*.o
	rm -f $(CLIENTTYPES_DIRECTORY)/*~
	rm -f $(CLIENTTYPES_DIRECTORY)/*.o
	rm -f server
	rm -f $(GRID_DIRECTORY)/gridtest

# Phony targets
.PHONY: all clean test

# List source files and other relevant information
sourcelist: Makefile *.md *.c *.h
	@echo $^