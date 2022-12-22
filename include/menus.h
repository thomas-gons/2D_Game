#ifndef __MENUS_H__
#define __MENUS_H__


#include "common.h"
#include "structs.h"
#include "game.h"
#include "map.h"


/************************* FUNCTIONS *************************/

/**
 * Create and render a menu template with a list of entry.
 * \param entry_list list of string that represents menu entries
 * \param nb_entry number of menu entries
*/
void menu_create_entry_template(char **entry_list, int nb_entry);

/**
 * Input and render loop to select a menu entry.
 * \param entry_list list of string that represents menu entries
 * \param nb_entry number of menu entries
 * \returns index of selected menu entry
*/
int8_t menu_select_entry(char **entry_list, int nb_entry);

/**
 * Create and render Difficulty menu window.
 * \returns Index of start menu entry, or -1 to go back to previous menu
*/
int8_t menu_difficulty();

/**
 * Create and render Victory Title and menu windows.
*/
void menu_victory();

/**
 * Create and render Defeat menu window.
*/
void menu_defeat();

/**
 * Create and render in-game pause menu window.
*/
void menu_pause();

/**
 * Render distances for all four directions around player current position.
*/
void distances_render();

/**
 * Render whether the distance or the obstacle character.
 * \param line line of cell
 * \param col column of cell
 * \param render_dist distance value to render, or -1 to render the obstacle character
*/
void render_dist_obstacle(uint8_t line, uint8_t col, int8_t render_dist);


#endif