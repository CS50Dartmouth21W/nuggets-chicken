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


void sendOK(player_t *player);

void sendGridInfo(game_t *game, addr_t *addr);
void sendGoldInfo(game_t *game, player_t *player, addr_t *addr, int n);
void sendDisplay(game_t *game, player_t *player, addr_t *addr);

player_t *getPlayerByAddr(game_t *game, const addr_t addr);
static void find_player(void *arg, const char *key, void *item);
static void sort_players(void *arg, const char *key, void *item);
static void broadcast(void *arg, const char *key, void *item);

#endif
