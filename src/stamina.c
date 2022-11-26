#include "stamina.h"


extern Game game;
extern Player *player;

void stamina_update_val() {
    if (player->stamina <= 0) {
        player->stamina = 0;
    }
    mvwprintw(  game.bar_win,
                STM_BAR_SIZE + STM_BAR_PAD_T,
                STM_BAR_PAD_L + 2,
                (player->stamina == 100) ? "%d" : ((player->stamina >= 10) ? " %d" : "  %d"), player->stamina);
}

void stamina_bar_render() {
    // Render stamina bar
    uint8_t stm_level = (player->stamina > STAMINA_HIGH) ? FORMAT_BGCOLOR_GREEN :
                        (player->stamina > STAMINA_MED) ?  FORMAT_BGCOLOR_YELLOW :
                                                           FORMAT_BGCOLOR_RED;
    uint8_t threshold = (uint8_t)((player->stamina * STM_BAR_SIZE) / 100);
    uint8_t i, j;
    for (i = 0; i < STM_BAR_SIZE - threshold; i++) {
        for (j = 0; j < STM_BAR_WIDTH; j++)
            mvwaddch(game.stm_bar, i, j, ' ' | COLOR_PAIR(FORMAT_COLOR_EMPTY));
    }
    for (; i < STM_BAR_SIZE; i++) {
        for (j = 0; j < STM_BAR_WIDTH; j++)
            mvwaddch(game.stm_bar, i, j, ' ' | COLOR_PAIR(stm_level));
    }
}

void stamina_fruit_stack_render() {
    // Render fruit stack
    mvwprintw(game.fruit_win, 0, 3, "FRUITS  %hhd/2", player->fruit_stack);
    mvwaddch(game.fruit_win, 0, 15, '@' | COLOR_PAIR(FORMAT_COLOR_GREEN));
}

void stamina_render() {
    mvwprintw(game.bar_win, 1, STM_BAR_PAD_L + 2, "STM");
    stamina_update_val();
    stamina_bar_render();
    stamina_fruit_stack_render();
}