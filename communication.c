/* communication - function to communicate with client
 *
 * All communication done wiht players and spectators. 
 * Responsible for networking, using message.h module
 *
 * Team: Nuggets
 *
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "./game.h"
#include "./player.h"
#include "./visibility.c"
#include "communication.h"
#include "./support/message.h"
#include "./libcs50/hashtable.h"

/*********** local function declarations *************/
static void broadcastDisplay(void *arg, const char *key, void *item);

/************************ getNumDigits ***********************/
/*
 * Return the number of digits in the integer argument
 * Usage: getNumDigits(rows)
 */
int getNumDigits(int a){
    return a == 0 ? 1 : (int)(ceil(log10(a)) + 1);
}

/*************************** quit ****************************/
/*
 * Build and send a string message to quit the game
 * See communication.h for detailed description.
 */
void quit(const addr_t addr, const char *reason) {
    // initialize message
    char quitMessage[sizeof(char) * strlen(reason) + 6]; 

    // concatenate QUIT and reason to message
    sprintf(quitMessage, "QUIT %s", reason);
    printf("%s\n", reason);

    message_send(addr, quitMessage);
}

/************************* quitGame **************************/
/*
 * Handle a player and spectator that quit the game
 * See communication.h for detailed description.
 */
void quitGame(game_t *game, addr_t addr) {
    addr_t *spectatorAddr = game->spectatorAddr;

    if(spectatorAddr != NULL && message_eqAddr(addr, *spectatorAddr)){
        // send quit message for a spectator
        quit(addr, "Thanks for Watching");

        // set the spectator address to null
        game->spectatorAddr = NULL;         
    } else {
        // send quit message for a player
        quit(addr, "Thanks for Playing");

        // decrease the current number of players in game
        game->playersJoined--;
    }
}

/************************** sendOK ***************************/
/*
 * Send okay message to player
 * See communication.h for detailed description.
 */
void sendOK(player_t *player){
    #define MESSAGESIZE 5
    char OkMessage[MESSAGESIZE];

    // concatenate OK and player ID to message
    sprintf(OkMessage, "OK %c", (char)(player->id + 'A'));

    message_send(player->addr, OkMessage);
}


/************************ sendGridInfo ***********************/
/*
 * Send grid maessage with rows and columns information
 * See communication.h for detailed description.
 */
void sendGridInfo(game_t *game, addr_t addr){
    int rows = game->rows;      // get number of rows from game
    int cols = game->cols;      // get number of columns from game

    // calculate the number of digits in rows and columns
    int numDigits = 6 + getNumDigits(rows) + getNumDigits(cols);
    char gridInfo[numDigits];   // create array for grid info

    // TODO: check for buffer overflow
    // concatenate GRID message with rows and columns
    sprintf(gridInfo, "GRID %d %d", rows, cols);

    message_send(addr, gridInfo);
}


// TODO: move to comm.h: n is the amount of gold found
// if player == NULL and n = 0, then it is from a spectator
/********************** sendGoldInfo *************************/
/*
 * Send gold maessage with amount of gold found
 * See communication.h for detailed description.
 */
void sendGoldInfo(game_t *game, player_t *player, addr_t addr, int n){
    // if player is null, set p to 0, else set p to player's gold value
    int p = (player == NULL) ? 0 : player->gold;
    int r = game->TotalGoldLeft;    // r holds the amount of gold left

    // calculate the number of digits in gold info
    int numDigits = 7 + getNumDigits(n) + getNumDigits(p) + getNumDigits(r);
    char goldInfo[numDigits];   // create array for gold info

    // concatenate GOLD message with 
    // gold n just collected, p purse, and r remaining gold in game
    sprintf(goldInfo, "GOLD %d %d %d", n, p, r);

    message_send(addr, goldInfo);
}

/*********************** sendDisplay *************************/
/*
 * Send display message with map information
 * See communication.h for detailed description.
 * TODO: loops don't differ between cases. Add more info or just keep 1 loop?
 */
void sendDisplay(game_t *game, addr_t addr){
    int rows = game->rows;      // get number of rows from game
    int cols = game->cols;      // get number of columns from game
    
    char displayInfo[8 + (rows+1) * cols];  // create array for display info message
    strcpy(displayInfo, "DISPLAY\n");       // copy DISPLAY to info message
    
    hashtable_iterate(game->players, NULL, broadcastDisplay);

    // spectator
    for(int i = 0; i < rows; i++) {
        // loop through rows, add to display info message with new line
        strcat(displayInfo, game->map[i]);
        strcat(displayInfo, "\n");
    }

    if (game->spectatorAddr != NULL){
        const addr_t addrr = *(game->spectatorAddr);
        message_send(addrr, displayInfo);
    }
}

/*********************** sendGameOver ************************/
/*
 * Send game over message
 * See communication.h for detailed description.
 * TODO: need for two messages?
 */
void sendGameOver(game_t *game, addr_t addr){
    #define MESSAGESIZE 700
    // allocate memory and concatenate GAME OVER message
    char *message = malloc(sizeof(char) * MESSAGESIZE);
    strcat(message, "GAME OVER:\n");
    char message2[MESSAGESIZE]; // create array for message 2

    hashtable_iterate(game->players, message, create_message);
    printf("%s\n", message);

    // copy message to message 2 and free message 
    strcpy(message2, message);  
    free(message);

    hashtable_iterate(game->players, message2, broadcast);
    
    // send message to spectator
    if (game->spectatorAddr != NULL){
        quit(*(game->spectatorAddr), message);
    }
}

/************************ htSearch ***************************/
/*
 * Struct used solely for searching, used by getPlayerAddr funct
 * See communication.h for detailed description.
 * TODO: move typedefs to top of file, or group together
 * TODO: combine struct with htSearch2?
 */
typedef struct htSearch {
    player_t *result;
    addr_t addr;
} htSearch_t;

typedef struct htSearch2 {
    player_t *result;
    char car;
} htSearch2_t;

/********************* getPlayerByAddr ***********************/
/*
 * Return player at given address
 * See communication.h for detailed description.
 */
player_t* getPlayerByAddr(game_t *game, const addr_t addr){
    // create obj to store given address
    htSearch_t *obj = malloc(sizeof(htSearch_t));
    obj->addr = addr;

    // iterate through game to find player at obj
    hashtable_iterate(game->players, obj, find_player);
    
    // store result and free obj
    player_t *result = obj->result;
    free(obj);

    return result;
}

/*********************** find_player *************************/
/*
 * Find player in game
 * See communication.h for detailed description.
 * TODO: update name convention to match other functions
 * TODO: add comments
 */
void find_player(void *arg, const char *key, void *item){
    htSearch_t *search = (htSearch_t *) arg;
    player_t *player = (player_t *) item;

    if (message_eqAddr(search->addr, player->addr)){
        search->result = player;
    }
}

/********************* getPlayerByChar ***********************/
/*
 * Return player with given character ID
 * See communication.h for detailed description.
 * TODO: remove comment
 */
player_t *getPlayerByChar(game_t *game, char c){
    // create obh to store given char
    htSearch2_t *obj = malloc(sizeof(htSearch2_t));
    obj->car = c;
    obj->result = NULL;

    hashtable_iterate(game->players, obj, find_player2);
    
    //free(obj->addr);
    // store result and free obj
    player_t *result = obj->result;
    free(obj); 

    return result;
}


/********************** find_player2 *************************/
/*
 * Return player with given player character name
 * See communication.h for detailed description.
 * TODO: change name and name format to match
 */
void find_player2(void *arg, const char *key, void *item){
    htSearch2_t *search = (htSearch2_t *) arg;

    // find and return player with matching ID
    if( search->car == ((player_t *) item)->letter){
        search->result = item;
    }
}

/******************** create_message *************************/
/*
 * Create a message with player information
 * See communication.h for detailed description.
 */
void create_message(void *arg, const char *key, void *item){
    player_t *player = (player_t *) item;
    char *message = (char *) arg;
    char line[26];      // array to store message

    // concatenate message with player letter and gold
    sprintf(line, "%c %10d %10s\n", player->letter, player->gold, key); 
    printf("%s", line);
    strcat(message, line);
}

/*********************** broadcast ***************************/
/*
 * Create broadcast message for a player quitting
 * See communication.h for detailed description.
 */
void broadcast(void *arg, const char *key, void *item){
    player_t *player = (player_t *) item;
    char* message = (char *) arg;

    quit(player->addr, message);
}

/*********************** broadcastDisplay ***************************/
/*
 * Create display message and send to players
 * See communication.h for detailed description.
 */
static void broadcastDisplay(void *arg, const char *key, void *item){

    player_t *player = (player_t *) item;
    int rows = player->game->rows;      // get number of rows and cols from game
    int cols = player->game->cols;      // get number of rows and cols from game

    char displayInfo[8 + (rows+1) * cols];  // create array for display info message
    strcpy(displayInfo, "DISPLAY\n");       // copy DISPLAY to info message

    updateVisibility(player);
    for (int i = 0; i < rows; i++) {
        // loop through rows, add to display info message with new line
        strcat(displayInfo, player->visibility[i]);
        strcat(displayInfo, "\n");
    }    
    message_send(player->addr, displayInfo);
}
