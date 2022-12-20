#ifndef __SAVE_H__
#define __SAVE_H__


#include "common.h"
#include "structs.h"
#include "util.h"
#include "map.h"
#include "player.h"


/************************* DEFINES *************************/

/**
 * 
*/


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
*/
void save_write_map(FILE *f);

/**
 * Write the player structure in the binary file
*/
void save_write_player(FILE *f);

/**
 * Write the game structure in the binary file
*/
void save_write_game(FILE *f);

/**
 * Open and read the save file
*/
void save_read_file();

/**
 * Read the map structure in the binary file
*/
void save_read_map(FILE *f);

/**
 * Read the player structure in the binary file
*/
void save_read_player(FILE *f);

/**
 * Read the game structure in the binary file
*/
void save_read_game(FILE *f);

/**
 * Free the file variables at the end of the saving
*/
void save_free();

/**
 * Run the saving
*/
void save_game();

/**
 * Data recovery from backup
 */
void save_charging_game();


#endif