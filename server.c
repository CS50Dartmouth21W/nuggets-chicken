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
#include "./libcs50/counters.h"
#include "./libcs50/hashtable.h"
#include "./communication.c"
#include "./support/message.h"
#include "./support/log.h"
#include "./game.h"
#include "./player.h"

game_t* map_loader(const char *file);
counters_t* gold_generator(game_t *game);

static const int MaxPlayers = 10;  // maximum number of players
static const int GoldTotal = 250;      // amount of gold in the game
static const int GoldMinNumPiles = 10; // minimum number of gold piles
static const int GoldMaxNumPiles = 30; // maximum number of gold piles
static const int timeout = 5000;


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
        } else{
            printf("Seed must be a positive number\n");
            return 1;
        }
    }

    game_t *game = map_loader(argv[1]);
    if(game == NULL) return 1;
 
    for (int i=0; i < game->rows; i++) {
        printf("%s\n", game->map[i]);
    }

    gold_generator(game);

    //player_t *John = player_new("John", game);
    //printf("%s %d %d\n", John->name, John->row, John->column);
   
    while(game->TotalGoldLeft > 0){
        message_init(stderr);
        message_loop(game, timeout, handleTimeout, handleInput, handleMessage);
        message_done();
    }

    game_delete(game);
    //counters_delete(goldcounts);
    //free(John);
    return 0;
}

// TODO: WRITE A GAME OVER FUNCTION PRINTING STUFF OUT

game_t* map_loader(const char *file) {
    FILE *fptr = fopen(file, "r"); 
    
    // if the file was not opened successfully
    if (fptr == 0) {
        printf("%s cannot be read\n", file);
        return NULL;
    }

    int rows = lines_in_file(fptr);

    game_t* game = NULL;

    if(rows > 0){
        char *map[rows];
        
        for (int i=0; i<rows; i++) {
            char *line = freadlinep(fptr);
            map[i] = line;
        }
        
        int cols = strlen(map[0]);
        game = game_new(map, rows, cols, MaxPlayers, GoldTotal);
    } else {
        printf("%s is an empty file", file);
    }

    fclose(fptr);
    return game;
}

// drops gold nuggets in a random number of random-sized piles, each pile at some spot in a room.
//
// TODO: currently, all gold creation is first pile heavy as it starts at 250
//      should implmeent a max size for a gold pile for even distribution 
counters_t* gold_generator(game_t *game) { 
        int rows = game->rows;
        int columns = game->cols;

        counters_t* goldcount = counters_new();
        
        // choose a random number of piles between GoldMaxNumPiles and GoldMinNumPiles
        int numPiles = (rand() % (GoldMaxNumPiles - GoldMinNumPiles + 1) + GoldMinNumPiles);

        printf("numPiles: %d\n", numPiles);

        int goldToDrop = GoldTotal;     // track how many gold nuggets still need to be dropped

        while (goldToDrop > 0) {

            int row = (rand() % (rows));
            int column = (rand() % (columns));
            
            // if the random location is an empty room spot, drop gold
            if (game->map[row][column] == '.') {
                int numGoldInPile;
                int position;
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
                game->map[row][column] = '*';
                
                // !!!!! i plan on replacing the counter with this !!!!!
                game->goldcounts[row][column] = numGoldInPile;

                printf("SETTING: %d %d %d\n", row, column, game->goldcounts[row][column]);
                position = row*(columns-1);
                position += column;
                counters_set(goldcount, position, numGoldInPile);

                goldToDrop -= numGoldInPile;
                numPiles--;
            }
        }

        printf("Gold to drop is: %d\n", goldToDrop);
        return goldcount;
}
