#ifndef __COMMON_H__
#define __COMMON_H__


#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>
#include <ncurses.h>
#include <locale.h>
#include <wchar.h>
#include <unistd.h>


/************************* DEFINES *************************/

/**
 * Ncurses windows sizes settings.
*/
#define MAP_LINES 40
#define MAP_COLS 80
#define BAR_SIZE 16
#define STM_BAR_SIZE (MAP_LINES - (HELP_SIZE + STM_BAR_PAD_T + STM_BAR_PAD_B) + 2)
#define STM_BAR_PAD_T 2
#define STM_BAR_PAD_B 6
#define STM_BAR_PAD_L 5
#define HELP_SIZE 16

/**
 * Colors for ncurses map output.
*/
#define FORMAT_COLOR_PATH 1
#define FORMAT_COLOR_OBS 2
#define FORMAT_COLOR_FRUIT 3
#define FORMAT_COLOR_PLAYER 4

/**
 * Colors for ncurses stamina bar output.
*/
#define FORMAT_COLOR_STM_HIGH 5
#define FORMAT_COLOR_STM_MED 6
#define FORMAT_COLOR_STM_LOW 7
#define FORMAT_COLOR_EMPTY 8


#endif