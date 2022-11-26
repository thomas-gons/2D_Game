#include "game.h"


Game game = {0};
Map *map = NULL;
Player *player = NULL;

int main() {
    run_game();
    return 0;
}