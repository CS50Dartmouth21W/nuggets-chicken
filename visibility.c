/*
 * visibility.c - nuggets game module for updating player map visibility
 * 
 * Team Chicken 21W
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "game.h"
#include <stdbool.h>
#include "math.h"

/**************** global function declarations ****************/
void updateVisibility(player_t *player);

/**************** local function declarations ****************/
static void dfs(int r, int c, int pr, int pc, player_t *player, bool** visited);
static bool isVisible(int r, int c, int pr, int pc, player_t *player);
static bool isValid(int r, int c, int rows, int cols, bool** visited);
static int min(int a, int b);
static int max(int a, int b);

// updates a player's visibility based off of a new point 
// (x,y) that it encouters. We use the game map to figure out the new visibility
void updateVisibility(player_t *player){
    int pr = player->row;
    int pc = player->col;
    
    game_t *game = player->game;
    int rows = game->rows;
    int cols = game->cols;
    
    // map to track visited points. Set all slots to false initially
    bool **visited = malloc(sizeof(bool *) * rows);
    for(int i = 0; i<rows; i++){
        visited[i] = malloc(sizeof(bool) * cols);
        for(int j = 0; j<cols; j++){
            visited[i][j] = false;
        }
    }
    
    // call dfs on the player's location
    dfs(pr, pc, pr, pc, player, visited);

    // free visited
    for (int i = 0; i<rows; i++){
        free(visited[i]);
    }
    free(visited);

    // mark spot on the visibillity map
    player->visibility[pr][pc] = '@'; 
}


// recursive function that uses depth first search to find all visible points
static void dfs(int r, int c, int pr, int pc, player_t *player, bool** visited){

    game_t *game = player->game;
    visited[r][c] = true;

    // loop through all neighboring points
    for (int i = r-1; i <= r+1; i++){
        for(int j = c-1; j <= c+1; j++){
            // if the point is in bounds and visible, 
            // update the visibility map and call recursively
            if( isValid(i, j, game->rows, game->cols, visited) && 
                isVisible(i, j, pr, pc, player)){

                player->visibility[i][j] = game->map[i][j];
                dfs(i, j, pr, pc, player, visited);
            }
        }
    }
}

// Helper function to check if a point is visible.
// makes assumption that there are a positive number of rows and columns 
// within a particular map
static bool isVisible(int r, int c, int pr, int pc, player_t *player){

    char **visibility = player->visibility;
    char **map = player->game->map; 
    int rows = player->game->rows;
    int cols = player->game->cols;
    
    if(pr == r){    // point on the same row
        for (int j = min(pc, c) + 1; j < max(pc, c); j++){
            if(map[r][j] != '.' && map[r][j] != '*') return false;
        }
    } else if(pc == c){ // point on same column
        for(int i = min(pr, r) + 1; i<max(pr, r); i++){
            if(map[i][c] != '.' && map[i][c] != '*') return false;
        }
    } else{

        //slope of line between point and player
        float m = (float)(pc - c)/(float)(pr - r);  

        int j1 = ((min(pr, r) == pr) ? pc : c);
        int j2 = ((min(pr, r) == pr) ? pc : c);
        if (m > 1) j1++;

        // loop through rows between the player and point
        for(int i = min(pr, r) + 1; i < max(pr, r); i++){

            j1 += floor(m);
            j2 += ceil(m);
  
            // check point is in bounds
            if (j2 < cols && j1 < cols && j1 >= 0 && j2 >= 0) {
                char ch1 = map[i][j1];
                char ch2 = map[i][j2];
                // return false if the two characters are not a room spot or gold
                if ((ch1 != '.' && ch1 != '*') && (ch2 != '.' && ch2 != '*')) {
                    return false;
                }
            } else {
                return false;
            }
        }

        // repeat for the case of steeper lines
        m = (float)(pr - r) / (float)(pc - c);
        
        j1 = ((min(pc, c) == pc) ? pr : r);
        j2 = ((min(pc, c) == pc) ? pr : r);
        
        // loop through column between the player and point
        for(int i = min(pc, c) + 1; i < max(pc, c); i++){

            // add the slope to the row (two versions, floor and ceiling)
            j1 += floor(m);
            j2 += ceil(m);
            if (min(pc, c) == c) {
                if (j2 > pr && m < 1) j2 = pr;
            }

            // calculate the new "point" to evaluate
            if (min(pc, c) == pc) {
                if (m>0) {
                    if (j2 > (r-1)) j2 = (r-1);
                } else {
                    if (j2 < (r+1)) j2 = (r+1);
                }
            }
            // check point is in bounds
            if (j2 < rows && j1 < rows && j1 >= 0 && j2 >= 0) {
                char ch1 = map[j1][i];
                char ch2 = map[j2][i];
                // return false if char is not a room spot or gold
                if ((ch1 != '.' && ch1 != '*') && (ch2 != '.' && ch2 != '*')){
                    return false; 
                }
            } else {
                return false;
            }
        }
    }

    return true;
}

// returns the minimum of two numbers
static int min(int a, int b){
    return a <= b ? a : b;
}

// returns the maximum of two numbers
static int max(int a, int b){
    return a >= b ? a : b;       
}

// checks if the point is within the bounds of the array
static bool isValid(int r, int c, int rows, int cols, bool** visited){
    return (r >= 0 && c >= 0 && r < rows && c < cols && !visited[r][c]);
}
