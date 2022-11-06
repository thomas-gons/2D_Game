#include "map.h"

extern Map *map;

// TODO: random path generation algorithm (Tiles ROAD): Melvyn

void map_random_fill() {
    srand(time(NULL));
    float distance, dn;
    float prob;
    // Tiles' probability distribution according to the difficulty
    float prob_fruit, tmp_prob_fruit;
    float prob_obs, tmp_prob_obs;

    for (uint8_t i = 0, j; i < MAP_SIZE; i++) {
        for (j = 0; j < MAP_SIZE*2; j++) {
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
            
            if (map->map_grid[i][j].cell_type == EMPTY) {
                map->map_grid[i][j].cell_type = (prob <= prob_fruit) ? FRUIT:
                    (prob <= prob_obs) ? OBSTACLE: ROAD;
            }

        }
    }
}