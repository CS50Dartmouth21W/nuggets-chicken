#ifndef __PLAYER_H
#define __PLAYER_H
#include "./game.h"

//typedef struct player player_t;
typedef struct player {
    char *name;
    int row;
    int column;
    int gold;
    game_t *game;
} player_t;


player_t *player_new(char *name, game_t *game);
void player_delete(player_t *player);

#endif
