/* header file for communication.c
 *
 * Team: Nuggets, 21W
 *
 */

#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H

#include <stdlib.h>
#include <stdbool.h>
#include "string.h"
#include "./support/message.h"
#include "./player.h"
#include "./game.h"
#include "math.h"
#include "./libcs50/hashtable.h"

/*************************** quit ****************************/
/*
 * Build and send a string message to quit the game
 * Caller provides address of player or spectator and a string containing reason for quitting
 * Function returns nothing
 */
void quit(const addr_t addr, const char *reason);

/************************* quitGame **************************/
/*
 * Handle a player and spectator that quit the game
 * Caller provides pointer to the game and the address of the quitting player or spectator
 * Function returns nothing
 */
void quitGame(game_t *game, addr_t addr);

/************************** sendOK ***************************/
/*
 * Send okay message to player
 * Caller provides pointer to the player struct
 * Function returns nothing
 */
void sendOK(player_t *player);

/************************ sendGridInfo ***********************/
/*
 * Send grid message with rows and columns information
 * Caller provides pointer to game struct and address of player or spectator
 * Function returns nothing
 */
void sendGridInfo(game_t *game, addr_t addr);

/********************** sendGoldInfo *************************/
/*
 * Send gold maessage with amount of gold found
 * Caller provides the pointer to game struct, pointer to player, player address, and gold collected
 * If player is null, the player's purse is set to 0
 * Function returns nothing
 */
void sendGoldInfo(game_t *game, player_t *player, addr_t addr, int n);

/*********************** sendDisplay *************************/
/*
 * Send display message with map information
 * Caller provides pointer to game struct, pointer to player struct, address of player
 * Function returns nothing
 * 
 */
void sendDisplay(game_t *game, player_t *player, addr_t addr);

/*********************** sendGameOver ************************/
/*
 * Send game over message to all players and spectator
 * Caller provides address of game struct and address of player
 * Function returns nothing
 */
void sendGameOver(game_t *game, addr_t addr);

/************************ getNumDigits ***********************/
/*
 * Return the number of digits in the integer argument
 * Caller provides an int
 */
int getNumDigits(int a);

/********************* getPlayerByAddr ***********************/
/*
 * Return pointer player at given address
 * Caller provides pointer to game and address of player
 */
player_t *getPlayerByAddr(game_t *game, const addr_t addr);

/********************* getPlayerByChar ***********************/
/*
 * Return pointer to player with given character ID
 * Caller provides pointer to game and character of the player as displayed on map
 */
player_t *getPlayerByChar(game_t *game, char c);

/*********************** find_player *************************/
/*
 * Find player in game
 * Helper function used by hashtable_iterate
 * Caller passes in htSearch_t struct as void *arg
 */
void find_player(void *arg, const char *key, void *item);

/********************** find_player2 *************************/
/*
 * Return player with given player character name
 * Helper function used by hashtable_iterate
 * Caller passes in htSearch2_t struct as void *arg
 */
void find_player2(void *arg, const char *key, void *item);


/******************** create_message *************************/
/*
 * Create a message with player information
 * Helper function used by hashtable_iterate
 * Caller passes in pointer to char as void *arg
 */
void create_message(void *arg, const char *key, void *item);

/*********************** broadcast ***************************/
/*
 * Create broadcast message for a player quitting
 * Helper function used by hashtable_iterate
 * Caller passes in pointer to char as void *arg
 */
void broadcast(void *arg, const char *key, void *item);


#endif