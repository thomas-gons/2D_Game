#ifndef __MAP_H__
#define __MAP_H__

#include "common.h"
#include "util.h"

#define MAP_SIZE 45
#define BAR_SIZE 16
#define MENU_SIZE 18

/// Cell state
#define OPEN 0
#define BLOCK 1

#define CENTER_X 45
#define CENTER_Y 23

#define MAX_DISTANCE 32.526913

// The probabilties add up according to this order
#define PROB_FRUIT 0.015
#define PROB_OBS 0.2

// thresholds for probabilties
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




void path_finder();
int recursiv_research1(uint8_t dim,Stack *path, Position coord,uint8_t verif_path);
int recursiv_research2(uint8_t dim, Stack *path, Position coord,uint8_t verif_path);

#endif