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
 * Open and read the save file
*/
void save_read_file();

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