#include "game.h"


Game game = {0};
Map *map = NULL;
Player *player = NULL;
SaveManager save = {0};

int main() {
    setlocale(LC_ALL, "");
    run_game();
    save_charging_game();
    return 0; 
}