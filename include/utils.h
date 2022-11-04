#ifndef __UTILS_H__
#define __UTILS_H__

#include "common.h"

/**
 * Postion in x and y axes.
*/
typedef struct {
    uint8_t x;
    uint8_t y;
} Pos;

/**
 * Node of doubly linked list.
*/
typedef struct Node {
    Pos position;
    struct Node *prev;
    struct Node *next;
} Node;

/**
 * Doubly linked list.
*/
typedef struct {
    Node *head;
    Node *tail;
} Db_list;


/**
 * Initialize a doubly linked list.
 * \returns a valid Db_list struct on success, NULL on error
*/
Db_list *list_init();

/**
 * Push a node to head of doubly linked list.
 * \param list doubly linked list
 * \param position data to push in linked list
*/
void list_push(Db_list *list, Pos position);

/**
 * Pop a node from tail of doubly linked list.
 * \param list doubly linked list
 * \returns tail node on success, NULL on error
*/
Node *list_pop(Db_list *list);

/**
 * Free all nodes in doubly linked list.
 * \param list doubly linked list
*/
void list_free(Db_list *list);

/**
 * Display linked list.
 * \param list doubly linked list
*/
void list_display(Db_list *list);

/**
 * Check if doubly linked list is empty.
 * \param list doubly linked list
 * \returns true on success, false on error
*/
bool list_is_empty(Db_list *list);

/**
 * Get length of linked list.
 * \param list doubly linked list
 * \returns number of nodes in list
*/
uint16_t list_len(Db_list *list);


#endif