#ifndef __MENUS_H__
#define __MENUS_H__


#include "common.h"
#include "structs.h"
#include "map.h"


/************************* DEFINES *************************/

/**
 * Render whether the distance or the obstacle.
*/
#define RENDER_DIST_OBSTACLE(line, col, value, horz) {                                                   \
    if ((value) == 'X') {                                                                                \
        wattron(game.dist_win, A_BOLD);                                                                  \
        mvwaddch(game.dist_win, (line), (col) + (horz), (char)(value) | COLOR_PAIR(FORMAT_COLOR_RED));   \
        wattroff(game.dist_win, A_BOLD);                                                                 \
    } else {                                                                                             \
        mvwprintw(game.dist_win, (line), (col), "%hhd", (value));                                        \
    }                                                                                                    \
}


/************************* FUNCTIONS *************************/

/**
 * Create template for menu with a list of entry.
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
 * Render distances for all four directions around player current position.
*/
void distances_render();


#endif