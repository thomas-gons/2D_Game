#ifndef __SAVE_H__
#define __SAVE_H__


#include "common.h"
#include "structs.h"
#include "util.h"
#include "map.h"
#include "player.h"


/************************* DEFINES *************************/

/**
 * Return length of array.
*/
#define LENGTH(arr) (sizeof (arr) / sizeof (arr[0]))


/************************* FUNCTIONS *************************/

/**
 * Parse the file and check if the extension is corresponding.
 * \param dir directory structure that contains filename
 * \param ext extension to check
 * \returns true on success, false on error
*/
bool parse_file_ext(const struct dirent *dir, const char *ext);

/**
 * Get number of files in a specific directory that matches a specific extension.
 * \param dir_path directory path
 * \param ext extension used to parse
 * \returns number of files
*/
uint8_t get_nb_files(const char *dir_path, const char *ext);

/**
 * Get all files in a specific directory, matching a specific extension.
 * \param dir_path directory path
 * \param ext extension used to parse
 * \param arr_files array of string containing file names
*/
void get_files(const char *dir_path, const char *ext, char **arr_files);

/**
 * Create the name of the save file.
 * \param ext save file extension
*/
void save_get_date(const char *ext);

/**
 * Open and write inside the save file.
*/
void save_write_file();

/**
 * Write the map structure in the binary file.
 * \param f the open binary file
*/
void save_write_map(FILE *f);

/**
 * Write the player structure in the binary file.
 * \param f the open binary file
*/
void save_write_player(FILE *f);

/**
 * Write all enemies structures in the binary file.
 * \param f the open binary file
*/
void save_write_enemy(FILE *f);

/**
 * Write the game structure in the binary file.
 * \param f the open binary file
*/
void save_write_game(FILE *f);

/**
 * Write a stack saved in a binary file.
 * \param stack stack to write in a binary file
 * \param f the open binary file
*/
void save_write_stack(Stack *stack, FILE *f);

/**
 * Open and read the save file.
 * \param file_name save file
*/
void save_read_file(const char *file_name);

/**
 * Read the map structure in the binary file.
 * \param f the open binary file
*/
void save_read_map(FILE *f);

/**
 * Read the player structure in the binary file.
 * \param f the open binary file
*/
void save_read_player(FILE *f);

/**
 * Read all enemies structure in the binary file.
 * \param f the open binary file
*/
void save_read_enemy(FILE *f);

/**
 * Read the game structure in the binary file.
 * \param f the open binary file
*/
void save_read_game(FILE *f);

/**
 * Read a stack saved in a binary file.
 * \param stack new empty stack that we will complete 
 * \param f the open binary file
 * \param player_stack indication whether it is the stack of a player's path or not
*/
void save_read_stack(Stack *stack, FILE *f, bool player_stack);

/**
 * Save current state of the game into a binary file.
 * \param ext save file extension
*/
void save_game(const char *ext);

/**
 * Free allocated memory for SaveManager structure.
*/
void save_free();

/**
 * Free a dynamic 2D string array.
 * \param arr dynamic 2D string array
 * \param arr_len length of array
*/
void str_2d_array_free(char **arr, uint8_t arr_len);


#endif