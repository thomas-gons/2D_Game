#include "menus.h"


extern Game game;
extern SaveManager save;
extern Map *map;
extern Player *player;
extern Level level;

void menu_create_entry_template(char **entry_list, int nb_entry, bool centered, bool ingame) {
    // Create the menu window
    uint8_t pad_l = (ingame == true) ? 3: 0;
    uint8_t pad_c = (ingame == true) ? 9: 0;
    game.menu_win = subwin( stdscr,
                            nb_entry + 4,
                            (uint8_t) (MAP_COLS + BAR_SIZE + 2)/2,
                            game.win_h/2 - (MAP_LINES + 2)/2 + 15 - pad_l,
                            game.win_w/2 - (MAP_COLS + BAR_SIZE + 2)/2 + 25 - pad_c
    );
    // Render menu entries
    box(game.menu_win, ACS_VLINE, ACS_HLINE);
    for (uint8_t i = 0; i < nb_entry; i++ ) {
        // Highlight the current menu entry
        if (i == 0) {
            wattron(game.menu_win, A_STANDOUT); 
        }
        else {
            wattroff(game.menu_win, A_STANDOUT);
        }
        // Center the text
        if (centered == true) {
            ncs_print_centered(game.menu_win, i + 2, entry_list[i]);
        } else {
            mvwprintw(game.menu_win, i + 2, 2, entry_list[i]);
        }
    }
    wrefresh(game.menu_win);
}

int8_t menu_select_entry(char **entry_list, int nb_entry, bool centered) {
    // Input loop to select menu entry
    int ch;
    int8_t i = 0;
    while ((ch = getch()) != KEY_ENTR) {
        if (centered == true) {
            ncs_print_centered(game.menu_win, i + 2, entry_list[i]);
        } else {
            mvwprintw(game.menu_win, i + 2, 2, entry_list[i]);
        }
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
        if (centered == true) {
            ncs_print_centered(game.menu_win, i + 2, entry_list[i]);
        } else {
            mvwprintw(game.menu_win, i + 2, 2, entry_list[i]);
        }
        wattroff(game.menu_win, A_STANDOUT );
        wrefresh(game.menu_win );
    }
    return i;
}

int8_t menu_level() {
    // Clear current render of menus
    ncs_destroy_win(game.menu_win);
    // Set menu entries
    const uint8_t nb_entry = 4;
    char *level_list[] = { "Easy", "Medium", "Hard", "Return to Title Menu" };
    // level menu, select a level
    menu_create_entry_template(level_list, nb_entry, true, false);
    int8_t select = menu_select_entry(level_list, nb_entry, true);
    if (select == nb_entry - 1) {
        // Return to Title Menu
        select = -1;
        system("aplay -q assets/sfx/fart.wav &");
    } else {
        // Set game level
        level = select + 1;
    }
    return select;
}

int8_t menu_select_file(char **arr_files, uint8_t arr_len) {
    // Clear current render of menus
    ncs_destroy_win(game.menu_win);
    // Save file menu, select a save file
    int8_t select;
    if (arr_len == 1) {
        menu_create_entry_template(arr_files, arr_len, true, false);
        select = menu_select_entry(arr_files, arr_len, true);
    } else {
        menu_create_entry_template(arr_files, arr_len, false, false);
        select = menu_select_entry(arr_files, arr_len, false);
    }
    if (select == arr_len - 1) {
        // Return to Title Menu
        select = -1;
        system("aplay -q assets/sfx/fart.wav &");
    }
    return select;
}

void menu_victory() {
    system("aplay -q assets/sfx/victory.wav &");
    // Clear current render of the game
    wclear(game.main_win);
    wclear(game.game_win);
    wclear(game.bar_win);
    wclear(game.dist_win);
    wclear(game.alert_win);
    refresh();
    ncs_create_victory_window();
    // Set menu entries
    const uint8_t nb_entry = 2;
    char *array_victory[] = { "Return to Title Menu", "Quit" };
    // Victory title and menu, select an entry
    menu_create_entry_template(array_victory, nb_entry, true, true);
    int8_t select = menu_select_entry(array_victory, nb_entry, true);
    // Save the game before processing the selected entry form menu
    save_game(HIST_EXT);
    if (select == nb_entry - 1) {
        // Quit
        game.reload_game = false;
    }
    game.keep_playing = false;
    ncs_destroy_win(game.title_win);
    ncs_destroy_win(game.menu_win);
}

void menu_defeat() {
    system("aplay -q assets/sfx/defeat.wav &");
    // Clear current render of the game
    wclear(game.main_win);
    wclear(game.game_win);
    wclear(game.bar_win);
    wclear(game.dist_win);
    wclear(game.alert_win);
    refresh();
    ncs_create_defeat_window();
    // Set menu entries
    const uint8_t nb_entry = 3;
    char *array_defeat[] = { "Restart Game", "Return to Title Menu", "Quit" };
    // Defeat menu, select an entry
    menu_create_entry_template(array_defeat, nb_entry, true, true);
    uint8_t select = menu_select_entry(array_defeat, nb_entry, true);
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
    ncs_destroy_win(game.title_win);
    ncs_destroy_win(game.menu_win);
}

void menu_pause() {
    // Clear current render under the menu
    werase(game.game_win);
    werase(game.bar_win);
    werase(game.dist_win);
    werase(game.alert_win);
    refresh();
    // Set menu entries
    const uint8_t nb_entry = 3;
    char *array_pause[] = { "Resume Game", "Save & Quit", "Quit" };
    // In-game Pause menu, select an entry
    menu_create_entry_template(array_pause, nb_entry, true, true);
    uint8_t select = menu_select_entry(array_pause, nb_entry, true);
    switch(select) {
    case 1:     // Save & Quit
        // Get played time
        game.end = time(NULL);
        save.play_time = game.end - game.begin;
        save_game(SAVE_EXT);
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