#include "save.h"
#include "common.h"

extern Map *map;
extern Player *player;
extern Game game;
extern SaveManager save;


void save_get_date(){
    save.date_time = calloc(25,sizeof(char));
    time_t now = time(NULL);
    struct tm *date = localtime(&now);

    //strftime( save.date_time, 25, "%d%m%Y_%H%M%S", date);
    strftime( save.date_time, 25, "%d%m%Y_%H%M%S.dat", date); // garder que pour les test
    printf("%s\n", save.date_time);
    
}

void save_file_name(int end) { // a modifier avec une variable global surement et a ajouter dans le .h
    if (end == 0) {
        strcat(save.date_time, ".save");
    }
    else {
        strcat(save.date_time, ".dat");
    }
    char *destination = "data/saves/" ;
    strcat(destination, save.date_time);
    save.date_time = destination;
}

// void save_write_file(){
//     FILE *f;
//     f = fopen(save.date_time, "wb");
//     if (f == NULL){
//         fprintf(stderr, "Error opened file %s\n", save.date_time);
//         exit(1);
//     }
//     else{
//         // Ecriture du temps de jeu
//         fwrite(&save.playing_time, sizeof(time_t), 1, f);
//         fwrite(";", sizeof(char*), 1, f);
//         // Ecriture de la structure map
//         fwrite(&map->level, sizeof(map->level), 1, f);
//         fwrite(";", sizeof(char*), 1, f);
//         for (uint8_t i = 0, j; i < MAP_LINES; i++){
//             for (j = 0; j < MAP_COLS; j++){
//                 fwrite(&map->map_grid[i][j].cell_type, sizeof(int), 1, f);
//                 fwrite(",", sizeof(char*), 1, f);
//                 fwrite(&map->map_grid[i][j].distance, sizeof(int*), 1, f);
//                 fwrite(",", sizeof(char*), 1, f);
//                 fwrite(&map->map_grid[i][j].visited, sizeof(bool), 1, f);
//                 fwrite(";", sizeof(char*), 1, f);
//             }
//         }
//         // Ecriture de la structure player
//         fwrite(&player->stamina, sizeof(int8_t), 1, f);
//         fwrite(";", sizeof(char*), 1, f);
//         fwrite(&player->fruit_stack, sizeof(int8_t), 1, f);
//         fwrite(";", sizeof(char*), 1, f);
//         printf("le frut stack à la base écirt : %d\n", player->fruit_stack);
//         fwrite(&player->distance, sizeof(uint16_t), 1, f);
//         fwrite(";", sizeof(char*), 1, f);
//         printf("la distance à la base écirte : %d\n", player->distance);
//         fwrite(&player->history, sizeof(Stack*), 1, f);
//         fwrite(";", sizeof(char*), 1, f);
//     }
//     fclose(f);
// }

void save_write_file(){
    FILE *f;
    f = fopen(save.date_time, "wb");
    if (f == NULL){
        fprintf(stderr, "Error opened file %s\n", save.date_time);
        exit(1);
    }
    else{
        // Ecriture du temps de jeu
        fwrite(&save.playing_time, sizeof(time_t), 1, f);
        fwrite(";", sizeof(char*), 1, f);
        // Ecriture de la structure map
        save_write_map(f);
        // Ecriture de la structure player
        save_write_player(f);
        // Ecriture de la structure game
        save_write_game(f);
    }
    fclose(f);
}

void save_write_map(FILE *f) {
    fwrite(&map->level, sizeof(map->level), 1, f);
    fwrite(";", sizeof(char*), 1, f);
    for (uint8_t i = 0, j; i < MAP_LINES; i++){
        for (j = 0; j < MAP_COLS; j++){
            fwrite(&map->map_grid[i][j].cell_type, sizeof(int), 1, f);
            fwrite(",", sizeof(char*), 1, f);
            fwrite(&map->map_grid[i][j].distance, sizeof(uint8_t), 2, f);
            fwrite(",", sizeof(char*), 1, f);
            fwrite(&map->map_grid[i][j].visited, sizeof(bool), 1, f);
            fwrite(";", sizeof(char*), 1, f);
        }
    }
}

void save_write_player(FILE *f) {
    fwrite(&player->stamina, sizeof(int8_t), 1, f);
    fwrite(";", sizeof(char*), 1, f);
    fwrite(&player->fruit_stack, sizeof(int8_t), 1, f);
    fwrite(";", sizeof(char*), 1, f);
    printf("le frut stack à la base écirt : %d\n", player->fruit_stack);
    fwrite(&player->distance, sizeof(uint16_t), 1, f);
    fwrite(";", sizeof(char*), 1, f);
    printf("la distance à la base écirte : %d\n", player->distance);
    // stack_display(player->history);
    fwrite(&player->history, sizeof(Stack*), 1, f);
    fwrite(";", sizeof(char*), 1, f);
}

void save_write_game(FILE *f){
    fwrite(&game.path, sizeof(Stack*), 1, f);
    fwrite(";", sizeof(char*), 1, f);
}

// void save_read_file(){
//     // printf("on est dans save_read_file\n"); //a enlever
//     FILE *f;
//     // printf("le nom du fichier est %s", save.date_time); //a enlever
//     char* fin_element;
//     // if (map != NULL){
//     //     printf("map n'est pas nulle\n");
//     // }
//     // map_init(EASY);
//     // player_init(EASY);
//     f = fopen(save.date_time, "rb");
//     if (f == NULL){
//         fprintf(stderr, "Error opened file %s\n", save.date_time);
//         return;
//     }
//     else{
//         // recuperation du temps de jeu
//         fread(&save.playing_time, sizeof(time_t), 1, f);
//         fread(&fin_element, sizeof(char*), 1, f);
//         // recuperation de la sauvegarde de la map
//         // printf("on est dans le else de save_charging_game\n"); //a enlever
//         fread(&map->level, sizeof(int), 1, f);
//         fread(&fin_element, sizeof(char*), 1, f);
//         // printf("map->level = %d\n", map->level); //a enlever
//         for (uint8_t i = 0, j; i < MAP_LINES; i++){
//             for (j = 0; j < MAP_COLS; j++){
//                 fread(&map->map_grid[i][j].cell_type, sizeof(int), 1, f);
//                 fread(&fin_element, sizeof(char*), 1, f);
//                 fread(&map->map_grid[i][j].distance, sizeof(int*), 1, f);
//                 fread(&fin_element, sizeof(char*), 1, f);
//                 fread(&map->map_grid[i][j].visited, sizeof(bool), 1, f);
//                 fread(&fin_element, sizeof(char*), 1, f);
//             }
//         }
//          //a enlever
//         // for (uint8_t i = 0, j; i < MAP_LINES; i++){
//         //     for (j = 0; j < MAP_COLS; j++){
//         //         printf("%d ", map->map_grid[i][j].cell_type);
//         //         printf("[%d,%d] ", map->map_grid[i][j].distance[0], map->map_grid[i][j].distance[1]);
//         //         printf("%d,", map->map_grid[i][j].visited);
//         //     }
//         //     printf("\n");
//         // }
//         // recuperation de la sauvegarde du player
//         fread(&player->stamina, sizeof(int8_t), 1, f);
//         fread(&fin_element, sizeof(char*), 1, f);
//         fread(&player->fruit_stack, sizeof(int8_t), 1, f);
//         fread(&fin_element, sizeof(char*), 1, f);
//         fread(&player->distance, sizeof(uint16_t), 1, f);
//         fread(&fin_element, sizeof(char*), 1, f);
//         // printf("Apres : player->stamina = %d, player->fruit_stack = %d, player->distant = %d\n", player->stamina, player->fruit_stack, player->distance); //a enlever
//         fread(&player->history, sizeof(Stack*), 1, f);
//         fread(&fin_element, sizeof(char*), 1, f);

//         // test pour voir si le player free fonctionne
//         // player->move = 0;
//         // player->on_obstacle = 0;
//         // player->pos.c = 0;
//         // player->pos.l = 0;
//         // printf("ça fonctionne\n"); //a enlever
//     }
//     // printf("on est sortie du else de save_read_file\n"); //a enlever
//     fclose(f);
// }

void save_read_file(){
    // printf("on est dans save_read_file\n"); //a enlever
    FILE *f;
    // printf("le nom du fichier est %s", save.date_time); //a enlever
    // if (map != NULL){
    //     printf("map n'est pas nulle\n");
    // }
    char* fin_element;
    // map_init(EASY);
    // player_init(EASY);
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
        // printf("on est dans le else de save_charging_game\n"); //a enlever
        save_read_map(f);
         //a enlever
        // for (uint8_t i = 0, j; i < MAP_LINES; i++){
        //     for (j = 0; j < MAP_COLS; j++){
        //         printf("%d ", map->map_grid[i][j].cell_type);
        //         printf("[%d,%d] ", map->map_grid[i][j].distance[0], map->map_grid[i][j].distance[1]);
        //         printf("%d,", map->map_grid[i][j].visited);
        //     }
        //     printf("\n");
        // }
        // recuperation de la sauvegarde du player
        save_read_player(f);

        // test pour voir si le player free fonctionne
        // player->move = 0;
        // player->on_obstacle = 0;
        // player->pos.c = 0;
        // player->pos.l = 0;
        // printf("ça fonctionne\n"); //a enlever

        // recuperation de la sauvegarde de game
        save_read_game(f);
    }
    // printf("on est sortie du else de save_read_file\n"); //a enlever
    fclose(f);
}

void save_read_map(FILE *f) {
    char* fin_element;
    fread(&map->level, sizeof(int), 1, f);
    fread(&fin_element, sizeof(char*), 1, f);
    // printf("map->level = %d\n", map->level); //a enlever
    for (uint8_t i = 0, j; i < MAP_LINES; i++){
        for (j = 0; j < MAP_COLS; j++){
            fread(&map->map_grid[i][j].cell_type, sizeof(int), 1, f);
            fread(&fin_element, sizeof(char*), 1, f);
            fread(&map->map_grid[i][j].distance, sizeof(uint8_t), 2, f);
            fread(&fin_element, sizeof(char*), 1, f);
            fread(&map->map_grid[i][j].visited, sizeof(bool), 1, f);
            fread(&fin_element, sizeof(char*), 1, f);
        }
    }
}

void save_read_player(FILE *f) {
    char* fin_element;
    fread(&player->stamina, sizeof(int8_t), 1, f);
    fread(&fin_element, sizeof(char*), 1, f);
    fread(&player->fruit_stack, sizeof(int8_t), 1, f);
    fread(&fin_element, sizeof(char*), 1, f);
    fread(&player->distance, sizeof(uint16_t), 1, f);
    fread(&fin_element, sizeof(char*), 1, f);
    // printf("Apres : player->stamina = %d, player->fruit_stack = %d, player->distant = %d\n", player->stamina, player->fruit_stack, player->distance); //a enlever
    Stack *tmp_history;
    fread(&tmp_history, sizeof(Stack*), 1, f);
    // fread(&player->history, sizeof(Stack*), 1, f);
    fread(&fin_element, sizeof(char*), 1, f);
    
    // recuperation dans le bonne ordre de la stack player history
    map_render_path(tmp_history);
    player->history = stack_change_order(tmp_history);
    map_render_path(player->history);
    stack_free(tmp_history);
    //player->pos = player->history->head->pos;
}

void save_read_game(FILE *f){
    char* fin_element;
    fread(&game.path, sizeof(Stack*), 1, f);
    fread(&fin_element, sizeof(char*), 1, f);
}

void save_free(){ //inutilisee
    free(save.date_time);
    map_free();
    player_free();
}

void save_game(){
    stack_display(player->history);
    save_get_date();
    save_write_file();
    stack_display(player->history);
}

void save_charging_game(){
    // printf("on est dans save_charging_game\n"); //a enlever
    save_read_file();
    //save_free();
}