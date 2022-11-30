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
    player->action = NO_ACTION;
    map->map_grid[player->pos.l][player->pos.c].visited = true;
    player->move = NONE;
    player->distance = 0;
    player->fruit_stack = 0;
    player->skip_map_render = false;
    player->history = stack_init();
    stack_push(player->history, player->pos, player->action);
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
    player->skip_map_render = false;
    player->action = NO_ACTION;
    player->move = NONE;
    switch (getch()) {
    case KEY_ESC:

        // TEMP /!\ To change with lucas menus to make a gameover screen + retry button...
        game.gameover = true;
        
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
    case KEY_SPACE:
        player_eat_fruit();
        break;
    default: break;
    }
    werase(game.alert_win);
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
        // If player tries to cross an obstacle, loses more stamina
        if (IS_OBSTACLE_CELL(line, col)) {
            player->skip_map_render = true;
            player->stamina -= STAMINA_COST_OBS;
            player->action = HIT_OBSTACLE;
            player_obstacle_alert(line, col);
        } else {
            // If player is on a fruit, he gains stamina
            if (IS_FRUIT_CELL(line, col)) {
                player_stack_fruit(line,col);
            }
            // Update player position in map
            player_visited_cell_alert(line, col);
            player->pos.l = line;
            player->pos.c = col;
            map->map_grid[line][col].visited = true;
        }
        // Stamina cost for moving one cell
        player->stamina -= STAMINA_COST;
        // Update player history
        stack_push(player->history, (Position) {.l=line, .c=col}, player->action);
        return (player->skip_map_render == true) ? true : false;
    }
    return true;
}

void player_obstacle_alert(uint8_t line, uint8_t col) {
    // TODO: change sfx => Aie ouille
    system("aplay -q assets/sfx/fart.wav &");
    // Render the obstacle to be disctincted
    mvwaddch(game.game_win, line, col, 'X' | COLOR_PAIR(FORMAT_COLOR_WHITE_BG_RED));
    wrefresh(game.game_win);
    
    player_alert_render("You have hit an obstacle ! You lost 10 STM !");
}

void player_visited_cell_alert(uint8_t line, uint8_t col) {
    if (map->map_grid[line][col].visited == true) {
        player_alert_render("You have already visited this cell !");
        int16_t index = stack_get_index(player->history, (Position) {.l=line, .c=col});
        // If the cell that will be visit has already been visited, render all cells in history between the two same cells 
        if (index > 0) {
            // TODO: make this player variable generic => skip map_render to let color changes appear on screen
            player->skip_map_render = true;
            Node *tmp = player->history->head;
            for (int16_t i = 0; i < index; i++) {
                switch (map->map_grid[tmp->pos.l][tmp->pos.c].cell_type) {
                case ROAD:
                    mvwaddch(game.game_win, tmp->pos.l, tmp->pos.c, '.' | COLOR_PAIR(FORMAT_COLOR_MAGENTA));
                    break;
                case FRUIT:
                    mvwaddch(game.game_win, tmp->pos.l, tmp->pos.c, '@' | COLOR_PAIR(FORMAT_COLOR_MAGENTA));
                    break;
                case NO_FRUIT:
                    mvwaddch(game.game_win, tmp->pos.l, tmp->pos.c, ',' | COLOR_PAIR(FORMAT_COLOR_MAGENTA));
                    break;
                default: break; 
                }
                tmp = tmp->next;
            }
        }
    }
}

void player_stack_fruit(uint8_t line, uint8_t col) {
    if (player->stamina >= STAMINA_LIMIT_TO_STACK_FRUITS) {
        if (player->fruit_stack < FRUIT_STACK_MAX) {
            system("aplay -q assets/sfx/fruit-pickup.wav &");
            player->fruit_stack++;
            map->map_grid[line][col].cell_type = NO_FRUIT;
            player->action = STACK_FRUIT;
            
            player_alert_render("You have put a fruit in your pocket ! ");
        }
    } else {
        system("aplay -q assets/sfx/eat-apple.wav &");
        player->stamina += STAMINA_GAIN;
        map->map_grid[line][col].cell_type = NO_FRUIT;
        player->action = EAT_FRUIT;
        
        player_alert_render("You have eaten a fruit ! You gained 10 STM !");
    }
}

void player_eat_fruit() {
    if (player->fruit_stack > FRUIT_STACK_MIN) {
        system("aplay -q assets/sfx/eat-apple.wav &");
        player->stamina += STAMINA_GAIN;
        player->fruit_stack--;
        player->action = EAT_STACKED_FRUIT;
        if (player->stamina > STAMINA_MAX) {
            player->stamina = STAMINA_MAX;
        }
        stack_push(player->history, (Position) {.l=player->pos.l, .c=player->pos.c}, player->action);
        player_alert_render("You have eaten a fruit from your pocket ! You gained 10 STM !");
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
    // Render distance stat
    mvwprintw(game.stats_win, 2, 3, "DISTANCE  %u", player->distance);
    // Render rewind stat
    mvwprintw(game.stats_win, 4, 3, "REWIND   .");
}

void player_alert_render(char *alert_msg) {
    mvwprintw(game.alert_win, 1, 1, alert_msg);
    box(game.alert_win, ACS_VLINE, ACS_HLINE);
    wrefresh(game.alert_win);
}

void player_render() {
    // wattron(game.stats_win, A_BOLD);
    // mvwaddch(game.game_win, player->pos.l, player->pos.c, '&' | COLOR_PAIR(FORMAT_COLOR_CYAN));
    // wattroff(game.stats_win, A_BOLD);
    wattron(game.game_win, COLOR_PAIR(FORMAT_COLOR_CYAN));
    mvwprintw(game.game_win, player->pos.l, player->pos.c, "\U0001F397");   //mdedal 1F396; ribbon 1F397; star 2726; chessking 265A;
    wattroff(game.game_win, COLOR_PAIR(FORMAT_COLOR_CYAN));
    player_stats_render();
}

void player_free() {
    stack_free(player->history);
    free(player);
}