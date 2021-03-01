/*
 * server.c
 * 
 * Jason Chen
 * 
 * Group Project
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"


void map_loader(FILE *fptr, char *map[]);
void gold_generator(char *map[]);

static const int GoldTotal = 250;      // amount of gold in the game
static const int GoldMinNumPiles = 10; // minimum number of gold piles
static const int GoldMaxNumPiles = 30; // maximum number of gold piles
int rows;
int columns;

int main(int argc, char* argv[]) {

    // check if number of parameters is correct
    if (argc != 2 && argc != 3) {
        printf("Input: ./server map.txt [seed]\n");
        return 1;
    }

    if (argc == 2) {
        
        srand(getpid());
    }

    if (argc == 3) {
        char *lastchar;
        int seed = strtold(argv[2], &lastchar);

        // check if seed is a positive number
        if (*lastchar == '\0' && seed > 0) {
            srand(seed);
        } else if (*lastchar != '\0') {
            printf("Seed must be a number\n");
            return 1;
        } else {
            printf("seed must be a positive number\n");
            return 1;
        }

    }



    FILE *fptr = fopen(argv[1], "r");

    // if the file was not opened successfully
    if (fptr == 0) {
        printf("map does not exist\n");
        return 1;
    } else {
        rows = lines_in_file(fptr);
        char *map[rows];

        map_loader(fptr, map);

        gold_generator(map);
        
        for (int i=0; i < rows; i++) {
            printf("%s\n", map[i]);
        }

        fclose(fptr);
    }


    return 0;
}


void map_loader(FILE *fptr, char *map[]) {
    
    int rows = lines_in_file(fptr);

    for (int i=0; i<rows; i++) {
        char *line = freadlinep(fptr);
        columns = strlen(line);
        map[i] = line;
    }
}

// drops gold nuggets in a random number of random-sized piles, each pile at some spot in a room. 
void gold_generator(char *map[]) {
        
        // choose a random number of piles between GoldMaxNumPiles and GoldMinNumPiles
        int numPiles = (rand() % (GoldMaxNumPiles - GoldMinNumPiles + 1) + GoldMinNumPiles);

        printf("numPiles: %d\n", numPiles);

        int goldToDrop = GoldTotal;     // track how many gold nuggets still need to be dropped

        while (goldToDrop > 0) {

            int row = (rand() % (rows + 1));
            int column = (rand() % (columns + 1));

            // if the random location is an empty room spot, drop gold
            if (map[row][column] == '.') {
                int numGoldInPile;
                if (numPiles == 1){
                    numGoldInPile = goldToDrop;    // drop all the gold left in the last pile
                } else {
                    int maxGoldInPile = goldToDrop - numPiles + 1;  // max gold that can be dropped in a pile (in order to have at least one gold per remaining pile)
                    numGoldInPile = (rand() % (maxGoldInPile - 1 + 1) + 1); // random number between 1 and maxGold
                }
                printf("Pile #: %d, numGoldInPile %d\n", numPiles, numGoldInPile);
                map[row][column] = '*';

                goldToDrop -= numGoldInPile;
                numPiles--;
            }
        }

        printf("Gold to drop is: %d\n", goldToDrop);
}

