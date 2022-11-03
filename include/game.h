#ifndef __GAME_H__
#define __GAME_H__

#include "common.h"
#include "player.h"
#include "map.h"

typedef struct {
    Map map;
    Player player;
    time_t date;
} Game;


#endif