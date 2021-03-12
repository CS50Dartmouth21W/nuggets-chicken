/* 
 * player.h - header file for nuggets player module
 *
 * 
 * The player struct is responsible for holding all the information about a player
 * A server client needs relating to identification information and game status
 * 
 * Team Chicken 21W
 * 
 */

#ifndef __PLAYER_H
#define __PLAYER_H
#include "./game.h"
#include "./support/message.h"

/**************** global types ****************/
/* stores all possible information 
the server needs about a player */
typedef struct player {
    char *name;
    int row;
    int col;
    int gold;
    int id;
    int inRoom;
    addr_t addr;
    game_t *game;
    char **visibility;
    char letter;
} player_t;

/**************** functions ****************/

/**************** player_new ****************/
/* Create a new player.
 * 
 * We return:
 *   pointer to a player.
 * Caller is responsible for:
 *   later calling player_delete.
 */
player_t *player_new(char *name, game_t *game, const addr_t addr);

/**************** player_delete ****************/
/* Delete player and visbility map
 *
 * Caller provides:
 *   valid player pointer,
 * We do:
 *   if player==NULL, do nothing.
 *   otherwise, we delete the player and all of its allocated memory
 * 
 */
void player_delete(void* item);

#endif
