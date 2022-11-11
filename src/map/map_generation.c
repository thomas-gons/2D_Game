#include "map.h"
#include "util.h"


extern Map *map;

void map_random_fill() {
    float distance, dn;
    float prob;
    // Tiles' probability distribution according to the difficulty
    float prob_fruit, tmp_prob_fruit;
    float prob_obs, tmp_prob_obs;

    for (uint8_t i = 0, j; i < MAP_LINES; i++) {
        for (j = 0; j < MAP_COLS; j++) {
            if ((!i && !j) || (i == MAP_LINES - 1 && j == MAP_COLS - 1)) {
                map->map_grid[i][j].cell_type = ROAD;
                continue;
            }
            prob_fruit = PROB_FRUIT / map->level;
            prob_obs = PROB_OBS * map->level;
            // distance between center and current position
            distance = sqrt(pow(CENTER_X - i, 2) + pow(CENTER_Y - j, 2));
            dn = distance / MAX_DISTANCE;
            // prob between 0 and 1
            prob = (float) rand() / RAND_MAX;
            tmp_prob_fruit = prob_fruit * (dn * LAMBDA_FRUIT);
            tmp_prob_obs = prob_obs / (dn * LAMBDA_OBS);

            // Tile type is based on the probability distribution model (radial here)
            prob_fruit = (tmp_prob_fruit > PROB_MAX_FRUIT) ?
                PROB_MAX_FRUIT: tmp_prob_fruit;
            prob_obs = (tmp_prob_obs > PROB_MAX_OBS) ?
                PROB_MAX_OBS: tmp_prob_obs;
            
            map->map_grid[i][j].cell_type = (prob <= prob_fruit) ? FRUIT:
                (prob <= prob_obs) ? OBSTACLE: ROAD;
        }
    }
}

void map_generate() {
    // While there is no path in the map, generate the map randomly and search
    // for at least one path using A* pathfinding algorithm (improved version of Dijkstra's algorithm)
    srand(time(NULL));
    Stack *path = NULL;
    do {
        map_random_fill();
        path = a_star();
    } while (!path);
    stack_free(path);
}