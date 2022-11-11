#include "game.h"


extern Game game;
extern Map *map;

void ncs_init() {
    initscr();
    // Don't echo the pressed key and hide the cursor
    noecho();
    curs_set(false);
    setlocale(LC_ALL, "");
    keypad(stdscr, TRUE);

    refresh();
}

void ncs_init_colors() {
    // Enable foreground colors and disable background colors
    use_default_colors();
    start_color();
    init_pair(FORMAT_COLOR_PATH, COLOR_CYAN, -1);
    init_pair(FORMAT_COLOR_OBS, COLOR_RED, -1);
    init_pair(FORMAT_COLOR_FRUIT, COLOR_GREEN, -1);
    init_pair(FORMAT_COLOR_PLAYER, COLOR_CYAN, -1);
}

void ncs_check_term_size() {
    // Get size of current terminal window
    getmaxyx(stdscr, game.win_y, game.win_x);
    if ((game.win_x < MAP_COLS + BAR_SIZE + 2) || (game.win_y < MAP_LINES + 2)) {
        ncs_quit();
        fprintf(stderr,
            "[ERROR] > Window is set to %d rows * %d cols.\n\t> Please enlarge it %d rows * %d cols minimun.\n",
            game.win_y, game.win_x, (MAP_LINES + 2), (MAP_COLS + BAR_SIZE + 2));
        exit(0);
    }
}

void ncs_create_windows() {
    // Create main window
    game.main_win = subwin( stdscr,
                            MAP_LINES + 2,
                            MAP_COLS + BAR_SIZE + 2,
                            game.win_y/2 - (MAP_LINES + 2)/2,
                            game.win_x/2 - (MAP_COLS + BAR_SIZE + 2)/2
    );
    // Create all sub windows
    game.game_win = subwin( stdscr,
                            MAP_LINES,
                            MAP_COLS,
                            game.win_y/2 - MAP_LINES/2,
                            game.win_x/2 - (MAP_COLS + BAR_SIZE)/2
    );
    game.bar_win = subwin(  stdscr,
                            MAP_LINES - MENU_SIZE + 2,
                            BAR_SIZE,
                            game.win_y/2 - MAP_LINES/2 - 1,
                            1 + game.win_x/2 + (MAP_COLS - BAR_SIZE)/2
    );
    game.help_win = subwin( stdscr,
                            MENU_SIZE + 1,
                            BAR_SIZE,
                            game.win_y/2 + MAP_LINES/2 - MENU_SIZE,
                            1 + game.win_x/2 + (MAP_COLS - BAR_SIZE)/2
    );
}


void ncs_refresh_windows() {
    // Render border for subwindows
    box(game.main_win, ACS_VLINE, ACS_HLINE);
    box(game.bar_win, ACS_VLINE, ACS_HLINE);
    box(game.help_win, ACS_VLINE, ACS_HLINE);
    // Render all windows
    wrefresh(game.main_win);
    wrefresh(game.game_win);
    wrefresh(game.bar_win);
    wrefresh(game.help_win);
}

void ncs_quit() {
    endwin();
    delwin(stdscr);
}

void game_loop() {
    // Main game loop, how it's done for every game
    game_init();
    while (!game.quit) {
        game_inputs();
        game_update();
        game_render();
    }
    game_quit();
}

void game_init() {
    game.quit = false;
    // Initialize ncurses game resources
    ncs_init();
    ncs_init_colors();
    ncs_check_term_size();
    ncs_create_windows();
    // Generate random map
    map_init(EASY);
    game.path = map_generate();
    // Create player
    player_init(map->level);
    // First render of game
    game_render();
}

void game_inputs() {
    player_inputs(&game.quit);
    // Maybe more
}

void game_update() {
    player_update();
    // Maybe more
}

void game_render() {
    map_render(game.game_win);
    player_render(game.game_win);
    ncs_refresh_windows();
}

void game_quit() {
    ncs_quit();
    map_free();
    stack_free(game.path);
    player_free();
}