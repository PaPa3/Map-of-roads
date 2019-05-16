#include "map.h"
#include "text_interface.h"

#include <stdio.h>

int main() {
    Map *map = newMap();
    if (map == NULL) {
        return 0;
    }

    uint32_t lineNumber = 0;
    int x;
    do {
        x = nextCommandTextInterface(map, ++lineNumber);
    } while (x == 0);

    deleteMap(map);
    return 0;
}
