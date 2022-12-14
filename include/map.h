#ifndef __MAP_H__
#define __MAP_H__


#include "common.h"
#include "structs.h"
#include "util.h"


/************************* DEFINES *************************/

/**
 * Distance values settings.
*/
#define CENTER_L (MAP_LINES / 2)
#define CENTER_C (MAP_COLS / 2)

#define MIN_DISTANCE 1
#define MAX_DISTANCE 10

/**
 * The probabilties add up according to this order.
*/
#define PROB_BONUS  0.03
#define PROB_OBS    0.18

/** 
 * Max values for probabilties.
*/
#define PROB_MAX_BONUS  0.05
#define PROB_MAX_OBS    0.25

/**
 * Lambda factors to adjust probabilties.
 * Extend or restrict the scope of the maximum probability.
*/
#define LAMBDA_BONUS    1.5
#define LAMBDA_OBS      2.2

/**
 * Cells settings.
*/
#define START ((Position) {.l=0, .c=0})
#define GOAL ((Position) {.l=MAP_LINES - 1, .c=MAP_COLS - 1})
#define MOVESET_LEN     4

/**
 * Tell if we consider the stamina to get the shortest path.
*/
#define CONSIDER_STM(with_stm, stm) ((with_stm) ? (stm > 0): (true))

/**
 * Check if position is within the map range.
*/
#define IS_NOT_OUT_OF_MAP(line, col) (                  \
    ((line) >= 0 && (line) < MAP_LINES) &&              \
    ((col) >= 0 && (col) < MAP_COLS)) ? true : false    \

/**
 * Check if position is an obstacle cell.
*/
#define IS_OBSTACLE_CELL(line, col) (                               \
    map->map_grid[line][col].cell_type == OBSTACLE) ? true : false  \

/**
 * Check if position is a bonus cell.
*/
#define IS_BONUS_CELL(line, col) (                               \
    map->map_grid[line][col].cell_type == BONUS) ? true : false  \


/************************* FUNCTIONS *************************/

/**
 * Initialize a map based on difficulty.
 * \param level game difficulty
*/
void map_init(Level level);

/**
 * Render map in terminal with ncurses.
*/
void map_render();

/**
 * Randomly fill the map with obstacles and BONUS.
*/
void map_random_fill();

/**
 * Generate the random map.
*/
Stack *map_generate();

/**
 * Display map with distances and the shortest path 
*/
void map_display_path(Stack *stack);

/**
 * Display map in terminal with stdout.
*/
void map_display();

/**
 * Display map in terminal with stdout, highlight the path and obstacles.
 * \param path valid path as a stack of cells
*/
void map_render_path(Stack *path, uint8_t color);

/**
 * Free allocated memory of map.
*/
void map_free();

/**
 * Search a valid path in the map.
 * \param heuristic matrix that stores distance between each cell and end cell
 * \param start start path's position
 * \param end end path's position
 * \param with_stm consider stamina in shortest path or not
 * \returns a valid path (stack of cells) on success, NULL on error.
*/
Stack *search_path(unsigned heuristic[MAP_LINES][MAP_COLS], Position start, Position end, bool with_stm);

/**
 * Implementation of A Star (A*) pathfinding algorithm.
 * \param start start path's position
 * \param end end path's position
 * \param with_stm consider stamina in shortest path or not
 * \returns a valid path (stack of cells) on success, NULL on error.
*/
Stack *a_star(Position start, Position end, bool with_stm);


#endif