#include <stdio.h>
#include <stdlib.h>
#include "./libcs50/hashtable.h"
#include "./game.h"
#include "./player.h"
#include "./support/message.h"

/*
 * This function is given a player name as well as the game the player is added to
 * and it returnns a new player that was previously on a point with a '.'
 * */
player_t *player_new(char *name, game_t *game, const addr_t addr) {
    player_t *player = malloc(sizeof(player_t));
    player->name = name;
    player->gold = 0;
    player->game = game;
    player->id = game->playersJoined;
    game->playersJoined++;
    
    player->visibility = malloc((game->rows + 1) * sizeof(char *));
    for(int i = 0; i<game->rows; i++){
        player->visibility = malloc((game->cols + 1) * sizeof(char*));
        for(int j = 0; j<game->cols; j++){
            player->visibility[i][j] = ' ';

        }
    }

    while (1) {
        int row = rand() % (game->rows);    // random number from 0 to row-1
        int col = rand() % (game->cols);

        // if the random location is an empty room spot, add a player to that spot
        if (game->map[row][col] == '.') {
            player->row = row;
            player->col = col;
            game->map[row][col] = (char)(player->id + 'A');
            player->visibility[row][col] = (char)(player->id + 'A');
            break;
        }
    }

    hashtable_insert(player->game->players, name, player);
    player->addr = addr;

    return player;
}

void player_delete(player_t *player){
    if(player != NULL) free(player);
}

