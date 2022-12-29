#ifndef __SAVE_H__
#define __SAVE_H__


#include "common.h"
#include "structs.h"
#include "util.h"
#include "map.h"
#include "player.h"


/************************* FUNCTIONS *************************/

/**
 * Create the name of the save file
*/
void save_get_date();

/**
 * Open and write inside the save file
*/
void save_write_file();

/**
 * Write the map structure in the binary file
 * \param f the open binary file
*/
void save_write_map(FILE *f);

/**
 * Write the player structure in the binary file
 * \param f the open binary file
*/
void save_write_player(FILE *f);

/**
 * Write the game structure in the binary file
 * \param f the open binary file
*/
void save_write_game(FILE *f);

/**
 * Write a stack saved in a binary file
 * \param stack stack to write in a binary file
 * \param f the open binary file
*/
void save_write_stack(Stack *stack, FILE *f);

/**
 * Open and read the save file
*/
void save_read_file();

/**
 * Read the map structure in the binary file
 * \param f the open binary file
*/
void save_read_map(FILE *f);

/**
 * Read the player structure in the binary file
 * \param f the open binary file
*/
void save_read_player(FILE *f);

/**
 * Read the game structure in the binary file
 * \param f the open binary file
*/
void save_read_game(FILE *f);

/**
 * Read a stack saved in a binary file
 * \param stack new empty stack that we will complete 
 * \param f the open binary file
 * \param player_stack indication whether it is the stack of a player's path or not
*/
void save_read_stack(Stack *stack, FILE *f, bool player_stack);

/**
 * Run the saving
*/
void save_game();

/**
 * Data recovery from backup
 */
void save_charging_game();


#endif