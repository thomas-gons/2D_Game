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
    int startmenu_choice = game_start_menu();
    if(startmenu_choice==0){
        while (!game.quit || !player->stamina) {
            game_inputs();
            game_update();
            game_render();
        }
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

int game_start_menu() {
    int ch, i = 0;
    char list[5][20] = { "Nouvelle partie", "Charger une partie", "Parametres", "Quitter",};
    char item[40];
    game.menu_win = subwin( stdscr,
                            MAP_LINES + 2,
                            MAP_COLS + BAR_SIZE + 2,
                            game.win_h/2 - (MAP_LINES + 2)/2,
                            game.win_w/2 - (MAP_COLS + BAR_SIZE + 2)/2
    );
    box(game.menu_win, ACS_VLINE, ACS_HLINE);
    for( i=0; i<4; i++ ) {
        if( i == 0 ) {
            wattron( game.menu_win, A_STANDOUT ); // highlights the first item.
        }
        else {
            wattroff( game.menu_win, A_STANDOUT );
        }
        sprintf(item, "%s",  list[i]);
        mvwprintw( game.menu_win, i+1, 2, "%s", item );
    }

    wrefresh( game.menu_win );
    i = 0;
    while(( ch = wgetch(game.menu_win)) != 'q'){ 
        // right pad with spaces to make the items appear with even width.
        sprintf(item, "%s",  list[i]); 
        mvwprintw( game.menu_win, i+1, 2, "%s", item ); 
        // use a variable to increment or decrement the value based on the input.
        switch( ch ) {
            case 'z':
            i--;
            if( i<0) {
                i= 4;
            }
            break;
            case 's':
            i++;
            if( i>4) {
                i= 0;
            }
            break;
        }
        wattron( game.menu_win, A_STANDOUT );
        sprintf(item, "%s",  list[i]);
        mvwprintw( game.menu_win, i+1, 2, "%s", item);
        wattroff( game.menu_win, A_STANDOUT );
        wrefresh( game.menu_win );
    }
    return i;


}
