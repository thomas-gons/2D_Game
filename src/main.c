#include "game.h"


Game game = {0};
Map *map = NULL;
Player *player = NULL;
Enemy *enemy = NULL;

int main() {
    setlocale(LC_ALL, "");
    srand(time(NULL));
    run_game();
    return 0; 
}