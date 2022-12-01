#include "map.h"
#include "util.h"


extern Map *map;

void map_random_fill() {
    float distance, dn;
    float prob;
    // Tiles' probability distribution according to the difficulty
    float prob_bonus, tmp_prob_bonus;
    float prob_obs, tmp_prob_obs;

    for (uint8_t l = 0; l < MAP_LINES; l++) {
        for (uint8_t c = 0; c < MAP_COLS; c++) {
            if ((!l && !c) || ((l == MAP_LINES - 1) && (c == MAP_COLS - 1))) {
                map->map_grid[l][c].cell_type = ROAD;
                continue;
            }
            prob_bonus = PROB_BONUS / map->level;
            prob_obs = PROB_OBS * map->level;
            // distance between center and current position
            distance = sqrt(pow(CENTER_L - l, 2) + pow(CENTER_C - c, 2));
            dn = distance / (sqrt(pow(MAP_LINES, 2) + pow(MAP_COLS, 2)));
            // prob between 0 and 1
            prob = (float) rand() / RAND_MAX;
            tmp_prob_bonus = prob_bonus * (dn * LAMBDA_BONUS);
            tmp_prob_obs = prob_obs / (dn * LAMBDA_OBS);

            // Tile type is based on the probability distribution model (radial here)
            prob_bonus = (tmp_prob_bonus > PROB_MAX_BONUS) ?
                PROB_MAX_BONUS : tmp_prob_bonus;
            prob_obs = (tmp_prob_obs > PROB_MAX_OBS) ?
                PROB_MAX_OBS : tmp_prob_obs;
            
            map->map_grid[l][c].cell_type = (prob <= prob_bonus) ? BONUS :
                (prob <= prob_obs) ? OBSTACLE : ROAD;
        }
    }
}

Stack *map_generate() {
    // While there is no path in the map, generate the map randomly and search
    // for at least one path using A* pathfinding algorithm (improved version of Dijkstra's algorithm)
    srand(time(NULL));
    Stack *path = NULL;
    do {
        map_random_fill();
        path = a_star();
        // think about path lenght
    } while (!path);
    return path;
}