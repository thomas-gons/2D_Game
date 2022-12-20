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
 * Get all files in a specific directory, matching a specific extension.
 * \param path directory path
 * \param ext extension used to parse
*/
void get_files(const char *path, const char *ext);


#endif