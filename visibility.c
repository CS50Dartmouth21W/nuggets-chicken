#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "game.h"
#include <stdbool.h>
#include "math.h"

void updateVisibility(player_t *player);
void dfs(int r, int c, int pr, int pc, char** visibility, char** map, bool** visited, int rows, int cols);
bool isVisible(int r, int c, int pr, int pc, char** visibility, char** map);
bool isValid(int r, int c, int rows, int cols, bool** visited);

int min(int a, int b);
int max(int a, int b);

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
    
    bool **visited = malloc(sizeof(bool *) * rows);
    for(int i = 0; i<rows; i++){
        visited[i] = malloc(sizeof(bool) * cols);
        for(int j = 0; j<cols; j++){
            visited[i][j] = false;
        }
    }
    
    dfs(pr, pc, pr, pc, visibility, map, visited, rows, cols);
    visibility[pr][pc] = player->letter; 
}

void dfs(int r, int c, int pr, int pc, char** visibility, char** map, bool** visited, int rows, int cols){
    visited[r][c] = true;

    for (int i = r-1; i <= r+1; i++){
        for(int j = c-1; j <= c+1; j++){
            
            if( isValid(i, j, rows, cols, visited) && isVisible(i, j, pr, pc, visibility, map)){
                visibility[i][j] = map[i][j];
                dfs(i, j, pr, pc, visibility, map, visited, rows, cols);
            }
        }
    }
}

// makes assumption that there are a positive number of rows
// and columns iwthin a particular map
bool isVisible(int r, int c, int pr, int pc, char** visibility, char** map){
    //char letter = map[r][c];
    
    if(pr == r){
        for (int j = min(pc, c) + 1; j < max(pc, c); j++){
            if(map[r][j] != '.' && map[r][j] != '*') return false;
        }
    } else if(pc == c){
        for(int i = min(pr, r) + 1; i<max(pr, r); i++){
            if(map[i][c] != '.' && map[i][c] != '*') return false;
        }
    }
    
    else{

        float m = (pc - c)/(pr - r);
        //int j = ((min(pr, r) == pr) ? pc : c);// + m;

        int j1 = ((min(pr, r) == pr) ? pc : c);// + m;
        int j2 = ((min(pr, r) == pr) ? pc : c);// + m;
        for(int i = min(pr, r) + 1; i < max(pr, r); i++){

            //j += m;
            //char ch = map[i][j];
            j1 += floor(m);
            j2 += ceil(m);
            /*if(ch != '.' && ch != '*'){
                return false;
            }*/
            char ch1 = map[i][j1];
            char ch2 = map[i][j2];
            if ((ch1 != '.' && ch1 != '*') && (ch2 != '.' && ch2 != '*')) return false;
        }

        m = (pr - r)/(pc - c);
        //j = ((min(pc, c) == pc) ? pr : r); //+ m;
        
        j1 = ((min(pc, c) == pc) ? pr : r);// + m;
        j2 = ((min(pc, c) == pc) ? pr : r);// + m;
        
        for(int i = min(pc, c) + 1; i < max(pc, c); i++){

            /*j += m;//ceil(m);
            char ch = map[j][i];
            
            if(ch != '.' && ch != '*'){
                return false;
            }*/

            j1 += floor(m);
            j2 += ceil(m);
            char ch1 = map[j1][i];
            char ch2 = map[j2][i];
            if ((ch1 != '.' && ch1 != '*') && (ch2 != '.' && ch2 != '*')) return false;
        }
    }

    return true;
}

int min(int a, int b){
    return a <= b ? a : b;
}

int max(int a, int b){
    return a >= b ? a : b;       
}

bool isValid(int r, int c, int rows, int cols, bool** visited){
    return (r >= 0 && c >= 0 && r < rows && c < cols && !visited[r][c]);
}
