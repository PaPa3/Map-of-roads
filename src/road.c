/** @file
 * Implementacja interfejsu klasy przechowującej odcinek drogowy.
 *
 * @author Paweł Pawlik <pp406289@students.mimuw.edu.pl>
 * @date 29.04.2019
 */

#include "road.h"

#include <stdlib.h>
#include <assert.h>

/** @brief Tworzy strukturę.
 * Tworzy strukturę @ref Road i ustawia jej pola.
 * @param[in] destination           - wskaźnik na miasto, do którego prowadzi odcinek;
 * @param[in] length                - długość odcinka drogowego;
 * @param[in] builtYear             - rok budowy.
 * @return Wskaźnik na utworzoną strukturę lub NULL, jeśli nie udało się
 * zaalokować pamięci.
 */
Road *newRoad(City *destination, unsigned length, int builtYear) {
    assert(destination);

    Road *result = malloc(sizeof(Road));
    if (result == NULL) {
        return NULL;
    }

    result->buildYearOrLastRepairYear = builtYear;
    result->length = length;
    result->destination = destination;
    result->isDeleted = false;

    return result;
}

/** @brief Dodaje drogę.
 * Dodaje drogę z miasta do innego miasta. Droga zostaje dodane na początek
 * listy dróg z danego miasta.
 * @param[in,out] from              - miasto początkowego odcinka drogowego;
 * @param[in,out] to                - miasto docelowe odcinka drogowego;
 * @param[in] length                - długość odcinka drogowego;
 * @param[in] builtYear             - rok budowy.
 * @return Wartość @p true jeśli udało się dodać lub @p false, jeśli nie
 * udało się zaalokować pamięci.
 */
bool addRoadFromTo(City *from, City *to, unsigned length, int builtYear) {
    assert(from);
    assert(to);

    Road *road = newRoad(to, length, builtYear);
    if (road == NULL) {
        return false;
    }

    if (insertList(from->roads->begin, road) == NULL) {
        free(road);
        return false;
    }

    return true;
}

/** @brief Dodaje drogę między dwoma miastami.
 * @param[in,out] city1             - wskaźnik na pierwsze miasto;
 * @param[in,out] city2             - wskaźnik na drugie miasto;
 * @param[in] length                - długość drogi;
 * @param[in] builtYear             - rok budowy drogi.
 * @return Wartość @p true jeśli dodano drogę lub @ false, jeśli nie udało się
 * zaalokować pamięci.
 */
bool addRoadModule(City *city1, City *city2, unsigned length, int builtYear) {
    assert(city1);
    assert(city2);

    ListIterator *iterator = city1->roads->begin;
    while (iterator != city1->roads->begin) {
        if (((Road *)iterator->data)->destination == city2) {
            return false;
        }
    }

    if (!addRoadFromTo(city1, city2, length, builtYear)) {
        return false;
    }

    if (!addRoadFromTo(city2, city1, length, builtYear)) {
        eraseList(city1->roads->begin, true);
        return false;
    }

    return true;
}

/** @brief Znajduje drogę.
 * Znajduje drogę z miasta do innego miasta.
 * @param[in] from                  - wskaźnik na początek odcinka drogowego;
 * @param[in] to                    - wskaźnik na koniec odcinka drogowego.
 * @return Wskaźnik na węzeł listy, który zawiera szukaną drogę lub NULL,
 * jeśli nie istnieje szukana droga.
 */
ListIterator *findRoadModule(City *from, City *to) {
    assert(from);
    assert(to);

    ListIterator *iterator = from->roads->begin;
    while (iterator != from->roads->end) {
        Road *road = iterator->data;
        if (road->destination == to) {
            return iterator;
        }
        iterator = iterator->next;
    }

    return NULL;
}

/** @brief Ustawia rok ostatniego remontu danej drogi.
 * Ustawia rok ostatniego drogi pomiędzy danymi miastami.
 * Ustawiany rok musi być większy niż poprzedni rok.
 * @param[in,out] city1             - wskaźnik na pierwsze miasto;
 * @param[in,out] city2             - wskaźnik na drugie miasto;
 * @param[in] repairYear            - rok remontu.
 * @return Wartość @p true udało się zmienić rok remontu lub @ false, jeśli
 * nie istnieje droga pomiędzy miastami lub rok remontu jest mniejszy niż
 * poprzedni.
 */
bool repairRoadModule(City *city1, City *city2, int repairYear) {
    assert(city1);
    assert(city2);

    ListIterator *iterator = findRoadModule(city1, city2);
    if (iterator == NULL) {
        return false;
    }

    Road *road = iterator->data;
    if (road->buildYearOrLastRepairYear > repairYear) {
        return false;
    }

    road->buildYearOrLastRepairYear = repairYear;
    ((Road *)findRoadModule(city2, city1)->data)->buildYearOrLastRepairYear
            = repairYear;

    return true;
}

/** @brief Ustawia parametr @ref Road.isDeleted danej drogi.
 * Ustawia parametr @ref Road.isDeleted drogi pomiędzy danymi miastami.
 * @param[in,out] city1             - wskaźnik na pierwsze miasto;
 * @param[in,out] city2             - wskaźnik na drugie miasto;
 * @param[in] newIsDeleted          - parametr do ustawienia.
 * @return Wartość @p true udało się ustawić parametr lub @ false, jeśli
 * nie istnieje droga pomiędzy miastami.
 */
bool setRoadIsDeletedTo(City *city1, City *city2, bool newIsDeleted) {
    assert(city1);
    assert(city2);

    ListIterator *iterator = findRoadModule(city1, city2);
    if (iterator == NULL) {
        return false;
    }

    Road *road = iterator->data;
    road->isDeleted = newIsDeleted;

    ((Road *)findRoadModule(city2, city1)->data)->isDeleted = newIsDeleted;

    return true;
}

/** @brief Usuwa drogę.
 * Usuwa istniejącą drogę pomiędzy danymi miastami.
 * @param[in,out] city1             - wskaźnik na pierwsze miasto;
 * @param[in,out] city2             - wskaźnik na drugie miasto.
 */
void removeRoadModule(City *city1, City *city2) {
    assert(city1);
    assert(city2);

    eraseList(findRoadModule(city1, city2), true);
    eraseList(findRoadModule(city2, city1), true);
}
