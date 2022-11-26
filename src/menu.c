#include "menu.h"


extern Game game;

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