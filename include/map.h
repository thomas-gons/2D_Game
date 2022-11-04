#ifndef __MAP_H__
#define __MAP_H__

#include "common.h"


typedef struct {
    enum {
        EMPTY,
        WALL,
        FRUIT,
        NO_FRUIT,
        OBSTACLE,
        FLAG
    } cell_type;
    bool visited;
} Cell;

typedef struct {
    uint8_t dim;
    uint16_t step;
    Cell **map_grid;
} Map;


void map_init(uint8_t dim);
void map_free();
void map_display();

#endif