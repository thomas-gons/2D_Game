#include "map.h"
#include "util.h"

extern Map *map;
int8_t moveset[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};


void map_random_fill() {
    srand(time(NULL));
    float distance, dn;
    float prob;
    // Tiles' probability distribution according to the difficulty
    float prob_fruit, tmp_prob_fruit;
    float prob_obs, tmp_prob_obs;

    for (uint8_t i = 0, j; i < MAP_SIZE; i++) {
        for (j = 0; j < MAP_SIZE*2; j++) {
            prob_fruit = PROB_FRUIT;
            prob_obs = PROB_OBS;
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

Stack *map_get_open_cells() {
    Stack *all_opened_cell = stack_init();
    for (uint8_t i = 0, j; i < MAP_SIZE; i++) {
        for (j = 0; j < MAP_SIZE*2; j++) 
            stack_push(all_opened_cell, (Position) {i, j});
    }
    return all_opened_cell;
}

Position map_get_random_obstacle(Stack *all_opened_cell) {
    Node *tmp = all_opened_cell->head;
    Position cell_pos;
    printf("len: %d\n", stack_len(all_opened_cell));
    uint16_t rand_idx = rand() % stack_len(all_opened_cell);
    for (uint16_t i = 0; tmp; tmp = tmp->next, i++) {
        if (i == rand_idx) {
            cell_pos = stack_remove(all_opened_cell, rand_idx);
            break;
        }
    }
    if (cell_pos.x == 0 && cell_pos.y == 0) {
        printf("ERROR : can not get the cell at index %u\n", rand_idx);
        exit(1);
    }
    map->map_grid[cell_pos.x][cell_pos.y].state = BLOCK;
    return cell_pos;
}

bool is_path_blocked(Stack *path, Position rand_block_pos) {
    for (; path->head; path->head = path->head->next) {
        if (path->head->pos.x == rand_block_pos.x &&
            path->head->pos.y == rand_block_pos.y)
            return true;
    }
    return false;
}

bool rec_research(Stack *path, Position coord, bool check_path) {
    int8_t tmp_x, tmp_y;
    for (uint8_t i = 0; i < 4; i++) {
        tmp_x = coord.x + moveset[i][0];
        tmp_y = coord.y + moveset[i][1];
        // check if the path exists and if it has not yet been taken
        if (IS_NOT_INDEX_ERROR(tmp_x, tmp_y) &&
            IS_NOT_PAST_POS(tmp_x, tmp_y, path->head->pos)) {
            // we can not take a path with an obstacle
            if (map->map_grid[tmp_x][tmp_y].state == OPEN && check_path) {
                coord.x = tmp_x;
                coord.y = tmp_y;
                // save the taken path
                stack_push(path, coord);
                // check if the current position is not the arrival
                if (coord.x != MAP_SIZE  - 1 || coord.y != MAP_SIZE - 1)
                    check_path = rec_research(path, coord, check_path);
                else
                    // no path is available
                    check_path = false;
            }
        }
    }
    // we start from a previous position with recursion by removing the last position in the stack
    if (check_path)
        stack_pop(path);
    
    return check_path;
}

Stack *path_finder() {
    Position coord = {0, 0};
    Stack *path = stack_init();
    stack_push(path, coord);
    rec_research(path, coord, true);
    return path;
}

void map_write_path(Stack* path) {
    for (; path->head; path->head = path->head->next)
        map->map_grid[path->head->pos.x][path->head->pos.y].cell_type = ROAD;
}


