#include "game.h"


extern Game game;

void ncs_init() {
    initscr();
    // Don't echo the pressed key and hide the cursor
    noecho();
    curs_set(false);
    refresh();
}

void ncs_check_term_size() {
    // Get size of terminal window
    getmaxyx(stdscr, game.win_y, game.win_x);
    if ((game.win_x < MAP_SIZE*2 + BAR_SIZE + 2) || (game.win_y < MAP_SIZE + 2)) {
        ncs_quit();
        printf("[ERROR] > Window is set to %d rows * %d cols.\n\t> Please enlarge it minimun %d rows * %d cols.\n",
            game.win_y, game.win_x, (MAP_SIZE + 2), (MAP_SIZE*2 + BAR_SIZE + 2));
        exit(0);
    }
}

void ncs_create_windows() {
    // Create main window
    game.main_win = subwin( stdscr,
                            MAP_SIZE + 2,
                            MAP_SIZE*2 + BAR_SIZE + 2,
                            game.win_y/2 - (MAP_SIZE + 2)/2,
                            game.win_x/2 - (MAP_SIZE*2 + BAR_SIZE + 2)/2);
    // Create sub windows
    game.game_win = subwin( stdscr,
                            MAP_SIZE,
                            MAP_SIZE*2,
                            game.win_y/2 - MAP_SIZE/2,
                            game.win_x/2 - (MAP_SIZE*2 + BAR_SIZE)/2 );
    game.bar_win = subwin(  stdscr,
                            MAP_SIZE - MENU_SIZE + 2,
                            BAR_SIZE,
                            game.win_y/2 - MAP_SIZE/2 - 1,
                            1 + game.win_x/2 + (MAP_SIZE*2 - BAR_SIZE)/2 );
    game.help_win = subwin( stdscr,
                            MENU_SIZE + 1,
                            BAR_SIZE,
                            game.win_y/2 + MAP_SIZE/2 - MENU_SIZE,
                            1 + game.win_x/2 + (MAP_SIZE*2 - BAR_SIZE)/2 );
    // Draw border for subwindows
    box(game.main_win, ACS_VLINE, ACS_HLINE);
    box(game.bar_win, ACS_VLINE, ACS_HLINE);
    box(game.help_win, ACS_VLINE, ACS_HLINE);
    // Render windows
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
    game_init();
    while (!game.quit) {
        game_inputs();
        game_update();
        game_render();
    }
    game_quit();
}

void game_init() {
    ncs_init();
    ncs_check_term_size();
    ncs_create_windows();
    map_init(EASY);
    map_random_fill();
    player_init();
    game.quit = false;
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
    clear();

    // NOT WORKING PROPERLY : not displaying boxes of other windows.
    // Maybe regroup render operations in a single one -> render map with player pos
    map_render(game.game_win);
    player_render(game.game_win);
    wrefresh(game.bar_win);
    wrefresh(game.help_win);
}

void game_quit() {
    map_free();
    player_free();
    ncs_quit();
}