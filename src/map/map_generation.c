#include "map.h"
#include "util.h"


extern Map *map;

void map_random_fill() {
    float distance, dn;
    float prob;
    // Tiles' probability distribution according to the difficulty
    float prob_bonus, prob_obs;
    for (uint8_t l = 0, c; l < MAP_LINES; l++) {
        for (c = 0; c < MAP_COLS; c++) {
            if ((!l && !c) || ((l == MAP_LINES - 1) && (c == MAP_COLS - 1))) {
                map->map_grid[l][c].cell_type = ROAD;
                continue;
            }
            // prob between 0 and 1
            prob = (float) rand() / RAND_MAX;

            // distance between center and current position
            distance = sqrt(pow(CENTER_L - l, 2) + pow(CENTER_C - c, 2));
            // normalize the distance to be able to exploit it
            dn = distance / (sqrt(pow(MAP_LINES, 2) + pow(MAP_COLS, 2)));
            prob_bonus = PROB_BONUS * (dn * LAMBDA_BONUS);
            prob_obs = PROB_OBS / (dn * LAMBDA_OBS);
            // Tile type is based on the probability distribution model (radial here)
            if (prob_bonus > PROB_MAX_BONUS)
                prob_bonus = PROB_MAX_BONUS;
            
            if (prob_obs > (PROB_MAX_OBS * map->level / 2))
                prob_obs = PROB_MAX_OBS * map->level / 2;
            
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