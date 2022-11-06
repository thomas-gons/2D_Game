#include "map.h"

extern Map *map;

// TODO: random path generation algorithm (Tiles ROAD): Melvyn

void map_random_fill() {
    srand(time(NULL));
    float prob;
    // Tiles' probability distribution according to the difficulty
    float prob_fruit = PROB_FRUIT / map->level;
    float prob_obs = PROB_OBS + 0.05 * map->level;
    for (uint8_t i = 0, j; i < MAP_SIZE; i++) {
        for (j = 0; j < MAP_SIZE*2; j++) {
            prob = (float) rand() / RAND_MAX;
            // Tile type is based on the probability distribution
            if (map->map_grid[i][j].cell_type == EMPTY) {
                map->map_grid[i][j].cell_type = (prob <= prob_fruit) ? FRUIT:
                    (prob <= prob_obs) ? OBSTACLE: ROAD;
            }
        }
    }
}