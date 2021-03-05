#include <stdlib.h>
#include <stdbool.h>
#include "string.h"
#include "./support/message.h"
#include "./player.h"
#include "./game.h"
#include "math.h"

bool handleTimeout(void *arg);
bool handleInput  (void *arg);
bool handleMessage(void *arg, const addr_t from, const char *message);


bool move(player_t *player, int dx, int dy);
bool continuousMove(player_t *player, int dx, int dy);


void sendOK(player_t *player);
void sendGridInfo(player_t *player);
void sendGoldInfo(player_t *player, int n);
void sendDisplay(player_t *player);



bool handleTimeout(void *arg){
    return true;
}

bool handleInput (void *arg){
    printf("input: %p", arg);
    return true;
}

// TODO: update move to indclude the from
//       work out handleTimeout
//       work onn handleInput

bool handleMessage(void *arg, const addr_t from, const char *message){
    if(message == NULL){
        return true;
    }

    game_t *game = (game_t *) arg;

    printf("message:  %s\n", message);
    // parse message
    char *cmd = strtok(message, " ");
    char *messageArg = strtok(NULL, " ");

    printf("cmd:  %s\n", cmd);
    if(strcmp(cmd,"PLAY") == 0){
        // add anew player 
        printf("ADDING A NEW PLAYEr\n");
        player_t *player = player_new(messageArg, game, from);

        if(game->playersJoined + 1 <= game->MaxPlayers){
            hashtable_insert(game->players, messageArg, player);
            game->playersJoined++;
        }
        sendOK(player);
        sendGridInfo(player);
        sendGoldInfo(player, 0);
        sendDisplay(player); 

    } else if(strcmp(cmd, "KEY") == 0){

        //
        printf("PLAYER PRESSED KEY : %s", messageArg); 

         switch(messageArg[0]){
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
    } else {
        printf("INVALID COMMAND\n");
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
        player->game->TotalGoldLeft -= newGold;
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

void sendOK(player_t *player){
    char *OkMessage[5];
    sprintf(OkMessage, "OK %c", player->name[0]);
    message_send(player->addr, OkMessage);
}

void sendGridInfo(player_t *player){
    int rows = player->game->rows;
    int cols = player->game->cols;
    int numDigits = 6 + getNumDigits(rows) + getNumDigits(cols);
    
    char *gridInfo[numDigits];
    // Note to self: check for buffer overflow
    sprintf(gridInfo, "GRID %d %d", rows, cols);
    message_send(player->addr, gridInfo);
}

// n is the amount of gold found
void sendGoldInfo(player_t *player, int n){
    int p = player->gold;
    int r = player->game->TotalGoldLeft;
    
    int numDigits = 7 + getNumDigits(n) + getNumDigits(p) + getNumDigits(r);
    
    char *goldInfo[numDigits];
    sprintf(goldInfo, "GOLD %d %d %d", n, p, r);
    message_send(player->addr, goldInfo);
}

void sendDisplay(player_t *player){
    int rows = player->game->rows;
    int cols = player->game->cols;
    char *grid[(rows+1) * cols + 1];
    
    for(int i = 0; i<rows; i++){
        strcat(grid, player->game->map[i]);
        strcat(grid, "\n");
    }
    //grid[sf] = '\0';

    char *displayInfo[8 + (rows+1) * cols];

    sprintf(displayInfo, "DISPLAY\n%s", grid);
    message_send(player->addr, displayInfo);
}

int getNumDigits(int a){
    return (int)(ceil(log10(a))+1);
}
