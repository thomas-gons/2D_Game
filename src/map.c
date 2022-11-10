#include "map.h"


extern Map *map;

void map_init(Level level) {
    map = calloc(1, sizeof *map);
    *map = (Map) {level, 0, NULL};
    map->map_grid = calloc(MAP_LINES, sizeof *map->map_grid);
    // Fill the map with empty and unvisited cells
    for (uint8_t i = 0; i < MAP_LINES; i++)
        map->map_grid[i] = calloc(MAP_COLS, sizeof *map->map_grid[i]);
        
}

void map_render(WINDOW *win) {
    // Enable foreground color and disable background color
    use_default_colors();
    start_color();
    init_pair(FORMAT_COLOR_FRUIT, COLOR_GREEN, -1);
    // if (can_change_color())
    //     init_color(COLOR_YELLOW, 1000, 651, 0);
    
    for (uint8_t i = 0, j; i < MAP_LINES; i++) {
        for (j = 0; j < MAP_COLS; j++) {
            switch (map->map_grid[i][j].cell_type) {
            case ROAD:
                mvwaddch(win, i, j, ' ');
                break;
            case OBSTACLE:
                // mvwaddstr(win, i, j, OBS_SYMBOL);
                mvwaddch(win, i, j, '#');
                break;
            case FRUIT:
                mvwaddch(win, i, j, '@' | COLOR_PAIR(FORMAT_COLOR_FRUIT));
                break;
            default: break;
            }
        }
    }
    wrefresh(win);
}



void map_free() {
    for (uint8_t i = 0; i < MAP_LINES; i++)
        free(map->map_grid[i]);
    free(map->map_grid);
}

void map_display() {
    for (uint8_t i = 0, j; i < MAP_LINES; i++) {
        for (j = 0; j < MAP_COLS; j++) {
            switch (map->map_grid[i][j].cell_type) {
                case ROAD:
                    printf("0 "); break;
                case FRUIT:
                    printf("2 "); break;
                default:
                    printf("1 "); break;
            }
        }
        printf("\n");
    }
}

void map_display_path_building(Stack *path){
    Node *tmp = path->head;
    for (uint8_t i = 0, j; i < MAP_LINES; i++) {
        for (j = 0; j < MAP_COLS; j++) {
            switch (map->map_grid[i][j].cell_type) {
            case OBSTACLE: 
                printf("\033[1;31m1\033[0m");
                break;
            case FRUIT:
                printf("@");
                break;
            default:
                for (; tmp; tmp = tmp->next) {
                    if (tmp->pos.x == i && tmp->pos.y == j) {
                        printf("\033[1;32m0\033[0m");
                        break;
                    }
                }
                if (!tmp)
                    printf("0");
                
                tmp = path->head;
                break;
            }
        }
        printf("\n");
    }
    printf("\n\n");
}