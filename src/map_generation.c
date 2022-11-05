#include "map.h"

extern Map *map;

void map_init(uint8_t dim, Level level) {
    map = calloc(1, sizeof *map);
    *map = (Map) {dim, level, 0, NULL};
    map->map_grid = calloc(1, sizeof *map->map_grid * dim);
    // Fill the map with empty and unvisited cells
    for (uint8_t i = 0, j; i < dim; i++) {
        // each line should be malloc
        map->map_grid[i] = calloc(dim, sizeof *map->map_grid[i]);
        for (j = 0; j < dim; j++) {
            map->map_grid[i][j] = (Cell) {EMPTY, .visited = false};
        }
    }
}

// TODO: random path generation algorithm (Tiles ROAD): Melvyn

void map_random_fill() {
    srand(time(NULL));
    float prob;
    // Tiles' probability distribution according to the difficulty
    float prob_fruit = PROB_FRUIT / map->level;
    float prob_obs = PROB_OBS + 0.05 * map->level;
    for (uint8_t i = 0, j; i < map->dim; i++) {
        for (j = 0; j < map->dim; j++) {
            prob = (float) rand() / RAND_MAX;
            // Tile type is based on the probability distribution
            if (map->map_grid[i][j].cell_type == EMPTY) {
                map->map_grid[i][j].cell_type = (prob <= prob_fruit) ? FRUIT:
                    (prob <= prob_obs) ? OBSTACLE: ROAD;
            }
        }
    }
}

// Simple free of the map structure
void map_free() {
    for (uint8_t i = 0; i < map->dim; i++)
        free(map->map_grid[i]);
    free(map->map_grid);
}