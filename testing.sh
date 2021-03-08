# testing for server.c
#
# CS 50 Nuggets

# valid input
./server maps/main.txt

# valid input with seed
./server maps/main.txt 1

# valid input
./server maps/fewspots.txt

# valid input
./server maps/narrow.txt 15

# valid input (high seed)
./server maps/narrow.txt 10000

# valid input
./server maps/hole.txt

# valid input
./server maps/big.txt 3000

# ------ error cases -----------

# too many arguments
./server maps/main.txt 3 hello

# too many arguments
./server maps/main.txt 3 hello there

# too few arguments
./server

# invalid seed
./server maps/main.txt three

# invalid seed
./server maps/main.txt 3.33

# invalid seed
./server maps/main.txt -0.01

# invalid seed
./server maps/main.txt 0

# invalid seed 
./server maps/main.txt -1

# invalid seed
./server maps/main.txt %___

# non-existent map file
./server dartmouthmap.txt 2



# testing for player

# Not sure exactly how we can connect client with server since the port changes everytime.
# Using 00000 right now

# spectator join
./player localhost 00000

# another spectator attempts to join
./player localhost 00000

# player joins
./player localhost 00000 p1

# player joins
./player localhost 00000 p2

# player joins
./player localhost 00000 p3

# player joins
./player localhost 00000 p4

# player joins (multiple words in name)
./player localhost 00000 big green

# player joins (mulitple words in name)
./player localhost 00000 dartmouth big green

# error cases

# too few arguments
./player

./player localhost
