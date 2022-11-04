#include "map.h"

Map *map = NULL;

int main() {
    map_init(10);
    map_display(map);
    map_free(map);
    return 0;
}