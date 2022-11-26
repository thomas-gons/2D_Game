#include "map.h"


extern Game game;
extern Map *map;

void map_init(Level level) {
    map = calloc(1, sizeof *map);
    if (!map) {
        fprintf(stderr, "[ERROR] > calloc, in func map_init\n");
        exit(2);
    }
    *map = (Map) {.level=level, .map_grid=NULL};
    map->map_grid = calloc(MAP_LINES, sizeof *map->map_grid);
    // Fill the map with empty and unvisited cells
    for (uint8_t i = 0, j; i < MAP_LINES; i++) {
        map->map_grid[i] = calloc(MAP_COLS, sizeof *map->map_grid[i]);
        for (j = 0; j < MAP_COLS; j++) {
            if (j != MAP_COLS - 1) {
                // right direction
                map->map_grid[i][j].distance[0] = (rand() % (MAX_DISTANCE - MIN_DISTANCE - 1)) + MIN_DISTANCE;
            }
            if (i != MAP_LINES - 1) {
                // bottom direction
                map->map_grid[i][j].distance[1] = (rand() % (MAX_DISTANCE - MIN_DISTANCE - 1)) + MIN_DISTANCE;
            }
        }
    }
}

void map_render() {
    for (uint8_t l = 0; l < MAP_LINES; l++) {
        for (uint8_t c = 0; c < MAP_COLS; c++) {
            switch (map->map_grid[l][c].cell_type) {
            case ROAD:
                mvwaddch(game.game_win, l, c,
                    ((map->map_grid[l][c].visited) ? '.' : ' ') | COLOR_PAIR(FORMAT_COLOR_CYAN));
                break;
            case OBSTACLE:
                mvwaddch(game.game_win, l, c, '%' | COLOR_PAIR(FORMAT_COLOR_RED));
                break;
            case FRUIT:
                mvwaddch(game.game_win, l, c, '@' | COLOR_PAIR(FORMAT_COLOR_GREEN));
                break;
            case NO_FRUIT:
                mvwaddch(game.game_win, l, c, ',' | COLOR_PAIR(FORMAT_COLOR_YELLOW));
                break;
            default: break;
            }
        }
    }
    mvwaddch(game.game_win, MAP_LINES - 1, MAP_COLS - 1, '#' | COLOR_PAIR(FORMAT_COLOR_CYAN));
}

void map_display() {
    for (uint8_t l = 0; l < MAP_LINES; l++) {
        for (uint8_t c = 0; c < MAP_COLS; c++) {
            printf("(🠖%hhu, 🠗%hhu) ", map->map_grid[l][c].distance[0], map->map_grid[l][c].distance[1]);
        }
        printf("\n");
    }
}

void map_render_path(Stack *path) {
    Node *tmp = path->head;
    for (; tmp; tmp = tmp->next) {
        mvwaddch(game.game_win, tmp->pos.l, tmp->pos.c, '+' | COLOR_PAIR(FORMAT_COLOR_CYAN));
    }
    wrefresh(game.game_win);
}

void map_free() {
    for (uint8_t l = 0; l < MAP_LINES; l++) {
        free(map->map_grid[l]);
    }
    free(map->map_grid);
}