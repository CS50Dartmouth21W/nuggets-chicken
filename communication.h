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
void quit(const addr_t addr, const char *reason);
player_t *getPlayerByAddr(game_t *game, const addr_t addr);
player_t *getPlayerByChar(game_t *game, char c);


#endif
