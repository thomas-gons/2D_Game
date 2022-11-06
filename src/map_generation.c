#include "map.h"
#include "util.h"

extern Map *map;

// TODO: random path generation algorithm (Tiles ROAD): Melvyn

void map_random_fill() {
    srand(time(NULL));
    float distance, dn;
    float prob;
    // Tiles' probability distribution according to the difficulty
    float prob_fruit, tmp_prob_fruit;
    float prob_obs, tmp_prob_obs;

    for (uint8_t i = 0, j; i < MAP_SIZE; i++) {
        for (j = 0; j < MAP_SIZE*2; j++) {
            prob_fruit = PROB_FRUIT / map->level;
            prob_obs = PROB_OBS * map->level;
            // distance between center and current position
            distance = sqrt(pow(CENTER_X - i, 2) + pow(CENTER_Y - j, 2));
            dn = distance / MAX_DISTANCE;
            // prob between 0 and 1
            prob = (float) rand() / RAND_MAX;
            tmp_prob_fruit = prob_fruit * (dn * LAMBDA_FRUIT);
            tmp_prob_obs = prob_obs / (dn * LAMBDA_OBS);

            // Tile type is based on the probability distribution model (radial here)
            prob_fruit = (tmp_prob_fruit > PROB_MAX_FRUIT) ?
                PROB_MAX_FRUIT: tmp_prob_fruit;
            prob_obs = (tmp_prob_obs > PROB_MAX_OBS) ?
                PROB_MAX_OBS: tmp_prob_obs;
            
            if (map->map_grid[i][j].cell_type == EMPTY) {
                map->map_grid[i][j].cell_type = (prob <= prob_fruit) ? FRUIT:
                    (prob <= prob_obs) ? OBSTACLE: ROAD;
            }

        }
    }
}


//use else if
int recursiv_research1(uint8_t dim, Stack *path, Position coord,uint8_t verif_path) {
    //can we go to the right?
    printf("coord x = %d coord y = %d\n",coord.x , coord.y);
    if(coord.x + 1 <= dim-1 && coord.x + 1 != path->head->pos.x ){
        if(map->map_grid[coord.x + 1][coord.y].state==OPEN && verif_path==0 ) {
            coord.x+=1;
            stack_push(path, coord);
            printf("ca va a droite\n");
            if(coord.x != dim-1 || coord.y != dim-1) {
                verif_path = recursiv_research1(dim,path,coord,verif_path) ; 
            }
            else {
                verif_path = 1;
            }         
        }
    }
    //can we go down?
    if(coord.y + 1 <= dim-1  && coord.y + 1 != path->head->pos.y ){
        if(map->map_grid[coord.x][coord.y + 1].state==OPEN && verif_path==0 ) {
            coord.y+=1;
            stack_push(path, coord);
            printf("ca va en bas\n");
            if(coord.x != dim-1 || coord.y != dim-1) {
                verif_path = recursiv_research1(dim,path,coord,verif_path) ; 
            }
            else {
                verif_path = 1;
            }   
        }
    }
    //can we go to the left?
    if(coord.x - 1 >= 0 && coord.x - 1 != path->head->pos.x ){
        if(map->map_grid[coord.x - 1][coord.y].state==OPEN && verif_path==0 ) {
            coord.x-=1;
            stack_push(path, coord);
            printf("ca va a gauche\n");
            if(coord.x != dim-1 || coord.y != dim-1) {
                verif_path = recursiv_research1(dim,path,coord,verif_path) ;  
            }
            else {
                verif_path = 1;
            }    
        }
    }
    //can we go up?
    if(coord.y - 1 >= 0 && coord.y - 1 != path->head->pos.y ){
        if(map->map_grid[coord.x][coord.y - 1].state==OPEN && verif_path==0 ) {
            coord.y-=1;
            stack_push(path, coord);
            printf("ca va en haut\n");
            if(coord.x != dim-1 || coord.y != dim-1) {
                verif_path = recursiv_research1(dim,path,coord,verif_path) ;  
            }
            else {
                verif_path = 1;
            }    
        }
    }
     //if cannot find a way go back
    if(verif_path != 1) {
        stack_pop(path);
    }
    return verif_path;
}



void path_finder() {
    Position coord;
    coord.x=0;
    coord.y=0;
    Stack *path = stack_init();
    stack_push(path, coord);
    uint8_t verif_path = 0;
    verif_path = recursiv_research1(MAP_SIZE, path, coord, verif_path);
    stack_display(path);
}