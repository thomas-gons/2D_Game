#include "menus.h"


extern Game game;
extern Map *map;
extern Player *player;
extern Level difficulty;

void menu_create_entry_template(char **entry_list, int nb_entry) {
    // Create the menu window
    game.menu_win = subwin( stdscr,
                            nb_entry + 4,
                            (uint8_t) (MAP_COLS + BAR_SIZE + 2)/2,
                            game.win_h/2 - (MAP_LINES + 2)/2 + 15,
                            game.win_w/2 - (MAP_COLS + BAR_SIZE + 2)/2 + 25
    );
    // Render menu entries
    for(uint8_t i = 0; i < nb_entry; i++ ) {
        // Highlight the current menu entry
        if (i == 0) {
            wattron(game.menu_win, A_STANDOUT); 
        }
        else {
            wattroff(game.menu_win, A_STANDOUT);
        }
        ncs_print_centered(game.menu_win, i + 2, entry_list[i]);
    }
    box(game.menu_win, ACS_VLINE, ACS_HLINE);
    wrefresh(game.menu_win);
}

int8_t menu_select_entry(char **entry_list, int nb_entry) {
    // Input loop to select menu entry
    int ch;
    int8_t i = 0;
    while ((ch = getch()) != KEY_ENTR) {
        ncs_print_centered(game.menu_win, i + 2, entry_list[i]);
        switch (ch) {
        case KEY_UP:
        case 'Z':
        case 'z':
            system("aplay -q assets/sfx/menu-entry.wav &");
            i--;
            if (i < 0) {
                i = nb_entry - 1;
            }
            break;
        case KEY_DOWN:
        case 'S':
        case 's':
            system("aplay -q assets/sfx/menu-entry.wav &");
            i++;
            if (i >= nb_entry) {
                i = 0;
            }
            break;
        default: break;
        }
        // Refresh rendering of menu entries
        wattron(game.menu_win, A_STANDOUT );
        ncs_print_centered(game.menu_win, i + 2, entry_list[i]);
        wattroff(game.menu_win, A_STANDOUT );
        wrefresh(game.menu_win );
    }
    return i;
}

int8_t menu_difficulty() {
    // Difficulty menu, select a difficulty
    const uint8_t nb_entry = 4;
    char *difficulty_list[] = { "Easy", "Medium", "Hard", "Return to Title Menu" };
    menu_create_entry_template(difficulty_list, nb_entry);
    int8_t select = menu_select_entry(difficulty_list, nb_entry);
    switch(select) {
    case 3:     // Return to Title Menu
        select = -1;
        break;
    default:    // Set game difficulty
        difficulty = select + 1;
        break;
    }
    return select;
}

void menu_victory() {
    // Clear current render of the game
    ncs_destroy_win(game.main_win);
    ncs_destroy_win(game.game_win);
    ncs_destroy_win(game.bar_win);
    ncs_destroy_win(game.dist_win);
    ncs_destroy_win(game.alert_win);
    // Victory title and menu, select an entry
    ncs_create_victory_window();
    const uint8_t nb_entry = 2;
    char *victory_list[] = { "Return to Title Menu", "Quit" };
    menu_create_entry_template(victory_list, nb_entry);
    uint8_t select = menu_select_entry(victory_list, nb_entry);

    // TODO: save the game before processing the selected entry form menu
    
    switch(select) {
    case 1:     // Quit
        game.reload_game = false;
        break;
    default: break;
    }
    game.keep_playing = false;
    ncs_destroy_win(game.title_win);
    ncs_destroy_win(game.menu_win);
}

void menu_defeat() {
    // // Clear current render of the game
    werase(game.main_win);
    werase(game.game_win);
    werase(game.bar_win);
    werase(game.dist_win);
    werase(game.alert_win);
    // Defeat menu, select an entry
    const uint8_t nb_entry = 3;
    char *defeat_list[] = { "Restart Game", "Return to Title Menu", "Quit" };
    menu_create_entry_template(defeat_list, nb_entry);
    uint8_t select = menu_select_entry(defeat_list, nb_entry);
    switch (select) {
    case 0:     // Restart Game
        game_restart();
        break;
    case 1:     // Return to Title Menu
        game.keep_playing = false;
        break;
    case 2:     // Quit
        game.keep_playing = false;
        game.reload_game = false;
        break;
    default: break;
    }
    ncs_destroy_win(game.menu_win);
}

void menu_pause() {
    // Clear current render under the menu
    werase(game.game_win);
    werase(game.bar_win);
    werase(game.dist_win);
    werase(game.alert_win);
    refresh();
    // In-game Pause menu, select an entry
    const uint8_t nb_entry = 4;
    char *esc_list[] = { "Resume Game", "Help & Game Rules", "Save & Quit", "Quit" };
    menu_create_entry_template(esc_list, nb_entry);
    uint8_t select = menu_select_entry(esc_list, nb_entry);
    switch(select) {
    case 1:     // Help & Game Rules
        // TODO: render Help and Rules window
        // game_help_rules();
        break;
    case 2:     // Save & Quit
        // TODO: call save function
        game.keep_playing = false;
        break;
    case 3:     // Quit
        game.keep_playing = false;
        game.reload_game = false;
        break;
    default: break;
    }
    ncs_destroy_win(game.menu_win);
}

void distances_render() {
    // Set character to obstacle char if player is near an obstacle
    int8_t right = (player->pos.c == MAP_COLS - 1) ? 0 : 
        (IS_OBSTACLE_CELL(player->pos.l, player->pos.c + 1)) ? -1 : map->map_grid[player->pos.l][player->pos.c].distance[0];
    int8_t down = (player->pos.l == MAP_LINES - 1) ? 0 : 
        (IS_OBSTACLE_CELL(player->pos.l + 1, player->pos.c)) ? -1 : map->map_grid[player->pos.l][player->pos.c].distance[1];
    int8_t left = (player->pos.c == 0) ? 0 : 
        (IS_OBSTACLE_CELL(player->pos.l, player->pos.c - 1)) ? -1 : map->map_grid[player->pos.l][player->pos.c - 1].distance[0];
    int8_t up = (player->pos.l == 0) ? 0 : 
        (IS_OBSTACLE_CELL(player->pos.l - 1, player->pos.c)) ? -1 : map->map_grid[player->pos.l - 1][player->pos.c].distance[1];
    // Render player character at center
    wattron(game.stats_win, A_BOLD);
    wattron(game.dist_win, COLOR_PAIR(FORMAT_COLOR_CYAN));
    mvwaddstr(game.dist_win, 6, BAR_SIZE / 2, PLAYER_CHAR);
    wattroff(game.dist_win, COLOR_PAIR(FORMAT_COLOR_CYAN));
    wattroff(game.stats_win, A_BOLD);

    // Render directional arrows and their distance value
    mvwprintw(game.dist_win, 6, (BAR_SIZE / 2) + 5, "  ");
    render_dist_obstacle(6, (BAR_SIZE / 2) + 6, right);
    mvwprintw(game.dist_win, 6, (BAR_SIZE / 2) + 3, "🠲");
    
    mvwprintw(game.dist_win, 10, BAR_SIZE / 2, "  ");
    render_dist_obstacle(10, BAR_SIZE / 2, down);
    mvwprintw(game.dist_win, 8, BAR_SIZE / 2, "🠳");
    
    mvwprintw(game.dist_win, 6, (BAR_SIZE / 2) - 7, "  ");
    render_dist_obstacle(6, (BAR_SIZE / 2) - ((left / 10) ? 7 : 6), left);
    mvwprintw(game.dist_win, 6, (BAR_SIZE / 2) - 4 , "🠰");

    mvwprintw(game.dist_win, 2, BAR_SIZE / 2, "  ");
    render_dist_obstacle(2, BAR_SIZE / 2, up);
    mvwprintw(game.dist_win, 4, BAR_SIZE / 2, "🠱");
}

void render_dist_obstacle(uint8_t line, uint8_t col, int8_t render_dist) {
    wattron(game.dist_win, A_BOLD);
    if (render_dist == -1) {
        // Render obstacle character
        wattron(game.dist_win, COLOR_PAIR(FORMAT_COLOR_RED));
        mvwprintw(game.dist_win, line, col, OBSTACLE_CHAR);
        wattroff(game.dist_win, COLOR_PAIR(FORMAT_COLOR_RED));
    } else {
        // Render distance value
        mvwprintw(game.dist_win, line, col, "%hhd", render_dist);
    }
    wattroff(game.dist_win, A_BOLD);
}