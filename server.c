/*
 * server.c - nuggets game module for the server main function
 * 
 * main responsibilities include parsing arguments 
 * and initializing other modules
 * 
 * Team Chicken 21W
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"
#include "./libcs50/hashtable.h"
#include "./movement.c"
#include "./support/message.h"
#include "./support/log.h"
#include "./game.h"
#include "./player.h"
#include <unistd.h>

/**************** file-local global variables ****************/
static const int MaxPlayers = 10;  // maximum number of players
static const int GoldTotal = 250;      // amount of gold in the game
static const int GoldMinNumPiles = 1; // minimum number of gold piles
static const int GoldMaxNumPiles = 1; // maximum number of gold piles
static const int timeout = 5000;

/**************** global function declarations ****************/
game_t* map_loader(const char *file);
void gold_generator(game_t *game);

/**************** main ****************/
int main(int argc, char* argv[]) {
    // check if number of parameters is correct
    if (argc != 2 && argc != 3) {
        printf("Input: ./server map.txt [seed]\n");
        return 1;
    }

    // random seed if no seed provided
    if (argc == 2) {
        srand(getpid());
    }

    // if seed argument is a positive integer, set it as the seed
    if (argc == 3) {
        char *lastchar;
        int seed = strtold(argv[2], &lastchar);

        // check if seed is a positive number
        if (*lastchar == '\0' && seed > 0) {
            srand(seed);
        } else{
            printf("Seed must be a positive number\n");
            return 1;
        }
    }

    // call maploader which loads map from file and initialized game struct
    game_t *game = map_loader(argv[1]);
    if(game == NULL) return 1;
    
    // generate gold randomly on the map
    gold_generator(game);
   
    // listen for messages
    message_init(stderr);
    message_loop(game, timeout, handleTimeout, handleInput, handleMessage);
    message_done();

    // free memory
    game_delete(game);
    return 0;
}

/**************** global function implementation ****************/

/**************** map_loader ****************/
/* Load a nuggets map from a file and create a new game
 * 
 * We return:
 *   pointer to a game struct.
 * Caller is responsible for:
 *   providing a file with a valid nuggets map
 */
game_t* map_loader(const char *file) {
    FILE *fptr = fopen(file, "r"); 
    
    // if the file was not opened successfully
    if (fptr == 0) {
        printf("%s cannot be read\n", file);
        return NULL;
    }

    int rows = lines_in_file(fptr);
    game_t* game = NULL;

    // loop through rows in map array of strings, adding each row of file to
    // the map array
    if(rows > 0){
        char *map[rows];
        
        for (int i=0; i<rows; i++) {
            char *line = freadlinep(fptr);
            map[i] = line;
        }
        
        // once the map is loaded successfully, initiate game struct
        int cols = strlen(map[0]);
        game = game_new(map, rows, cols, MaxPlayers, GoldTotal);
    } else {
        printf("%s is an empty file", file);
    }

    fclose(fptr);
    return game;
}

/**************** gold_generator ****************/
/* Drop gold piles onto a nuggets map randomly
 *
 * Caller is responsible for:
 *   providing a non-null game struct with non-null instance variables
 */
void gold_generator(game_t *game) { 
    int rows = game->rows;
    int columns = game->cols;

    // choose a random number of piles between GoldMaxNumPiles and GoldMinNumPiles
    int numPiles = (rand() % (GoldMaxNumPiles - GoldMinNumPiles + 1) + GoldMinNumPiles);

    printf("numPiles: %d\n", numPiles);

    int goldToDrop = GoldTotal;     // track how many gold nuggets still need to be dropped

    while (goldToDrop > 0) {
            
        // get a random row and column
        int row = (rand() % (rows));
        int column = (rand() % (columns));
            
        // if the random location is an empty room spot, drop gold
        if (game->map[row][column] == '.') {
            int numGoldInPile;
            if (numPiles == 1) {
                // drop all the gold left in the last pile
                numGoldInPile = goldToDrop;    
            } else {
                // max gold that can be dropped in a pile 
                // (in order to have at least one gold per remaining pile)
                int maxGoldInPile = goldToDrop - numPiles + 1; 

                // random number between 1 and maxGold
                numGoldInPile = (rand() % (maxGoldInPile) + 1); 
            }
                
                
            printf("Pile #: %d, numGoldInPile %d\n", numPiles, numGoldInPile);
            // set the spot on the map as a gold pile and keep track of the amount of gold
            game->map[row][column] = '*';
            game->goldcounts[row][column] = numGoldInPile;

            printf("SETTING: %d %d %d\n", row, column, game->goldcounts[row][column]);

            //decrement remaining gold and piles
            goldToDrop -= numGoldInPile;
            numPiles--;
            }
        }

    printf("Gold to drop is: %d\n", goldToDrop);
}
