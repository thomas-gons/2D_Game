#ifndef __GAME_H__
#define __GAME_H__

#include "common.h"
#include "player.h"
#include "map.h"


/**
 * Game structure to handle game resources.
*/
typedef struct {
    bool quit;
    uint16_t win_x;
    uint16_t win_y;
    WINDOW *main_win;
    WINDOW *game_win;
    WINDOW *bar_win;
    WINDOW *help_win;
    time_t date;
} Game;


/**
 * Initialize ncurses library. 
*/
void ncs_init();

/**
 * Check the terminal size and quit if to small.
*/
void ncs_check_term_size();

/**
 * Create all sub windows.
*/
void ncs_create_windows();

/**
 * Quit ncurses library.
*/
void ncs_quit();

/**
 * Main game loop.
*/
void game_loop();

/**
 * Initialize all game resources.
*/
void game_init();

/**
 * Handle game inputs.
*/
void game_inputs();

/**
 * Update current game state based on inputs.
*/
void game_update();

/**
 * Render current game state.
*/
void game_render();

/**
 * Quit game and free all resources.
*/
void game_quit();


#endif