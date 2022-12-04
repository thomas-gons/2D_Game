#include "util.h"
#include "map.h"


extern Map *map;
extern Game game;

int8_t moveset[MOVESET_LEN][2] = {  // {line, col}
    {0, 1},     // Right
    {1, 0},     // Down
    {0, -1},    // Left
    {-1, 0}     // Top         
}; 

Stack *search_path(unsigned heuristic[MAP_LINES][MAP_COLS]) {
    unsigned cost;
    // closed array will be filled with all tested position
    unsigned closed[MAP_LINES][MAP_COLS] = {0};
    closed[START.l][START.c] = 1;
    int action[MAP_LINES][MAP_COLS] = {0};

    uint8_t l = START.l, c = START.c;
    int8_t tmp_l, tmp_c;
    // g -> the movement cost to move from the starting point to a given square on the grid,
    //  following the path generated to get there
    int g = 0, f = g + heuristic[l][c], tmp_g, tmp_f;

    PQueue *opened = pqueue_init();
    pqueue_enqueue(opened, (Point) {(Position) {.l=l, .c=c}, f, g});
    bool found = false;
    while (!found) {
        if (!opened->nb_points) {
            return NULL;
        }
        Point next_cell = pqueue_dequeue(opened);
        if (next_cell.pos.l == END.l && next_cell.pos.c == END.c)
            found = true;
        else {
            for (uint8_t i = 0; i < MOVESET_LEN; i++) {
                tmp_l = next_cell.pos.l + moveset[i][0];
                tmp_c = next_cell.pos.c + moveset[i][1];
                if (IS_NOT_OUT_OF_MAP(tmp_l, tmp_c)) {
                    if (!closed[tmp_l][tmp_c] && heuristic[tmp_l][tmp_c] != 999) {
                        switch (i) {
                            case 0:
                                // right
                                cost = map->map_grid[next_cell.pos.l][next_cell.pos.c].distance[0];
                                break;
                            case 1:
                                // bottom
                                cost = map->map_grid[next_cell.pos.l][next_cell.pos.c].distance[1];
                                break;
                            case 2:
                                // left
                                cost = map->map_grid[next_cell.pos.l][next_cell.pos.c - 1].distance[0];
                                break;
                            default:
                                // top
                                cost = map->map_grid[next_cell.pos.l - 1][next_cell.pos.c].distance[1];
                                break;
                        }
                        tmp_g = next_cell.g + cost;
                        tmp_f = tmp_g + heuristic[tmp_l][tmp_c];
                        pqueue_enqueue(opened, (Point) {(Position) {.l=tmp_l, .c=tmp_c}, tmp_f, tmp_g});
                        closed[tmp_l][tmp_c] = 1;
                        action[tmp_l][tmp_c] = i;
                    }
                }
            }
        }
    }
    pqueue_free(opened);
    Stack *inverted_path = stack_init();
    l = END.l;
    c = END.c;
    stack_push(inverted_path, (Position) {.l=l, .c=c}, NO_ACTION);
    unsigned distance = 0;
    while (l != START.l || c != START.c) {
        switch (action[l][c]) {
            case 0: distance += map->map_grid[l][c - 1].distance[0]; break;
            case 1: distance += map->map_grid[l - 1][c].distance[1]; break;
            case 2: distance += map->map_grid[l][c].distance[0]; break;
            case 3: distance += map->map_grid[l][c].distance[1]; break;
        }
        tmp_l = l - moveset[action[l][c]][0];
        tmp_c = c - moveset[action[l][c]][1];
        l = tmp_l;
        c = tmp_c;
        stack_push(inverted_path, (Position) {.l=l, .c=c}, NO_ACTION);
    }
    mvwprintw(game.dist_win, 13, 3, "BEST DISTANCE");
    mvwprintw(game.dist_win, 14, 8, "%u", distance);
    // revert the path
    return inverted_path;
}

Stack *a_star() {
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
    return search_path(heuristic);
}