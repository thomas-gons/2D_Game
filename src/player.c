#include "player.h"
#include "map.h"

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

void input_player(){
    initscr();         // initialize curses
    noecho();          // doesn't display typed characters
    keypad(stdscr, 1); // allows the capture of special keys, here the arrows from the keyboard
    clear();
    bool leave = false; //leave value
 
    while (!leave)
    {
        int c = getch(); // Read the keyboard keys
        switch (c)
        {
        case KEY_DOWN: 
            leave = true;
            player->move = UP;
            player_update();  
            break;
        case KEY_UP:    
            leave = true;
            player->move = DOWN;
            player_update();  
            break;
        case KEY_RIGHT: 
            leave = true;
            player->move = RIGHT;
            player_update();  
            break;
        case KEY_LEFT:  
            leave = true;
            player->move = LEFT;
            player_update();  
            break; 
        case 27:
            leave = true;
            //cas touche escape key
            break;
        }
    }
 
    echo();
    endwin(); // quits curses
}

void player_update() {
    // TODO :
    // Check collisions : if move valid => player->nb_move++; OK
    // nb_move reset to 0 if nv_move >= maximum value => player->stamina decreasing

    switch (player->move) {
    case DOWN:
        if (map->map_grid[player->pos.x][player->pos.y++].cell_type != OBSTACLE || map->map_grid[player->pos.x][player->pos.y++].cell_type != EMPTY){   //checks if the move is valid
            player->pos.y++;                                                                                                                            // if the move is valid, the position of the payer is changed
            player->nb_move++;
        }
        break;
    case RIGHT:
        if (map->map_grid[player->pos.x++][player->pos.y].cell_type != OBSTACLE || map->map_grid[player->pos.x++][player->pos.y].cell_type != EMPTY){
            player->pos.x++;
            player->nb_move++;
        }
        break;
    case UP:
        if (map->map_grid[player->pos.x][player->pos.y--].cell_type != OBSTACLE || map->map_grid[player->pos.x][player->pos.y--].cell_type != EMPTY){
            player->pos.y--;
            player->nb_move++;
        }
        break;
    case LEFT:
        if (map->map_grid[player->pos.x--][player->pos.y].cell_type != OBSTACLE || map->map_grid[player->pos.x--][player->pos.y].cell_type != EMPTY){
            player->pos.x--;
            player->nb_move++;
        }
        break;
    default: break;
    }
}

void player_render(WINDOW *win) {
    mvwaddch(win, player->pos.y, player->pos.x, 'P' | COLOR_PAIR(FORMAT_COLOR_PLAYER));
}

void player_free() {
    free(player);
}