#ifndef __GAME_H__
#define __GAME_H__


#include "common.h"
#include "structs.h"
#include "map.h"
#include "player.h"
#include "stamina.h"


/************************* DEFINES *************************/

#define MAIN_WIN_L0 (game.win_h/2 - (MAP_LINES + 2)/2)
#define MAIN_WIN_C0 (game.win_w/2 - (MAP_COLS + BAR_SIZE + 2)/2)

#define GAME_WIN_L0 (game.win_h/2 - MAP_LINES/2)
#define GAME_WIN_C0 (game.win_w/2 - (MAP_COLS + BAR_SIZE)/2)

#define BAR_WIN_L0  (game.win_h/2 - MAP_LINES/2 - 1)
#define BAR_WIN_C0  (1 + game.win_w/2 + (MAP_COLS - BAR_SIZE)/2)

#define STM_BAR_L0  (game.win_h/2 - (MAP_LINES/2 -  STM_BAR_PAD_T + 1))
#define STM_BAR_C0  (1 + game.win_w/2 + (MAP_COLS - BAR_SIZE)/2 + STM_BAR_PAD_L)

#define HELP_WIN_L0 (1 + game.win_h/2 + MAP_LINES/2 - HELP_SIZE)
#define HELP_WIN_C0 (1 + game.win_w/2 + (MAP_COLS - BAR_SIZE)/2)


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
 * Free all resources.
*/
void game_free();

/**
 * Quit game.
*/
void game_quit();


#endif