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
    EAT_FRUIT,
    STACK_FRUIT,
    EAT_STACKED_FRUIT,
    REWIND
} Action;

/**
 * Node of stack.
*/
typedef struct Node {
    Position pos;
    Action action;
    struct Node *next;
} Node;

/**
 * Stack structure.
*/
typedef struct Stack {
    Node *head;
    Node *tail;
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
 * Node of queue.
*/
typedef struct NodeP {
    Point p;
    struct NodeP *next;
} NodeP;

/**
 * Queue strcuture.
*/
typedef struct Queue {
    NodeP *head;
    unsigned nb_points;
} Queue;


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
 * Initialize a queue.
 * \returns a valid queue on success, NULL on error
*/
Queue *queue_init();

/**
 * Enqueue item in queue.
 * \param q queue
 * \param p data of node to enqueue
*/
void queue_enqueue(Queue *q, Point p);

/**
 * Dequeue a queue.
 * \param q queue.
 * \returns data of dequeued node
*/
Point queue_dequeue(Queue *q);

/**
 * Free allocated memory of queue.
*/
void queue_free(Queue *q);


#endif