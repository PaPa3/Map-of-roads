/** @file
 * Main do obsługi interfejsu tekstowego mapy dróg.
 *
 * @author Paweł Pawlik <pp406289@students.mimuw.edu.pl>
 * @date 18.05.2019
 */

#include "map.h"
#include "text_interface.h"

#include <stdio.h>

/** @brief Funkcja main.
 * @return Wartość @p 0.
 */
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
