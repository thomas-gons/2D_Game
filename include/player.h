#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "common.h"
#include "util.h"

/**
 * Player movements.
*/
typedef enum {
    DOWN,
    LEFT,
    UP,
    RIGHT
} move;

/**
 * Player structure.
*/
typedef struct {
    Position pos;
    uint8_t stamina;
    bool is_obstacle;
    char *inputs;
} Player;




#endif