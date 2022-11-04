#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "common.h"
#include "utils.h"


typedef struct {
    Pos position;
    uint8_t stamina;
    bool is_obstacle;
} *Player;

#endif