#ifndef __MAP_H__
#define __MAP_H__


#include "common.h"
#include "util.h"

#define MAP_LINES 40
#define MAP_COLS 80
#define BAR_SIZE 16
#define MENU_SIZE 16

/**
 * Ncurses windows sizes settings.
*/
#define MAP_LINES 40
#define MAP_COLS 80
#define BAR_SIZE 16
#define MENU_SIZE 16

#define CENTER_X MAP_LINES / 2
#define CENTER_Y MAP_COLS / 2

#define MAX_DISTANCE 32.526913

/**
 * The probabilties add up according to this order.
*/
#define PROB_FRUIT 0.015
#define PROB_OBS 0.2

/** 
 * Max values for probabilties.
*/
#define PROB_MAX_FRUIT 0.02
#define PROB_MAX_OBS 0.35

/**
 * Lambda factors to adjust probabilties.
 * Extend or restrict the scope of the maximum probability.
*/
#define LAMBDA_FRUIT 0.8
#define LAMBDA_OBS 1.7

/**
 * Cells settings.
*/
#define START ((Position) {0, 0})
#define END ((Position) {MAP_LINES - 1, MAP_COLS - 1})
#define MOVESET_LEN 4

/**
 * Check if position is within the map range.
*/
#define IS_NOT_INDEX_ERROR(tmp_x, tmp_y) (              \
    (tmp_x >= 0 && tmp_x < MAP_LINES) &&                \
    (tmp_y >= 0 && tmp_y < MAP_COLS)) ? true: false     \


/**
 * Game difficulty.
*/
typedef enum Level {
    EASY = 1,
    MEDIUM,
    HARD
} Level ;

/**
 * All types of cells.
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
 * Cell structure.
*/
typedef struct Cell {
    Cell_type cell_type;
    bool visited;
} Cell;

/**
 * Map structure, matrix of Cell structures.
*/
typedef struct Map {
    Level level;
    uint16_t step;
    Cell **map_grid;
} Map;


/**
 * Initialize a map based on difficulty.
 * \param level game difficulty
*/
void map_init(Level level);

/**
 * Render map in terminal with ncurses.
 * \param win ncurses window
*/
void map_render(WINDOW *win);

/**
 * Randomly fill the map with obstacles and fruits.
*/
void map_random_fill();

/**
 * Generate the random map.
*/
void map_generate();

/**
 * Display map in terminal with stdout.
*/
void map_display();

/**
 * Display map in terminal with stdout, highlight the path and obstacles.
 * \param path valid path as a stack of cells
*/
void map_display_path_building(Stack *path);

/**
 * Free allocated memory of map.
*/
void map_free();

/**
 * Search a valid path in the map.
 * \param heuristic matrix that stores distance between each cell and end cell
 * \param cost movement cost to reach another direct adjacent cell (moveset)
 * \returns a valid path (stack of cells) on success, NULL on error.
*/
Stack *search_path(unsigned heuristic[MAP_LINES][MAP_COLS], int cost);

/**
 * Implementation of A Star (A*) pathfinding algorithm.
 * \returns a valid path (stack of cells) on success, NULL on error.
*/
Stack *a_star();

#endif