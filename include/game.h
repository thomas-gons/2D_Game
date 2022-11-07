#ifndef __GAME_H__
#define __GAME_H__

#include "common.h"
#include "player.h"
#include "map.h"

typedef struct {
    uint16_t win_x;
    uint16_t win_y;
    WINDOW *main_win;
    WINDOW *game_win;
    WINDOW *bar_win;
    WINDOW *help_win;
    time_t date;
} Game;


/**
 * 
*/
void ncs_init();

/**
 * 
*/
void ncs_check_term_size();

/**
 * 
*/
void ncs_create_windows();

/**
 * 
*/
void ncs_quit();

#endif