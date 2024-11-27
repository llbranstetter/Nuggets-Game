# Makefile for CS50 Nuggets Game
#
# Big D Nuggies

.PHONY: all clean

############## default: make all libs and programs ##########
all: 
# (cd $L && if [ ! -r ./support/support.a ]; then make $L.a; fi) # Added a space before the closing bracket
	make -C support
	make -C server
	make -C client

gridtest:
	make gridtest -C server

############### TAGS for emacs users ##########
TAGS:  Makefile */Makefile */*.c */*.h */*.md */*.sh
	etags $^

test:
#Empty for now

############## clean  ##########
clean:
	rm -f *~
	make -C support clean
	make -C server clean
	make -C client clean


