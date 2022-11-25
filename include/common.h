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
 * Key settings.
*/
#define KEY_ESC 27

/**
 * Ncurses windows sizes settings.
*/
#define MAP_LINES       40
#define MAP_COLS        80
#define BAR_SIZE        16
#define STM_BAR_SIZE    (MAP_LINES - (HELP_SIZE + STM_BAR_PAD_T + STM_BAR_PAD_B) + 2)
#define STM_BAR_PAD_T   2
#define STM_BAR_PAD_B   6
#define STM_BAR_PAD_L   5
#define HELP_SIZE       14

/**
 * Colors for ncurses character output.
*/
#define FORMAT_COLOR_EMPTY      1
#define FORMAT_COLOR_GREEN      2
#define FORMAT_COLOR_CYAN       3
#define FORMAT_COLOR_YELLOW     4
#define FORMAT_COLOR_RED        5
#define FORMAT_COLOR_MAGENTA    6

#endif