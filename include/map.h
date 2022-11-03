#ifndef __MAP_H__
#define __MAP_H__

#include "common.h"

typedef struct {
    enum {
        WALL,
        EMPTY,
        FRUIT,
        NO_FRUIT,
        OBSTACLE,
        FLAG
    } Cell_type;
    bool visited;
} Cell;

typedef struct {
    uint8_t dim;
    unsigned step;
    Cell *map_grid;
} *Map;

#endif