#ifndef __MAP_H__
#define __MAP_H__

#include "common.h"

#define MAP_SMALL 10
#define MAP_MEDIUM 25
#define MAP_LARGE 50

// Cell state
#define OPEN 0
#define BLOCK 1

// The probabilties add up according to this order
#define PROB_FRUIT 0.03
#define PROB_OBS 0.2

#define OBS_SYMBOL "\U0001faa8 "
#define FRUIT_SYMBOL "\u2605 "

typedef enum {
    EASY = 1,
    MEDIUM,
    HARD
} Level;

typedef enum {
    EMPTY = -1,
    ROAD,
    FRUIT,
    NO_FRUIT,
    OBSTACLE,
    FLAG
} Cell_type;

typedef struct {
    Cell_type cell_type;
    uint8_t state: 1;
    bool visited;
} Cell;

typedef struct {
    uint8_t dim;
    Level level;
    uint16_t step;
    Cell **map_grid;
} Map;


void map_init(uint8_t dim, Level level);
void map_random_fill();
void map_free();
void map_display();

#endif