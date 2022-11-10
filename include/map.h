#ifndef __MAP_H__
#define __MAP_H__

#include "common.h"
#include "util.h"

#define MAP_LINES 40
#define MAP_COLS 80
#define BAR_SIZE 16
#define MENU_SIZE 16

#define CENTER_X MAP_LINES / 2
#define CENTER_Y MAP_COLS / 2

#define MAX_DISTANCE 32.526913

// The probabilties add up according to this order
#define PROB_FRUIT 0.015
#define PROB_OBS 0.2

// Max values for probabilties
#define PROB_MAX_FRUIT 0.02
#define PROB_MAX_OBS 0.35

/**
 * Lambda factors to adjust probabilties
 * Extend or restrict the scope of the maximum probability
*/
#define LAMBDA_FRUIT 0.8
#define LAMBDA_OBS 1.7

#define FORMAT_COLOR_FRUIT 1

#define IS_NOT_INDEX_ERROR(tmp_x, tmp_y) (              \
    (tmp_x >= 0 && tmp_x < MAP_LINES) &&                \
    (tmp_y >= 0 && tmp_y < MAP_COLS)) ? true: false     \

/**
 * 
*/
typedef enum Level {
    EASY = 1,
    MEDIUM,
    HARD
} Level ;

/**
 * 
*/
typedef enum Cell_type {
    EMPTY = -1,
    ROAD,
    FRUIT,
    NO_FRUIT,
    OBSTACLE,
    FLAG
} Cell_type;

/**
 * 
*/
typedef struct {
    Cell_type cell_type;
    bool visited;
} Cell;

/**
 * 
*/
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

/**
 * 
*/
void map_display_path_building(Stack *path);

/**
 * 
*/
Stack *search_path(unsigned heuristic[MAP_LINES][MAP_COLS], Position init, Position goal, int cost);

/**
 * 
*/
Stack *a_star();

#endif