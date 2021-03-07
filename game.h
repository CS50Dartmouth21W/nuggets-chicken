#ifndef __GAME_H
#define __GAME_H
#include <stdbool.h>
#include "./libcs50/hashtable.h"
#include "./support/message.h"

//typedef struct game game_t;
typedef struct game {
    bool isover;
    int cols;
    int rows;
    bool hasSpectator;
    addr_t *spectatorAddr;
    int playersJoined;
    int MaxPlayers;
    int TotalGoldLeft;
    // hashes name -> player object
    hashtable_t *players;
    //counters_t goldcounts;
    int **goldcounts;
    char* map[];
} game_t;


game_t *game_new(char *map[], int rows, int cols, int MaxPlayers, int TotalGold);
void game_delete(game_t *game);

#endif
