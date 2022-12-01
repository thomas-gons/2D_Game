#ifndef __PLAYER_H__
#define __PLAYER_H__


#include "common.h"
#include "structs.h"
#include "util.h"
#include "game.h"
#include "map.h"


/************************* DEFINES *************************/

/**
 * Stamina settings.
*/
#define STAMINA_EASY    100
#define STAMINA_MEDIUM  70
#define STAMINA_HARD    50

#define STAMINA_COST        1
#define STAMINA_COST_OBS    10
#define STAMINA_GAIN        10

#define STAMINA_MAX         100
#define STAMINA_MIN         0
#define STAMINA_LIMIT_TO_STACK_FRUITS 80

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
 * Check if player is colliding with map borders or obstacles. Returns true whether not colliding or false otherwise.
 * \param line line of cell to be checked
 * \param column column of cell to be checked
*/
bool player_check_collisions(uint8_t line, uint8_t col);

/**
 * Alert player of collision with an obstacle.
 * \param line cell line of obstacle
 * \param column cell column of obstacle
*/
void player_obstacle_alert(uint8_t line, uint8_t col);

/**
 * Player stack fruit.
*/
void player_stack_fruit(uint8_t line, uint8_t col);

/**
 * Player eat stacked fruit.
*/
void player_eat_fruit();

/**
 * Render fruit stack and distance travelled by the player in terminal with ncurses
*/
void player_stats_render();

/**
 * Render player in terminal with ncurses.
*/
void player_render();

/**
 * Free allocated memory for player.
*/
void player_free();


#endif