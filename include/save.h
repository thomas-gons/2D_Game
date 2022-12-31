#ifndef __SAVE_H__
#define __SAVE_H__


#include "common.h"
#include "structs.h"
#include "util.h"


/************************* DEFINES *************************/

/**
 * 
*/

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
 * Free a dynamic 2D string array.
 * \param arr dynamic 2D string array
 * \param arr_len length of array
*/
void str_2d_array_free(char **arr, uint8_t arr_len);


#endif