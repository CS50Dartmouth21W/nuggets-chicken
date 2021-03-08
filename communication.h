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


void quit(const addr_t addr, const char *reason);
void quitGame(game_t *game, addr_t addr);

void sendOK(player_t *player);

void sendGridInfo(game_t *game, addr_t addr);
void sendGoldInfo(game_t *game, player_t *player, addr_t addr, int n);
void sendDisplay(game_t *game, player_t *player, addr_t addr);
void sendGameOver(game_t *game, addr_t addr);

int getNumDigits(int a);

player_t *getPlayerByAddr(game_t *game, const addr_t addr);
player_t *getPlayerByChar(game_t *game, char c);

void find_player(void *arg, const char *key, void *item);
void find_player2(void *arg, const char *key, void *item);
void sort_players(void *arg, const char *key, void *item);
void create_message(void *arg, const char *key, void *item);
void broadcast(void *arg, const char *key, void *item);


#endif

