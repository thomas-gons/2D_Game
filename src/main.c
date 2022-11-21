#include "game.h"


Game game = {0};
Map *map = NULL;
Player *player = NULL;

int main() {
    main_loop();
    return 0;
}