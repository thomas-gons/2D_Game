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
 * Ncurses windows sizes settings.
*/
#define MAP_LINES 40
#define MAP_COLS 80
#define BAR_SIZE 16
#define MENU_SIZE 16

/**
 * Key settings.
*/
#define KEY_ENTR 10
#define KEY_ESC 27

/**
 * Colors for ncurses output.
*/
#define FORMAT_COLOR_PATH 1
#define FORMAT_COLOR_OBS 2
#define FORMAT_COLOR_FRUIT 3
#define FORMAT_COLOR_PLAYER 4


#endif