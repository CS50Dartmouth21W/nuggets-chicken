#ifndef __PLAYER_H
#define __PLAYER_H
#include "./game.h"
#include "./support/message.h"
//typedef struct player player_t;
typedef struct player {
    char *name;
    int row;
    int column;
    int gold;
    addr_t addr;
    game_t *game;
    char *visibility[];
} player_t;


player_t *player_new(char *name, game_t *game, const addr_t addr);
void player_delete(player_t *player);

#endif
