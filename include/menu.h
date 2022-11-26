#ifndef __MENU_H__
#define __MENU_H__


#include "common.h"
#include "structs.h"


/************************* FUNCTIONS *************************/

/**
 * Create template for menu with a list of entry.
 * \param entry_list list of string that represents menu entries
 * \param nb_entry number of menu entries
*/
void menu_create_entry_template(char **entry_list, int nb_entry);

/**
 * Input and render loop to select a menu entry.
 * \param entry_list list of string that represents menu entries
 * \param nb_entry number of menu entries
 * \returns index of selected menu entry
*/
int8_t menu_select_entry(char **entry_list, int nb_entry);


#endif