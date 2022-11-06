#include "map.h"

extern Map *map;

// TODO: random path generation algorithm (Tiles ROAD): Melvyn

void map_random_fill() {
    srand(time(NULL));
    float distance, dn;
    float prob;
    // Tiles' probability distribution according to the difficulty
    float prob_fruit;
    float prob_obs;

    for (uint8_t i = 0, j; i < MAP_SIZE; i++) {
        for (j = 0; j < MAP_SIZE*2; j++) {
            prob_fruit = PROB_FRUIT / map->level;
            prob_obs = PROB_OBS * map->level;
            // distance between center and current position
            distance = sqrt(pow(CENTER_X - i, 2) + pow(CENTER_Y - j, 2));
            dn = distance / MAX_DISTANCE;
            // prob between 0 and 1
            
            prob = (float) rand() / RAND_MAX;
            // Tile type is based on the probability distribution
            prob_fruit = (prob_fruit * (dn * LAMBDA_FRUIT) > PROB_MAX_FRUIT) ?
                PROB_MAX_FRUIT: prob_fruit * (dn * LAMBDA_FRUIT);
            prob_obs = (prob_obs / (dn * LAMBDA_OBS) > PROB_MAX_OBS) ?
                PROB_MAX_OBS: prob_obs / (dn * LAMBDA_OBS);
            
            if (map->map_grid[i][j].cell_type == EMPTY) {
                map->map_grid[i][j].cell_type = (prob <= prob_fruit) ? FRUIT:
                    (prob <= prob_obs) ? OBSTACLE: ROAD;
            }

        }
    }
}