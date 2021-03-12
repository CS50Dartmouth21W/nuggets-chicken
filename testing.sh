#!/bin/bash
# testing for server.c
#
# CS 50 Nuggets
# Team Chicken

# Testing for server.c

# valid input
./server maps/main.txt <a.txt

# valid input with seed
./server maps/main.txt 1 <a.txt

# valid input
./server maps/fewspots.txt <a.txt

# valid input
./server maps/narrow.txt 15 <a.txt

# valid input (high seed)
./server maps/narrow.txt 10000 <a.txt

# valid input
./server maps/hole.txt <a.txt

# valid input
./server maps/big.txt 3000 <a.txt

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
echo here
./server maps/main.txt 0

# invalid seed
echo here2
./server maps/main.txt -1

# invalid seed
./server maps/main.txt %___

# non-existent map file
./server dartmouthmap.txt 2

exit 0
