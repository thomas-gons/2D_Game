#include "player.h"
#include "map.h"
#include "stamina.h"


extern Player *player;
extern Map *map;


void stamina_update(WINDOW *bar_win) {
    char stamina[3];
    // convert stamina amount into string
    sprintf(stamina, "%d", player->stamina); 
    if (player->stamina < 10) {
        stamina[2] = stamina[0];
        stamina[1] = ' ';
        stamina[0] = ' ';
    }
    else if (player->stamina < 100) {
        stamina[2] = stamina[1];
        stamina[1] = stamina[0];
        stamina[0] = ' ';
    }
    mvwaddstr(bar_win, STM_BAR_SIZE + STM_BAR_PAD_T, STM_BAR_PAD_L + 1, stamina);
}

void stamina_bar_render(WINDOW *stm_bar) {
    uint8_t stm_level = (player->stamina > STAMINA_HIGH) ? FORMAT_COLOR_STM_HIGH:
                        (player->stamina > STAMINA_MED) ?  FORMAT_COLOR_STM_MED:
                                                           FORMAT_COLOR_STM_LOW;
    uint8_t threshold = (uint8_t)((player->stamina * STM_BAR_SIZE) / 100), i, j;
    for (i = 0; i < 18 - threshold; i++) {
        for (j = 0; j < 5; j++)
            mvwaddch(stm_bar, i, j, ' ' | COLOR_PAIR(FORMAT_COLOR_EMPTY));
    }
    for (; i < STM_BAR_SIZE; i++) {
        for (j = 0; j < 5; j++)
            mvwaddch(stm_bar, i, j, ' ' | COLOR_PAIR(stm_level));
    }
}