/** @file
 * Implementacja interfejsu klasy przechowującej drogę krajową.
 *
 * @author Paweł Pawlik <pp406289@students.mimuw.edu.pl>
 * @date 29.04.2019
 */

#include "route.h"
#include "heap.h"
#include "road.h"
#include "string_builder.h"

#include <stdlib.h>
#include <assert.h>

#define INFINITY 1000000000000000ll ///< stała oznaczająca nieskończoność

/** @brief Usuwa strukturę.
 * @param[in,out] route         - wskaźnik na drogę krajową do usunięcia.
 */
void deleteRouteModule(Route *route) {
    assert(route);

    deleteList(route->cities, false);
    free(route);
}

/** @brief Porównuje dwie drogi krajowe.
 * Pierwsza droga jest lepsza jeśli ma mniejszą odległość. Jeśli dane dwie
 * drogi mają taką samą odległość to lepsza jest ta, której
 * najdawniej wybudowany odcinek jest najmłodszy.
 * @param[in] distance1         - odległość pierwszej drogi;
 * @param[in] oldestRoute1      - najdawniej wybudowany odcinek pierwszej drogi;
 * @param[in] distance2         - odległość drugiej drogi;
 * @param[in] oldestRoute2      - najdawniej wybudowany odcinek drugiej drogi.
 * @return Zwraca 0 jeśli obie drogi są tak samo dobre. Zwraca ujemną wartość
 * jeśli pierwsza droga jest lepsza niż druga. Zwraca dodatnią wartość
 * w przeciwnym przypadku.
 */
int64_t compareRoutes(int64_t distance1, int64_t oldestRoute1,
                      int64_t distance2, int64_t oldestRoute2) {
    if (distance1 == distance2) {
        return oldestRoute2 - oldestRoute1;
    }

    return distance1 - distance2;
}

/** @brief Minimum dwóch liczb.
 * @param x                     - pierwsza liczba;
 * @param y                     - druga liczba.
 * @return Minimum dwóch liczb.
 */
int64_t minInt64_t(int64_t x, int64_t y) {
    if (x < y) {
        return x;
    } else {
        return y;
    }
}

/** @brief Symuluje algorytm dijsktry.
 * Znajduję najkrótszą ścieżkę z miasta @p from do miasta @p to oraz do miasta
 * @p to2 (jeśli @p to2 != NULL). Ścieżka ta nie wchodzi do wierzchołków
 * znajdujących się na drodze krajowej @p route
 * (oprócz @p from, @p to oraz @p to2). Jeśli @p knownOldestRoad == 0
 * to ścieżki porównywane są jak w funkcji @ref compareRoutes. W przeciwnym
 * wypadku ścieżki porównywane są jedynie po odległość oraz pomijane są wtedy
 * odcinki drogowe, których rok budowy lub remontu jest starszy niż wartość
 * @p knownOldestRoad.
 * @param[in] route             - wskaźnik na drogę krajową;
 * @param[in] from              - wskaźnik na miasto startowe algorytmu;
 * @param[in] to                - wskaźnik na docelowe miasto;
 * @param[in] to2               - wskaźnik na docelowe miasto lub NULL;
 * @param[in,out] listOfCities  - wskaźnik na wszystkie miasta na danej mapie.
 * @param[in] knownOldestRoad   - rok, od którego mamy zacząć uwzględniać odcinki.
 * @return Wartość @p true lub @p false jeśli nie udało się zaalokować pamięci.
 */
bool dijkstraRouteModule(Route *route, City *from, City *to, City *to2,
                         List *listOfCities, int64_t knownOldestRoad) {
    assert(route);
    assert(from);
    assert(to);
    assert(listOfCities);

    Heap *heap = newHeap();
    if (heap == NULL) {
        return false;
    }

    /* Ustawiamy odległości miast na nieskończoność. */
    ListIterator *iterator = listOfCities->begin;
    while (iterator != listOfCities->end) {
        ((City *)iterator->data)->distance = INFINITY;
        ((City *)iterator->data)->oldestRoadOnRoute = INFINITY;
        iterator = iterator->next;
    }

    /* Ustawiamy odległości miast na drodze krajowe na -1
     * przez co ich nie odwiedzimy. */
    if (route->cities != NULL) {
        iterator = route->cities->begin;
        while (iterator != route->cities->end) {
            ((City *)iterator->data)->distance = -1;
            iterator = iterator->next;
        }
    }

    from->distance = 0;
    from->oldestRoadOnRoute = INFINITY;
    from->isRouteUnequivocal = true;
    if (!pushHeap(heap, 0, INFINITY, from)) {
        deleteHeap(heap);
        return false;
    }

    /* Ustawiamy odległości miast docelowych, aby móc je odwiedzić nawet
     * jeśli są na drodze krajowej. */
    to->distance = INFINITY;
    if (to2 != NULL) {
        to2->distance = INFINITY;
    }

    City *ptr;
    while (heap->size != 0) {
        ptr = heap->data[1];

        if (heap->keys[1]->distance != ptr->distance) {
            popHeap(heap);
            continue;
        }

        if (knownOldestRoad == 0 &&
                heap->keys[1]->oldestRoad != ptr->oldestRoadOnRoute) {
            popHeap(heap);
            continue;
        }

        popHeap(heap);

        iterator = ptr->roads->begin;
        Road *road;
        while (iterator != ptr->roads->end) {
            road = iterator->data;

            if (road->isDeleted) {
                iterator = iterator->next;
                continue;
            }

            if (knownOldestRoad != 0 &&
                    knownOldestRoad > road->buildYearOrLastRepairYear) {
                iterator = iterator->next;
                continue;
            }

            int64_t distance = ptr->distance + road->length;
            int64_t oldestRoute = minInt64_t(ptr->oldestRoadOnRoute,
                                             road->buildYearOrLastRepairYear);
            int64_t c = compareRoutes(distance, oldestRoute,
                                  road->destination->distance,
                                  road->destination->oldestRoadOnRoute);

            if (knownOldestRoad != 0) {
                c = distance - road->destination->distance;
            }

            if (c < 0) {
                road->destination->distance = distance;
                road->destination->oldestRoadOnRoute = oldestRoute;
                road->destination->isRouteUnequivocal = ptr->isRouteUnequivocal;
                road->destination->previousOnRoute = ptr;

                if (!pushHeap(heap, distance, oldestRoute, road->destination)) {
                    deleteHeap(heap);
                    return false;
                }
            } else if (c == 0) {
                road->destination->isRouteUnequivocal = false;
            }

            iterator = iterator->next;
        }
    }

    deleteHeap(heap);

    return true;
}

/** @brief Zanjduje drogę między miastami.
 * Znajduje drogę krajową z miasta @p from do miasta @p to lub do miasta
 * @p to2 (jeśli @p to2 != NULL). Droga ta nie wchodzi do wierzchołków
 * znajdujących się na drodce krajowej @p route
 * (oprócz @p from, @p to oraz @p to2). Wśród istniejących odcinków dróg
 * wyszukuje najkrótszą drogę. Jeśli jest więcej niż jeden sposób takiego wyboru,
 * to dla każdego wariantu wyznacza wśród wybranych w nim odcinków dróg ten,
 * który był najdawniej wybudowany lub remontowany i wybiera wariant
 * z odcinkiem, który jest najmłodszy.
 * @param[in] route             - wskaźnik na drogę krajową;
 * @param[in] from              - wskaźnik na miasto startowe algorytmu;
 * @param[in] to                - wskaźnik na docelowe miasto;
 * @param[in] to2               - wskaźnik na docelowe miasto lub NULL;
 * @param[in] listOfCities      - wskaźnik na wszystkie miasta na danej mapie.
 * @return Wskaźnik na listę zawierającą szukaną drogę lub NULL jeśli droga nie
 * jest jednoznaczna lub nie udało się zaalokować pamięci.
 */
List *findRouteModule(Route *route, City *from, City *to, City *to2,
                      List *listOfCities) {
    assert(route);
    assert(from);
    assert(to);
    assert(listOfCities);

    if (!dijkstraRouteModule(route, from, to, to2, listOfCities, 0)) {
        return NULL;
    }

    int64_t knownOldestRoad = to->oldestRoadOnRoute;
    if (to2 != NULL) {
        if (to->distance == to2->distance) {
            knownOldestRoad = -minInt64_t(-knownOldestRoad,
                                          -to2->oldestRoadOnRoute); //max
        } else if (to->distance > to2->distance) {
            knownOldestRoad = to2->oldestRoadOnRoute;
        }
    }

    if (knownOldestRoad == INFINITY) {
        return NULL;
    }

    if (!dijkstraRouteModule(route, from, to, to2, listOfCities,
                             knownOldestRoad)) {
        return NULL;
    }

    if (to2 != NULL) {
        int64_t c = compareRoutes(to->distance, to->oldestRoadOnRoute,
                                  to2->distance, to2->oldestRoadOnRoute);
        if (c == 0) {
            return NULL;
        } else if (c > 0) {
            to = to2;
        }
    }

    if (to->distance == INFINITY || !to->isRouteUnequivocal) {
        return NULL;
    }

    /* Odzyskiwanie trasy szukanej drogi. */
    List *result = newList();
    if (result == NULL) {
        return NULL;
    }

    City *ptr = to;
    while (true) {
        if (insertList(result->begin, ptr) == NULL) {
            deleteList(result, false);
            return NULL;
        }

        if (ptr == from) {
            break;
        }

        ptr = ptr->previousOnRoute;
    }

    return result;
}

/** @brief Tworzy strukturę.
 * Tworzy drogę krajową pomiędzy dwoma miastami i nadaje jej podany numer.
 * Wśród istniejących odcinków dróg wyszukuje najkrótszą drogę. Jeśli jest
 * więcej niż jeden sposób takiego wyboru, to dla każdego wariantu wyznacza
 * wśród wybranych w nim odcinków dróg ten, który był najdawniej wybudowany lub
 * remontowany i wybiera wariant z odcinkiem, który jest najmłodszy.
 * @param[in] routeId           - numer drogi krajowej;
 * @param[in] city1             - wskaźnik na pierwsze miasto;
 * @param[in] city2             - wskaźnik na drugie miasto;
 * @param[in] listOfCities      - wskaźnik na wszystkie miasta na danej mapie.
 * @return Wartość @p true, jeśli droga krajowa została utworzona.
 * Wartość @p false, jeśli wystąpił błąd: nie można
 * jednoznacznie wyznaczyć drogi krajowej między podanymi miastami lub nie udało
 * się zaalokować pamięci.
 */
Route *newRouteModule(unsigned routeId, City *city1, City *city2,
                      List *listOfCities) {
    Route *result = malloc(sizeof(Route));
    if (result == NULL) {
        return NULL;
    }

    result->routeId = routeId;
    result->cities = NULL;
    result->cities = findRouteModule(result, city1, city2, NULL, listOfCities);
    if (result->cities == NULL) {
        free(result);
        return NULL;
    }

    return result;
}

/** @brief Poprawia drogę krajową.
 * Poprawia drogę krajową po usunięciu odcinka drogi między dwoma miastami.
 * Jeśli usunięcie tego odcinka drogi
 * spowodowało przerwanie ciągu rogi krajowej, to uzupełnia ją
 * istniejącymi odcinkami dróg w taki sposób, aby była najkrótsza. Jeśli jest
 * więcej niż jeden sposób takiego uzupełnienia, to dla każdego wariantu
 * wyznacza wśród dodawanych odcinków drogi ten, który był najdawniej wybudowany
 * lub remontowany i wybiera wariant z odcinkiem, który jest najmłodszy.
 * @param[in,out] route         - wskaźnik drogę krajową do poprawienie;
 * @param[in] city1             - wskaźnik na pierwsze miasto;
 * @param[in] city2             - wskaźnik na drugie miasto;
 * @param listOfCities          - wskaźnik na wszystkie miasta na danej mapie.
 * @return Wartość @p true jeśli udało się poprawić drogę krajową, lub @p false,
 * jeśli nie udało się zaalokować pamięci.
 */
bool findNewRouteAfterRemovingRoad(Route *route, City *city1, City *city2,
                                   List *listOfCities) {
    assert(route);
    assert(city1);
    assert(city2);
    assert(listOfCities);

    route->wasChanged = false;

    ListIterator *iterator = route->cities->begin;
    while (iterator != route->cities->end &&
           iterator->data != city1 && iterator->data != city2) {
        iterator = iterator->next;
    }

    if (iterator == route->cities->end ||
            iterator->next == route->cities->end) {
        return true;
    }

    if (iterator->next->data != city1 && iterator->next->data != city2) {
        return true;
    }

    List *list;
    if (iterator->data == city1) {
        list = findRouteModule(route, city1, city2, NULL, listOfCities);
    } else {
        list = findRouteModule(route, city2, city1, NULL, listOfCities);
    }

    if (list == NULL) {
        return false;
    }

    eraseList(list->begin, false);
    eraseList(list->end->previous, false);
    spliceList(iterator->next, list);
    deleteList(list, false);

    route->wasChanged = true;
    return true;
}

/** @brief Cofa zmiany wywołane przez @ref findNewRouteAfterRemovingRoad.
 * Cofa zmiany wywołane przez ostatnie użycie
 * @ref findNewRouteAfterRemovingRoad. Miasta (argumenty funkcji) muszą być
 * takie same jak przy ostatnim wywołaniu @ref findNewRouteAfterRemovingRoad
 * dla danej drogi krajowej.
 * @param[in,out] route         - wskaźnik drogę krajową;
 * @param[in] city1             - wskaźnik na pierwsze miasto;
 * @param[in] city2             - wskaźnik na drugie miasto.
 */
void undoFindNewRouteAfterRemovingRoad(Route *route, City *city1, City *city2) {
    assert(route);
    assert(city1);
    assert(city2);

    if (!route->wasChanged) {
        return;
    }

    ListIterator *iterator = route->cities->begin;
    while (iterator != route->cities->end &&
           iterator->data != city1 && iterator->data != city2) {
        iterator = iterator->next;
    }

    iterator = iterator->next;
    while (iterator->data != city1 && iterator->data != city2) {
        iterator = iterator->next;
        eraseList(iterator->previous, false);
    }
}

/**
 * @brief Wydłuża drogę krajową do podanego miasta.
 * Dodaje do drogi krajowej nowe odcinki dróg do podanego miasta w taki sposób,
 * aby nowy fragment drogi krajowej był najkrótszy. Jeśli jest więcej niż jeden
 * sposób takiego wydłużenia, to dla każdego wariantu wyznacza wśród dodawanych
 * odcinków dróg ten, który był najdawniej wybudowany.
 * @param[in,out] route         - wskaźnik drogę krajową;
 * @param[in] city              - wskaźnik na miasto;
 * @param[in] listOfCities      - wskaźnik na wszystkie miasta na danej mapie.
 * @return Wartość @p true, jeśli droga krajowa została wydłużona.
 * Wartość @p false, jeśli wystąpił błąd: nie można jednoznacznie
 * wyznaczyć nowego fragmentu drogi krajowej lub nie udało się zaalokować
 * pamięci.
 */
bool findNewRouteAfterExtend(Route *route, City *city, List *listOfCities) {
    assert(route);
    assert(city);
    assert(listOfCities);

    List *list = findRouteModule(route, city, route->cities->begin->data,
                                 backList(route->cities), listOfCities);
    if (list == NULL) {
        return false;
    }

    if (backList(list) == route->cities->begin->data) {
        eraseList(list->end->previous, false);
        spliceList(route->cities->begin, list);
        deleteList(list, false);
    } else {
        eraseList(list->end->previous, false);
        reverseList(list);
        spliceList(route->cities->end, list);
        deleteList(list, false);
    }

    return true;
}

/** @brief Znajduję drogę krajową na liście.
 * Znajduję drogę krajową o danym numerze na liście.
 * @param[in] list              - wskaźnik na listę dróg krajowych;
 * @param[in] routeId           - numer szukanej drogi krajowej.
 * @return Wskaźnik na szukaną drogę krajową lub NULL, jeśli na liści nie ma
 * szukanej drogi krajowej.
 */
Route *findRouteOnList(List *list, unsigned routeId) {
    assert(list);

    ListIterator *iterator = list->begin;
    while (iterator != list->end) {
        if (((Route *)iterator->data)->routeId == routeId) {
            return iterator->data;
        }
        iterator = iterator->next;
    }

    return NULL;
}

/** @brief Udostępnia informacje o drodze krajowej.
 * Zwraca wskaźnik na napis, który zawiera informacje o drodze krajowej. Alokuje
 * pamięć na ten napis. Zaalokowaną pamięć trzeba zwolnić za pomocą funkcji free.
 * @param[in] route             - wskaźnik na drogę krajową.
 * @return Wskaźnik na napis lub NULL, gdy nie udało się zaalokować pamięci.
 */
char *descriptionRouteModule(Route *route) {
    assert(route);

    StringBuilder *result = newStringBuilder();
    if (result == NULL) {
        return NULL;
    }

    if (!appendStringBuilderInteger(result, route->routeId)) {
        deleteStringBuilder(result, true);
        return NULL;
    }

    ListIterator *iterator = route->cities->begin;
    City *city = iterator->data;
    if (!appendStringBuilderString(result, city->name)) {
        deleteStringBuilder(result, true);
        return NULL;
    }

    while (iterator != route->cities->end->previous) {
        Road *road = findRoadModule(iterator->data, iterator->next->data)->data;

        if (!appendStringBuilderInteger(result, road->length)) {
            deleteStringBuilder(result, true);
            return NULL;
        }

        if (!appendStringBuilderInteger(result, road->buildYearOrLastRepairYear)) {
            deleteStringBuilder(result, true);
            return NULL;
        }

        iterator = iterator->next;
        city = iterator->data;

        if (!appendStringBuilderString(result, city->name)) {
            deleteStringBuilder(result, true);
            return NULL;
        }
    }

    /* Usuwamy niepotrzebny średnik z napisu. */
    result->data[result->size - 1] = 0;
    char *ptr = result->data;
    deleteStringBuilder(result, false);

    return ptr;
}
