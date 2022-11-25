#ifndef __TYPES_H__
#define __TYPES_H__


#include <ncurses.h>
#include "util.h"

/************************* GAME *************************/

/**
 * Game structure, handle game resources.
*/
typedef struct Game {
    bool quit;
    bool gameover;
    uint16_t win_w;
    uint16_t win_h;
    WINDOW *main_win;
    WINDOW *game_win;
    WINDOW *bar_win;
    WINDOW *stm_bar;
    WINDOW *fruit_win;
    WINDOW *help_win;
    Stack *path;
    time_t date;
} Game;


/************************* MAP *************************/

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


/************************* PLAYER *************************/

/**
 * Player movements.
*/
typedef enum Move {
    NONE = -1,
    DOWN,
    LEFT,
    UP,
    RIGHT
} Move;

/**
 * Player structure, handle player data.
*/
typedef struct Player {
    Position pos;
    Move move;
    //uint8_t nb_move;
    int8_t stamina;
    uint8_t fruit_stack;
    bool on_obstacle;
} Player;


#endif