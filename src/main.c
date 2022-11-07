#include "game.h"

Game game = {0};
Map *map = NULL;
Player *player = NULL;

int main() {
    setlocale(LC_ALL, "");
    // Initialize ncurses window and sub windows.
    ncs_init();
    ncs_check_term_size();
    ncs_create_windows();

    // /!\ TEMP: Create the map, render it.
    map_init(EASY);
/* --- MELVYN PART --- */

    Stack *path = NULL;
    // do {
        // add obstacle randomly
        // if the obstacle is a part of the path we find another path
        path = path_finder();
        stack_display(path);
    // while (path)
    // write the path to the map
    stack_free(path);

/* --- MELVYN PART --- */
    map_random_fill();
    map_render(game.game_win);
    map_free();
    // /!\ TEMP: Render player and flag.
    mvwaddch(game.game_win, 0, 0, 'P');
    mvwaddch(game.game_win, MAP_SIZE - 1, MAP_SIZE*2 - 1, 'F');
    wrefresh(game.game_win);

    getch();
    
    // Clean resources and quit ncurses.
    ncs_quit();

    return 0;
}