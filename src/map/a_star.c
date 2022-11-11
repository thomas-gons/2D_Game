#include "util.h"
#include "map.h"


extern Map *map;
int8_t moveset[MOVESET_LEN][2] = {  // {line, col}
    {1, 0},     // Down 
    {0, 1},     // Right
    {-1, 0},    // Up
    {0, -1}     // Left         
}; 

Stack *search_path(unsigned heuristic[MAP_LINES][MAP_COLS], int cost) {
    // closed array will be filled with all tested position
    unsigned closed[MAP_LINES][MAP_COLS] = {0};
    closed[START.l][START.c] = 1;
    int action[MAP_LINES][MAP_COLS] = {0};

    uint8_t l = START.l, c = START.c;
    int8_t tmp_l, tmp_c;
    // g -> the movement cost to move from the starting point to a given square on the grid,
    //  following the path generated to get there
    int g = 0, f = g + heuristic[l][c], tmp_g, tmp_f;

    Queue *opened = queue_init();
    queue_enqueue(opened, (Point) {(Position) {.l=l, .c=c}, f, g});

    bool found = false, resign = false;
    
    while (!found && !resign) {
        if (!opened->nb_points) {
            return NULL;
        }
        Point next_cell = queue_dequeue(opened);
        if (next_cell.pos.l == END.l && next_cell.pos.c == END.c)
            found = true;
        else {
            for (uint8_t i = 0; i < MOVESET_LEN; i++) {
                tmp_l = next_cell.pos.l + moveset[i][0];
                tmp_c = next_cell.pos.c + moveset[i][1];
                if (IS_OUT_OF_MAP(tmp_l, tmp_c)) {
                    if (!closed[tmp_l][tmp_c] && !IS_OBSTACLE_CELL(tmp_l, tmp_c)) {
                        tmp_g = next_cell.g + cost;
                        tmp_f = tmp_g + heuristic[tmp_l][tmp_c];
                        queue_enqueue(opened, (Point) {(Position) {.l=tmp_l, .c=tmp_c}, tmp_f, tmp_g});
                        closed[tmp_l][tmp_c] = 1;
                        action[tmp_l][tmp_c] = i;
                    }
                }
            }
        }
    }
    queue_free(opened);
    Stack *inverted_path = stack_init();
    l = END.l;
    c = END.c;
    stack_push(inverted_path, (Position) {.l=l, .c=c});
    while (l != START.l || c != START.c) {
        tmp_l = l - moveset[action[l][c]][0];
        tmp_c = c - moveset[action[l][c]][1];
        l = tmp_l;
        c = tmp_c;
        stack_push(inverted_path, (Position) {.l=l, .c=c});
    }
    // revert the path
    return inverted_path;
}

Stack *a_star() {
    // cost -> the movement cost to reach an adjacent cell
    unsigned cost = 1;
    unsigned heuristic[MAP_LINES][MAP_COLS] = {0};

    for (uint8_t l = 0; l < MAP_LINES; l++) {
        for (uint8_t c = 0; c < MAP_COLS; c++) {
            // h -> estimated cost of moving from a specific grid square to the final destination
            heuristic[l][c] = abs(l - END.l) + abs(c - END.c);
            // If the cell type is OBSTACLE, then the heuristic should infinite (big value in our case)
            if (IS_OBSTACLE_CELL(l, c)) {
                heuristic[l][c] = 999;
            }
        }
    }
    return search_path(heuristic, cost);
}