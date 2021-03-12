/*
 * visibility.h - header file for visibility module
 * 
 * Team Chicken 21W
 * 
 */


#ifndef __VISIBILITY_H
#define __VISIBILITY_H
#include "player.h"

/**************** updateVisibility ****************/
/* 
 * Updates the visibility map of a player struct
 * based on current location and status of main game map
 */
void updateVisibility(player_t *player);

#endif
