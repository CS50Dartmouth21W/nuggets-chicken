#ifndef __GAME_H
#define __GAME_H
#include <stdbool.h>
#include "./libcs50/hashtable.h"

//typedef struct game game_t;
typedef struct game {
    bool isover;
    int cols;
    int rows;
    bool hasSpectator;
    int totalGoldCollected;
    int playersJoined;
    // hashes name -> player object
    hashtable_t *players;
    //counters_t goldcounts;
    int **goldcounts;
    char* map[];
} game_t;


game_t *game_new(char *map[], int rows, int cols, int MaxPlayers);
void game_delete(game_t *game);

#endif
