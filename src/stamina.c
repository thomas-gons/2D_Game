#include "stamina.h"


extern Game game;
extern Player *player;

void stamina_update_val() {
    if (player->stamina <= 0) {
        player->stamina = 0;
    }
    mvwprintw(  game.bar_win,
                STM_BAR_SIZE + STM_BAR_PAD_T,
                STM_BAR_PAD_L + 1,
                (player->stamina == 100) ? "%d" : ((player->stamina >= 10) ? " %d" : "  %d"), player->stamina);
}

void stamina_bar_render() {
    stamina_update_val();
    uint8_t stm_level = (player->stamina > STAMINA_HIGH) ? FORMAT_COLOR_STM_HIGH :
                        (player->stamina > STAMINA_MED) ?  FORMAT_COLOR_STM_MED :
                                                           FORMAT_COLOR_STM_LOW;
    uint8_t threshold = (uint8_t)((player->stamina * STM_BAR_SIZE) / 100);
    uint8_t i, j;
    for (i = 0; i < 18 - threshold; i++) {
        for (j = 0; j < 5; j++)
            mvwaddch(game.stm_bar, i, j, ' ' | COLOR_PAIR(FORMAT_COLOR_EMPTY));
    }
    for (; i < STM_BAR_SIZE; i++) {
        for (j = 0; j < 5; j++)
            mvwaddch(game.stm_bar, i, j, ' ' | COLOR_PAIR(stm_level));
    }
}