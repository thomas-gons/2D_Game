#ifndef __UTILS_H__
#define __UTILS_H__

#include "common.h"


typedef struct {
    uint8_t x, y;
} Pos;

typedef struct Node {
    Pos position;
    struct Node *prev;
    struct Node *next;
} Node, *Db_list;


#endif