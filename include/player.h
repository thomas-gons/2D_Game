#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "common.h"


typedef struct {
    uint8_t x, y;
} Pos;

typedef struct Node {
    Pos position;
    struct Node *prev;
    struct Node *next;
} Node, *Db_list;

typedef struct {
    Pos position;
    uint8_t stamina;
    bool is_obstacle;
} *Player;

#endif