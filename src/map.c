#include "map.h"

extern Map *map;

void map_init(Level level) {
    map = calloc(1, sizeof *map);
    *map = (Map) {level, 0, NULL};
    map->map_grid = calloc(1, sizeof *map->map_grid * MAP_SIZE);
    // Fill the map with empty and unvisited cells
    for (uint8_t i = 0, j; i < MAP_SIZE; i++) {
        // each line should be malloc
        map->map_grid[i] = calloc(MAP_SIZE*2, sizeof *map->map_grid[i]);
        for (j = 0; j < MAP_SIZE*2; j++) {
            map->map_grid[i][j] = (Cell) {EMPTY, .visited = false, .state = OPEN};
        }
    }
}

void map_render(WINDOW *win) {
    for (uint8_t i = 0, j; i < MAP_SIZE; i++) {
        for (j = 0; j < MAP_SIZE*2; j++) {
            switch (map->map_grid[i][j].cell_type) {
            case EMPTY:
            case ROAD:
                mvwaddch(win, i, j, '.');
                break;
            case OBSTACLE:
                mvwaddch(win, i, j, '#');
                break;
            case FRUIT:
                mvwaddch(win, i, j, '@');
                break;
            default: break;
            }
        }
    }
    wrefresh(win);
}



void map_free() {
    for (uint8_t i = 0; i < MAP_SIZE; i++)
        free(map->map_grid[i]);
    free(map->map_grid);
}

void map_display() {
    for (uint8_t i = 0, j; i < MAP_SIZE; i++) {
        for (j = 0; j < MAP_SIZE*2; j++) {
            switch (map->map_grid[i][j].cell_type) {
                case EMPTY:
                case ROAD:
                    printf(" "); break;
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