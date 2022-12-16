#include "game.h"


Game game = {0};
Map *map = NULL;
Player *player = NULL;
Level difficulty = EASY;

int main() {
    setlocale(LC_ALL, "");
    game.reload_game = true;
    game.victory = false;
    while(game.reload_game == true){
        
        run_game();
    }
    return 0; 
}