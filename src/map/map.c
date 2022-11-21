#include "map.h"


extern Game game;
extern Map *map;

void map_init(Level level) {
    map = calloc(1, sizeof *map);
    if (!map) {
        fprintf(stderr, "[ERROR] > calloc, in func map_init\n");
        exit(1);
    }
    *map = (Map) {.level=level, .map_grid=NULL};
    map->map_grid = calloc(MAP_LINES, sizeof *map->map_grid);
    // Fill the map with empty and unvisited cells
    for (uint8_t i = 0; i < MAP_LINES; i++) {
        map->map_grid[i] = calloc(MAP_COLS, sizeof *map->map_grid[i]);
    }
}

void map_render() {
    for (uint8_t l = 0; l < MAP_LINES; l++) {
        for (uint8_t c = 0; c < MAP_COLS; c++) {
            switch (map->map_grid[l][c].cell_type) {
            case ROAD:
                mvwaddch(game.game_win, l, c,
                    ((map->map_grid[l][c].visited) ? '.' : ' ') | COLOR_PAIR(FORMAT_COLOR_PLAYER));
                break;
            case OBSTACLE:
                mvwaddch(game.game_win, l, c, '%' | COLOR_PAIR(FORMAT_COLOR_OBS));
                break;
            case FRUIT:
                mvwaddch(game.game_win, l, c, '@' | COLOR_PAIR(FORMAT_COLOR_FRUIT));
                break;
            default: break;
            }
        }
    }
}

void map_display() {
    for (uint8_t l = 0; l < MAP_LINES; l++) {
        for (uint8_t c = 0; c < MAP_COLS; c++) {
            switch (map->map_grid[l][c].cell_type) {
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

void map_render_path(Stack *path) {
    Node *tmp = path->head;
    for (; tmp; tmp = tmp->next) {
        mvwaddch(game.game_win, tmp->pos.l, tmp->pos.c, '+' | COLOR_PAIR(FORMAT_COLOR_PATH));
    }
    wrefresh(game.game_win);
}

void map_free() {
    for (uint8_t l = 0; l < MAP_LINES; l++) {
        free(map->map_grid[l]);
    }
    free(map->map_grid);
}