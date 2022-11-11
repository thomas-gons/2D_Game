#include "map.h"


extern Map *map;

void map_init(Level level) {
    map = calloc(1, sizeof *map);
    *map = (Map) {level, 0, NULL};
    map->map_grid = calloc(MAP_LINES, sizeof *map->map_grid);
    // Fill the map with empty and unvisited cells
    for (uint8_t i = 0; i < MAP_LINES; i++) {
        map->map_grid[i] = calloc(MAP_COLS, sizeof *map->map_grid[i]);
    }
}

void map_render(WINDOW *win) {
    for (uint8_t i = 0, j; i < MAP_LINES; i++) {
        for (j = 0; j < MAP_COLS; j++) {
            switch (map->map_grid[i][j].cell_type) {
            case ROAD:
                mvwaddch(win, i, j, ' ');
                break;
            case OBSTACLE:
                mvwaddch(win, i, j, '%' | COLOR_PAIR(FORMAT_COLOR_OBS));
                break;
            case FRUIT:
                mvwaddch(win, i, j, '@' | COLOR_PAIR(FORMAT_COLOR_FRUIT));
                break;
            default: break;
            }
        }
    }
}

void map_display() {
    for (uint8_t i = 0, j; i < MAP_LINES; i++) {
        for (j = 0; j < MAP_COLS; j++) {
            switch (map->map_grid[i][j].cell_type) {
            case ROAD:
                printf("0 ");
                break;
            case FRUIT:
                printf("2 ");
                break;
            default:
                printf("1 ");
                break;
            }
        }
        printf("\n");
    }
}

void map_render_path(WINDOW *win, Stack *path) {
    Node *tmp = path->head;
    for (; tmp; tmp = tmp->next)
        mvwaddch(win, tmp->pos.x, tmp->pos.y, '+' | COLOR_PAIR(FORMAT_COLOR_PATH));
    
    wrefresh(win);
}

void map_free() {
    for (uint8_t i = 0; i < MAP_LINES; i++) {
        free(map->map_grid[i]);
    }
    free(map->map_grid);
}