#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "game.h"
#include <stdbool.h>

void updateVisibility(player_t *player);

void dfs(int row, int col, int pr, int pc, char** visibility, char** map, bool** visited);

bool isVisible(int row, int col, int pr, int pc, char** visibility, char** map, bool** visited);
// updates a player's visibility based off of a new point 
// (x,y) that it encouters. We use the game map to figure out the new visibility
//
void updateVisibility(player_t *player){
    int pr = player->row;
    int pc = player->col;
    game_t *game = player->game;
    char **visibility = player->visibility;
    char **map = game->map; 
    int rows = game->rows;
    int cols = game->cols;

    bool visited[rows][cols];


    dfs(pr, pc, pr, pc, visibility, map, visited); 
}

void dfs(int row, int col, int pr, int pc, char** visibility, char** map, bool** visited){

    visited[row][col] = true;

    for (int i = row-1; i <= row+1; i++){
        for(int j = col-1; j <= col+1; j++){
            
            if( !visited[i][j] && isVisible(i, j, pr, pc, visibility, map, visited)){
                dfs(i, j, pr, pc, visibility, map, visited);
            }
        }
    }
}

// makes assumption that there are a positive number of rows
// and columns iwthin a particular map
bool isVisible(int row, int col, int pr, int pc, char** visibility, char** map, bool** visited){

    int rows = sizeof(visited)/sizeof(visited[0]);
    int cols = sizeof(visited[0] )/sizeof(visited[0][0]);

    char c = map[row][col];
    if(row < 0 || col < 0 || row >= rows || col >= cols){ 
        return false;
    }

    float m = (pc - col)/(pr - row);
    for(int i = row; i <= pr; i++){
        int j = col + (m * i);
        if(map[i][j] != '.'){
            return false;
        }
    }
     

    return true;
}
