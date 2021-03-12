# Makefile for the server
#
# Jack Benhayon 21W

L = ./libcs50
S = ./support
CC=gcc
CFLAGS=-Wall -pedantic -std=c11 -ggdb -I$L
PROG = server
OBJS = server.o game.o player.o communication.o $S/message.o $S/log.o visibility.o
LLIBS = $L/libcs50.a

.PHONY: all clean test

all: $(PROG)

# executable depends on object files
$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS) -o $(PROG) $(LIBS) $(LLIBS) -lm

# object files depend on include files
server.o: $L/file.h $L/counters.h ./movement.c ./communication.h $S/log.h $S/message.h
movement.o:
communication.o :
player.o : 
game.o : 
$S/message.o :
$S/log.o : 	

test: $(PROG)
	./testing.sh

valgrind: $(PROG)
	


clean:
	rm -f $(PROG) core
	rm -f *~ *.o
	rm -rf *.dSYM
