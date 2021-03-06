#include <stdlib.h>
#include <stdbool.h>
#include "string.h"
#include "./support/message.h"
#include "./player.h"
#include "./game.h"
#include "math.h"
#include "./libcs50/hashtable.h"

bool handleTimeout(void *arg);
bool handleInput  (void *arg);
bool handleMessage(void *arg, const addr_t from, const char *message);


bool move(player_t *player, int dx, int dy);
bool continuousMove(player_t *player, int dx, int dy);


void sendOK(player_t *player);
void sendGridInfo(player_t *player);
void sendGoldInfo(player_t *player, int n);
void sendDisplay(player_t *player);

player_t *getPlayerByAddr(game_t *game, const addr_t addr);
static void find_player(void *arg, const char *key, void *item);
static void sort_players(void *arg, const char *key, void *item);
static void broadcast(void *arg, const char *key, void *item);

void quit(const addr_t addr, const char *reason){
    char quitMessage[sizeof(char)*strlen(reason) + 6]; 
    sprintf(quitMessage, "QUIT %s", reason);
    printf("%s\n", reason);  
    message_send(addr, quitMessage);
}

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

    printf("message:  %s\n", message);
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
                sendGridInfo(player);
                sendGoldInfo(player, 0);
                sendDisplay(player); 
            }
        } else {
            quit(from, "Game is full: no more players can join.");
        }

    } else if(strcmp(cmd, "KEY") == 0){

        player_t *player = getPlayerByAddr(game, from);
        
        if(strcmp(messageArg,"") == 0){
            return false;
        }

        switch(messageArg[0]){
        case 'Q':
            // quit game
            quit(from, "Thanks for Playing");
            return true;

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

         sendDisplay(player);
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
    printf("\n%s\n",player->name);
    game_t *game = player->game;
    int newrow = player->row - dy;
    int newcol = player->col + dx;

    char c = game->map[newrow][newcol];
    printf("new row: %d\n", newrow);
    printf("new col: %d\n", newcol);
    printf("CHARACTER IS: %c\n", c);
    printf("\n");
    
    if((newrow >= 0 || newcol >= 0 || newrow < game->rows || newcol < game->cols)
        && (c == '*' || c == '.' || c == '#')){

        game->map[player->row][player->col] = '.';
        game->map[newrow][newcol] = '@';
        player->row = newrow;
        player->col = newcol;

        if(c == '*'){
            int newGold = game->goldcounts[newrow][newcol];
            player->gold += newGold; 
            game->goldcounts[newrow][newcol] = 0;
            game->TotalGoldLeft -= newGold;
            sendGoldInfo(player, newGold);
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

void sendOK(player_t *player){
    char *OkMessage[5];
    sprintf(OkMessage, "OK %c", (char)(player->id + 'A'));
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
    game_t *game = player->game;
    int rows = game->rows;
    int cols = game->cols;
    char *displayInfo[8 + (rows+1) * cols];
    strcpy(displayInfo, "DISPLAY\n");

    for(int i = 0; i<rows; i++){
        strcat(displayInfo, game->map[i]);
        strcat(displayInfo, "\n");
    }

    message_send(player->addr, displayInfo);
}

void sendGameOver(addr_t addr, game_t *game){
    player_t *sorted[game->MaxPlayers];

    hashtable_iterate(game->players, sorted, sort_players);
    
    char* message[11 + game->MaxPlayers * 24];
    strcat(message, "GAME OVER:\n");
    
    for (int i = 0; i<game->MaxPlayers; i++){
        player_t *player = sorted[i];
        if(player == NULL){
            break;
        }

        char *line[24];
        // Here are the values for the printing format
        // number can't be more than 10 chars
        // first 10 letters of real name
        sprintf(line, "%c %10d %10s\n", (char)('A' + i), player->gold, player->name); 
        strcat(message, line);
    }
    printf("%s\n", message);
    hashtable_iterate(game->players, message, broadcast);
}

int getNumDigits(int a){
    return a == 0 ? 1 : (int)(ceil(log10(a))+1);
}

// struct used solely for searching
typedef struct htSearch {
    player_t *result;
    addr_t addr;
} htSearch_t;

player_t *getPlayerByAddr(game_t *game, const addr_t addr){
    htSearch_t *obj = malloc(sizeof(htSearch_t));
    obj->addr = addr;
    obj->result = NULL;

    hashtable_iterate(game->players, obj, find_player);
    
    //free(obj->addr);
    player_t *result = obj->result;
    free(obj); 
    return result;
}

static void find_player(void *arg, const char *key, void *item){
    htSearch_t *search = (htSearch_t *) arg;
    if(message_eqAddr(search->addr, ((player_t *) item)->addr)){
        search->result = item;
    }
}

static void sort_players(void *arg, const char *key, void *item){
    player_t **sorted = (player_t *) arg;
    player_t *player = (player_t *) item;
    sorted[player->id] = player;
}

static void broadcast(void *arg, const char *key, void *item){
    char* message = (char *) arg;
    player_t *player = (player_t *) item;    
    quit(player->addr, message);
}
