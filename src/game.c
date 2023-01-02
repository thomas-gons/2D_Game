#include "game.h"


extern Game game;
extern SaveManager save;
extern Map *map;
extern Player *player;
extern Enemy *enemy;
extern Level difficulty;

void ncs_init() {
    initscr();
    cbreak();
    noecho();
    notimeout(stdscr, false);
    curs_set(false);
    keypad(stdscr, true);
}

void ncs_init_colors() {
    // Enable foreground colors and background colors
    use_default_colors();
    start_color();
    init_pair(FORMAT_COLOR_EMPTY, -1, -1);
    init_pair(FORMAT_COLOR_GREEN, COLOR_GREEN, -1);
    init_pair(FORMAT_COLOR_CYAN, COLOR_CYAN, -1);
    init_pair(FORMAT_COLOR_YELLOW, COLOR_YELLOW, -1);
    init_pair(FORMAT_COLOR_RED, COLOR_RED, -1);
    init_pair(FORMAT_COLOR_MAGENTA, COLOR_MAGENTA, -1);
    
    init_pair(FORMAT_BGCOLOR_GREEN, -1, COLOR_GREEN);
    init_pair(FORMAT_BGCOLOR_YELLOW, -1, COLOR_YELLOW);
    init_pair(FORMAT_BGCOLOR_RED, -1, COLOR_RED);

    init_pair(FORMAT_COLOR_WHITE_BG_RED, COLOR_WHITE, COLOR_RED);
}

void ncs_check_term_size() {
    // Get size of current terminal window
    getmaxyx(stdscr, game.win_h, game.win_w);
    if ((game.win_w < MAP_COLS + BAR_SIZE + 2) || (game.win_h < MAP_LINES + 5)) {
        ncs_quit();
        fprintf(stderr,
            "\n[ERROR] > Window is set to [%d x %d] rows x cols.\n\t> Please enlarge it to minimum [%d x %d] rows x cols.\n\t> Enlarge it to Fullscreen for better experience.\n",
            game.win_h, game.win_w, (MAP_LINES + 5), (MAP_COLS + BAR_SIZE + 2));
        exit(1);
    }
}

void ncs_create_title_window() {
    game.title_win = subwin(stdscr,
                            7,
                            75,
                            TITLE_WIN_L0,
                            TITLE_WIN_C0
    );
    box(game.title_win, ACS_VLINE, ACS_HLINE);
    wattron(game.title_win, A_BOLD);
    mvwprintw(game.title_win, 1, 1, "  ______  _______  _______  _____    _____    _______  ___ ___  ___ ___  ");
    mvwprintw(game.title_win, 2, 1, " |      ||   _   ||_     _||     |  |     |  |       ||   |   ||   |   | ");
    mvwprintw(game.title_win, 3, 1, " |   --- |       | _|   |_ |     |_ |     |_ |   |   ||   |   ||-     -| ");
    mvwprintw(game.title_win, 4, 1, " |______||___|___||_______||_______||_______||_______||_______||___|___| ");
    wrefresh(game.title_win);
}

void ncs_create_victory_window() {
    game.title_win = subwin(stdscr,
                            7,
                            60,
                            game.win_h/2 - (MAP_LINES + 2)/2 + 6,
                            game.win_w/2 - (MAP_COLS + BAR_SIZE + 2)/2 + 22
    );
    wattron(game.title_win, A_BOLD);
    mvwprintw(game.title_win, 1, 1, " __ __  _____  _____  _____  _____  _____  __ __    _");
    mvwprintw(game.title_win, 2, 1, "|  |  ||     ||     ||_   _||     || __  ||  |  |  | |");
    mvwprintw(game.title_win, 3, 1, "|  |  / |   | |  ---   | |  |  |  ||    -||_   _|  \\ /");
    mvwprintw(game.title_win, 4, 1, "|____/ |_____||_____|  |_|  |_____||__|__|  |_|     O");
    wrefresh(game.title_win);
}

void ncs_create_game_windows() {
    // Create main window
    game.main_win = subwin( stdscr,
                            MAP_LINES + 2,
                            MAP_COLS + BAR_SIZE + 2,
                            MAIN_WIN_L0,
                            MAIN_WIN_C0
    );
    // Create all sub windows
    game.game_win = subwin( stdscr,
                            MAP_LINES,
                            MAP_COLS,
                            GAME_WIN_L0,
                            GAME_WIN_C0
    );
    game.bar_win = subwin(  stdscr,
                            MAP_LINES - DIST_SIZE + 3,
                            BAR_SIZE + 1,
                            BAR_WIN_L0,
                            BAR_WIN_C0
    );
    game.stm_bar = subwin(  stdscr,
                            STM_BAR_SIZE,
                            STM_BAR_WIDTH,
                            STM_BAR_L0,
                            STM_BAR_C0
    );
    game.stats_win = subwin(stdscr,
                            6,
                            BAR_SIZE + 1,
                            DIST_WIN_L0 - 5,
                            BAR_WIN_C0
    );
    game.dist_win = subwin( stdscr,
                            DIST_SIZE + 3,
                            BAR_SIZE + 1,
                            DIST_WIN_L0,
                            DIST_WIN_C0
    );
    game.alert_win = subwin(stdscr,
                            4,
                            MAP_COLS + 2,
                            ALERT_WIN_L0,
                            ALERT_WIN_C0
    );
}

void ncs_refresh_game_windows() {
    // Render border for game subwindows
    box(game.main_win, ACS_VLINE, ACS_HLINE);
    box(game.bar_win, ACS_VLINE, ACS_HLINE);
    box(game.stm_bar, ACS_VLINE, ACS_HLINE);
    box(game.dist_win, ACS_VLINE, ACS_HLINE);
    box(game.alert_win, ACS_VLINE, ACS_HLINE);
    // Render game windows
    wrefresh(game.main_win);
    wrefresh(game.game_win);
    wrefresh(game.bar_win);
    wrefresh(game.stm_bar);
    wrefresh(game.stats_win);
    wrefresh(game.dist_win);
    wrefresh(game.alert_win);
}

void ncs_print_centered(WINDOW *win, uint8_t line, const char *msg) {
    uint8_t half_len = (uint8_t) strlen(msg) / 2;
    uint8_t ajusted_col = (uint8_t) (win->_maxx / 2) - half_len;
    mvwprintw(win, line, ajusted_col, msg);
}

void ncs_destroy_win(WINDOW *win) {
    werase(win);
    delwin(win);
}

void ncs_quit() {
    ncs_destroy_win(stdscr);
    endwin();
}

void run_game() {
    time_t begin;
    time_t end;
    game_init();
    // Start menu entries that handle going back to previous menu
    int8_t select = 0;
    do {
        select = game_start_menu();
    } while (select == -1);
    // Process selected menu entry
    switch (select) {
    case 0:     // New Game
        begin = time(NULL);
        game_init_new_game(difficulty);
        game.keep_playing = true;
        while (game.keep_playing == true) {
            // Main game loop
            game.victory = false;
            game.gameover = false;
            game_loop();
            // Get played time
            end = time(NULL);
            save.playing_time = end - begin; 
        }
        game_free();
        break;
    case 1:     // Load Saved Game
        // TODO: load game resources from a save file (.save)
        // game_load_saved_game();
        break;
    case 2:     // History
        // TODO: history menu to select a saved game (.dat) and replay it
        break;
    case 3:     // Quit
        game.reload_game = false;
        break;
    default: break;
    }    
    game_quit();
}

void game_init() {
    // Initialize ncurses resources
    ncs_init();
    ncs_check_term_size();
    ncs_init_colors();
    ncs_create_title_window();
}

int8_t game_start_menu() {
    // Title and Start menu, select an entry
    ncs_create_title_window();
    const uint8_t nb_entry = 4;
    char *start_list[] = { "New Game", "Load Saved Game", "History", "Quit" };
    menu_create_entry_template(start_list, nb_entry);
    int8_t select = menu_select_entry(start_list, nb_entry);
    switch (select) {
    case 0:     // New Game
        ncs_destroy_win(game.menu_win);
        if (game_difficulty_menu() == -1) {
            // Go back to previous menu
            select = -1;
        }
        break;
    case 1:     // Load Saved Game
        // TODO: menu to select a saved but unfinished game (*.save)
        //       and return index of selected file in list
        break;
    case 2:     // History
        // TODO: menu to select a saved and finished game (*.dat)
        //       and return index of selected file in list
        break;
    case 3:     // Quit sfx
        system("aplay -q assets/sfx/fart.wav &");
        break;
    }
    ncs_destroy_win(game.title_win);
    ncs_destroy_win(game.menu_win);
    return select;
}

int8_t game_difficulty_menu() {
    // Difficulty menu, select a difficulty
    const uint8_t nb_entry = 4;
    char *difficulty_list[] = { "Easy", "Medium", "Hard", "Return to Title Menu" };
    menu_create_entry_template(difficulty_list, nb_entry);
    int8_t select = menu_select_entry(difficulty_list, nb_entry);
    switch (select) {
    case 3:     // Return to Title Menu
        select = -1;
        break;
    default:    // Set game difficulty
        difficulty = select + 1;
        break;
    }
    return select;
}

void game_init_new_game(Level difficulty) {
    // Create game subwindows
    ncs_create_game_windows();
    // Initialize map according the difficulty
    map_init(difficulty);
    // Initialize player entity
    player_init(map->level);
    // Generate random map
    game.path_stm = map_generate();
    game.path_dist = a_star(START, GOAL, false);
    // Initialize enemies entities
    enemy_init();
    // First render of game
    game_render();
}

void game_loop() {
    system("aplay -q assets/sfx/among-us.wav &");
    // Main loop that handle game logic
    while (!game.victory && !game.gameover) {
        game_inputs();
        // Condition to exit the game loop
        if (!game.keep_playing || !game.reload_game) {
            return;
        }
        game_update();
        game_render();
        game_check_win();
        // Limit framerate to 16ms per frame ~ 60fps
        usleep(16000);
    }
    // Render end game title and menu, victory or defeat
    if (game.victory == true) {
        game_victory_menu();
    } else {
        game_defeat_menu();
    }
}

void game_inputs() {
    player_inputs();
}

void game_update() {
    player_update();
}

void game_render() {
    if (player->anim_action == false) {
        map_render();
    }
    player_render();
    stamina_render();
    distances_render();
    ncs_refresh_game_windows();
}

void game_check_win() {
    // Check for player stamina level
    if (player->stamina <= STAMINA_MIN) {
        game.victory = false;
        game.gameover = true;
        return;
    }
    // Check if player has reach the goal cell
    if ((player->pos.l == MAP_LINES - 1) && (player->pos.c == MAP_COLS - 1)) {
        // TODO: change sfx
        system("aplay -q assets/sfx/youu.wav &");
        game.victory = true;
        game.gameover = false;
    }
}

void game_victory_menu() {
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
    // Save the game before processing the selected entry form menu
    save_game(".dat");
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

void game_defeat_menu() {
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

void game_pause_menu() {
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
        save_game(".save");
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

void game_restart() {
    map_visual_reset();
    player_free();
    player_init(map->level);
    for (uint8_t i = 0; i < ENEMY_NB; i++) {
        enemy[i].current.l = enemy[i].house.l;
        enemy[i].current.c = enemy[i].house.c;
    }
}

void game_free() {
    stack_free(game.path_stm);
    stack_free(game.path_dist);
    map_free();
    player_free();
    free(enemy);
}

void game_quit() {
    ncs_quit();
}