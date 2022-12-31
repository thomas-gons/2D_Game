#include "map.h"


extern Game game;
extern Map *map;
extern Player *player;
extern Level level;

void map_init() {
    map = calloc(1, sizeof (*map));
    if (!map) {
        fprintf(stderr, "[ERROR] > calloc, in func map_init\n");
        exit(2);
    }
    *map = (Map) {.level=level, .map_grid=NULL};
    map->map_grid = calloc(MAP_LINES, sizeof (*map->map_grid));
    // Fill the map with empty and unvisited cells
    for (uint8_t i = 0, j; i < MAP_LINES; i++) {
        map->map_grid[i] = calloc(MAP_COLS, sizeof (*map->map_grid[i]));
        for (j = 0; j < MAP_COLS; j++) {
            if (j != MAP_COLS - 1) {
                // Right direction
                map->map_grid[i][j].distance[0] = (rand() % (MAX_DISTANCE - MIN_DISTANCE - 1)) + MIN_DISTANCE;
            }
            if (i != MAP_LINES - 1) {
                // Bottom direction
                map->map_grid[i][j].distance[1] = (rand() % (MAX_DISTANCE - MIN_DISTANCE - 1)) + MIN_DISTANCE;
            }
        }
    }
}

void map_render() {
    wattron(game.game_win, A_BOLD);
    for (uint8_t l = 0; l < MAP_LINES; l++) {
        for (uint8_t c = 0; c < MAP_COLS; c++) {
            switch (map->map_grid[l][c].cell_type) {
            case ROAD:
                mvwaddch(game.game_win, l, c,
                    ((map->map_grid[l][c].visited) ? PATH_VISITED_CHAR : ' ') | COLOR_PAIR(FORMAT_COLOR_CYAN));
                break;
            case OBSTACLE:
                wattron(game.game_win, COLOR_PAIR(FORMAT_COLOR_RED));
                mvwprintw(game.game_win, l, c, OBSTACLE_CHAR);
                wattroff(game.game_win, COLOR_PAIR(FORMAT_COLOR_RED));
                break;
            case BONUS:
                wattron(game.game_win, COLOR_PAIR(FORMAT_COLOR_GREEN));
                mvwaddstr(game.game_win, l, c, BONUS_CHAR);
                wattroff(game.game_win, COLOR_PAIR(FORMAT_COLOR_GREEN));
                break;
            case NO_BONUS:
                mvwaddch(game.game_win, l, c, ',' | COLOR_PAIR(FORMAT_COLOR_YELLOW));
                break;
            default: break;
            }
        }
    }
    mvwaddstr(game.game_win, MAP_LINES - 1, MAP_COLS - 1, FLAG_CHAR);
    wattroff(game.game_win, A_BOLD);
}

void map_display_path(Stack *stack) {
    printf("\nMAP_DISPLAY\n");
    SNode *tmp;
    for (uint8_t l = 0; l < MAP_LINES; l++) {
        for (uint8_t c = 0; c < MAP_COLS; c++) {
            if (map->map_grid[l][c].cell_type == OBSTACLE) {
                printf("\033[31;1mX\033[0m ");
            } else {
                for (tmp = stack->head; tmp; tmp = tmp->next) {
                    if (tmp->pos.l == l && tmp->pos.c == c) {
                        printf("\033[32;1m+\033[0m ");
                        break;
                    }
                }
                if (!tmp) {
                    printf(". ");
                }
            }
        }
        printf("\n");
    }
}

void map_display() {
    printf("\nMAP_DISPLAY\n");
    for (uint8_t l = 0, c; l < MAP_LINES; l++) {
        for (c = 0; c < MAP_COLS; c++) {
            if (map->map_grid[l][c].cell_type == OBSTACLE) {
                printf("\033[31;1mX\033[0m ");
            } else if (map->map_grid[l][c].cell_type == BONUS) {
                printf("\033[32;1m@\033[0m ");
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

void map_render_path(Stack *path, uint8_t color) {
    SNode *tmp = path->head;
    for (; tmp; tmp = tmp->next) {
        if (map->map_grid[tmp->pos.l][tmp->pos.c].cell_type == BONUS) {
            wattron(game.game_win, COLOR_PAIR(FORMAT_COLOR_GREEN));
            mvwaddstr(game.game_win, tmp->pos.l, tmp->pos.c, BONUS_CHAR);
            wattroff(game.game_win, COLOR_PAIR(FORMAT_COLOR_GREEN));
        } else {
            wattron(game.game_win, COLOR_PAIR(color));
            mvwaddch(game.game_win, tmp->pos.l, tmp->pos.c, PATH_REPLAY_CHAR);
            wattroff(game.game_win, COLOR_PAIR(color));
        }
    }
    wrefresh(game.game_win);
}

void map_free() {
    for (uint8_t l = 0; l < MAP_LINES; l++) {
        free(map->map_grid[l]);
    }
    free(map->map_grid);
    free(map);
}

void map_visual_reset() {
    SNode *tmp = player->history->head;
    while (tmp != NULL) {
        if (map->map_grid[tmp->pos.l][tmp->pos.c].cell_type == NO_BONUS) {
            map->map_grid[tmp->pos.l][tmp->pos.c].cell_type = BONUS;
        }
        map->map_grid[tmp->pos.l][tmp->pos.c].visited = false;
        tmp = tmp->next;
    }
}