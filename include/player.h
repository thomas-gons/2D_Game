#ifndef __PLAYER_H__
#define __PLAYER_H__


#include "common.h"
#include "util.h"
#include "map.h"


/**
 * Key settings.
*/
#define KEY_ESC 27

/**
 * Stamina values at game start.
*/
#define STAMINA_EASY 100
#define STAMINA_MEDIUM 70
#define STAMINA_HARD 50


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
    uint8_t nb_move;
    uint8_t stamina;
    bool is_obstacle;
} Player;


/**
 * Initialize the player.
 * \param level difficulty of game
*/
void player_init(Level level);

/**
 * Handle inputs using ncurses library.
 * \param quit game quit indicator
*/
void player_inputs(bool *quit);

/**
 * Update player position based on inputs.
*/
void player_update();

/**
 * Render player in terminal with ncurses.
 * \param win ncurses window
*/
void player_render(WINDOW *win);

/**
 * Free allocated memory for player.
*/
void player_free();

#endif