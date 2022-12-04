#ifndef __UTIL_H__
#define __UTIL_H__


#include "common.h"


/************************* STRUCTS *************************/

/**
 * Postion in lines and columns.
*/
typedef struct {
    uint8_t l;
    uint8_t c;
} Position;

/**
 * Player action.
*/
typedef enum Action {
    NO_ACTION = -1,
    HIT_OBSTACLE,
    USE_BONUS,
    STACK_BONUS,
    USE_STACKED_BONUS,
    REWIND
} Action;

/**
 * SNode of stack.
*/
typedef struct SNode {
    Position pos;
    Action action;
    struct SNode *next;
} SNode;

/**
 * Stack structure.
*/
typedef struct Stack {
    SNode *head;
    SNode *tail;
} Stack;

/**
 * Structure for opened cells in A* algorithm.
*/
typedef struct Point {
    Position pos;
    unsigned f;
    unsigned g;
} Point;

/**
 * Node of pqueue.
*/
typedef struct PQNode {
    Point p;
    struct PQNode *next;
} PQNode;

/**
 * Priority Queue strcuture.
*/
typedef struct PQueue {
    PQNode *head;
    unsigned nb_points;
} PQueue;


/************************* FUNCTIONS *************************/

/**
 * Initialize a stack.
 * \returns a valid stack on success, NULL on error
*/
Stack *stack_init();

/**
 * Push a node to head of stack.
 * \param stack stack
 * \param pos position to push in stack
 * \param act action to push in stack
*/
void stack_push(Stack *stack, Position pos, Action act);

/**
 * Pop a node from head of stack.
 * \param stack stack
*/
void stack_pop(Stack *stack);

/**
 * Remove a node from stack.
 * \param stack stack
 * \param index index of node to remove
 * \returns data of removed node
*/
Position stack_remove(Stack *stack, uint16_t index);

/**
 * Get index of stack element of specific data.
 * \param stack stack
 * \param pos data to search the index
 * \returns index of node on success, -1 on error
*/
int16_t stack_get_index(Stack *stack, Position pos);

/**
 * Free allocated memory of stack.
 * \param stack stack
*/
void stack_free(Stack *stack);

/**
 * Display stack.
 * \param stack stack
*/
void stack_display(Stack *stack);

/**
 * Check if stack is empty.
 * \param stack stack
 * \returns true on success, false on error
*/
bool stack_is_empty(Stack *stack);

/**
 * Get length of linked stack.
 * \param stack stack
 * \returns number of nodes in stack
*/
uint16_t stack_len(Stack *stack);

/**
 * Initialize a pqueue.
 * \returns a valid pqueue on success, NULL on error
*/
PQueue *pqueue_init();

/**
 * Enqueue item in pqueue according priority.
 * \param q pqueue
 * \param p data of node to enqueue
*/
void pqueue_enqueue(PQueue *q, Point p);

/**
 * Dequeue a pqueue.
 * \param q pqueue.
 * \returns data of dequeued node
*/
Point pqueue_dequeue(PQueue *q);

/**
 * Free allocated memory of pqueue.
*/
void pqueue_free(PQueue *q);

/**
 *  Display the pqueue
*/
void pqueue_display(PQueue *q);

#endif