#include <stdlib.h>
#include <stdbool.h>
#include "./support/message.h"
#include "./player.h"
#include "./game.h"

bool handleTimeout(void *arg);
bool handleInput  (void *arg);
bool handleMessage(void *arg, const addr_t from, const char *message);


bool move(player_t *player, int dx, int dy);
bool continuousMove(player_t *player, int dx, int dy);

bool handleTimeout(void *arg){
    return true;
}

bool handleInput (void *arg){

    return true;
}

// TODO: update move to indclude the from
//       work out handleTimeout
//       work onn handleInput

bool handleMessage(void *arg, const addr_t from, const char *message){
    switch(message[0]){
        case 'Q':
            // quit game
            return true;//quit(arg);
        // singular move 
        case 'h':
            // move left
            return move((player_t *) arg, -1, 0); 
        case 'l':
            // move right
            return move((player_t *) arg, 1, 0); 
        case 'j':
            // move down
            return move((player_t *) arg, 0, -1); 
        case 'k':
            // move up
            return move((player_t *) arg, 0, 1); 
        case 'y':
            // move up and left
            return move((player_t *) arg, -1, 1);
        case 'u':
            // move up and right
            return move((player_t *) arg, 1, 1);
        case 'b':
            // move down and left
            return move((player_t *) arg, -1, -1);
        case 'n':
            // move down and right
            return move((player_t *) arg, 1, -1);

        // continuous movement
        case 'H':
            // move left
            return continuousMove((player_t *) arg, -1, 0);
        case 'L':
            // move right
            return continuousMove((player_t *) arg, 1, 0);
        case 'J':
            // move down
            return continuousMove((player_t *) arg, 0, -1);
        case 'K':
            // move up
            return continuousMove((player_t *) arg, 0, 1);
        case 'Y':
            // move up and left
            return continuousMove((player_t *) arg, -1, 1);
        case 'U':
            // move up and right
            return continuousMove((player_t *) arg, 1, 1);
        case 'B':
            // move down and left
            return continuousMove((player_t *) arg, -1, -1);
        case 'N':
            // move down and right
            return continuousMove((player_t *) arg, 1, -1);

    }
    return false;
}

/*
 * move will move the player to the right by dx
 * and to upwards by dy
 */
bool move(player_t *player, int dx, int dy){
    // check if is spectator or player

    int newrow = player->row + dy;
    int newcol = player->column + dx;

    if(newrow < 0 || newcol < 0 || newrow > player->game->rows || newcol > player->game->cols){
        // can't move there
        return false;
    }

    char c = player->game->map[newrow][newcol] == '.';

    if(c == '*' || c == '.'){
        player->row = newrow;
        player->column = newcol;
    } else {
        // can't move there
        return false;   
    }

    if(c == '*'){
        int newGold = player->game->goldcounts[newrow][newcol];
        player->gold += newGold; 
        player->game->goldcounts[newrow][newcol] = 0;
        player->game->totalGoldCollected += newGold;
    }

    return true;
}

bool continuousMove(player_t *player, int dx, int dy){
    if(move(player, dx, dy)){
        while(move(player, dx, dy)){}
        return true;
    }else{
        return false;
    }
}
