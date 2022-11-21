#include "game.h"

extern Game game;
extern Player *player;
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

    init_pair(FORMAT_COLOR_STM_HIGH, -1, COLOR_GREEN);
    init_pair(FORMAT_COLOR_STM_MED, -1, COLOR_YELLOW);
    init_pair(FORMAT_COLOR_STM_LOW, -1, COLOR_RED);
    init_pair(FORMAT_COLOR_EMPTY, -1, -1);
}

void ncs_check_term_size() {
    // Get size of current terminal window
    getmaxyx(stdscr, game.win_h, game.win_w);
    if ((game.win_w < MAP_COLS + BAR_SIZE + 2) || (game.win_h < MAP_LINES + 2)) {
        ncs_quit();
        fprintf(stderr,
            "[ERROR] > Window is set to %d rows * %d cols.\n\t> Please enlarge it %d rows * %d cols minimum.\n",
            game.win_h, game.win_w, (MAP_LINES + 2), (MAP_COLS + BAR_SIZE + 2));
        exit(2);
    }
}

void ncs_create_windows() {
    // Create main window
    game.main_win = subwin( stdscr,
                            MAP_LINES + 2,
                            MAP_COLS + BAR_SIZE + 2,
                            game.win_h/2 - (MAP_LINES + 2)/2,
                            game.win_w/2 - (MAP_COLS + BAR_SIZE + 2)/2
    );
    // Create all sub windows
    game.game_win = subwin( stdscr,
                            MAP_LINES,
                            MAP_COLS,
                            game.win_h/2 - MAP_LINES/2,
                            game.win_w/2 - (MAP_COLS + BAR_SIZE)/2
    );
    game.bar_win = subwin(  stdscr,
                            MAP_LINES - MENU_SIZE + 2,
                            BAR_SIZE,
                            game.win_h/2 - MAP_LINES/2 - 1,
                            1 + game.win_w/2 + (MAP_COLS - BAR_SIZE)/2
    );
    game.stm_bar = subwin ( stdscr,
                            STM_BAR_SIZE ,
                            BAR_SIZE - 2 * STM_BAR_PAD_L,
                            game.win_h/2 - (MAP_LINES/2 -  STM_BAR_PAD_T + 1),
                            1 + game.win_w/2 + (MAP_COLS - BAR_SIZE)/2 + STM_BAR_PAD_L
    );
    game.help_win = subwin( stdscr,
                            MENU_SIZE + 1,
                            BAR_SIZE,
                            game.win_h/2 + MAP_LINES/2 - MENU_SIZE,
                            1 + game.win_w/2 + (MAP_COLS - BAR_SIZE)/2
    );
}


void ncs_refresh_windows() {
    // Render border for subwindows
    box(game.main_win, ACS_VLINE, ACS_HLINE);
    box(game.bar_win, ACS_VLINE, ACS_HLINE);
    box(game.stm_bar, ACS_VLINE, ACS_HLINE);
    box(game.help_win, ACS_VLINE, ACS_HLINE);
    // Render all windows
    wrefresh(game.main_win);
    wrefresh(game.game_win);
    wrefresh(game.bar_win);
    wrefresh(game.stm_bar);
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
    stamina_init(game.bar_win);
    game_render();
}

void game_inputs() {
    player_inputs(&game.quit);
    // Maybe more
}

void game_update() {
    player_update();
    stamina_update(game.bar_win);
    if (player->stamina <= 0)
        game_quit();
    // Maybe more
}

void game_render() {
    map_render(game.game_win);
    player_render(game.game_win);
    stamina_bar_render(game.stm_bar);
    ncs_refresh_windows();
}

void game_quit() {
    ncs_quit();
    map_free();
    stack_free(game.path);
    player_free();
}