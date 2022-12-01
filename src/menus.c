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

void distances_render() {
    // Set character to obstacle char if player is near an obstacle
    int8_t right = (player->pos.c == MAP_COLS - 1) ? 0 : 
        (IS_OBSTACLE_CELL(player->pos.l, player->pos.c + 1)) ? -1 : map->map_grid[player->pos.l][player->pos.c].distance[0];
    int8_t down = (player->pos.l == MAP_LINES - 1) ? 0 : 
        (IS_OBSTACLE_CELL(player->pos.l + 1, player->pos.c)) ? -1 : map->map_grid[player->pos.l][player->pos.c].distance[1];
    int8_t left = (player->pos.c == 0) ? 0 : 
        (IS_OBSTACLE_CELL(player->pos.l, player->pos.c - 1)) ? -1 : map->map_grid[player->pos.l][player->pos.c - 1].distance[0];
    int8_t up = (player->pos.l == 0) ? 0 : 
        (IS_OBSTACLE_CELL(player->pos.l - 1, player->pos.c)) ? -1 : map->map_grid[player->pos.l - 1][player->pos.c].distance[1];
    // Render player character at center
    wattron(game.stats_win, A_BOLD);
    wattron(game.dist_win, COLOR_PAIR(FORMAT_COLOR_CYAN));
    mvwprintw(game.dist_win, 6, BAR_SIZE / 2, PLAYER_CHAR);
    wattroff(game.dist_win, COLOR_PAIR(FORMAT_COLOR_CYAN));
    wattroff(game.stats_win, A_BOLD);

    // Render directional arrows and their distance value
    mvwprintw(game.dist_win, 2, BAR_SIZE / 2, "  ");
    render_dist_obstacle(2, BAR_SIZE / 2, up);
    mvwprintw(game.dist_win, 4, BAR_SIZE / 2, "🠱");

    mvwprintw(game.dist_win, 6, (BAR_SIZE / 2) - 7, "  ");
    render_dist_obstacle(6, (BAR_SIZE / 2) - ((left / 10) ? 7 : 6), left);
    mvwprintw(game.dist_win, 6, (BAR_SIZE / 2) - 4 , "🠰");

    mvwprintw(game.dist_win, 6, (BAR_SIZE / 2) + 5, "  ");
    render_dist_obstacle(6, (BAR_SIZE / 2) + 6, right);
    mvwprintw(game.dist_win, 6, (BAR_SIZE / 2) + 3, "🠲");

    mvwprintw(game.dist_win, 10, BAR_SIZE / 2, "  ");
    render_dist_obstacle(10, BAR_SIZE / 2, down);
    mvwprintw(game.dist_win, 8, BAR_SIZE / 2, "🠳");
}

void render_dist_obstacle(uint8_t line, uint8_t col, int8_t render_dist) {
    wattron(game.dist_win, A_BOLD);
    if (render_dist == -1) {
        // Render obstacle character
        wattron(game.dist_win, COLOR_PAIR(FORMAT_COLOR_RED));
        mvwprintw(game.dist_win, line, col, OBSTACLE_CHAR);
        wattroff(game.dist_win, COLOR_PAIR(FORMAT_COLOR_RED));
    } else {
        // Render distance value
        mvwprintw(game.dist_win, line, col, "%hhd", render_dist);
    }
    wattroff(game.dist_win, A_BOLD);
}