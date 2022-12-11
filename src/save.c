#include "save.h"

extern Map *map;
extern Player *player;
extern SaveManager save;


void save_get_date(){
    save.date_time = calloc(25,sizeof(char));
    time_t now = time(NULL);
    struct tm *date = localtime(&now);

    strftime( save.date_time, 25, "%d_%m_%Y_%H-%M-%S.dat", date);
    printf("%s\n", save.date_time);
    
}

void save_write_file(){
    FILE *f;
    printf("%s\n", save.date_time);
    f = fopen(save.date_time, "wb");

    printf("\nje suis dans un bug ou quoi? \n");
    printf("\n\ny a un truc la dedans %d\n\n", player->distance);
    if (f == NULL){
        fprintf(stderr, "Error opened file %s\n", save.date_time);
        exit(1);
    }
    else{
        // Ecriture du temps de jeu
        fwrite(&save.playing_time, sizeof(time_t), 1, f);
        fwrite(";", sizeof(char*), 1, f);
        // Ecriture de la structure map
        fwrite(&map->level, sizeof(map->level), 1, f);
        fwrite(";", sizeof(char*), 1, f);
        for (uint8_t i = 0, j; i < MAP_LINES; i++){
            for (j = 0; j < MAP_COLS; j++){
                fwrite(&map->map_grid[i][j].cell_type, sizeof(int), 1, f);
                fwrite(",", sizeof(char*), 1, f);
                fwrite(&map->map_grid[i][j].distance, sizeof(int*), 1, f);
                fwrite(",", sizeof(char*), 1, f);
                fwrite(&map->map_grid[i][j].visited, sizeof(bool), 1, f);
                fwrite(";", sizeof(char*), 1, f);
            }
        }
        // Ecriture de la structure player
        fwrite(&player->stamina, sizeof(int), 1, f);
        fwrite(";", sizeof(char*), 1, f);
        fwrite(&player->fruit_stack, sizeof(int), 1, f);
        fwrite(";", sizeof(char*), 1, f);
        fwrite(&player->distance, sizeof(int), 1, f);
        fwrite(";", sizeof(char*), 1, f);
        printf("\n\n%d\n\n", player->distance);
        fwrite(&player->history, sizeof(Stack*), 1, f);
        fwrite(";", sizeof(char*), 1, f);
    }
    fclose(f);
}

void save_read_file(){
    printf("on est dans save_read_file\n"); //a enlever
    FILE *f;
    printf("le nom du fichier est %s", save.date_time); //a enlever
    char* fin_element;
    // if (map != NULL){
    //     printf("map n'est pas nulle\n");
    // }
    map_init(EASY);
    player_init(EASY);
    f = fopen(save.date_time, "rb");
    if (f == NULL){
        fprintf(stderr, "Error opened file %s\n", save.date_time);
        return;
    }
    else{
        // recuperation du temps de jeu
        fread(&save.playing_time, sizeof(time_t), 1, f);
        fread(&fin_element, sizeof(char*), 1, f);
        // recuperation de la sauvegarde de la map
        printf("on est dans le else de save_charging_game\n"); //a enlever
        fread(&map->level, sizeof(int), 1, f);
        fread(&fin_element, sizeof(char*), 1, f);
        printf("map->level = %d\n", map->level); //a enlever
        for (uint8_t i = 0, j; i < MAP_LINES; i++){
            for (j = 0; j < MAP_COLS; j++){
                fread(&map->map_grid[i][j].cell_type, sizeof(int), 1, f);
                fread(&fin_element, sizeof(char*), 1, f);
                fread(&map->map_grid[i][j].distance, sizeof(int*), 1, f);
                fread(&fin_element, sizeof(char*), 1, f);
                fread(&map->map_grid[i][j].visited, sizeof(bool), 1, f);
                fread(&fin_element, sizeof(char*), 1, f);
            }
        }
         //a enlever
        for (uint8_t i = 0, j; i < MAP_LINES; i++){
            for (j = 0; j < MAP_COLS; j++){
                printf("%d ", map->map_grid[i][j].cell_type);
                printf("[%d,%d] ", map->map_grid[i][j].distance[0], map->map_grid[i][j].distance[1]);
                printf("%d,", map->map_grid[i][j].visited);
            }
            printf("\n");
        }
        // recuperation de la sauvegarde du player
        printf("player->stamina = %d, player->fruit_stack = %d, player->distant = %d", player->stamina, player->fruit_stack, player->distance); //a enlever
        fread(&player->stamina, sizeof(int8_t), 1, f);
        fread(&fin_element, sizeof(char*), 1, f);
        fread(&player->fruit_stack, sizeof(int8_t), 1, f);
        fread(&fin_element, sizeof(char*), 1, f);
        fread(&player->distance, sizeof(uint16_t), 1, f);
        fread(&fin_element, sizeof(char*), 1, f);
        printf("player->stamina = %d, player->fruit_stack = %d, player->distant = %d", player->stamina, player->fruit_stack, player->distance); //a enlever
        fread(&player->history, sizeof(Stack*), 1, f);
        fread(&fin_element, sizeof(char*), 1, f);

        // test pour voir si le player free fonctionne
        // player->move = 0;
        // player->on_obstacle = 0;
        // player->pos.c = 0;
        // player->pos.l = 0;
        printf("ça fonctionne\n"); //a enlever
    }
    printf("on est sortie du else de save_read_file\n"); //a enlever
    fclose(f);
}

void save_free(){
    free(save.date_time);
    map_free();
    player_free();
}

void save_game(){
    save_get_date();
    save_write_file();
}

void save_charging_game(){
    printf("on est dans save_charging_game\n"); //a enlever
    save_read_file();
    save_free();
}