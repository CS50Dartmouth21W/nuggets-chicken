#include <stdio.h>
#include <stdlib.h>
#include "./libcs50/hashtable.h"
#include "./game.h"
#include "./player.h"

/*typedef struct game {
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
} game_t;*/


// initializes a game struct
game_t *game_new(char *map[], int rows, int cols, int MaxPlayers) {
    game_t *game = malloc(sizeof(game_t) + (rows + 1) * (cols + 1) * sizeof(char));
    game->rows = rows;
    game->cols = cols;
    game->goldcounts = malloc((rows + 1) * sizeof(int *));

    for (int i = 0; i < rows; i++){
        game->map[i] = map[i];
        game->goldcounts[i] = malloc((cols + 1) * sizeof(int));
        for(int j = 0; j<cols; j++){
            game->goldcounts[i][j] = 0;
        }
    }

    game->players = hashtable_new(MaxPlayers);
    //counters_t *goldcounts = gold_generator(game);

    return game;
}

void game_delete(game_t *game){
    for (int i = 0; i < game->rows; i++){
        free(game->map[i]);
    }
    //TODO: FREE OTHER STUFF
    free(game);
}

