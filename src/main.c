#include "game.h"


Game game = {0};
Map *map = NULL;
Player *player = NULL;

int main() {
    setlocale(LC_ALL, "");
    run_game();
    return 0; 
}