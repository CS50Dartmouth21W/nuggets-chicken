#include <stdio.h>
#include <stdlib.h>
#include "./libcs50/hashtable.h"
#include "./game.h"
#include "./player.h"

/*typedef struct player {
    char *name;
    int row;
    int column;
    int gold;
    game_t *game;
} player_t;*/

/*
 * This function is given a player name as well as the game the player is added to
 * and it returnns a new player that was previously on a point with a '.'
 * */
player_t *player_new(char *name, game_t *game) {
    player_t *player = malloc(sizeof(player_t));
    player->name = name;
    player->gold = 0;
    player->game = game;

    while (1) {
        int row = rand() % (game->rows);    // random number from 0 to row-1
        int column = rand() % (game->cols);

        // if the random location is an empty room spot, add a player to that spot
        if (game->map[row][column] == '.') {
            player->row = row;
            player->column = column;
            game->map[row][column] = '@';
            break;
        }
    }

    hashtable_insert(player->game->players, name, player);
    return player;
}

