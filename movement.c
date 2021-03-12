/*
 * movement.c - nuggets game module for handling messages.
 * 
 * Defines functions required by message_loop() as well
 * as helper functions to process messages, like move()
 * 
 * Team Chicken 21W
 * 
 */

#include <stdlib.h>
#include <stdbool.h>
#include "string.h"
#include "./support/message.h"
#include "./player.h"
#include "./game.h"
#include "./libcs50/hashtable.h"
#include "communication.h"
#include "visibility.h"

/**************** global function declarations ****************/
bool handleTimeout(void *arg);
bool handleInput  (void *arg);
bool handleMessage(void *arg, const addr_t from, const char *message);
bool move(player_t *player, int dx, int dy);

/**************** local function declarations ****************/
static void find_player3(void *arg, const char *key, void *item);
static int nameconflict(game_t *game, char* name);

// dummy input used by message_loop
bool handleTimeout(void *arg){
    printf("Timeout.\n");
    return true;
}

// dummy input used by message_loop
bool handleInput (void *arg){
    printf("Input: %s\n", (char *) arg);
    return true;
}

/*
 * HandlMessage handle a message const char *message from const addr_t from
 *
 * This function will parse the message into a command and argument
 * If the command is PLAY, a new player is added
 * If the command is SPECTATE, a new spectator is added
 * If the command is KEY, then a player is either moved or quits
 *
 */
bool handleMessage(void *arg, const addr_t from, const char *message){
    if(message == NULL){
        return true;
    }

    game_t *game = (game_t *) arg;

    // parse message
    char *cmd = strtok(message, " ");
    char *messageArg = strtok(NULL, " ");

    // create a new player struct if first word is "PLAY"
    if(strcmp(cmd,"PLAY") == 0){
        // ADD A NEW PLAYER
        if(game->playersJoined + 1 <= game->MaxPlayers){
            if(nameconflict(game, messageArg) != 0){
                quit(from, "Sorry - you must provide player's name.");
            }else{
                player_t *player = player_new(messageArg, game, from);
                // add to hashtable of players
                hashtable_insert(game->players, messageArg, player);   
                game->playersJoined++;

                // send messages
                sendOK(player);
                sendGridInfo(game, from);
                sendGoldInfo(game, player, from, 0);
                updateVisibility(player);
                sendDisplay(game, from);
            }
        } else {
            // send quit message if game is full
            quit(from, "Game is full: no more players can join.");  
        }

    } else if(strcmp(cmd, "SPECTATE") == 0){
        // ADD A SPECTATOR
        addSpectator(game, from);
        printf("spectator address is from: %p\n", from);
        sendGridInfo(game, from);
        sendGoldInfo(game, NULL, from, 0);
        sendDisplay(game, from);

    } else if(strcmp(cmd, "KEY") == 0){
        // MOVE A PLAYER OR QUIT
        player_t *player = getPlayerByAddr(game, from);
        
        if(strcmp(messageArg,"") == 0){
            return false;
        }

        switch(messageArg[0]){
            // quit game, if nobody left, game over
            case 'Q': quitGame(game, from); return game->playersJoined == 0;

            // singular move 
            case 'h': move(player, -1,  0); break; // move left
            case 'l': move(player,  1,  0); break; // move right
            case 'j': move(player,  0, -1); break; // move down
            case 'k': move(player,  0,  1); break; // move up
            case 'y': move(player, -1,  1); break; // move up and left
            case 'u': move(player,  1,  1); break; // move up and right
            case 'b': move(player, -1, -1); break; // move down and left
            case 'n': move(player,  1, -1); break; // move down and right

            // continuous movement
            case 'H': while(move(player, -1,  0)); break; // move left
            case 'L': while(move(player,  1,  0)); break; // move right
            case 'J': while(move(player,  0, -1)); break; // move down
            case 'K': while(move(player,  0,  1)); break; // move up
            case 'Y': while(move(player, -1,  1)); break; // move up and left
            case 'U': while(move(player,  1,  1)); break; // move up and right
            case 'B': while(move(player, -1, -1)); break; // move down and left
            case 'N': while(move(player,  1, -1)); break; // move down and right
        
            // key is invalid
            default: printf("INVALID KEY: %c\n", messageArg[0]);
        }

        if(game->TotalGoldLeft == 0){
            sendGameOver(game, player->addr);
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
    game_t *game = player->game;
    int newrow = player->row - dy;
    int newcol = player->col + dx;

    
    if(newrow >= 0 && newcol >= 0 && newrow < game->rows && newcol < game->cols){

        char c = game->map[newrow][newcol];

        if(c != ' ' && c != '-' && c != '|' && c != '+'){
        
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
                // switch location with another player
                player_t *player2 = getPlayerByChar(game, c);
                int store = player->inRoom;
                player->inRoom = player2->inRoom;
                player2->inRoom = store;
                game->map[player->row][player->col] = c;
                player2->row = player->row;
                player2->col = player->col;
            }

            // update the location of the player
            game->map[newrow][newcol] = player->letter;
            player->row = newrow;
            player->col = newcol;

            // update status when player collects gold
            if(c == '*'){
                int newGold = game->goldcounts[newrow][newcol];
                player->gold += newGold; 
                game->goldcounts[newrow][newcol] = 0;
                game->TotalGoldLeft -= newGold;
                sendGoldInfo(game, player, player->addr, newGold);
            }

            sendDisplay(game, player->addr);
            // if (game->spectatorAddr != NULL) {
            //     sendDisplay(game, *(game->spectatorAddr));
            // }
            return true;
        } 
    }

    return false;
}

// struct holding name and result
typedef struct htSearch3 {
    int result;
    char* name;
} htSearch3_t;

// returns whether there is a name conflict
static int nameconflict(game_t *game, char* name){
    htSearch3_t *obj = malloc(sizeof(htSearch3_t));
    obj->name = name;
    obj->result = 0;

    // search through the hashtable of players for name conflicts
    hashtable_iterate(game->players, obj, find_player3);    
    
    //free(obj->addr);
    int result = obj->result;
    free(obj);
    return result;
}

// helper method to compare player names
static void find_player3(void *arg, const char *key, void *item){
    htSearch3_t *search = (htSearch3_t *) arg;
    if(strcmp(search->name, key) == 0){
        search->result++;
    }
}
