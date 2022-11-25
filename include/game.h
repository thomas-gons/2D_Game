#ifndef __GAME_H__
#define __GAME_H__


#include "common.h"
#include "structs.h"
#include "menu.h"
#include "map.h"
#include "player.h"


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
 * Create a ncurses window to render the game title.
*/
void ncs_create_title_window();

/**
 * Create all ncurses game windows.
*/
void ncs_create_game_windows();

/**
 * Refresh ncurses game windows.
*/
void ncs_refresh_game_windows();

/**
 * Clear and destroy a ncurses window.
 * \param win ncurses window
*/
void ncs_destroy_win(WINDOW *win);

/**
 * Quit ncurses library.
*/
void ncs_quit();

/**
 * Run the game.
*/
void run_game();

/**
 * Initialize all game resources.
*/
void game_init();

/**
 * Title window and start menu window.
 * \returns Index of start menu entry
*/
uint8_t game_start_menu();

/**
 * Initialize in-game resources.
*/
void game_init_new_game();

/**
 * Game loop.
*/
void game_loop();

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
 * Free all game resources.
*/
void game_free();

/**
 * Quit game.
*/
void game_quit();


#endif