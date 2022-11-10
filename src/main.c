#include "game.h"


Game game = {0};
Map *map = NULL;
Player *player = NULL;

int main() {
    srand(time(NULL));
    setlocale(LC_ALL, "");
    // Initialize ncurses window and sub windows.
    // ncs_init();
    // ncs_check_term_size();
    // ncs_create_windows();

    map_init(EASY);
    Stack *path = NULL;
    // while there is no path in the map, generate the map randomly and search
    // for at least one path using A* pathfinding algorithm (improved version of Dijkstra's algorithm)
    do {
        map_random_fill();
        path = a_star();
    } while (!path);

    stack_free(path);
    map_render(game.game_win);
    mvwaddch(game.game_win, 0, 0, 'P');
    mvwaddch(game.game_win, MAP_LINES - 1, MAP_COLS - 1, 'F');
    wrefresh(game.game_win);
    getch();
    
    // Clean resources and quit ncurses.
    map_free();
    ncs_quit();


    return 0;
}