#ifndef __GAME_H__
#define __GAME_H__


#include "common.h"
#include "structs.h"
#include "map.h"
#include "player.h"
#include "stamina.h"


/************************* FUNCTIONS *************************/

/**
 * Initialize ncurses library. 
*/
void ncs_init();

/**
 * Initialize ncurses colors. 
*/
void ncs_init_colors();

/**
 * Check the terminal size and quit if to small.
*/
void ncs_check_term_size();

/**
 * Create all ncurses windows.
*/
void ncs_create_windows();

/**
 * Refresh ncurses windows render.
*/
void ncs_refresh_windows();

/**
 * Quit ncurses library.
*/
void ncs_quit();

/**
 * Main application loop.
*/
void main_loop();

/**
 * Game loop that handle the game logic.
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