#include "player.h"


extern Player *player;
extern Map *map;

void player_init(Level level) {
    player = calloc(1, sizeof *player);
    player->pos = (Position) {0, 0};
    player->move = NONE;
    player->nb_move = 0;
    player->is_obstacle = false;
    switch (level) {
    case EASY:
        player->stamina = STAMINA_EASY;
        break;
    case MEDIUM:
        player->stamina = STAMINA_MEDIUM;
        break;
    case HARD:
        player->stamina = STAMINA_HARD;
        break;
    default: break;
    }
}

void player_inputs(bool *quit) {
    switch (getch()) {
    case KEY_ESC:
        *quit = true;
        break;
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
    player->move = NONE;
}

void player_render(WINDOW *win) {
    mvwaddch(win, player->pos.y, player->pos.x, 'P' | COLOR_PAIR(FORMAT_COLOR_PLAYER));
}

void player_free() {
    free(player);
}