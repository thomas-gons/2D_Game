#include "util.h"
#include "map.h"


extern Map *map;
extern Player *player;
extern Game game;

int8_t moveset[MOVESET_LEN][2] = {  // {line, col}
    {0, 1},     // Right
    {1, 0},     // Down
    {0, -1},    // Left
    {-1, 0}     // Up         
}; 

Stack *search_path(unsigned heuristic[MAP_LINES][MAP_COLS], Position start, Position end, bool with_stm) {
    unsigned cost;
    // closed array will be filled with all tested position
    unsigned closed[MAP_LINES][MAP_COLS] = {0};
    closed[start.l][start.c] = 1;
    int action[MAP_LINES][MAP_COLS] = {0};

    uint8_t l = start.l, c = start.c;
    int8_t tmp_l, tmp_c;
    // g -> the movement cost to move from the starting point to a given square on the grid,
    //  following the path generated to get there
    uint16_t g = 0, f = g + heuristic[l][c], tmp_g, tmp_f;
    int8_t tmp_stm;

    PQueue *opened = pqueue_init();
    pqueue_enqueue(opened, (Point) {(Position) {.l=l, .c=c}, player->stamina, f, g});
    bool found = false;
    while (!found) {
        if (!opened->nb_points) {
            pqueue_free(opened);
            return NULL;
        }
        Point next_cell = pqueue_dequeue(opened);
        if (next_cell.pos.l == end.l && next_cell.pos.c == end.c) {
            found = true;
        } else {
            for (uint8_t i = 0; i < MOVESET_LEN; i++) {
                tmp_l = next_cell.pos.l + moveset[i][0];
                tmp_c = next_cell.pos.c + moveset[i][1];
                if (IS_NOT_OUT_OF_MAP(tmp_l, tmp_c)) {
                    if (!closed[tmp_l][tmp_c] && heuristic[tmp_l][tmp_c] != 999 && CONSIDER_STM(with_stm, next_cell.stm)) {
                        switch (i) {
                        case 0:     // Right
                            cost = map->map_grid[next_cell.pos.l][next_cell.pos.c].distance[0];
                            break;
                        case 1:     // Down
                            cost = map->map_grid[next_cell.pos.l][next_cell.pos.c].distance[1];
                            break;
                        case 2:     // Left
                            cost = map->map_grid[next_cell.pos.l][next_cell.pos.c - 1].distance[0];
                            break;
                        default:    // Up
                            cost = map->map_grid[next_cell.pos.l - 1][next_cell.pos.c].distance[1];
                            break;
                        }
                        tmp_g = next_cell.g + cost;
                        tmp_f = tmp_g + heuristic[tmp_l][tmp_c];
                        tmp_stm = next_cell.stm - 1;
                        if (map->map_grid[next_cell.pos.l][next_cell.pos.c].cell_type == BONUS) {
                            tmp_stm += 10;
                        }
                        pqueue_enqueue(opened, (Point) {(Position) {.l=tmp_l, .c=tmp_c}, tmp_stm, tmp_f, tmp_g});
                        closed[tmp_l][tmp_c] = 1;
                        action[tmp_l][tmp_c] = i;
                    }
                }
            }
        }
    }
    pqueue_free(opened);
    Stack *inverted_path = stack_init();
    l = end.l;
    c = end.c;
    stack_push(inverted_path, (Position) {.l=l, .c=c}, NO_ACTION);
    unsigned path_len = 0;
    while (l != start.l || c != start.c) {
        switch (action[l][c]) {
            case 0: path_len += map->map_grid[l][c - 1].distance[0]; break;
            case 1: path_len += map->map_grid[l - 1][c].distance[1]; break;
            case 2: path_len += map->map_grid[l][c].distance[0]; break;
            case 3: path_len += map->map_grid[l][c].distance[1]; break;
        }
        tmp_l = l - moveset[action[l][c]][0];
        tmp_c = c - moveset[action[l][c]][1];
        l = tmp_l;
        c = tmp_c;
        stack_push(inverted_path, (Position) {.l=l, .c=c}, NO_ACTION);
    }
    if (with_stm) {
        game.path_stm_len = path_len;
    } else {
        game.path_dist_len = path_len;
    }
    // revert the path
    return inverted_path;
}

Stack *a_star(Position start, Position end, bool with_stm) {
    unsigned heuristic[MAP_LINES][MAP_COLS] = {0};

    for (uint8_t l = 0; l < MAP_LINES; l++) {
        for (uint8_t c = 0; c < MAP_COLS; c++) {
            // h -> estimated cost of moving from a specific grid square to the final destination
            heuristic[l][c] = abs(l - end.l) + abs(c - end.c);
            // If the cell type is OBSTACLE, then the heuristic should infinite (big value in our case)
            if (IS_OBSTACLE_CELL(l, c)) {
                heuristic[l][c] = 999;
            }
        }
    }
    return search_path(heuristic, start, end, with_stm);
}