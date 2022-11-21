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
                            MAP_LINES - HELP_SIZE + 2,
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
                            HELP_SIZE + 1,
                            BAR_SIZE,
                            game.win_h/2 + MAP_LINES/2 - HELP_SIZE,
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
    wrefresh(game.game_win);
    wrefresh(game.bar_win);
    wrefresh(game.stm_bar);
    // wrefresh(game.help_win);
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

    // TEMP /!\ To move in another encapsulating function
    mvwprintw(game.bar_win, 1, STM_BAR_PAD_L + 1, "STM");
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
    usleep(500000);
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
    stamina_bar_render();
    ncs_refresh_windows();
    if (player->stamina <= 0) {
        game.gameover = true;
        
        // TEMP /!\ To change with lucas menus to make a gameover screen + retry button...
        game.quit = true;
    }
}

void game_quit() {
    ncs_quit();
    map_free();
    stack_free(game.path);
    player_free();
}