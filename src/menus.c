#include "menus.h"


extern Game game;
extern Map *map;
extern Player *player;

void menu_create_entry_template(char **entry_list, int nb_entry) {
    // Create the menu window
    game.menu_win = subwin( stdscr,
                            nb_entry + 2,
                            (MAP_COLS + BAR_SIZE + 2)/2,
                            game.win_h/2 - (MAP_LINES + 2)/2 + 15,
                            game.win_w/2 - (MAP_COLS + BAR_SIZE + 2)/2 + 25
    );
    // Render menu entries
    for(uint8_t i = 0; i < nb_entry; i++ ) {
        // Highlight the current menu entry
        if (i == 0) {
            wattron(game.menu_win, A_STANDOUT); 
        }
        else {
            wattroff(game.menu_win, A_STANDOUT);
        }
        mvwprintw( game.menu_win, i+1, 2, "%s", entry_list[i] );
    }
    box(game.menu_win, ACS_VLINE, ACS_HLINE);
    wrefresh(game.menu_win);
}

int8_t menu_select_entry(char **entry_list, int nb_entry) {
    // Input loop to select menu entries
    int ch;
    int8_t i = 0;
    while ((ch = getch()) != KEY_ENTR) {
        mvwprintw(game.menu_win, i + 1, 2, "%s", entry_list[i] );
        switch (ch) {
        case KEY_UP:
        case 'Z':
        case 'z':
            system("aplay -q assets/sfx/menu-entry.wav &");
            i--;
            if (i < 0) {
                i = nb_entry - 1;
            }
            break;
        case KEY_DOWN:
        case 'S':
        case 's':
            system("aplay -q assets/sfx/menu-entry.wav &");
            i++;
            if (i >= nb_entry) {
                i = 0;
            }
            break;
        default: break;
        }
        // Refresh rendering of menu entries
        wattron(game.menu_win, A_STANDOUT );
        mvwprintw(game.menu_win, i+1, 2, "%s", entry_list[i]);
        wattroff(game.menu_win, A_STANDOUT );
        wrefresh(game.menu_win );
    }
    return i;
}

void help_render() {
    uint8_t right = (player->pos.c == MAP_COLS - 1) ? 0: map->map_grid[player->pos.c][player->pos.l].distance[0];
    uint8_t bottom = (player->pos.l == MAP_LINES - 1) ? 0: map->map_grid[player->pos.c][player->pos.l].distance[1];
    uint8_t left = (player->pos.c == 0) ? 0: map->map_grid[player->pos.c - 1][player->pos.l].distance[0];
    uint8_t top = (player->pos.l == 0) ? 0: map->map_grid[player->pos.c][player->pos.l - 1].distance[1];

    mvwaddch(game.help_win, 5, BAR_SIZE / 2, '&' | COLOR_PAIR(FORMAT_COLOR_CYAN));

    mvwprintw(game.help_win, 1, BAR_SIZE / 2, "  ");
    mvwprintw(game.help_win, 1, BAR_SIZE / 2, "%hhd", top);
    mvwprintw(game.help_win, 3, BAR_SIZE / 2, "🠱");

    mvwprintw(game.help_win, 5, (BAR_SIZE / 2) - 7, "  ");
    mvwprintw(game.help_win, 5, (BAR_SIZE / 2) - ((left / 10) ? 7: 6), "%hhd", left);
    mvwprintw(game.help_win, 5, (BAR_SIZE / 2) - 4 , "🠰");

    mvwprintw(game.help_win, 5, (BAR_SIZE / 2) + 5, "  ");
    mvwprintw(game.help_win, 5, (BAR_SIZE / 2) + 5, "%hhd", right);
    mvwprintw(game.help_win, 5, (BAR_SIZE / 2) + 3 , "🠲");

    mvwprintw(game.help_win, 9, BAR_SIZE / 2, "  ");
    mvwprintw(game.help_win, 9, BAR_SIZE / 2, "%hhd", bottom);
    mvwprintw(game.help_win, 7, BAR_SIZE / 2, "🠳");

    mvwprintw(game.help_win, 11, 5, "DISTANCES");
}