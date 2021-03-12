/* 
 * player.c - nuggets game module for server side player info
 *
 * see player.h for high level overview
 *
 * Team Chicken 21W
 */


#include <stdio.h>
#include <stdlib.h>
#include "./libcs50/hashtable.h"
#include "./game.h"
#include "./player.h"
#include "./support/message.h"

/****************** global functions *************/

/**************** player_new ****************/
player_t *player_new(char *name, game_t *game, const addr_t addr) {
    // allocate memory, start initializing parameters
    player_t *player = malloc(sizeof(player_t));
    player->name = name;
    player->gold = 0;
    player->game = game;
    player->inRoom = 1;
    player->id = game->playersJoined;
    player->addr = addr;

    //allocate memory for the visibility map
    player->visibility = malloc((game->rows + 1) * sizeof(char *));
    for(int i = 0; i<game->rows; i++){
        player->visibility[i] = malloc((game->cols + 1) * sizeof(char));
        for(int j = 0; j<game->cols; j++){
            player->visibility[i][j] = ' ';
        }
        // to prevent uninitialized value error in sendDisplay
        player->visibility[i][game->cols] = '\0';
    }

    // set the players letter to appear on map
    player->letter = (char)(player->id + 'A');

    // find a random starting point
    while (1) {
        int row = rand() % (game->rows); 
        int col = rand() % (game->cols);

        // if the random location is an empty room spot, add a player to that spot
        if (game->map[row][col] == '.') {
            player->row = row;
            player->col = col;
            game->map[row][col] = player->letter;
            player->visibility[row][col] = player->letter;
            break;
        }
    }

    // insert player into the hashtable of players and return it
    hashtable_insert(player->game->players, name, player);
    return player;

}

/**************** player_delete ****************/
void player_delete(void* item){
    // get the player from the item in the hashtable, then free it.
    player_t *player = (player_t *) item;
    if(player != NULL) {
        for(int i = 0; i<player->game->rows; i++){
            free(player->visibility[i]);
        }
        free(player->visibility);

        free(player);
    }
}
