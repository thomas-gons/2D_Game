#include "map.h"

extern Map *map;

void map_display() {
    for (uint8_t i = 0, j; i < map->dim; i++) {
        for (j = 0; j < map->dim; j++)
            printf("%d ", map->map_grid[i][j].cell_type);
        printf("\n");
    }
}