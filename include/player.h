#ifndef __PLAYER_H__
#define __PLAYER_H__


#include "common.h"
#include "structs.h"
#include "util.h"
#include "game.h"
#include "menu.h"
#include "map.h"


/************************* DEFINES *************************/

/**
 * Stamina settings.
*/
#define STAMINA_EASY    100
#define STAMINA_MEDIUM  80
#define STAMINA_HARD    60

#define STAMINA_COST            1
#define STAMINA_COST_OBS        10
#define STAMINA_GAIN            10
#define STAMINA_GAIN_ENM_DEFEAT 25

#define STAMINA_MAX             100
#define STAMINA_MIN             0
#define STAMINA_LIMIT_TO_STACK_BONUS 80

/**
 * Bonus stack settings
*/
#define BONUS_STACK_MIN 0
#define BONUS_STACK_MAX 2

#define ENEMY_SPEED         2
#define ENEMY_NB            1
#define ENEMY_INIT_DIST     (sqrt(pow((MAP_LINES / 2), 2) + pow((MAP_COLS / 2), 2)))


/************************* FUNCTIONS *************************/

/**
 * Initialize the player.
*/
void player_init();

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
 * \param col column of cell to be checked
 * \returns true on success, false on error.
*/
bool player_is_colliding(uint8_t line, uint8_t col);

/**
 * Alert player of collision with an obstacle.
 * \param line line of obstacle
 * \param col column of obstacle
*/
void player_obstacle_alert(uint8_t line, uint8_t col);

/**
 * Alert player of cell already visited.
 * \param line line of cell
 * \param col column of cell
*/
void player_visited_cell_alert(uint8_t line, uint8_t col);

/**
 * Player stack bonus.
*/
void player_stack_bonus(uint8_t line, uint8_t col);

/**
 * Player use stacked bonus.
*/
void player_use_bonus();

/**
 * Player use rewind.
*/
void player_rewind();

/**
 * Substract distance of player when rewind.
 * \param curr current node in history
 * \param prev previous node in history
*/
void player_substract_dist(SNode *curr, SNode *prev);

/**
 * Render bonus stack and distance travelled by the player in terminal with ncurses
*/
void player_stats_render();

/**
 * Render an alert message.
 * \param alert_msg alert message to be rendered
*/
void player_alert_render(const char *__restrict __fmt, ...);

/**
 * Render player in terminal with ncurses.
*/
void player_render();

/**
 * Free allocated memory for player.
*/
void player_free();

/**
 * Initialize enemy entities.
*/
void enemy_init();

/**
 * Update enemy to chase the player.
*/
void enemy_chase_player();

/**
 *  Render all enemies entity.
*/
void enemy_render();


#endif