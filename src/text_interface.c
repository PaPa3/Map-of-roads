/** @file
 * Implementacja interfejsu udostępniającego interfejs tekstowy mapy dróg.
 *
 * @author Paweł Pawlik <pp406289@students.mimuw.edu.pl>
 * @date 16.05.2019
 */

#include "text_interface.h"
#include "reader.h"
#include "map.h"
#include "string_utilities.h"
#include "city.h"
#include "route.h"
#include "road.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <inttypes.h>

/** @brief Obsługuję polecenie "addRoad" z wejścia.
 * Obsługuję polecenie typu "addRoad city1 city2 length builtYear".
 * @p line zawiera wczytaną linię, gdzie pierwsze słowo jest równe
 * "addRoad". Sprawdza poprawność pozostałych argumentów. Jeśli są poprawne
 * to wywołuje odpowiednią funkcję @ref addRoad.
 * Jeśli polecenie jest niepoprawne składniowo lub jego wykonanie
 * zakończyło się błędem, czyli odpowiednia funkcja zakończyła się wynikiem
 * @p false lub @p NULL, to wypisuje na standardowe wyjście diagnostyczne
 * jednoliniowy komunikat: ERROR @p lineNumber.
 * Funkcja usuwa strukturę @p line.
 * @param[in,out] map       - wskźnik na mapę;
 * @param[in] lineNumber    - numer aktualnie obsługiwanej linii wejścia.
 * @param[in,out] line      - wskażnik na wiersz zawierający polecenie.
 */
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

    /* Jeśli którykolwiek z argumentów był niepoprawny składniowo to teraz
     * to wykryjemy. */
    if (!addRoad(map, cityName1, cityName2, length, builtYear)) {
        fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
    }

    deleteList(line, true);
}

/** @brief Obsługuję polecenie "repairRoad" z wejścia.
 * Obsługuję polecenie typu "repairRoad city1 city2 repairYear".
 * @p line zawiera wczytaną linię, gdzie pierwsze słowo jest równe
 * "repairRoad". Sprawdza poprawność pozostałych argumentów. Jeśli są poprawne
 * to wywołuje odpowiednią funkcję @ref repairRoad.
 * Jeśli polecenie jest niepoprawne składniowo lub jego wykonanie
 * zakończyło się błędem, czyli odpowiednia funkcja zakończyła się wynikiem
 * @p false lub @p NULL, to wypisuje na standardowe wyjście diagnostyczne
 * jednoliniowy komunikat: ERROR @p lineNumber.
 * Funkcja usuwa strukturę @p line.
 * @param[in,out] map       - wskźnik na mapę;
 * @param[in] lineNumber    - numer aktualnie obsługiwanej linii wejścia.
 * @param[in,out] line      - wskażnik na wiersz zawierający polecenie.
 */
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

    /* Jeśli którykolwiek z argumentów był niepoprawny składniowo to teraz
     * to wykryjemy. */
    if (!repairRoad(map, cityName1, cityName2, repairYear)) {
        fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
    }

    deleteList(line, true);
}

/** @brief Obsługuję polecenie "getRouteDescription" z wejścia.
 * Obsługuję polecenie typu "getRouteDescription routeId".
 * @p line zawiera wczytaną linię, gdzie pierwsze słowo jest równe
 * "getRouteDescription". Sprawdza poprawność pozostałych argumentów.
 * Jeśli są poprawne to wywołuje odpowiednią funkcję @ref getRouteDescription
 * oraz wypisuje jej wynik na standardowe wyjście.
 * Jeśli polecenie jest niepoprawne składniowo lub jego wykonanie
 * zakończyło się błędem, czyli odpowiednia funkcja zakończyła się wynikiem
 * @p false lub @p NULL, to wypisuje na standardowe wyjście diagnostyczne
 * jednoliniowy komunikat: ERROR @p lineNumber.
 * Funkcja usuwa strukturę @p line.
 * @param[in,out] map       - wskźnik na mapę;
 * @param[in] lineNumber    - numer aktualnie obsługiwanej linii wejścia.
 * @param[in,out] line      - wskażnik na wiersz zawierający polecenie.
 */
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
    if (errno == EILSEQ) {
        fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
        deleteList(line, true);
        return;
    }

    char *result = (char *) getRouteDescription(map, routeId);

    if (result == NULL) {
        fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
    } else {
        printf("%s\n", result);
        free(result);
    }

    deleteList(line, true);
}

/** @brief Sprawdza czy polecenie "addRoute" z wejścia jest poprawne.
 * Obsługuję polecenie typu
 * "numer drogi krajowej;nazwa miasta;długość odcinka drogi;rok budowy".
 * @p line zawiera wczytaną linię zawierjącą co najmniej jedno słowo.
 * Sprawdza poprawność wszystkich argumentów (w tym czy można dodać odpowiednie
 * odcinki na mapę dróg).
 * @param[in] map           - wskźnik na mapę;
 * @param[in] line          - wskażnik na wiersz zawierający polecenie.
 * @return Wartość @p jeśli polecenie jest poprawne lub @p false w przeciwnym
 * wypadku.
 */
bool checkIfRouteCanBeAdded(Map *map, List *line) {
    assert(map);
    assert(line);

    uint32_t lineSize = sizeList(line);
    assert(lineSize > 0);

    ListIterator *iterator = line->begin;
    unsigned routeId = stringToUnsigned(iterator->data);

    if (lineSize % 3 != 2 || lineSize < 5 || routeId < 1 || 999 < routeId ||
            findRouteOnList(map->routes, routeId) != NULL) {
        return false;
    }

    /* Teraz, do końca kodu, przeglądamy po kolei wszystkie
     * wymagane odcinki drogowe i
     * sprawdzamy czy są poprawne. Sprawdzamy również czy nie ma cykli,
     * czyli czy jakieś miasto nie powtarza się dwa razy. */
    iterator = iterator->next;
    char *previousCityName = iterator->data;
    if (!isStringValidCityName(previousCityName)) {
        return false;
    }
    City *previousCity = findCityOnList(map->cities, previousCityName);

    for (uint32_t i = 4; i < lineSize; i += 3) {
        /* Spradzamy czy miasto previousCityName nie powtorza się na liście. */
        if (findStringOnList(iterator->next, line->end, previousCityName)) {
            return false;
        }

        iterator = iterator->next;
        unsigned length = stringToUnsigned(iterator->data);
        if (length == 0) {
            return false;
        }

        iterator = iterator->next;
        int builtYear = stringToInt(iterator->data);
        if (builtYear == 0) {
            return false;
        }

        iterator = iterator->next;
        char *cityName = iterator->data;
        if (!isStringValidCityName(cityName)) {
            return false;
        }
        City *city = findCityOnList(map->cities, cityName);

        /* Jeśli istniały miasta previousCity i city oraz odcinek drogowy
         * między nimi, to musimy sprawdzić czy nie kłóci się on z tym,
         * który chcemy dodać. */
        if (previousCity != NULL && city != NULL) {
            ListIterator *roadIterator = findRoadModule(previousCity, city);
            if (roadIterator != NULL) {
                Road *road = roadIterator->data;
                if (road->length != length ||
                        road->buildYearOrLastRepairYear > builtYear) {
                    return false;
                }
            }
        }

        previousCityName = cityName;
        previousCity = city;
    }

    return true;
}

/** @brief Obsługuję polecenie "addRoute" z wejścia.
 * Obsługuję polecenie typu
 * "numer drogi krajowej;nazwa miasta;długość odcinka drogi;rok budowy".
 * @p line zawiera wczytaną linię zawierjącą co najmniej jedno słowo.
 * Sprawdza poprawność wszystkich argumentów (w tym czy można dodać odpowiednie
 * odcinki na mapę dróg). Jeśli są poprawne
 * to wywołuje po kolei odpowiednie funkcje @ref addRoad. Jeśli zabraknie
 * pamięci to zmiany pozostają na mapie. Na koniec dodawana jest nowa droga
 * krajowa.
 * Jeśli polecenie jest niepoprawne składniowo lub jego wykonanie
 * zakończyło się błędem, czyli odpowiednia funkcja zakończyła się wynikiem
 * @p false lub @p NULL, to wypisuje na standardowe wyjście diagnostyczne
 * jednoliniowy komunikat: ERROR @p lineNumber.
 * Funkcja usuwa strukturę @p line.
 * @param[in,out] map       - wskźnik na mapę;
 * @param[in] lineNumber    - numer aktualnie obsługiwanej linii wejścia.
 * @param[in,out] line      - wskażnik na wiersz zawierający polecenie.
 */
void addRouteTextInterface(Map *map, uint32_t lineNumber, List *line) {
    assert(map);
    assert(line);

    if (!checkIfRouteCanBeAdded(map, line)) {
        fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
        deleteList(line, true);
        return;
    }

    uint32_t lineSize = sizeList(line);
    assert(lineSize > 0);

    ListIterator *iterator = line->begin;
    unsigned routeId = stringToUnsigned(iterator->data);

    /* Teraz, do końca kodu, dodajemy po kolei odcinki drogowe do mapy oraz
     * dodajmy po kolei miasta do tworzonej drogi krajowej. */
    iterator = iterator->next;
    char *previousCityName = iterator->data;
    City *city = findCityInsertIfNecessary(map->cities, previousCityName);
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
        char *cityName = iterator->data;

        if (!updateRoad(map, previousCityName, cityName, length, builtYear)) {
            fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
            deleteList(line, true);
            deleteRouteModule(route);
            return;
        }

        if (insertList(route->cities->end,
                       findCityOnList(map->cities, cityName)) == NULL) {
            fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
            deleteList(line, true);
            deleteRouteModule(route);
            return;
        }

        previousCityName = cityName;
    }

    deleteList(line, true);

    if (insertList(map->routes->end, route) == NULL) {
        fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
        deleteRouteModule(route);
    }
}

/** @brief Obsługuje pojedynczy wiersz wejścia.
 * Czyta pojedynczy wiersz i wywołuję odpowiednie operacje na danej mapie.
 * Wiersz to pojedyncza linia wejścia zakończona znakiem '\n'.
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
 * wartość @p EOF jeśli wiersz zakończył się znakiem @p EOF.
 */
int nextCommandTextInterface(Map *map, uint32_t lineNumber) {
    assert(map);

    List *line = NULL;
    int x = nextLineReader(&line);

    /* Sprawdzmy czy wystąpił jakiś błąd podczas wczytywania. */
    if (x != 0) {
        if (x == 1) {
            fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
        }

        if (x == EOF) {
            char *commandName = line->begin->data;
            uint32_t lineSize = sizeList(line);

            /* Niepuste wiersze niezakończone znakiem '\n' są błędne. */
            if (!(commandName[0] == 0 && lineSize == 1)) {
                fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
            }

            deleteList(line, true);
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
