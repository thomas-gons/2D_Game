#ifndef __GAME_H__
#define __GAME_H__


#include "common.h"
#include "structs.h"
#include "menus.h"
#include "map.h"
#include "player.h"
#include "stamina.h"
#include "save.h"


/************************* DEFINES *************************/

#define TITLE_WIN_L0 (game.win_h/2 - (MAP_LINES + 2)/2 + 6)
#define TITLE_WIN_C0 (game.win_w/2 - (MAP_COLS + BAR_SIZE + 2)/2 + 13)

#define MAIN_WIN_L0 0
#define MAIN_WIN_C0 (game.win_w/2 - (MAP_COLS + BAR_SIZE + 2)/2)

#define GAME_WIN_L0 1
#define GAME_WIN_C0 (game.win_w/2 - (MAP_COLS + BAR_SIZE)/2)

#define BAR_WIN_L0  0
#define BAR_WIN_C0  (game.win_w/2 + (MAP_COLS - BAR_SIZE)/2)

#define STM_BAR_L0  (STM_BAR_PAD_T)
#define STM_BAR_C0  (game.win_w/2 + (MAP_COLS - BAR_SIZE)/2 + STM_BAR_PAD_L + 1)

#define DIST_WIN_L0 (MAP_LINES - DIST_SIZE + 2)
#define DIST_WIN_C0 (game.win_w/2 + (MAP_COLS - BAR_SIZE)/2)

#define ALERT_WIN_L0 MAP_LINES + 1
#define ALERT_WIN_C0 MAIN_WIN_C0


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
 * Check if the terminal size is large enough for game rendering.
*/
void ncs_check_term_size();

/**
 * Create a ncurses window to render the game title.
*/
void ncs_create_title_window();

/**
 * Create a ncurses window to render the Victory title.
*/
void ncs_create_victory_window();

/**
 * Create all ncurses game windows.
*/
void ncs_create_game_windows();

/**
 * Refresh ncurses game windows.
*/
void ncs_refresh_game_windows();

/**
 * Print a centered text in an ncurses window.
 * \param win ncurses window
 * \param line line to print the message
 * \param msg message to print
*/
void ncs_print_centered(WINDOW *win, uint8_t line, const char *msg);

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
 * Create and render Title and Start menu windows.
 * \returns Index of start menu entry, or -1 to go back to previous menu
*/
int8_t game_start_menu();

/**
 * Create and render Difficulty menu window.
 * \returns Index of start menu entry, or -1 to go back to previous menu
*/
int8_t game_difficulty_menu(); 

/**
 * Initialize in-game resources.
 * \param difficulty game difficulty
*/
void game_init_new_game(Level difficulty);

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
 * Check if game is win.
*/
void game_check_win();

/**
 * Create and render Victory Title and menu windows.
*/
void game_victory_menu();

/**
 * Create and render Defeat menu window.
*/
void game_defeat_menu();

/**
 * Create and render in-game pause menu window.
*/
void game_pause_menu();

/**
 * Restart game to original state, keep same same, reset player and enemies.
*/
void game_restart();

/**
 * Free all game resources.
*/
void game_free();

/**
 * Quit game.
*/
void game_quit();


#endif