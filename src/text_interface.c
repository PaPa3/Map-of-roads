/** @file
 * Implementacja interfejsu udostępniającego interfejs tekstowy mapy dróg.
 *
 * @author Paweł Pawlik <pp406289@students.mimuw.edu.pl>
 * @date 16.05.2019
 */

#include "text_interface.h"
#include "reader.h"
#include "map.h"
#include "string_to_integer.h"
#include "city.h"
#include "route.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <inttypes.h>

void addRoadTextInterface(Map *map, uint32_t lineNumber, List *line) {
    assert(map);
    assert(line);

    if (sizeList(line) != 5) {
        fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
        deleteList(line, true);
        return;
    }

    ListIterator *iterator = line->begin->next;
    char *cityName1 = iterator->data;

    iterator = iterator->next;
    char *cityName2 = iterator->data;

    iterator = iterator->next;
    unsigned length = stringToUnsigned(iterator->data);

    iterator = iterator->next;
    int builtYear = stringToInt(iterator->data);

    if (!addRoad(map, cityName1, cityName2, length, builtYear)) {
        fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
    }

    deleteList(line, true);
}

void repairRoadTextInterface(Map *map, uint32_t lineNumber, List *line) {
    assert(map);
    assert(line);

    if (sizeList(line) != 4) {
        fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
        deleteList(line, true);
        return;
    }

    ListIterator *iterator = line->begin->next;
    char *cityName1 = iterator->data;

    iterator = iterator->next;
    char *cityName2 = iterator->data;

    iterator = iterator->next;
    int repairYear = stringToInt(iterator->data);

    if (!repairRoad(map, cityName1, cityName2, repairYear)) {
        fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
    }

    deleteList(line, true);
}

void getRouteDescriptionTextInterface(Map *map, uint32_t lineNumber, List *line) {
    assert(map);
    assert(line);

    if (sizeList(line) != 2) {
        fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
        deleteList(line, true);
        return;
    }

    ListIterator *iterator = line->begin->next;
    unsigned routeId = stringToUnsigned(iterator->data);

    char *result = (char *) getRouteDescription(map, routeId);

    if (result == NULL) {
        fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
    } else {
        printf("%s\n", result);
        free(result);
    }

    deleteList(line, true);
}

// sprawdza najpierw czy jest poprawne składniowo
void addRouteTextInterface(Map *map, uint32_t lineNumber, List *line) {
    assert(map);
    assert(line);

    uint32_t lineSize = sizeList(line);
    assert(lineSize > 0);

    ListIterator *iterator = line->begin;
    unsigned routeId = stringToUnsigned(iterator->data);

    if (lineSize % 3 != 2 || lineSize < 5 || routeId < 1 || 999 < routeId ||
            findRouteOnList(map->routes, routeId) != NULL) {
        fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
        deleteList(line, true);
        return;
    }

    for (uint32_t i = 1; i < lineSize; i++) {
        iterator = iterator->next;

        if (i % 3 == 1) { // nazwa miasta
            if (!isStringValidCityName(iterator->data)) {
                fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
                deleteList(line, true);
                return;
            }
        } else if (i % 3 == 2) { // długość odcinka
            if (stringToInt(iterator->data) == 0) {
                fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
                deleteList(line, true);
                return;
            }
        } else { // rok budowy lub ostatniego remontu
            if (stringToUnsigned(iterator->data) == 0) {
                fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
                deleteList(line, true);
                return;
            }
        }
    }

    iterator = line->begin->next;
    char *previousCity = iterator->data;
    City *city = findCityInsertIfNecessary(map->cities, previousCity);
    if (city == NULL) {
        fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
        deleteList(line, true);
        return;
    }
    Route *route = newRouteModule(routeId, city, city, map->cities);
    if (route == NULL) {
        fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
        deleteList(line, true);
        return;
    }

    for (uint32_t i = 4; i < lineSize; i += 3) {
        iterator = iterator->next;
        unsigned length = stringToUnsigned(iterator->data);

        iterator = iterator->next;
        int builtYear = stringToInt(iterator->data);

        iterator = iterator->next;

        if (!updateRoad(map, previousCity, iterator->data, length, builtYear)) {
            fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
            deleteList(line, true);
            deleteRouteModule(route);
            return;
        }

        if (insertList(route->cities->end,
                       findCityOnList(map->cities, iterator->data)) == NULL) {
            fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
            deleteList(line, true);
            deleteRouteModule(route);
            return;
        }
    }

    deleteList(line, true);

    if (insertList(map->routes->end, route) == NULL) {
        fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
        deleteRouteModule(route);
    }
}

//TODO usun
void wypisz(List *line) {
    ListIterator *it = line->begin;
    while (it != line->end) {
        printf("%s;", (char *) it->data);
        it = it->next;
    }
    printf("\n");
    fflush(stdout);
}

/** @brief Obsługuje pojedynczy wiersz wejścia.
 * Czyta pojedynczy wiersz i wywołuję odpowiednie operacje na danej mapie.
 * Jeśli wiersz jest postaci:
 *  - numer drogi krajowej;nazwa miasta;długość odcinka drogi;rok budowy lub
 *  - ostatniego remontu;nazwa miasta;długość odcinka drogi;rok budowy lub
 *  - ostatniego remontu;nazwa miasta;…;nazwa miasta - to dodaje drogę
 *      o takim przebiegu do mapy;
 *  - addRoad;city1;city2;length;builtYear - to wywołuje funkcję @ref addRoad
 *      z podanymi parametrami;
 *  - repairRoad;city1;city2;repairYear - to wywołuje funckję @ref repairRoad
 *      z podanymi parametrami;
 *  - getRouteDescription;routeId - to wywołuje funkcję @ref getRouteDescription
 *      z podanymi parametrem. Jeśli wynik działania tej funkcji jest inny niż
 *      NULL, to wypisuje na standardowe wyjście jedną linię z wynikiem
 *      działania tej funkcji.
 *
 * Jeśli polecenie jest niepoprawne składniowo lub jego wykonanie
 * zakończyło się błędem, czyli odpowiednia funkcja zakończyła się wynikiem
 * @p false lub @p NULL, to wypisuje na standardowe wyjście diagnostyczne
 * jednoliniowy komunikat: ERROR @p lineNumber.
 * @param[in,out] map       - wskźnik na mapę;
 * @param[in] lineNumber    - numer aktualnie obsługiwanej linii wejścia.
 * @return Wartość @p 0 jeśli operacja zakończyła się sukcesem,
 * wartość @p 2 jeśli nie udało się zaalokować pamięci lub
 * wartość @p EOF jeśli nie ma już więcej wierszy na wejściu.
 */
int nextCommandTextInterface(Map *map, uint32_t lineNumber) {
    assert(map);

    printf("start\n");
    fflush(stdout);

    List *line = NULL;
    int x = nextLineReader(&line);
    wypisz(line);
    if (x != 0) {
        if (x == 1) {
            fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
        }

        return x;
    }

    char *commandName = line->begin->data;
    uint32_t lineSize = sizeList(line);

    /* Jeśli linia do zignorowania. */
    if (commandName[0] == '#' || (commandName[0] == 0 && lineSize == 1)) {
        deleteList(line, true);
        return 0;
    }

    if (strcmp(commandName, "addRoad") == 0) {
        addRoadTextInterface(map, lineNumber, line);
    } else if (strcmp(commandName, "repairRoad") == 0) {
        repairRoadTextInterface(map, lineNumber, line);
    } else if (strcmp(commandName, "getRouteDescription") == 0) {
        getRouteDescriptionTextInterface(map, lineNumber, line);
    } else {
        addRouteTextInterface(map, lineNumber, line);
    }

    return 0;
}
