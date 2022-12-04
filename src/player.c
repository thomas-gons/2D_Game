#include "player.h"


extern Game game;
extern Map *map;
extern Player *player;

extern int8_t moveset[MOVESET_LEN][2];

void player_init(Level level) {
    player = calloc(1, sizeof *player);
    if (!player) {
        fprintf(stderr, "[ERROR] > calloc, in func player_init\n");
        exit(2);
    }
    player->pos = (Position) {.l=0, .c=0};
    map->map_grid[player->pos.l][player->pos.c].visited = true;
    player->action = NO_ACTION;
    player->move = NO_MOVE;
    player->distance = 0;
    player->bonus_stack = 0;
    player->anim_action = false;
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
    if (player->anim_action) {
        player->anim_action = false;
        map_render();
    }
    player->action = NO_ACTION;
    player->move = NO_MOVE;
    cbreak();
    halfdelay(12);
    switch (getch()) {
    case KEY_ESC:
        // TEMP /!\ To change with lucas menus to make a gameover screen + retry button...
        game.gameover = true;
        break;
    case KEY_RIGHT:
    case 'D':
    case 'd':
        player->move = RIGHT;
        break;
    case KEY_DOWN:
    case 'S':
    case 's':
        player->move = DOWN;
        break;
    case KEY_LEFT:
    case 'Q':
    case 'q':
        player->move = LEFT;
        break;
    case KEY_UP:
    case 'Z':
    case 'z':
        player->move = UP;
        break;
    case KEY_SPACE:
        player_use_bonus();
        break;
    default: break;
    }
}

void player_update() {
    // Get the correct move coordinates
    int8_t *move = moveset[player->move];
    if (player->move == NO_MOVE || player_is_colliding(player->pos.l + move[0], player->pos.c + move[1])) {
        return;
    }
        
    // If there is no collision, player can move forward
    switch (player->move) {
    case RIGHT:
        player->distance += map->map_grid[player->pos.l][player->pos.c].distance[0];
        break;
    case DOWN:
        player->distance += map->map_grid[player->pos.l][player->pos.c].distance[1];
        break;
    case LEFT:
        player->distance += map->map_grid[player->pos.l][player->pos.c - 1].distance[0];
        break;
    case UP:
        player->distance += map->map_grid[player->pos.l - 1][player->pos.c].distance[1];
        break;
    default: break;
    }
    // Stamina cost for moving one cell
    player->stamina -= STAMINA_COST;
    // Update player position and history
    player->pos.l += move[0];
    player->pos.c += move[1];
    player_visited_cell_alert(player->pos.l, player->pos.c);
    map->map_grid[player->pos.l][player->pos.c].visited = true;
    stack_push(player->history, (Position) {.l=player->pos.l, .c=player->pos.c}, player->action);
}

bool player_is_colliding(uint8_t line, uint8_t col) {
    werase(game.alert_win);
    // Check for out of bound values
    if (IS_NOT_OUT_OF_MAP(line, col)) {         
        // If player tries to cross an obstacle, loses more stamina
        if (IS_OBSTACLE_CELL(line, col)) {
            player->anim_action = true;
            player->stamina -= STAMINA_COST_OBS;
            player->action = HIT_OBSTACLE;
            player_obstacle_alert(line, col);
        } else {
            // If player is on a bonus cell, he gains stamina or stack a bonus
            if (IS_BONUS_CELL(line, col))  {
                player_stack_bonus(line,col);
            }
        }
        return (player->anim_action == true) ? true : false;
    }
    return true;
}

void player_obstacle_alert(uint8_t line, uint8_t col) {
    // TODO: change sfx => Aie ouille
    system("aplay -q assets/sfx/fart.wav &");
    // Render the obstacle to be disctincted
    wattron(game.game_win, A_BOLD);
    wattron(game.game_win, COLOR_PAIR(FORMAT_COLOR_WHITE_BG_RED));
    mvwaddstr(game.game_win, line, col, OBSTACLE_CHAR);
    wattroff(game.game_win, COLOR_PAIR(FORMAT_COLOR_WHITE_BG_RED));
    wattron(game.game_win, A_BOLD);
    wrefresh(game.game_win);
    
    player_alert_render("You have hit an obstacle ! You lost 10 STM !");
}

void player_visited_cell_alert(uint8_t line, uint8_t col) {
    if (map->map_grid[line][col].visited == true) {
        player_alert_render("You have already visited this cell !");
        int16_t index = stack_get_index(player->history, (Position) {.l=line, .c=col});
        // If the cell that will be visit has already been visited, render all cells in history between the two same cells 
        if (index > 0) {
            player->anim_action = true;
            SNode *tmp = player->history->head;
            for (int16_t i = 0; i < index; i++) {
                switch (map->map_grid[tmp->pos.l][tmp->pos.c].cell_type) {
                case ROAD:
                    mvwaddch(game.game_win, tmp->pos.l, tmp->pos.c, PATH_VISITED_CHAR | COLOR_PAIR(FORMAT_COLOR_MAGENTA));
                    break;
                case BONUS:
                    wattron(game.game_win, COLOR_PAIR(FORMAT_COLOR_MAGENTA));
                    mvwaddstr(game.game_win, tmp->pos.l, tmp->pos.c, BONUS_CHAR);
                    wattroff(game.game_win, COLOR_PAIR(FORMAT_COLOR_MAGENTA));
                    break;
                case NO_BONUS:
                    mvwaddch(game.game_win, tmp->pos.l, tmp->pos.c, ',' | COLOR_PAIR(FORMAT_COLOR_MAGENTA));
                    break;
                default: break; 
                }
                tmp = tmp->next;
            }
        }
    }
}

void player_stack_bonus(uint8_t line, uint8_t col) {
    if (player->stamina >= STAMINA_LIMIT_TO_STACK_BONUS && player->bonus_stack < BONUS_STACK_MAX) {
        system("aplay -q assets/sfx/fruit-pickup.wav &");
        player_alert_render("You have put a bonus in your pocket ! ");
        player->bonus_stack++;
        map->map_grid[line][col].cell_type = NO_BONUS;
        player->action = STACK_BONUS;
    } else {
        system("aplay -q assets/sfx/eat-apple.wav &");
        player_alert_render("You have used a bonus ! You gained 10 STM !");
        player->stamina += STAMINA_GAIN;
        map->map_grid[line][col].cell_type = NO_BONUS;
        player->action = USE_BONUS;
    }
}

void player_use_bonus() {
    if (player->bonus_stack > BONUS_STACK_MIN) {
        system("aplay -q assets/sfx/eat-apple.wav &");
        player_alert_render("You have used a bonus from your pocket ! You gained 10 STM !");
        player->stamina += STAMINA_GAIN;
        player->bonus_stack--;
        player->action = USE_STACKED_BONUS;
        if (player->stamina > STAMINA_MAX) {
            player->stamina = STAMINA_MAX;
        }
        stack_push(player->history, (Position) {.l=player->pos.l, .c=player->pos.c}, player->action);
    }
}

void player_stats_render() {
    // Render bonus stack
    mvwprintw(game.stats_win, 0, 3, "BONUS");
    mvwprintw(game.stats_win, 0, 12, ".  .");
    for (uint8_t i = 0; i < player->bonus_stack; i++) {
        wattron(game.stats_win, A_BOLD);
        wattron(game.stats_win, COLOR_PAIR(FORMAT_COLOR_GREEN));
        mvwaddstr(game.stats_win, 2, 12 + i * 3, BONUS_CHAR);
        wattroff(game.stats_win, COLOR_PAIR(FORMAT_COLOR_GREEN));
        wattroff(game.stats_win, A_BOLD);
    }
    // Render rewind stat
    mvwprintw(game.stats_win, 2, 3, "REWIND   .");
    // Render distance stat
    mvwprintw(game.stats_win, 4, 3, "DISTANCE  %u", player->distance);
    wattroff(game.stats_win, A_BOLD);
    wrefresh(game.stats_win);
}

void player_alert_render(char *alert_msg) {
    mvwprintw(game.alert_win, 1, 2, alert_msg);
    box(game.alert_win, ACS_VLINE, ACS_HLINE);
    wrefresh(game.alert_win);
}

void player_render() {
    wattron(game.stats_win, A_BOLD);
    wattron(game.game_win, COLOR_PAIR(FORMAT_COLOR_CYAN));
    mvwaddstr(game.game_win, player->pos.l, player->pos.c, PLAYER_CHAR);
    wattroff(game.game_win, COLOR_PAIR(FORMAT_COLOR_CYAN));
    wattroff(game.stats_win, A_BOLD);
    player_stats_render();
}

void player_free() {
    stack_free(player->history);
    free(player);
}