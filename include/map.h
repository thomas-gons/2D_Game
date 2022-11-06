#ifndef __MAP_H__
#define __MAP_H__

#include "common.h"

#define MAP_SIZE 45
#define BAR_SIZE 16
#define MENU_SIZE 18

/// Cell state
#define OPEN 0
#define BLOCK 1

#define CENTER_X 23
#define CENTER_Y 45

#define MAX_DISTANCE 32.526913

// The probabilties add up according to this order
#define PROB_FRUIT 0.01
#define PROB_OBS 0.2

// thresholds for 
#define PROB_MAX_FRUIT 0.06
#define PROB_MAX_OBS 0.2

// lambda factors to adjust probabilties
#define LAMBDA_FRUIT 0.8
#define LAMBDA_OBS 0.9

#define OBS_SYMBOL "\U0001faa8"
#define FRUIT_SYMBOL "\u2605"

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
    Level level;
    uint16_t step;
    Cell **map_grid;
} Map;


/**
 * 
*/
void map_init(Level level);

/**
 * 
*/
void map_render(WINDOW *win);

/**
 * 
*/
void map_free();

/**
 * 
*/
void map_random_fill();




/**
 * 
*/
void map_display();

#endif