#include "util.h"
#include "map.h"


extern Map *map;
int8_t moveset[4][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

Stack *search_path(unsigned heuristic[MAP_LINES][MAP_COLS], Position init, Position goal, int cost) {
    // closed array will be filled with all tested position
    unsigned closed[MAP_LINES][MAP_COLS] = {0};
    closed[init.x][init.y] = 1;
    int action[MAP_LINES][MAP_COLS] = {0};

    uint8_t x = init.x, y = init.y;
    int8_t tmp_x, tmp_y;
    // g -> the movement cost to move from the starting point to a given square on the grid,
    //  following the path generated to get there
    int g = 0, f = g + heuristic[x][y], tmp_g, tmp_f;


    // Heap *opened = heap_create();
    Queue *opened = queue_init();
    queue_enqueue(opened, (Point) {(Position) {x, y}, f, g});

    bool found = false, resign = false;
    
    while (!found && !resign) {
        if (!opened->nb_points)
            return NULL;

        Point next_cell = queue_dequeue(opened);
        if (next_cell.pos.x == goal.x && next_cell.pos.y == goal.y)
            found = true;
        else {
            for (uint8_t i = 0; i < 4; i++) {
                tmp_x = next_cell.pos.x + moveset[i][0];
                tmp_y = next_cell.pos.y + moveset[i][1];
                if (IS_NOT_INDEX_ERROR(tmp_x, tmp_y)) {
                    if (!closed[tmp_x][tmp_y] && map->map_grid[tmp_x][tmp_y].cell_type != OBSTACLE) {
                        tmp_g = next_cell.g + cost;
                        tmp_f = tmp_g + heuristic[tmp_x][tmp_y];
                        queue_enqueue(opened, (Point) {(Position) {tmp_x, tmp_y}, tmp_f, tmp_g});
                        closed[tmp_x][tmp_y] = 1;
                        action[tmp_x][tmp_y] = i;
                    }
                }
            }
        }
    }
    queue_free(opened);
    Stack *inverted_path = stack_init();
    x = goal.x;
    y = goal.y;
    stack_push(inverted_path, (Position) {x, y});
    while (x != init.x || y != init.y) {
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
    Position init = {0, 0}, goal = {MAP_LINES - 1, MAP_COLS - 1};
        // cost -> the movement cost to reach an adjacent cell
    unsigned cost = 1;
    uint8_t i, j;

    unsigned heuristic[MAP_LINES][MAP_COLS] = {0};

    for (i = 0; i < MAP_LINES; i++) {
        for (j = 0; j < MAP_COLS; j++) {
            // h -> estimated cost of moving from a specific grid squareto the final destination
            heuristic[i][j] = abs(i - goal.x) + abs(j - goal.y);
            // if the cell type is OBSTACLE, then the heuristic should infinite (quasi)
            if (map->map_grid[i][j].cell_type == OBSTACLE)
                heuristic[i][j] = 999;
        }
    }
    return search_path(heuristic, init, goal, cost);
}