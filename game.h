/*
 * game.h - header file for game module
 * 
 * Team Chicken 21W
 * 
 */


#ifndef __GAME_H
#define __GAME_H
#include <stdbool.h>
#include "./libcs50/hashtable.h"
#include "./support/message.h"

// struct tracking the status of the game
typedef struct game {
    bool isover;
    int cols;
    int rows;
    addr_t spectatorAddr;
    bool spectator;
    int playersJoined;
    int numPlayersTotal;
    int MaxPlayers;
    int TotalGoldLeft;
    // hashes name -> player object
    hashtable_t *players;
    //counters_t goldcounts;
    int **goldcounts;
    char* map[];
} game_t;

/**************** game_new ****************/
/* Returns a new game struct
 * Caller provides an array of pointers to chars, number of rows, columns, max players, and total gold.
 */

game_t *game_new(char *map[], int rows, int cols, int MaxPlayers, int TotalGold);

/**************** game_delete ****************/
/* Deletes game struct
 * Caller provides pointer to the game
 */
void game_delete(game_t *game);

/**************** addSpectator ****************/
/* Initializes the spectator address in the game struct
 * Caller provides pointer to the game, and the address of spectator
 */
void addSpectator(game_t *game, addr_t addr);

#endif
