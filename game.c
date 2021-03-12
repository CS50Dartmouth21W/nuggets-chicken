/*
 * game.c - game module for tracking status of nuggets game
 * 
 * Team Chicken 21W
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include "./libcs50/hashtable.h"
#include "./game.h"
#include "./player.h"
#include "./communication.h"

// initializes a game struct
game_t *game_new(char *map[], int rows, int cols, int MaxPlayers, int TotalGold) {
    game_t *game = malloc(sizeof(game_t) + (rows + 1) * (cols + 1) * sizeof(char));
    game->rows = rows;
    game->cols = cols;

    game->TotalGoldLeft = TotalGold;
    game->goldcounts = malloc((rows + 1) * sizeof(int *));

    game->spectator = false;

    // memory allocation for array that tracks gold pile sizes
    for (int i = 0; i < rows; i++){
        game->map[i] = map[i];
        game->goldcounts[i] = malloc((cols + 1) * sizeof(int));
        for(int j = 0; j<cols; j++){
            game->goldcounts[i][j] = 0;
        }
    }

    game->players = hashtable_new(MaxPlayers);
    game->MaxPlayers = MaxPlayers;
    game->playersJoined = 0;
    //counters_t *goldcounts = gold_generator(game);

    return game;
}

// deletes the game struct
void game_delete(game_t *game){
    for (int i = 0; i < game->rows; i++){
        free(game->map[i]);
        free(game->goldcounts[i]);
    }
    hashtable_delete(game->players, (void (*)(void *)) player_delete);
    free(game->goldcounts);
    //TODO: FREE OTHER STUFF
    free(game);
}

// initializes the spectator address in the game struct
void addSpectator(game_t *game, addr_t addr){
    if(game->spectator){
        // tell current spectator to quit
        quit((game->spectatorAddr), "You have been replaced by a new spectator.");
    }
    game->spectatorAddr = addr;
    game->spectator = true;
}
