#include "util.h"
#include "map.h"

extern Map *map;
extern int8_t moveset[4][2];

Stack *search_path(unsigned heuristic[MAP_SIZE][MAP_SIZE*2], Position init, Position goal, int cost) {
    // closed array will be filled with all tested position
    unsigned closed[MAP_SIZE][MAP_SIZE*2] = {0};
    closed[init.x][init.y] = 1;
    int action[MAP_SIZE][MAP_SIZE*2] = {0};

    uint8_t x = init.x, y = init.y;
    int8_t tmp_x, tmp_y;
    // g -> the movement cost to move from the starting point to a given square on the grid, following the path generated to get there
    int g = 0, f = g + heuristic[x][y], tmp_g, tmp_f;


    Heap *opened = heap_create();
    heap_push(opened, (Point) {(Position) {x, y}, f, g});

    bool found = false, resign = false;
    while (!found && !resign) {
        if (!opened->nb_points)
            return NULL;
        
        Point next_cell = heap_pop(opened);
        if (next_cell.pos.x == goal.x && next_cell.pos.y == goal.y)
            found = true;
        else {
            for (uint8_t i = 0; i < 4; i++) {
                tmp_x = next_cell.pos.x + moveset[i][0];
                tmp_y = next_cell.pos.y + moveset[i][1];
                if (IS_NOT_INDEX_ERROR(tmp_x, tmp_y)) {
                    if ((!closed[tmp_x][tmp_y] && !closed[tmp_x][tmp_y]) ||
                        map->map_grid[tmp_x][tmp_y].cell_type != OBSTACLE) {
                        tmp_g = g + cost;
                        tmp_f = g + heuristic[tmp_x][tmp_y];
                        heap_push(opened, (Point) {(Position) {tmp_x, tmp_y}, tmp_f, tmp_g});
                        closed[tmp_x][tmp_y] = 1;
                        action[tmp_x][tmp_y] = i;
                    }
                }
            }
        }
    }
    
    Stack *inverted_path = stack_init();
    x = goal.x;
    y = goal.y;
    stack_push(inverted_path, (Position) {x, y});
    while (x != init.x && y != init.y) {
        tmp_x = x - moveset[action[x][y]][0];
        tmp_y = y - moveset[action[x][y]][1];
        x = tmp_x;
        y = tmp_y;
        stack_push(inverted_path, (Position) {x, y});
    }

    // revert the path
    return inverted_path;
}

Stack *a_star() {
    Position init = {0, 0}, goal = {MAP_SIZE*2 - 1, MAP_SIZE - 1};
    // cost -> the movement cost to reach an adjacent cell
    unsigned cost = 1;
    uint8_t i, j;

    unsigned heuristic[MAP_SIZE][MAP_SIZE*2] = {0};

    for (i = 0; i < MAP_SIZE; i++) {
        for (j = 0; j < MAP_SIZE*2; j++) {
            // h -> estimated cost of moving from a specific grid squareto the final destination
            heuristic[i][j] = abs(i - goal.x) + abs(j - goal.y);
            // if the cell type is OBSTACLE, then the heuristic should infinite (quasi)
            if (map->map_grid[i][j].state == BLOCK)
                heuristic[i][j] = UINT32_MAX;
        }
    }
    return search_path(heuristic, init, goal, cost);
}