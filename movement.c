#include <stdlib.h>
#include <stdbool.h>
#include "string.h"
#include "./support/message.h"
#include "./player.h"
#include "./game.h"
#include "math.h"
#include "./libcs50/hashtable.h"
#include "communication.h"

bool handleTimeout(void *arg);
bool handleInput  (void *arg);
bool handleMessage(void *arg, const addr_t from, const char *message);
bool move(player_t *player, int dx, int dy);
bool continuousMove(player_t *player, int dx, int dy);

bool handleTimeout(void *arg){
    return true;
}

bool handleInput (void *arg){
    printf("input: %p", arg);
    return true;
}

// TODO: update move to indclude the from
//       work out handleTimeout
//       work on handleInput
bool handleMessage(void *arg, const addr_t from, const char *message){
    if(message == NULL){
        return true;
    }

    game_t *game = (game_t *) arg;

    // parse message
    char *cmd = strtok(message, " ");
    char *messageArg = strtok(NULL, " ");

    if(strcmp(cmd,"PLAY") == 0){
        // add a new player 
        player_t *player = player_new(messageArg, game, from);

        if(game->playersJoined + 1 <= game->MaxPlayers){
            if(strcmp(messageArg, "") == 0){
                quit(from, "Sorry - you must provide player's name.");
            }else{
                hashtable_insert(game->players, messageArg, player);
                game->playersJoined++;

                sendOK(player);
                sendGridInfo(game, &from);
                sendGoldInfo(game, player, &from, 0);
                sendDisplay(game, player, &from);
            }
        } else {
            quit(from, "Game is full: no more players can join.");
        }
    } else if(strcmp(cmd, "SPECTATE") == 0){
        // add a spectator
        addSpectator(game, &from);
        sendGridInfo(game, &from);
        sendGoldInfo(game, NULL, &from, 0);

        sendDisplay(game, NULL, &from);
    } else if(strcmp(cmd, "KEY") == 0){
        player_t *player = getPlayerByAddr(game, from);
        
        if(strcmp(messageArg,"") == 0){
            return false;
        }

        switch(messageArg[0]){
        case 'Q':
            // quit game
            quitGame(game, from);
            return false;

        // singular move 
        case 'h':
            // move left
            move(player, -1, 0);
            break;
        case 'l':
            // move right
            move(player, 1, 0); 
            break;
        case 'j':
            // move down
            move(player, 0, -1); 
            break;
        case 'k':
            // move up
            move(player, 0, 1); 
            break;
        case 'y':
            // move up and left
            move(player, -1, 1);
            break;
        case 'u':
            // move up and right
             move(player, 1, 1);
            break;
        case 'b':
            // move down and left
            move(player, -1, -1);
            break;
        case 'n':
            // move down and right
            move(player, 1, -1);
            break;

        // continuous movement
        case 'H':
            // move left
            continuousMove(player, -1, 0);
            break;
        case 'L':
            // move right
            continuousMove(player, 1, 0);
            break;
        case 'J':
            // move down
            continuousMove(player, 0, -1);
            break;
        case 'K':
            // move up
            continuousMove(player, 0, 1);
            break;
        case 'Y':
            // move up and left
            continuousMove(player, -1, 1);
            break;
        case 'U':
            // move up and right
            continuousMove(player, 1, 1);
            break;
        case 'B':
            // move down and left
            continuousMove(player, -1, -1);
            break;
        case 'N':
            // move down and right
            continuousMove(player, 1, -1);
            break;
         }

         sendDisplay(game, player, &from);
         if(game->TotalGoldLeft == 0){
            sendGameOver(player->addr, game);
            return true;
         }

    } else {
        printf("INVALID COMMAND\n");
    }
    
    return false;
}

/*
 * move will move the player to the right by dx
 * and to upwards by dy
 * 
 * it will return false when it can't move to a psot
 */
bool move(player_t *player, int dx, int dy){
    // TODO: check if is spectator or player
    game_t *game = player->game;
    int newrow = player->row - dy;
    int newcol = player->col + dx;

    char c = game->map[newrow][newcol];
    printf("new row: %d\n", newrow);
    printf("new col: %d\n", newcol);
    printf("CHARACTER IS: %c\n", c);
    
    if((newrow >= 0 || newcol >= 0 || newrow < game->rows || newcol < game->cols)
        && c != ' ' && c != '-' && c != '|' && c != '+'){
        
        // check whether player is in room or not before replacing character
        if (player->inRoom == 1) {
            game->map[player->row][player->col] = '.';
        } else {
            game->map[player->row][player->col] = '#';
        }

        // update whether player's new location will be in room or hallway
        if (game->map[newrow][newcol] == '.') {
            player->inRoom = 1;
        } else if (game->map[newrow][newcol] == '#'){
            player->inRoom = 0;
        } else if (game->map[newrow][newcol] == '*'){
            player->inRoom = 1;
        } else {
            player_t *player2 = getPlayerByChar(game, c);
            int store = player->inRoom;
            player->inRoom = player2->inRoom;
            player2->inRoom = store;
            game->map[player->row][player->col] = c;
            player2->row = player->row;
            player2->col = player->col;
        }

        game->map[newrow][newcol] = player->letter;
        player->row = newrow;
        player->col = newcol;

        if(c == '*'){
            int newGold = game->goldcounts[newrow][newcol];
            player->gold += newGold; 
            game->goldcounts[newrow][newcol] = 0;
            game->TotalGoldLeft -= newGold;
            sendGoldInfo(game, player, &(player->addr), newGold);
        }

        return true;
    } else{
        return false;
    }
}

bool continuousMove(player_t *player, int dx, int dy){
    if(move(player, dx, dy)){
        while(move(player, dx, dy)){}
        return true;
    }else{
        return false;
    }
}
