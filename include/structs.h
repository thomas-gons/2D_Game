#ifndef __STRUCTS_H__
#define __STRUCTS_H__


#include <ncurses.h>
#include "util.h"


/************************* GAME *************************/

/**
 * Game structure, handle game resources.
*/
typedef struct Game {
    bool gameover;
    uint16_t win_w;
    uint16_t win_h;
    WINDOW *title_win;
    WINDOW *menu_win;
    WINDOW *main_win;
    WINDOW *game_win;
    WINDOW *bar_win;
    WINDOW *stm_bar;
    WINDOW *stats_win;
    WINDOW *dist_win;
    WINDOW *alert_win;
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
} Level;

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
    uint8_t distance[2];
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
    NO_MOVE = -1,
    RIGHT,
    DOWN,
    LEFT,
    UP,
    UP_LEFT,
    UP_RIGHT,
    DOWN_LEFT,
    DOWN_RIGHT
} Move;

/**
 * Player structure, handle player data.
*/
typedef struct Player {
    Position pos;
    Action action;
    Move move;
    int8_t stamina;
    int8_t fruit_stack;
    uint16_t distance;
    bool anim_action;
    Stack *history;
} Player;


#endif