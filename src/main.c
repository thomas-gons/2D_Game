#include "map.h"

Map *map = NULL;

int main() {
    map_init(MAP_MEDIUM, MEDIUM);
    map_random_fill();
    map_display();
    map_free();
    return 0;
}