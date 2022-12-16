#include "player.h"


extern Game game;
extern Map *map;
extern Player *player;

void player_init(Level level) {
    player = calloc(1, sizeof *player);
    if (!player) {
        fprintf(stderr, "[ERROR] > calloc, in func player_init\n");
        exit(2);
    }
    player->pos = (Position) {.l=0, .c=0};
    map->map_grid[player->pos.l][player->pos.c].visited = true;
    player->move = NONE;
    player->distance = 0;
    player->fruit_stack = 0;
    player->on_obstacle = false;
    player->history = stack_init();
    switch (level) {
    case EASY:
        player->stamina = STAMINA_EASY;
        break;
    case MEDIUM:
        player->stamina = STAMINA_MEDIUM;
        break;
    case HARD:
        player->stamina = STAMINA_HARD;
        break;
    default: break;
    }
}

void player_inputs() {
    player->on_obstacle = false;
    player->move = NONE;
    switch (getch()) {
    case KEY_ESC:
        game_pause_menu();
        break;
    case KEY_DOWN:
    case 'S':
    case 's':
        player->move = DOWN;
        break;
    case KEY_RIGHT:
    case 'D':
    case 'd':
        player->move = RIGHT;
        break;
    case KEY_UP:
    case 'Z':
    case 'z':
        player->move = UP;
        break;
    case KEY_LEFT:
    case 'Q':
    case 'q':
        player->move = LEFT;
        break;
    // case 'E':
    // case 'e':
    case KEY_SPACE:
        player_eat_fruit();
        break;
    default: break;
    }
}

void player_update() {
    switch (player->move) {
    case RIGHT:
        if (!player_check_collisions(player->pos.l, player->pos.c + 1))
            player->distance += (player->pos.c - 1 >= MAP_COLS - 1) ? 0 : map->map_grid[player->pos.l][player->pos.c - 1].distance[0];
        break;
    case DOWN:
        if (!player_check_collisions(player->pos.l + 1, player->pos.c)) 
            player->distance += (player->pos.l - 1 >= MAP_LINES - 1) ? 0 : map->map_grid[player->pos.l - 1][player->pos.c].distance[1];
        break;
    case LEFT:
        if (!player_check_collisions(player->pos.l, player->pos.c - 1))
            player->distance += (player->pos.c < 0) ? 0 : map->map_grid[player->pos.l][player->pos.c].distance[0];
        break;
    case UP:
        if (!player_check_collisions(player->pos.l - 1, player->pos.c))
            player->distance += (player->pos.l < 0) ? 0 : map->map_grid[player->pos.l][player->pos.c].distance[1];
        break;
    default: break;
    }
}

bool player_check_collisions(uint8_t line, uint8_t col) {
    // Check for out of bound values
    if (IS_OUT_OF_MAP(line, col)) {         
        // If player tries to cross an obstacle, loses way more stamina
        if (IS_OBSTACLE_CELL(line, col)) {
            player->on_obstacle = true;
            player->stamina -= STAMINA_COST_OBS;
            player_obstacle_alert(line, col);
        } else {
            // If player is on a fruit, he gains stamina
            if (IS_FRUIT_CELL(line, col)) {
                player_stack_fruit(line,col);
            }
            // Update player position in map
            player->pos.l = line;
            player->pos.c = col;
            map->map_grid[line][col].visited = true;
            stack_push(player->history, (Position) {.l=line, .c=col});
        }
        // Stamina cost for moving one cell
        player->stamina -= STAMINA_COST;
        return (player->on_obstacle == true) ? true : false;
    }
    return true;
}

void player_obstacle_alert(uint8_t line, uint8_t col) {
    // TODO: change sfx => Aie ouille
    system("aplay -q assets/sfx/caillou.wav &");

    mvwaddch(game.game_win, line, col, 'X' | COLOR_PAIR(FORMAT_COLOR_WHITE_BG_RED));
    wrefresh(game.game_win);

    // TODO: add an alert to block input => give an input to delete alert
    usleep(200000);
}

void player_stack_fruit(uint8_t line, uint8_t col) {
    if (player->stamina >= STAMINA_LIMIT_TO_STACK_FRUITS) {
        if (player->fruit_stack < FRUIT_STACK_MAX) {
            system("aplay -q assets/sfx/fruit-pickup.wav &");
            player->fruit_stack++;
            map->map_grid[line][col].cell_type = NO_FRUIT;
        }
    } else {
        system("aplay -q assets/sfx/eat-apple.wav &");
        player->stamina += STAMINA_GAIN;
        map->map_grid[line][col].cell_type = NO_FRUIT;
    }
}

void player_eat_fruit() {
    if (player->fruit_stack > FRUIT_STACK_MIN) {
        system("aplay -q assets/sfx/eat-apple.wav &");
        player->stamina += STAMINA_GAIN;
        player->fruit_stack--;
        if (player->stamina > STAMINA_MAX) {
            player->stamina = STAMINA_MAX;
        }
    }
}

void player_stats_render() {
    // Render fruit stack
    mvwprintw(game.stats_win, 0, 3, "FRUITS");
    mvwprintw(game.stats_win, 0, 12, ".  .");
    for (uint8_t i = 0; i < player->fruit_stack; i++) {
        wattron(game.stats_win, A_BOLD);
        mvwaddch(game.stats_win, 0, 12 + i * 3, '@' | COLOR_PAIR(FORMAT_COLOR_GREEN));
        wattroff(game.stats_win, A_BOLD);
    }
    // render distance stat
    mvwprintw(game.stats_win, 2, 3, "DISTANCE  %u", player->distance);
}

void player_render() {
    wattron(game.stats_win, A_BOLD);
    mvwaddch(game.game_win, player->pos.l, player->pos.c, '&' | COLOR_PAIR(FORMAT_COLOR_CYAN));
    wattroff(game.stats_win, A_BOLD);
    player_stats_render();
}

void player_free() {
    stack_free(player->history);
    free(player);
}