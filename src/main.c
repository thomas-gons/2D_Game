#include "game.h"


Game game = {0};
SaveManager save = {0};
Map *map = NULL;
Player *player = NULL;
Enemy *enemy = NULL;
Level level = EASY;

int main() {
    srand(time(NULL));
    setlocale(LC_ALL, "");
    game.reload_game = true;
    while (game.reload_game == true) {
        run_game();
    }
    return 0; 
}