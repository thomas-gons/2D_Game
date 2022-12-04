#include "game.h"


Game game = {0};
Map *map = NULL;
Player *player = NULL;

float __lambda_bonus = 0.2;

int main() {
    setlocale(LC_ALL, "");
    run_game();
    return 0; 
}