#ifndef __COMMON_H__
#define __COMMON_H__


#include <stdio.h>
#include <stdlib.h>
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
#define KEY_ENTR    10
#define KEY_ESC     27
#define KEY_SPACE   32

/**
 * Ncurses windows sizes settings.
*/
#define MAP_LINES       40
#define MAP_COLS        80
#define BAR_SIZE        18
#define STM_BAR_SIZE  (MAP_LINES - (HELP_SIZE + STM_BAR_PAD_T + STM_BAR_PAD_B) + 2)
#define STM_BAR_WIDTH   7
#define STM_BAR_PAD_T   2
#define STM_BAR_PAD_B   6
#define STM_BAR_PAD_L   ((BAR_SIZE - STM_BAR_WIDTH) / 2)
#define HELP_SIZE       13

/**
 * Colors for ncurses character output.
*/
#define FORMAT_COLOR_EMPTY      1
#define FORMAT_COLOR_WHITE      2
#define FORMAT_COLOR_GREEN      3
#define FORMAT_COLOR_CYAN       4
#define FORMAT_COLOR_YELLOW     5
#define FORMAT_COLOR_RED        6
#define FORMAT_COLOR_MAGENTA    7

#define FORMAT_BGCOLOR_GREEN    8
#define FORMAT_BGCOLOR_YELLOW   9
#define FORMAT_BGCOLOR_RED      10


#endif