#include "player.h"
#include "map.h"


extern Game game;
extern Map *map;
extern Player *player;

void player_init(Level level) {
    player = calloc(1, sizeof *player);
    if (!player) {
        fprintf(stderr,"[ERROR] > calloc, in func player_init\n");
        exit(1);
    }
    player->pos = (Position) {.l=0, .c=0};
    map->map_grid[player->pos.l][player->pos.c].visited = true;
    player->move = NONE;
    player->nb_move = 0;
    player->on_obstacle = false;
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
    player->on_obstacle = false;
    player->move = NONE;
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
    switch (player->move) {
    case DOWN:
        player_check_collisions(player->pos.l + 1, player->pos.c);
        break;
    case RIGHT:
        player_check_collisions(player->pos.l, player->pos.c + 1);
        break;
    case UP:
        player_check_collisions(player->pos.l - 1, player->pos.c);
        break;
    case LEFT:
        player_check_collisions(player->pos.l, player->pos.c - 1);
        break;
    default: break;
    }
}

void player_check_collisions(uint8_t line, uint8_t col) {
    // Check for out of bound values
    if (IS_OUT_OF_MAP(line, col)) {         
        // If player tries to cross an obstacle, loses way more stamina
        if (IS_OBSTACLE_CELL(line, col)) {
            player->on_obstacle = true;
            player->stamina -= STAMINA_LOSS_OBS;
        } else {
            // Update player position in map
            player->pos.l = line;
            player->pos.c = col;
            map->map_grid[line][col].visited = true;
            player->nb_move++;
        }
        // Player loses stamina every MV_LIMIT_COUNT steps
        if ((player->nb_move >= MV_LIMIT_COUNT) && (player->on_obstacle == false)) {
            player->stamina -= STAMINA_LOSS;
            player->nb_move = 0;
        }
    }
}

void player_render() {
    mvwaddch(game.game_win, player->pos.l, player->pos.c, 'P' | COLOR_PAIR(FORMAT_COLOR_PLAYER));
    // Maybe more
}

void player_free() {
    // Free stack of player positions
    free(player);
}