#include "game.h"


extern Game game;
extern Map *map;
extern Player *player;
extern SaveManager save;

void ncs_init() {
    initscr();
    // Don't echo the pressed key and hide the cursor
    noecho();
    curs_set(false);
    keypad(stdscr, true);
    scrollok(stdscr, false);
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
    if ((game.win_w < MAP_COLS + BAR_SIZE + 2) || (game.win_h < MAP_LINES + 2)) {
        ncs_quit();
        fprintf(stderr,
            "[ERROR] > Window is set to %d rows * %d cols.\n\t> Please enlarge it %d rows * %d cols minimum,\n\t> or to fullscreen.\n",
            game.win_h, game.win_w, (MAP_LINES + 2), (MAP_COLS + BAR_SIZE + 2));
        exit(1);
    }
}

void ncs_create_title_window() {
    game.title_win = subwin(stdscr,
                            7,
                            75,
                            game.win_h/2 - (MAP_LINES + 2)/2 + 6,
                            game.win_w/2 - (MAP_COLS + BAR_SIZE + 2)/2 + 13
    );
    box(game.title_win, ACS_VLINE, ACS_HLINE);
    wattron(game.title_win, A_BOLD);
    mvwprintw(game.title_win, 1, 1, "  ______  _______  _______  _____    _____    _______  ___ ___  ___ ___  ");
    mvwprintw(game.title_win, 2, 1, " |      ||   _   ||_     _||     |_ |     |_ |       ||   |   ||   |   | ");
    mvwprintw(game.title_win, 3, 1, " |   --- |       | _|   |_ |       ||       ||   -   ||   |   ||-     -| ");
    mvwprintw(game.title_win, 4, 1, " |______||___|___||_______||_______||_______||_______||_______||___|___| ");
    wrefresh(game.title_win);
}

void ncs_create_game_windows() {
    // Create main window
    game.main_win = subwin( stdscr,
                            MAP_LINES + 2,
                            MAP_COLS + BAR_SIZE + 3,
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
                            MAP_LINES - HELP_SIZE + 3,
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
                            4,
                            BAR_SIZE + 1,
                            HELP_WIN_L0 - 4,
                            BAR_WIN_C0
    );
    game.dist_win = subwin( stdscr,
                            HELP_SIZE,
                            BAR_SIZE + 1,
                            HELP_WIN_L0,
                            HELP_WIN_C0
    );
}


void ncs_refresh_game_windows() {
    // Render border for game subwindows
    box(game.main_win, ACS_VLINE, ACS_HLINE);
    box(game.bar_win, ACS_VLINE, ACS_HLINE);
    box(game.stm_bar, ACS_VLINE, ACS_HLINE);
    box(game.dist_win, ACS_VLINE, ACS_HLINE);
    // Render game windows
    // wrefresh(game.main_win);
    wrefresh(game.game_win);
    wrefresh(game.bar_win);
    wrefresh(game.stm_bar);
    wrefresh(game.stats_win);
    wrefresh(game.dist_win);
}

void ncs_destroy_win(WINDOW *win) {
    wclear(win);
    delwin(win);
}

void ncs_quit() {
    ncs_destroy_win(stdscr);
    endwin();
}

void run_game() {
    game_init();
    time_t begin = time(NULL);
    // Menu entries
    switch (game_start_menu()) {
    case 0: // New game
        game_init_new_game();
        game_loop();
        time_t end = time(NULL);
        save.playing_time = end - begin;
        save_game();
        // TODO: Game over screen + menu
        game_free();
        break;
    case 1: // Load game save
        // game_load_saved_game();
        break;
    case 2: // Quit game
        game_quit();
        break;
    default: break;
    }    
    game_quit();
}

void game_init() {
    game.gameover = false;
    // Initialize ncurses resources
    ncs_init();
    ncs_init_colors();
    ncs_check_term_size();
    ncs_create_title_window();
}

uint8_t game_start_menu() {
    // Start menu, show game title
    ncs_create_title_window();
    // Start menu, select an entry
    char *first_menu_list[] = { "Nouvelle partie", "Charger une partie", "Quitter" };
    menu_create_entry_template(first_menu_list, 3);
    uint8_t choice = menu_select_entry(first_menu_list, 3);
    // Play sound effects
    if (choice == 0) {
        system("aplay -q assets/sfx/among-us.wav &");
    }
    else if (choice == 2) {
        system("aplay -q assets/sfx/fart.wav &");
    }
    ncs_destroy_win(game.title_win);
    ncs_destroy_win(game.menu_win);
    
    return choice;
}

void game_init_new_game() {
    // Create game subwindows
    ncs_create_game_windows();
    // Generate random map
    map_init(EASY);
    game.path = map_generate();
    // Initialize player entity
    player_init(map->level);
    // First render of game
    game_render();
}

bool game_check_win() {
    if ((player->pos.l == MAP_LINES - 1) && (player->pos.c == MAP_COLS - 1)) {
        system("aplay -q assets/sfx/youu.wav &");
        return true;
    }
    return false;
}

void game_loop() {
    while (!game_check_win() && !game.gameover) {
        game_inputs();
        game_update();
        game_render();
        // Limit framerate to ~ 60fps
        usleep(16000);
    }
    // TEMP /!\ To change with lucas menus to make a gameover screen + retry button...
    usleep(300000);
}

void game_inputs() {
    player_inputs();
    // Maybe more
}

void game_update() {
    player_update();
    // Maybe more
}

void game_render() {
    map_render();
    player_render();
    stamina_render();
    distances_render();

    ncs_refresh_game_windows();
    if (player->stamina <= STAMINA_MIN) {
        game.gameover = true;
    }
}

void game_free() {
    map_free();
    stack_free(game.path);
    player_free();
}

void game_quit() {
    ncs_quit();
}