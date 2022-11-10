#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "common.h"
#include "util.h"

/**
 * 
*/
typedef struct {
    Position pos;
    uint8_t stamina;
    bool is_obstacle;
} Player;

#endif