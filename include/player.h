#ifndef __PLAYER_H__
#define __PLAYER_H__


#include "common.h"
#include "structs.h"
#include "util.h"
#include "map.h"


/************************* DEFINES *************************/

/**
 * Stamina settings.
*/
#define STAMINA_EASY    100
#define STAMINA_MEDIUM  70
#define STAMINA_HARD    50

#define STAMINA_LOSS    1
#define STAMINA_LOSS_OBS 10
#define STAMINA_GAIN    10

#define STAMINA_MAX     100
#define STAMINA_MIN     0

/**
 * Fruit stack settings
*/
#define FRUIT_STACK_MIN 0
#define FRUIT_STACK_MAX 2


/************************* FUNCTIONS *************************/

/**
 * Initialize the player.
 * \param level difficulty of game
*/
void player_init(Level level);

/**
 * Handle inputs using ncurses library.
*/
void player_inputs();

/**
 * Update player position based on inputs.
*/
void player_update();

/**
 * Check if player is colliding with map borders or obstacles.
 * \param line line of cell to be checked
 * \param column column of cell to be checked
*/
void player_check_collisions(uint8_t line, uint8_t col);

/**
 * player stack fruit
*/
void player_stack_fruit(uint8_t line, uint8_t col);

/**
 * player stack fruit
*/
void player_eat_fruit();

/**
 * Render player in terminal with ncurses.
*/
void player_render();

/**
 * Free allocated memory for player.
*/
void player_free();


#endif