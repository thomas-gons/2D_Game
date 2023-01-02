#include "save.h"
#include "common.h"


extern Game game;
extern SaveManager save;
extern Map *map;
extern Player *player;
extern Enemy *enemy;

void save_get_date(const char *ext) {
    save.date_time = calloc(25, sizeof (char));
    time_t now = time(NULL);
    struct tm *date = localtime(&now);
    strftime(save.date_time, 25, "%d%m%Y_%H%M%S", date);
    strcat(save.date_time, ext);
    save.file_name = calloc(100, sizeof (char));
    strcat(save.file_name, "data/saves/");
    strcat(save.file_name, save.date_time);
}

void save_write_file() {
    FILE *f;
    f = fopen(save.file_name, "wb");
    if (f == NULL) {
        fprintf(stderr, "Error opened file %s\n", save.file_name);
        exit(1);
    } else {
        // Writing the playing time
        fwrite(&save.playing_time, sizeof (time_t), 1, f);
        fwrite(";", sizeof (char), 1, f);
        // Writing the game struture
        save_write_game(f);
        // Writing the map struture
        save_write_map(f);
        // Writing the player struture
        save_write_player(f);
        // Writing the enemies strutures
        save_write_enemy(f);
    }
    fclose(f);
}

void save_write_game(FILE *f){
    save_write_stack(game.path_dist, f);
    save_write_stack(game.path_stm, f);
}

void save_write_map(FILE *f) {
    fwrite(&map->level, sizeof (map->level), 1, f);
    fwrite(";", sizeof (char), 1, f);
    for (uint8_t i = 0, j; i < MAP_LINES; i++) {
        for (j = 0; j < MAP_COLS; j++) {
            fwrite(&map->map_grid[i][j].cell_type, sizeof (int), 1, f);
            fwrite(";", sizeof (char), 1, f);
            fwrite(&map->map_grid[i][j].distance, sizeof (uint8_t), 2, f);
            fwrite(";", sizeof (char), 1, f);
            fwrite(&map->map_grid[i][j].visited, sizeof (bool), 1, f);
            fwrite(";", sizeof (char), 1, f);
        }
    }
}

void save_write_player(FILE *f) {
    fwrite(&player->stamina, sizeof (int8_t), 1, f);
    fwrite(";", sizeof (char), 1, f);
    fwrite(&player->bonus_stack, sizeof (int8_t), 1, f);
    fwrite(";", sizeof (char), 1, f);
    fwrite(&player->rewind_cnt, sizeof (uint8_t), 1, f);
    fwrite(";", sizeof (char), 1, f);
    fwrite(&player->distance, sizeof (uint16_t), 1, f);
    fwrite(";", sizeof (char), 1, f);
    save_write_stack(player->history, f);
}

void save_write_enemy(FILE *f) {
    for (uint8_t i = 0; i < ENEMY_NB; i++) {
        fwrite(&enemy->current.c, sizeof (uint8_t), 1, f);
        fwrite(",", sizeof (char), 1, f);
        fwrite(&enemy->current.l, sizeof (uint8_t), 1, f);
        fwrite(";", sizeof (char), 1, f);
        fwrite(&enemy->house.c, sizeof (uint8_t), 1, f);
        fwrite(",", sizeof (char), 1, f);
        fwrite(&enemy->house.l, sizeof (uint8_t), 1, f);
        fwrite(";", sizeof (char), 1, f);
        fwrite(&enemy->alive, sizeof (bool), 1, f);
        fwrite(";", sizeof (char), 1, f);
    }
}

void save_write_stack(Stack *stack, FILE *f){
    SNode *tmp = stack->head;
    while (tmp != NULL) {
        fwrite(&tmp->pos.c, sizeof (uint8_t), 1, f);
        fwrite(",", sizeof (char), 1, f);
        fwrite(&tmp->pos.l, sizeof (uint8_t), 1, f);
        fwrite(",", sizeof (char), 1, f);
        fwrite(&tmp->action, sizeof (Action), 1, f);
        tmp = tmp->next;
        if (tmp == NULL) {
            fwrite(";", sizeof (char), 1, f);
        } else {
            fwrite(",", sizeof (char), 1, f);
        }
    }
}

void save_read_file() {
    FILE *f;
    char end_char;
    f = fopen(save.file_name, "rb");
    if (f == NULL) {
        fprintf(stderr, "Error opened file %s\n", save.file_name);
        return;
    } else {
        // Recovery of the playing time
        fread(&save.playing_time, sizeof (time_t), 1, f);
        fread(&end_char, sizeof (char), 1, f);
        // Recovery of the backup of game
        save_read_game(f);
        // Recovery of the backup of map
        save_read_map(f);
        // Recovery of the backup of player
        save_read_player(f);
        // Recovery of the backup of enemies
        save_read_enemy(f);
    }
    fclose(f);
}

void save_read_game(FILE *f) {
    save_read_stack(game.path_dist, f, false);
    save_read_stack(game.path_stm, f, false);
}

void save_read_map(FILE *f) {
    char end_char;
    fread(&map->level, sizeof (int), 1, f);
    fread(&end_char, sizeof (char), 1, f);
    for (uint8_t i = 0, j; i < MAP_LINES; i++) {
        for (j = 0; j < MAP_COLS; j++) {
            fread(&map->map_grid[i][j].cell_type, sizeof (int), 1, f);
            fread(&end_char, sizeof (char), 1, f);
            fread(&map->map_grid[i][j].distance, sizeof (uint8_t), 2, f);
            fread(&end_char, sizeof (char), 1, f);
            fread(&map->map_grid[i][j].visited, sizeof (bool), 1, f);
            fread(&end_char, sizeof (char), 1, f);
        }
    }
}

void save_read_player(FILE *f) {
    char end_char;
    fwrite(&player->stamina, sizeof (int8_t), 1, f);
    fwrite(&end_char, sizeof (char), 1, f);
    fwrite(&player->bonus_stack, sizeof (int8_t), 1, f);
    fwrite(&end_char, sizeof (char), 1, f);
    fwrite(&player->rewind_cnt, sizeof (uint8_t), 1, f);
    fwrite(&end_char, sizeof (char), 1, f);
    fwrite(&player->distance, sizeof (uint16_t), 1, f);
    fwrite(&end_char, sizeof (char), 1, f);
    Stack *tmp_history = stack_init();
    save_read_stack(tmp_history, f, true);
    // Recovery of the stack player history in the right order
    player->history = stack_change_order(tmp_history, player->history);
    player->pos = player->history->head->pos;
}

void save_read_enemy(FILE *f) {
    char end_char;
    for (uint8_t i = 0; i < ENEMY_NB; i++) {
        fread(&enemy->current.c, sizeof (uint8_t), 1, f);
        fread(&end_char, sizeof (char), 1, f);
        fread(&enemy->current.l, sizeof (uint8_t), 1, f);
        fread(&end_char, sizeof (char), 1, f);
        fread(&enemy->house.c, sizeof (uint8_t), 1, f);
        fread(&end_char, sizeof (char), 1, f);
        fread(&enemy->house.l, sizeof (uint8_t), 1, f);
        fread(&end_char, sizeof (char), 1, f);
        fread(&enemy->alive, sizeof (bool), 1, f);
        fread(&end_char, sizeof (char), 1, f);
    }
}

void save_read_stack(Stack *stack, FILE *f, bool player_stack) {
    char end_char;
    uint8_t line;
    uint8_t col;
    Action act;
    bool end_stack = false;
    do {
        fread(&col, sizeof (uint8_t), 1, f);
        fread(&end_char, sizeof (char), 1, f);
        fread(&line, sizeof (uint8_t), 1, f);
        fread(&end_char, sizeof (char), 1, f);
        fread(&act, sizeof (Action), 1, f);
        fread(&end_char, sizeof (char), 1, f);
        stack_push(stack, (Position) {.l=line, .c=col}, act);
        if (end_char == ';') {
            end_stack = true;
        }
    } while (end_stack == false);
    if (player_stack == true) {
        stack_push(stack, (Position) {.l=0, .c=0}, NO_ACTION);
    }
}

void save_game(const char *ext) {
    save_get_date(ext);
    save_write_file();
}

void save_charging_game() {
    save_read_file();
    free(save.date_time);
    free(save.file_name);
}