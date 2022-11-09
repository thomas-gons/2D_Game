#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "common.h"
#include "util.h"


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
 * Player structure.
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
*/
void player_init();

/**
 * Handle inputs for playing using ncurses library.
 * \param quit game quit indicator
*/
void player_inputs(bool *quit);

/**
 * Update player position based on inputs.
*/
void player_update();

/**
 * Render player on window.
 * \param win ncurses window
*/
void player_render(WINDOW *win);

/**
 * Free allocated memory for player.
*/
void player_free();


#endif