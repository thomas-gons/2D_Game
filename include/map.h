#ifndef __MAP_H__
#define __MAP_H__


#include "common.h"
#include "util.h"


/**
 * Ncurses windows sizes settings.
*/
#define MAP_LINES 40
#define MAP_COLS 80
#define BAR_SIZE 16
#define MENU_SIZE 16

#define CENTER_L (MAP_LINES / 2)
#define CENTER_C (MAP_COLS / 2)

#define MAX_DISTANCE sqrt(pow(MAP_LINES, 2) + pow(MAP_COLS, 2))

/**
 * The probabilties add up according to this order.
*/
#define PROB_FRUIT 0.03
#define PROB_OBS 0.175

/** 
 * Max values for probabilties.
*/
#define PROB_MAX_FRUIT 0.06
#define PROB_MAX_OBS 0.25

/**
 * Lambda factors to adjust probabilties.
 * Extend or restrict the scope of the maximum probability.
*/
#define LAMBDA_FRUIT 2
#define LAMBDA_OBS 2.2

/**
 * Cells settings.
*/
#define START ((Position) {.l=0, .c=0})
#define END ((Position) {.l=MAP_LINES - 1, .c=MAP_COLS - 1})
#define MOVESET_LEN 4

/**
 * Path settings.
*/
#define PATH_LEN (MAX_DISTANCE * 2)

/**
 * Check if position is within the map range.
*/
#define IS_OUT_OF_MAP(line, col) (             \
    (line >= 0 && line < MAP_LINES) &&              \
    (col >= 0 && col < MAP_COLS)) ? true : false    \

/**
 * Check if position is an obstacle cell.
*/
#define IS_OBSTACLE_CELL(line, col) (                               \
    map->map_grid[line][col].cell_type == OBSTACLE) ? true : false  \


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
    Cell **map_grid;
} Map;


/**
 * Initialize a map based on difficulty.
 * \param level game difficulty
*/
void map_init(Level level);

/**
 * Render map in terminal with ncurses.
 * \param game_win ncurses game window
*/
void map_render(WINDOW *game_win);

/**
 * Randomly fill the map with obstacles and fruits.
*/
void map_random_fill();

/**
 * Generate the random map.
*/
Stack *map_generate();

/**
 * Display map in terminal with stdout.
*/
void map_display();

/**
 * Display map in terminal with stdout, highlight the path and obstacles.
 * \param game_win ncurses game window
 * \param path valid path as a stack of cells
*/
void map_render_path(WINDOW *game_win, Stack *path);

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