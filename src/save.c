#include "save.h"


extern Game game;
extern SaveManager save;
extern Map *map;
extern Player *player;
extern Enemy *enemy;

bool parse_file_ext(const struct dirent *read_file, const char *ext) {
    if (!read_file) {
       return false;
    }
    // Get only regular files that are not read_fileectories
    if (read_file->d_type == DT_REG && read_file->d_type != DT_DIR) {
        // Extract the file extension to compare
        const char *f_ext = strrchr(read_file->d_name, '.');
        if ((!f_ext) || (f_ext == read_file->d_name)) {
            return false;
        } else {
            if (strcmp(f_ext, ext) == 0) {
                return true;
            }
        }
    }
    return false;
}

uint8_t get_nb_files(const char *dir_path, const char *ext) {
    uint8_t nb_files = 0;
    // Read the entire directory and parse files based on the extension
    struct dirent *read_file;
    DIR *dir = opendir(dir_path);
    if (dir) {
        while ((read_file = readdir(dir)) != NULL) {
            if (parse_file_ext(read_file, ext)) {
                nb_files++;
            }
        }
        closedir(dir);
    }
    return nb_files;
}

void get_files(const char *dir_path, const char *ext, char **arr_files) {
    uint8_t i = 0;
    uint8_t name_len = 0;
    // Read the entire directory and parse files based on the extension
    struct dirent *read_file;
    DIR *dir = opendir(dir_path);
    if (dir) {
        while ((read_file = readdir(dir)) != NULL) {
            if (parse_file_ext(read_file, ext)) {
                // Get lenght of file name
                name_len = 1 + strlen(read_file->d_name);
                // Copy string in allocated array
                arr_files[i] = (char *) calloc(name_len, sizeof (char));
                strncpy(arr_files[i], read_file->d_name, name_len);
                i++;
            }
        }
        closedir(dir);
        // Add "Return to Title Menu" string to end of array
        arr_files[i] = (char *) calloc(21, sizeof (char));
        strncpy(arr_files[i], "Return to Title Menu", 21);
    }
}

void save_get_date(const char *ext) {
    uint8_t ext_len = strlen(ext);
    save.date_time = calloc(20 + ext_len, sizeof (char));
    time_t now = time(NULL);
    struct tm *date = localtime(&now);
    strftime(save.date_time, 20 + ext_len, "%d-%m-%Y_%H:%M:%S", date);
    strncat(save.date_time, ext, ext_len);
    save.file_name = calloc(12 + 20 + ext_len, sizeof (char));
    strncat(save.file_name, "data/saves/", 12);
    strncat(save.file_name, save.date_time, 20 + ext_len);
}

void save_write_file() {
    FILE *f;
    f = fopen(save.file_name, "wb");
    if (f == NULL) {
        fprintf(stderr, "\x1b[1m%s:\x1b[0m In function '%s':\033[31m%s:%i: error: cannot open file '%s'\033[0m\n",
            __FILE__, __func__, __FILE__, ((__LINE__) - 3), save.file_name);
        exit(3);
    } else {
        // Writing the played time
        fwrite(&save.play_time, sizeof (time_t), 1, f);
        // Writing the game struture
        save_write_game(f);
        // Writing the map struture
        save_write_map(f);
        // Writing the player struture
        save_write_player(f);
        // Writing the enemies strutures
        // save_write_enemy(f);
    }
    fclose(f);
}

void save_write_game(FILE *f){
    save_write_stack(game.path_dist, f);
    save_write_stack(game.path_stm, f);
}

void save_write_map(FILE *f) {
    fwrite(&map->level, sizeof (Level), 1, f);
    for (uint8_t i = 0, j; i < MAP_LINES; i++) {
        for (j = 0; j < MAP_COLS; j++) {
            fwrite(&map->map_grid[i][j].cell_type, sizeof (Cell_type), 1, f);
            fwrite(&map->map_grid[i][j].distance, sizeof (uint8_t), 2, f);
            fwrite(&map->map_grid[i][j].visited, sizeof (bool), 1, f);
        }
    }
}

void save_write_player(FILE *f) {
    fwrite(&player->stamina, sizeof (int8_t), 1, f);
    fwrite(&player->bonus_stack, sizeof (int8_t), 1, f);
    fwrite(&player->rewind_cnt, sizeof (uint8_t), 1, f);
    fwrite(&player->distance, sizeof (uint16_t), 1, f);
    save_write_stack(player->history, f);
}

void save_write_enemy(FILE *f) {
    for (uint8_t i = 0; i < ENEMY_NB; i++) {
        fwrite(&enemy->current.c, sizeof (uint8_t), 1, f);
        fwrite(&enemy->current.l, sizeof (uint8_t), 1, f);
        fwrite(&enemy->house.c, sizeof (uint8_t), 1, f);
        fwrite(&enemy->house.l, sizeof (uint8_t), 1, f);
        fwrite(&enemy->alive, sizeof (bool), 1, f);
    }
}

void save_write_stack(Stack *stack, FILE *f){
    SNode *tmp = stack->head;
    while (tmp != NULL) {
        fwrite(&tmp->pos.c, sizeof (uint8_t), 1, f);
        fwrite(&tmp->pos.l, sizeof (uint8_t), 1, f);
        fwrite(&tmp->action, sizeof (Action), 1, f);

        tmp = tmp->next;
        if (tmp == NULL) {
            fwrite(";", sizeof (char), 1, f);
        } else {
            fwrite(",", sizeof (char), 1, f);
        }
    }
}

void save_read_file(const char *file_name) {
    FILE *f;
    f = fopen(file_name, "rb");
    if (f == NULL) {
        fprintf(stderr, "\x1b[1m%s:\x1b[0m In function '%s':\033[31m%s:%i: error: cannot open file '%s'\033[0m\n",
            __FILE__, __func__, __FILE__, ((__LINE__) - 3), file_name);
        exit(3);
    } else {
        // Recovery of the played time
        fread(&save.play_time, sizeof (time_t), 1, f);
        // Recovery of the backup of game
        save_read_game(f);
        // Recovery of the backup of map
        save_read_map(f);
        // Recovery of the backup of player
        save_read_player(f);
        // Recovery of the backup of enemies
        // save_read_enemy(f);
    }
    fclose(f);
}

void save_read_game(FILE *f) {
    save_read_stack(game.path_dist, f, false);
    save_read_stack(game.path_stm, f, false);
}

void save_read_map(FILE *f) {
    fread(&map->level, sizeof (Level), 1, f);
    for (uint8_t i = 0, j; i < MAP_LINES; i++) {
        for (j = 0; j < MAP_COLS; j++) {
            fread(&map->map_grid[i][j].cell_type, sizeof (Cell_type), 1, f);
            fread(&map->map_grid[i][j].distance, sizeof (uint8_t), 2, f);
            fread(&map->map_grid[i][j].visited, sizeof (bool), 1, f);
        }
    }
}

void save_read_player(FILE *f) {
    fwrite(&player->stamina, sizeof (int8_t), 1, f);

    printw(" %u ", player->stamina);
    refresh();

    fwrite(&player->bonus_stack, sizeof (int8_t), 1, f);

    printw("%u ", player->bonus_stack);
    refresh();

    fwrite(&player->rewind_cnt, sizeof (uint8_t), 1, f);

    printw("%u ", player->rewind_cnt);
    refresh();
    
    fwrite(&player->distance, sizeof (uint16_t), 1, f);
    
    printw("%u ", player->distance);
    refresh();

    Stack *tmp_history = stack_init();
    save_read_stack(tmp_history, f, true);
    // Recovery of the stack player history in the right order
    stack_pop(player->history);
    player->history = stack_change_order(tmp_history, player->history);
    player->pos = player->history->head->pos;
}

void save_read_enemy(FILE *f) {
    for (uint8_t i = 0; i < ENEMY_NB; i++) {
        fread(&enemy->current.c, sizeof (uint8_t), 1, f);
        fread(&enemy->current.l, sizeof (uint8_t), 1, f);
        fread(&enemy->house.c, sizeof (uint8_t), 1, f);
        fread(&enemy->house.l, sizeof (uint8_t), 1, f);
        fread(&enemy->alive, sizeof (bool), 1, f);
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
        fread(&line, sizeof (uint8_t), 1, f);
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

void save_free() {
    if (save.date_time) {
        free(save.date_time);
    }
    if (save.file_name) {
        free(save.file_name);
    }
    if (save.load_files) {
        str_2d_array_free(save.load_files, get_nb_files(SAVES_DIR_PATH, SAVE_EXT) + 1);
    }
    if (save.history_files) {
        str_2d_array_free(save.history_files, get_nb_files(SAVES_DIR_PATH, DAT_EXT) + 1);
    }
}

void str_2d_array_free(char **arr, uint8_t arr_len) {
    for (uint8_t i = 0; i < arr_len; i++) {
        free(arr[i]);
    }
    free(arr);
}