#ifndef __MAP_H__
#define __MAP_H__


#include "common.h"
#include "util.h"


#define MAP_SIZE 40
#define BAR_SIZE 16
#define MENU_SIZE 16

/// Cell state
#define OPEN 0
#define BLOCK 1

#define CENTER_X 20
#define CENTER_Y 35

#define MAX_DISTANCE 32.526913

// The probabilties add up according to this order
#define PROB_FRUIT 0.015
#define PROB_OBS 0.1

// thresholds for probabilties
#define PROB_MAX_FRUIT 0.02
#define PROB_MAX_OBS 0.25

// lambda factors to adjust probabilties
#define LAMBDA_FRUIT 0.8
#define LAMBDA_OBS 2.0

#define OBS_SYMBOL "\U0001faa8"
#define FRUIT_SYMBOL "@"

#define FORMAT_COLOR_FRUIT 1

#define IS_NOT_INDEX_ERROR(tmp_x, tmp_y) (                  \
    (tmp_x >= 0 && tmp_x < MAP_SIZE) &&                     \
    (tmp_y >= 0 && tmp_y < MAP_SIZE)) ? true: false         \

#define IS_NOT_PAST_POS(tmp_x, tmp_y, prev_pos) (           \
    (tmp_x != prev_pos.x) &&                                \
    (tmp_y != prev_pos.y)) ? true: false                    \


typedef enum Level {
    EASY = 1,
    MEDIUM,
    HARD
} Level;

typedef enum Cell_type {
    EMPTY = -1,
    ROAD,
    FRUIT,
    NO_FRUIT,
    OBSTACLE,
    FLAG
} Cell_type;

typedef struct Cell {
    Cell_type cell_type;
    uint8_t state: 1;
    bool visited;
} Cell;

typedef struct Map{
    Level level;
    uint16_t step;
    Cell **map_grid;
} Map;


/**
 * Initialize a map by memory allocation.
*/
void map_init(Level level);

/**
 * Render the map on window.
 * \param win ncurses window
*/
void map_render(WINDOW *win);

/**
 * Free alloacted memory from map.
*/
void map_free();

/**
 * Randomly fill the map with obstacles and fruits.
*/
void map_random_fill();




/**
 * 
*/
void map_display();

Position map_get_random_obstacle(Stack *all_opened_cell);
bool rec_research(Stack *path, Position coord, bool check_path);

Stack *path_finder();

#endif