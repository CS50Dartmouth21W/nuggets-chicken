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

#endif
