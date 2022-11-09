#include "player.h"


extern Player *player;

void player_init() {
    player = calloc(1, sizeof *player);
    player->pos = (Position) {0, 0};
    player->move = NONE;
    player->nb_move = 0;
    // player.stamina is parameter
    player->is_obstacle = false;
}

void player_inputs(bool *quit) {
    // NOT WORKING PROPERLY : Arrow keys, Escape key

    switch (getch()) {
    // case KEY_EXIT:
    //     *quit = true;
    //     break;
    case KEY_DOWN:
    case 'S':
    case 's':
        player->move = DOWN;
        break;
    case KEY_RIGHT:
    case 'D':
    case 'd':
        player->move = RIGHT;
        break;
    case KEY_UP:
    case 'Z':
    case 'z':
        player->move = UP;
        break;
    case KEY_LEFT:
    case 'Q':
    case 'q':
        player->move = LEFT;
        break;
    default: break;
    }
}

void player_update() {
    // TODO :
    // Check collisions : if move valid => player->nb_move++;
    // nb_move reset to 0 if nv_move >= maximum value => player->stamina decreasing

    switch (player->move) {
    case DOWN:
        player->pos.y++;
        break;
    case RIGHT:
        player->pos.x++;
        break;
    case UP:
        player->pos.y--;
        break;
    case LEFT:
        player->pos.x--;
        break;
    default: break;
    }
}

void player_render(WINDOW *win) {
    // render player at his pos
    mvwaddch(win, player->pos.y, player->pos.x, 'P');
    wrefresh(win);
}

void player_free() {
    free(player);
}