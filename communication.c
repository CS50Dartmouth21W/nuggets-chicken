#include <stdlib.h>
#include <stdbool.h>
#include "string.h"
#include "./support/message.h"
#include "./player.h"
#include "./game.h"
#include "math.h"
#include "./libcs50/hashtable.h"
#include "communication.h"
#include "./visibility.c"

void quit(const addr_t addr, const char *reason){
    char quitMessage[sizeof(char)*strlen(reason) + 6]; 
    sprintf(quitMessage, "QUIT %s", reason);
    printf("%s\n", reason);  
    message_send(addr, quitMessage);
}

void quitGame(game_t *game, addr_t addr){
    addr_t *spectatorAddr = game->spectatorAddr;

    if(spectatorAddr != NULL && message_eqAddr(addr, *spectatorAddr)){
        // coming from a spectator
        game->spectatorAddr = NULL;
        quit(addr, "Thanks for Watching");
    }else{
        // coming from a player
        quit(addr, "Thanks for Playing");
        game->playersJoined--;
    }
}

void sendOK(player_t *player){
    char OkMessage[5];
    sprintf(OkMessage, "OK %c", (char)(player->id + 'A'));
    message_send(player->addr, OkMessage);
}


void sendGridInfo(game_t *game, addr_t addr){
    int rows = game->rows;
    int cols = game->cols;
    int numDigits = 6 + getNumDigits(rows) + getNumDigits(cols);
    
    char gridInfo[numDigits];
    // Note to self: check for buffer overflow
    sprintf(gridInfo, "GRID %d %d", rows, cols);
    message_send(addr, gridInfo);
}


// n is the amount of gold found
// if player == NULL and n = 0, then it is from a spectator
void sendGoldInfo(game_t *game, player_t *player, addr_t addr, int n){
    
    int p = player == NULL ? 0 : player->gold;
    int r = game->TotalGoldLeft;
    int numDigits = 7 + getNumDigits(n) + getNumDigits(p) + getNumDigits(r);
    
    char goldInfo[numDigits];
    sprintf(goldInfo, "GOLD %d %d %d", n, p, r);
    message_send(addr, goldInfo);
}

// if player == NULL, then it is from spectator
void sendDisplay(game_t *game, player_t *player, addr_t addr){
    int rows = game->rows;
    int cols = game->cols;
    
    char displayInfo[8 + (rows+1) * cols];
    strcpy(displayInfo, "DISPLAY\n");
    
    if (player != NULL){
        // player
        for(int i = 0; i<rows; i++){
            strcat(displayInfo, player->visibility[i]);
            strcat(displayInfo, "\n");
        }
    } else {
        // spectator
        for(int i = 0; i<rows; i++){
            strcat(displayInfo, game->map[i]);
            strcat(displayInfo, "\n");
        }
    }
    message_send(addr, displayInfo);
}

void sendGameOver(game_t *game, addr_t addr){
    player_t *sorted[game->MaxPlayers];

    hashtable_iterate(game->players, sorted, sort_players);
    
    char message[11 + game->MaxPlayers * 25];
    strcat(message, "GAME OVER:\n");
    
    for (int i = 0; i<game->MaxPlayers; i++){
        player_t *player = sorted[i];
        if(player == NULL){
            break;
        }

        char line[24];
        printf("%c %d %s\n", (char)('A' + i), player->gold, player->name);
        // Here are the values for the printing format
        // number can't be more than 10 chars
        // first 10 letters of real name
        sprintf(line, "%c %10d %10s\n", (char)('A' + i), player->gold, player->name); 
        printf("%s", line);
        strcat(message, line);
    }
    printf("%s\n", message);
    hashtable_iterate(game->players, message, broadcast);
    // send message to spectator
    
    if(game->spectatorAddr != NULL){
        quit(*(game->spectatorAddr), message);
    }
}

int getNumDigits(int a){
    return a == 0 ? 1 : (int)(ceil(log10(a))+1);
}

// struct used solely for searching
typedef struct htSearch {
    player_t *result;
    addr_t addr;
} htSearch_t;

player_t* getPlayerByAddr(game_t *game, const addr_t addr){
    htSearch_t *obj = malloc(sizeof(htSearch_t));
    obj->addr = addr;

    hashtable_iterate(game->players, obj, find_player);
    
    //free(obj->addr);
    player_t *result = obj->result;
    free(obj); 
    return result;
}

void find_player(void *arg, const char *key, void *item){
    htSearch_t *search = (htSearch_t *) arg;
    player_t *player = (player_t *) item;

    if(message_eqAddr(search->addr, player->addr)){
        search->result = player;
    }
}

void sort_players(void *arg, const char *key, void *item){
    player_t **sorted = (player_t **) arg;
    player_t *player = (player_t *) item;
    sorted[player->id] = player;
}

void broadcast(void *arg, const char *key, void *item){
    char* message = (char *) arg;
    player_t *player = (player_t *) item;    
    quit(player->addr, message);
}
