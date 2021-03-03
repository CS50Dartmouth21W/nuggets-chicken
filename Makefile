# Makefile for server.c
#
# Jason Chen
# CS50, March 2, 2021

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
PROG = server
OBJS = server.o file.o
LIBS = support/support.a

.PHONY: all clean

#all: crawler

# executable depends on object files
$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(PROG)

# object files depend on include files

file.o: file.c

server.o: file.h

test: testing.sh
	bash -v testing.sh

clean:
	rm -f $(PROG)
	rm -f *~ *.o
	rm -rf *.dSYM