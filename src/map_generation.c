#include "map.h"

extern Map *map;


void map_init(uint8_t dim) {
    map = malloc(sizeof *map);
    map->dim = dim;
    map->step = 0;
    map->map_grid = malloc(sizeof *map->map_grid * dim);
    for (uint8_t i = 0, j; i < dim; i++) {
        map->map_grid[i] = malloc(sizeof *map->map_grid[i] * dim);
        for (j = 0; j < dim; j++)
            map->map_grid[i][j] = (Cell) {EMPTY, false};

    }
}

void map_free() {
    for (uint8_t i = 0; i < map->dim; i++)
        free(map->map_grid[i]);
    free(map->map_grid);
}