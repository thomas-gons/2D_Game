#ifndef __STAMINA_H__
#define __STAMINA_H__


#include "common.h"

#define STAMINA_HIGH 66.66
#define STAMINA_MED 33.33
#define STAMINA_LOW 0


/**
 * 
*/
void stamina_init(WINDOW *bar_win);

/**
 * 
*/
void stamina_update(WINDOW *bar_win);

/**
 * 
*/
void stamina_bar_render(WINDOW *stm_bar);

#endif