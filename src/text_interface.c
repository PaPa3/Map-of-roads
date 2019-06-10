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
 * Funkcja akceptuje liczby z wiodącymi zerami.
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
 * Funkcja akceptuje liczby z wiodącymi zerami.
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
 * Funkcja akceptuje liczby z wiodącymi zerami.
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

/** @brief Sprawdza czy polecenie "addRoute" z wejścia nie zawiera cyklu.
 * Obsługuję polecenie typu
 * "numer drogi krajowej;nazwa miasta;długość odcinka drogi;rok budowy".
 * @p line zawiera wczytaną linię zawierjącą co najmniej jedno słowo.
 * Sprawdza czy droga krajowa zwiera cykl zakładająć poprawność argumentów.
 * @param[in] map           - wskźnik na mapę;
 * @param[in] line          - wskażnik na wiersz zawierający polecenie.
 * @return Wartość @p jeśli droga krajowa nie zawiera cykli lub @p false
 * w przeciwnym przypadku.
 */
bool checkIfRouteDoesNotHaveCycle(List *line) {
    assert(line);

    List *list = newList();
    if (list == NULL) {
        return false;
    }

    ListIterator *iterator = line->begin->next;
    while (iterator != line->end) {
        if (insertList(list->end, iterator->data) == NULL) {
            deleteList(list, false);
            return false;
        }

        for (uint32_t i = 0; i < 3; i++) {
            iterator = iterator->next;
        }
    }

    sortListOfStrings(list);
    iterator = list->begin->next;
    while (iterator != list->end) {
        if (strcmp(iterator->previous->data, iterator->data) == 0) {
            deleteList(list, false);
            return false;
        }

        iterator = iterator->next;
    }

    deleteList(list, false);
    return true;
}

/** @brief Sprawdza czy polecenie "addRoute" z wejścia jest poprawne.
 * Obsługuję polecenie typu
 * "numer drogi krajowej;nazwa miasta;długość odcinka drogi;rok budowy".
 * @p line zawiera wczytaną linię zawierjącą co najmniej jedno słowo.
 * Sprawdza poprawność wszystkich argumentów (w tym czy można dodać odpowiednie
 * odcinki na mapę dróg).
 * Funkcja akceptuje liczby z wiodącymi zerami.
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

    if (!checkIfRouteDoesNotHaveCycle(line)) {
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
//        if ((i - 1) % 900 == 0) {printf("numer %" PRIu32 "\n", i); fflush(stdout);}
//        /* Spradzamy czy miasto previousCityName nie powtorza się na liście. */
//        if (findStringOnList(iterator->next, line->end, previousCityName)) {
//            return false;
//        }

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
 * Funkcja akceptuje liczby z wiodącymi zerami.
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

        /* Dodajemy odpowiedni odcinek drogowy jeśli trzeba. */
        if (!updateRoad(map, previousCityName, cityName, length, builtYear)) {
            fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
            deleteList(line, true);
            deleteRouteModule(route);
            return;
        }

        /* Dodajemy miasto do drogi krajowej. */
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

    /* Dodajemy utworzoną drogę krajową do mapy dróg. */
    if (insertList(map->routes->end, route) == NULL) {
        fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
        deleteRouteModule(route);
    }
}

/** @brief Obsługuję polecenie "newRoute" z wejścia.
 * Obsługuję polecenie typu "newRoute routeId city1 city2".
 * @p line zawiera wczytaną linię, gdzie pierwsze słowo jest równe
 * "newRoute". Sprawdza poprawność pozostałych argumentów. Jeśli są poprawne
 * to wywołuje odpowiednią funkcję @ref newRoute.
 * Jeśli polecenie jest niepoprawne składniowo lub jego wykonanie
 * zakończyło się błędem, czyli odpowiednia funkcja zakończyła się wynikiem
 * @p false lub @p NULL, to wypisuje na standardowe wyjście diagnostyczne
 * jednoliniowy komunikat: ERROR @p lineNumber.
 * Funkcja akceptuje liczby z wiodącymi zerami.
 * Funkcja usuwa strukturę @p line.
 * @param[in,out] map       - wskźnik na mapę;
 * @param[in] lineNumber    - numer aktualnie obsługiwanej linii wejścia.
 * @param[in,out] line      - wskażnik na wiersz zawierający polecenie.
 */
void newRouteTextInterface(Map *map, uint32_t lineNumber, List *line) {
    assert(map);
    assert(line);

    if (sizeList(line) != 4) {
        fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
        deleteList(line, true);
        return;
    }

    ListIterator *iterator = line->begin->next;
    unsigned routeId = stringToUnsigned(iterator->data);

    iterator = iterator->next;
    char *cityName1 = iterator->data;

    iterator = iterator->next;
    char *cityName2 = iterator->data;

    /* Jeśli którykolwiek z argumentów był niepoprawny składniowo to teraz
     * to wykryjemy. */
    if (!newRoute(map, routeId, cityName1, cityName2)) {
        fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
    }

    deleteList(line, true);
}

/** @brief Obsługuję polecenie "extendRoute" z wejścia.
 * Obsługuję polecenie typu "extendRoute routeId city".
 * @p line zawiera wczytaną linię, gdzie pierwsze słowo jest równe
 * "extendRoute". Sprawdza poprawność pozostałych argumentów. Jeśli są poprawne
 * to wywołuje odpowiednią funkcję @ref extendRoute.
 * Jeśli polecenie jest niepoprawne składniowo lub jego wykonanie
 * zakończyło się błędem, czyli odpowiednia funkcja zakończyła się wynikiem
 * @p false lub @p NULL, to wypisuje na standardowe wyjście diagnostyczne
 * jednoliniowy komunikat: ERROR @p lineNumber.
 * Funkcja akceptuje liczby z wiodącymi zerami.
 * Funkcja usuwa strukturę @p line.
 * @param[in,out] map       - wskźnik na mapę;
 * @param[in] lineNumber    - numer aktualnie obsługiwanej linii wejścia.
 * @param[in,out] line      - wskażnik na wiersz zawierający polecenie.
 */
void extendRouteTextInterface(Map *map, uint32_t lineNumber, List *line) {
    assert(map);
    assert(line);

    if (sizeList(line) != 3) {
        fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
        deleteList(line, true);
        return;
    }

    ListIterator *iterator = line->begin->next;
    unsigned routeId = stringToUnsigned(iterator->data);

    iterator = iterator->next;
    char *cityName = iterator->data;

    /* Jeśli którykolwiek z argumentów był niepoprawny składniowo to teraz
     * to wykryjemy. */
    if (!extendRoute(map, routeId, cityName)) {
        fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
    }

    deleteList(line, true);
}

/** @brief Obsługuję polecenie "removeRoad" z wejścia.
 * Obsługuję polecenie typu "removeRoad city1 city2".
 * @p line zawiera wczytaną linię, gdzie pierwsze słowo jest równe
 * "removeRoad". Sprawdza poprawność pozostałych argumentów. Jeśli są poprawne
 * to wywołuje odpowiednią funkcję @ref removeRoad.
 * Jeśli polecenie jest niepoprawne składniowo lub jego wykonanie
 * zakończyło się błędem, czyli odpowiednia funkcja zakończyła się wynikiem
 * @p false lub @p NULL, to wypisuje na standardowe wyjście diagnostyczne
 * jednoliniowy komunikat: ERROR @p lineNumber.
 * Funkcja usuwa strukturę @p line.
 * @param[in,out] map       - wskźnik na mapę;
 * @param[in] lineNumber    - numer aktualnie obsługiwanej linii wejścia.
 * @param[in,out] line      - wskażnik na wiersz zawierający polecenie.
 */
void removeRoadTextInterface(Map *map, uint32_t lineNumber, List *line) {
    assert(map);
    assert(line);

    if (sizeList(line) != 3) {
        fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
        deleteList(line, true);
        return;
    }

    ListIterator *iterator = line->begin->next;
    char *cityName1 = iterator->data;

    iterator = iterator->next;
    char *cityName2 = iterator->data;

    /* Jeśli którykolwiek z argumentów był niepoprawny składniowo to teraz
     * to wykryjemy. */
    if (!removeRoad(map, cityName1, cityName2)) {
        fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
    }

    deleteList(line, true);
}

/** @brief Obsługuję polecenie "removeRoute" z wejścia.
 * Obsługuję polecenie typu "removeRoute routeId".
 * @p line zawiera wczytaną linię, gdzie pierwsze słowo jest równe
 * "removeRoute". Sprawdza poprawność pozostałych argumentów. Jeśli są poprawne
 * to wywołuje odpowiednią funkcję @ref removeRoute.
 * Jeśli polecenie jest niepoprawne składniowo lub jego wykonanie
 * zakończyło się błędem, czyli odpowiednia funkcja zakończyła się wynikiem
 * @p false lub @p NULL, to wypisuje na standardowe wyjście diagnostyczne
 * jednoliniowy komunikat: ERROR @p lineNumber.
 * Funkcja akceptuje liczby z wiodącymi zerami.
 * Funkcja usuwa strukturę @p line.
 * @param[in,out] map       - wskźnik na mapę;
 * @param[in] lineNumber    - numer aktualnie obsługiwanej linii wejścia.
 * @param[in,out] line      - wskażnik na wiersz zawierający polecenie.
 */
void removeRouteTextInterface(Map *map, uint32_t lineNumber, List *line) {
    assert(map);
    assert(line);

    if (sizeList(line) != 2) {
        fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
        deleteList(line, true);
        return;
    }

    ListIterator *iterator = line->begin->next;
    unsigned routeId = stringToUnsigned(iterator->data);

    /* Jeśli którykolwiek z argumentów był niepoprawny składniowo to teraz
     * to wykryjemy. */
    if (!removeRoute(map, routeId)) {
        fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
    }

    deleteList(line, true);
}

/** @brief Obsługuje pojedynczy wiersz wejścia.
 * Czyta pojedynczy wiersz i wywołuję odpowiednie operacje na danej mapie.
 * Jeśli wiersz jest postaci:
 *  - numer drogi krajowej;nazwa miasta;długość odcinka drogi;rok budowy lub
 *    ostatniego remontu;nazwa miasta;długość odcinka drogi;rok budowy lub
 *    ostatniego remontu;nazwa miasta;…;nazwa miasta - to dodaje drogę
 *      o takim przebiegu do mapy;
 *  - addRoad;city1;city2;length;builtYear - to wywołuje funkcję @ref addRoad
 *      z podanymi parametrami;
 *  - repairRoad;city1;city2;repairYear - to wywołuje funckję @ref repairRoad
 *      z podanymi parametrami;
 *  - getRouteDescription;routeId - to wywołuje funkcję @ref getRouteDescription
 *      z podanymi parametrem. Jeśli wynik działania tej funkcji jest inny niż
 *      NULL, to wypisuje na standardowe wyjście jedną linię z wynikiem
 *      działania tej funkcji;
 *  - newRoute;routeId;city1;city2 - to wywołuje funkcję @ref newRoute
 *      z podanymi parametrami;
 *  - extendRoute;routeId;city - to wywołuje funkcję @ref extendRoute
 *      z podanymi parametrami;
 *  - removeRoad;city1;city2 - to wywołuje funkcję @ref removeRoad
 *      z podanymi parametrami;
 *  - removeRoute;routeId - to wywołuje funkcję @ref removeRoute
 *      z podanymi parametrami.
 *
 * Jeśli polecenie jest niepoprawne składniowo lub jego wykonanie
 * zakończyło się błędem, czyli odpowiednia funkcja zakończyła się wynikiem
 * @p false lub @p NULL, to wypisuje na standardowe wyjście diagnostyczne
 * jednoliniowy komunikat: ERROR @p lineNumber.
 * Funkcja akceptuje liczby z wiodącymi zerami.
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

    /* Jeśli linia do zignorowania. */
    if (x == 3) {
        deleteList(line, true);
        return 0;
    }

    /* Sprawdzmy czy wystąpił jakiś błąd podczas wczytywania. */
    if (x != 0) {
        if (x == 1) {
            fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);

            /* Błąd numer 1 jest poprawnie przechwycony tutaj, dlatego zwracamy 0. */
            x = 0;
        }

        if (x == EOF) {
            char *commandName = line->begin->data;
            uint32_t lineSize = sizeList(line);

            /* Niepuste wiersze niezakończone znakiem '\n' są błędne. */
            if (!(commandName[0] == 0 && lineSize == 1)) {
                fprintf(stderr, "ERROR %" PRIu32 "\n", lineNumber);
            }
        }

        if (line != NULL) {
            deleteList(line, true);
        }

        return x;
    }

    char *commandName = line->begin->data;
//    uint32_t lineSize = sizeList(line);
//
//    /* Jeśli linia do zignorowania. */
//    if (commandName[0] == '#' || (commandName[0] == 0 && lineSize == 1)) {
//        deleteList(line, true);
//        return 0;
//    }

    if (strcmp(commandName, "addRoad") == 0) {
        addRoadTextInterface(map, lineNumber, line);
    } else if (strcmp(commandName, "repairRoad") == 0) {
        repairRoadTextInterface(map, lineNumber, line);
    } else if (strcmp(commandName, "getRouteDescription") == 0) {
        getRouteDescriptionTextInterface(map, lineNumber, line);
    } else if (strcmp(commandName, "newRoute") == 0) {
        newRouteTextInterface(map, lineNumber, line);
    } else if (strcmp(commandName, "extendRoute") == 0) {
        extendRouteTextInterface(map, lineNumber, line);
    } else if (strcmp(commandName, "removeRoad") == 0) {
        removeRoadTextInterface(map, lineNumber, line);
    } else if (strcmp(commandName, "removeRoute") == 0) {
        removeRouteTextInterface(map, lineNumber, line);
    } else {
        addRouteTextInterface(map, lineNumber, line);
    }

    return 0;
}
