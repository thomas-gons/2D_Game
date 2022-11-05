#include "map.h"

extern Map *map;

void map_display() {
    for (uint8_t i = 0, j; i < map->dim; i++) {
        for (j = 0; j < map->dim; j++) {
            switch (map->map_grid[i][j].cell_type) {
                case EMPTY:
                case ROAD:
                    printf("  "); break;
                case OBSTACLE:
                    printf(OBS_SYMBOL); break;
                case FRUIT:
                    printf(FRUIT_SYMBOL); break;
                default:
            }
        }
        printf("\n");
    }
}