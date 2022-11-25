#include "game.h"


extern Game game;
extern Map *map;
extern Player *player;

void ncs_init() {
    initscr();
    // Don't echo the pressed key and hide the cursor
    noecho();
    curs_set(false);
    setlocale(LC_ALL, "");
    keypad(stdscr, TRUE);
}

void ncs_init_colors() {
    // Enable foreground colors and disable background colors
    use_default_colors();
    start_color();
    init_pair(FORMAT_COLOR_EMPTY, -1, -1);
    init_pair(FORMAT_COLOR_GREEN, COLOR_GREEN, -1);
    init_pair(FORMAT_COLOR_CYAN, COLOR_CYAN, -1);
    init_pair(FORMAT_COLOR_YELLOW, COLOR_YELLOW, -1);
    init_pair(FORMAT_COLOR_RED, COLOR_RED, -1);
    init_pair(FORMAT_COLOR_MAGENTA, COLOR_MAGENTA, -1);
}

void ncs_check_term_size() {
    // Get size of current terminal window
    getmaxyx(stdscr, game.win_h, game.win_w);
    if ((game.win_w < MAP_COLS + BAR_SIZE + 2) || (game.win_h < MAP_LINES + 2)) {
        ncs_quit();
        fprintf(stderr,
                "[ERROR] > Window is set to %d rows * %d cols.\n\t> Please enlarge it %d rows * %d cols minimum.\n",
                game.win_h, game.win_w, (MAP_LINES + 2), (MAP_COLS + BAR_SIZE + 2)
        );
        exit(2);
    }
}

void ncs_create_windows() {
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
                            MAP_LINES - HELP_SIZE + 3,
                            BAR_SIZE,
                            BAR_WIN_L0,
                            BAR_WIN_C0
    );
    game.stm_bar = subwin(  stdscr,
                            STM_BAR_SIZE ,
                            BAR_SIZE - STM_BAR_PAD_L * 2,
                            STM_BAR_L0,
                            STM_BAR_C0
    );
    game.fruit_win = subwin(stdscr,
                            2,
                            BAR_SIZE,
                            HELP_WIN_L0 - 3,
                            BAR_WIN_C0
    );
    game.help_win = subwin( stdscr,
                            HELP_SIZE,
                            BAR_SIZE,
                            HELP_WIN_L0,
                            HELP_WIN_C0
    );
}


void ncs_refresh_windows() {
    // Render border for subwindows
    box(game.main_win, ACS_VLINE, ACS_HLINE);
    box(game.bar_win, ACS_VLINE, ACS_HLINE);
    box(game.stm_bar, ACS_VLINE, ACS_HLINE);
    box(game.help_win, ACS_VLINE, ACS_HLINE);
    // Refresh all game windows.
    wrefresh(game.game_win);
    wrefresh(game.bar_win);
    wrefresh(game.stm_bar);
    wrefresh(game.fruit_win);
    wrefresh(game.help_win);
}

void ncs_quit() {
    endwin();
    delwin(stdscr);
}

void main_loop() {
    game_init();
    while (!game.quit) {
        game_loop();
    }
    game_quit();
}

void game_init() {
    game.quit = false;
    game.gameover = false;
    // Initialize ncurses game resources
    ncs_init();
    ncs_init_colors();
    ncs_check_term_size();
    ncs_create_windows();
    // Generate random map
    map_init(EASY);
    game.path = map_generate();
    // Initialize player entity
    player_init(map->level);
    // First render of game
    game_render();
}

void game_loop() {
    while (!game.gameover) {
        game_inputs();
        game_update();
        game_render();
        usleep(16000);
    }
    // Game over screen + menu
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
    // stamina_win_render();

    ncs_refresh_windows();
    if (player->stamina <= STAMINA_MIN) {
        game.gameover = true;
        
        // TEMP /!\ To change with lucas menus to make a gameover screen + retry button...
        game.quit = true;
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