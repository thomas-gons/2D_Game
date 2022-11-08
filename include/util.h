#ifndef __UTIL_H__
#define __UTIL_H__

#include "common.h"

/**
 * Postion in x and y axes.
*/
typedef struct {
    uint8_t x;
    uint8_t y;
} Position;

/**
 * Node of stack.
*/
typedef struct Node {
    Position pos;
    struct Node *next;
} Node;

/**
 * Stack.
*/
typedef struct {
    Node *head;
    Node *tail;
} Stack;

typedef struct {
    Position pos;
    unsigned height;
    int f_score;
    int g_score;
    bool wall;
    bool goal;
    bool start;
    int parent_x;
    int parent_y;
} Point;

typedef struct {
    Point *points;
    uint16_t points_len;
    uint16_t len;
    unsigned memory_size;
} PriorityQ;

/**
 * Initialize a stack.
 * \returns a valid Stack struct on success, NULL on error
*/
Stack *stack_init();

/**
 * Push a node to head of stack.
 * \param stack stack
 * \param pos data to push in stack
*/
void stack_push(Stack *stack, Position pos);

/**
 * Pop a node from head of stack.
 * \param stack stack
*/
void stack_pop(Stack *stack);

/**
 * Free all nodes in stack.
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

#define PRIORITY_Q_IS_EMPTY(prio_q) (prio_q->points_len) ? true: false
#define PRIORITY_Q_GET_POINT(prio_q, index) (index > prio_q->len) ? NULL: &prio_q->points[index]
#define COMPARE_POINTS(p, q) (p.f_score > q.f_score) ? 1: (p.f_score > q.f_score) ? -1: 0

void insert(PriorityQ *prio_q, Point p);
Point pop(PriorityQ *prio_q);
void heapify(PriorityQ *prio_q, unsigned index);
void priorityQ_free(PriorityQ *prio_q);

#endif