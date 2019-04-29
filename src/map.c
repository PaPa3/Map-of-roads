/** @file
 * Implementacja interfejsu klasy przechowującej mapę dróg krajowych
 *
 * @author Łukasz Kamiński <kamis@mimuw.edu.pl>, Marcin Peczarski <marpe@mimuw.edu.pl>
 * @author Paweł Pawlik <pp406289@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 29.04.2019
 */

#include "map.h"
#include "city.h"
#include "road.h"
#include "list.h"
#include "road.h"
#include "route.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/**
 * Struktura przechowująca mapę dróg krajowych.
 */
typedef struct Map {
    List *cities;        ///< Lista miast na mapie
    List *routes;        ///< Lista dróg krajowych na mapie
} Map;

/** @brief Tworzy nową strukturę.
 * Tworzy nową, pustą strukturę niezawierającą żadnych miast, odcinków dróg ani
 * dróg krajowych.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Map *newMap() {
    Map *result = malloc(sizeof(Map));
    if (result == NULL) {
        return NULL;
    }

    result->cities = newList();
    if (result->cities == NULL) {
        free(result);
        return NULL;
    }

    result->routes = newList();
    if (result->routes == NULL) {
        deleteList(result->cities, false);
        free(result);
        return NULL;
    }

    return result;
}

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p map.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param[in] map        – wskaźnik na usuwaną strukturę.
 */
void deleteMap(Map *map) {
    if (map == NULL) {
        return;
    }

    ListIterator *iterator = map->cities->begin;
    while (iterator != map->cities->end) {
        deleteCity(iterator->data);
        iterator = iterator->next;
    }
    deleteList(map->cities, false);

    iterator = map->routes->begin;
    while (iterator != map->routes->end) {
        deleteRouteModule(iterator->data);
        iterator = iterator->next;
    }
    deleteList(map->routes, false);

    free(map);
}

/** @brief Sprawdza zgodność dancyh ze specyfikacją zadania.
 * Poprawne dane to takie, w których wskaźnik na mapę nie jest równy NULL,
 * wskaźniki na napisy, które nie są równe NULL, wskazują na poprawny napis
 * oraz jeśli oba wskaźniki na napisy nie są równe NULL, to napisy muszą
 * być różne.
 * @param[in] map       - wskaźnik na mapę;
 * @param[in] cityName1 - wskaźnik na pierwszy napis;
 * @param[in] cityName2 - wskaźnik na drugi napis.
 * @return Wartość @p true jeśli dane są poprawne lub @p false w przeciwnym
 * przypadku.
 */
bool isValidInput(Map *map, const char *cityName1, const char *cityName2) {
    if (map == NULL) {
        return false;
    }

    if (cityName1 != NULL && !isStringValidCityName(cityName1)) {
        return false;
    }

    if (cityName2 != NULL && !isStringValidCityName(cityName2)) {
        return false;
    }

    if (cityName1 != NULL && cityName2 != NULL &&
            strcmp(cityName1, cityName2) == 0) {
        return false;
    }

    return true;
}

/** @brief Dodaje do mapy odcinek drogi między dwoma różnymi miastami.
 * Jeśli któreś z podanych miast nie istnieje, to dodaje go do mapy, a następnie
 * dodaje do mapy odcinek drogi między tymi miastami.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] cityName1  – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] cityName2  – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] length     – długość w km odcinka drogi;
 * @param[in] builtYear  – rok budowy odcinka drogi.
 * @return Wartość @p true, jeśli odcinek drogi został dodany.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * wartość, obie podane nazwy miast są identyczne, odcinek drogi między tymi
 * miastami już istnieje lub nie udało się zaalokować pamięci.
 */
bool addRoad(Map *map, const char *cityName1, const char *cityName2,
             unsigned length, int builtYear) {
    if (!isValidInput(map, cityName1, cityName2) || length == 0 || builtYear == 0) {
        return false;
    }

    City *city1 = findCityInsertIfNecessary(map->cities, cityName1);
    City *city2 = findCityInsertIfNecessary(map->cities, cityName2);
    if (city1 == NULL || city2 == NULL) {
        return false;
    }

    if (findRoadModule(city1, city2) != NULL) {
        return false;
    }

    return addRoadModule(city1, city2, length, builtYear);
}

/** @brief Modyfikuje rok ostatniego remontu odcinka drogi.
 * Dla odcinka drogi między dwoma miastami zmienia rok jego ostatniego remontu
 * lub ustawia ten rok, jeśli odcinek nie był jeszcze remontowany.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] cityName1  – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] cityName2  – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] repairYear – rok ostatniego remontu odcinka drogi.
 * @return Wartość @p true, jeśli modyfikacja się powiodła.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * wartość, któreś z podanych miast nie istnieje, nie ma odcinka drogi między
 * podanymi miastami, podany rok jest wcześniejszy niż zapisany dla tego odcinka
 * drogi rok budowy lub ostatniego remontu.
 */
bool repairRoad(Map *map, const char *cityName1, const char *cityName2, int repairYear) {
    if (!isValidInput(map, cityName1, cityName2) || repairYear == 0) {
        return false;
    }

    City *city1 = findCityOnList(map->cities, cityName1);
    City *city2 = findCityOnList(map->cities, cityName2);
    if (city1 == NULL || city2 == NULL) {
        return false;
    }

    return repairRoadModule(city1, city2, repairYear);
}

/** @brief Łączy dwa różne miasta drogą krajową.
 * Tworzy drogę krajową pomiędzy dwoma miastami i nadaje jej podany numer.
 * Wśród istniejących odcinków dróg wyszukuje najkrótszą drogę. Jeśli jest
 * więcej niż jeden sposób takiego wyboru, to dla każdego wariantu wyznacza
 * wśród wybranych w nim odcinków dróg ten, który był najdawniej wybudowany lub
 * remontowany i wybiera wariant z odcinkiem, który jest najmłodszy.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeId    – numer drogi krajowej;
 * @param[in] cityName1  – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] cityName2  – wskaźnik na napis reprezentujący nazwę miasta.
 * @return Wartość @p true, jeśli droga krajowa została utworzona.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * wartość, istnieje już droga krajowa o podanym numerze, któreś z podanych
 * miast nie istnieje, obie podane nazwy miast są identyczne, nie można
 * jednoznacznie wyznaczyć drogi krajowej między podanymi miastami lub nie udało
 * się zaalokować pamięci.
 */
bool newRoute(Map *map, unsigned routeId,
              const char *cityName1, const char *cityName2) {
    if (!isValidInput(map, cityName1, cityName2)) {
        return false;
    }

    if (routeId < 1 || 999 < routeId) {
        return false;
    }

    City *city1 = findCityOnList(map->cities, cityName1);
    City *city2 = findCityOnList(map->cities, cityName2);
    if (city1 == NULL || city2 == NULL) {
        return false;
    }

    if (findRouteOnList(map->routes, routeId) != NULL) {
        return false;
    }

    Route *route = newRouteModule(routeId, city1, city2, map->cities);
    if (route == NULL) {
        return false;
    }

    if (insertList(map->routes->end, route) == NULL) {
       deleteRouteModule(route);
       return false;
    }

    return true;
}

/** @brief Wydłuża drogę krajową do podanego miasta.
 * Dodaje do drogi krajowej nowe odcinki dróg do podanego miasta w taki sposób,
 * aby nowy fragment drogi krajowej był najkrótszy. Jeśli jest więcej niż jeden
 * sposób takiego wydłużenia, to dla każdego wariantu wyznacza wśród dodawanych
 * odcinków dróg ten, który był najdawniej wybudowany lub remontowany i wybiera
 * wariant z odcinkiem, który jest najmłodszy.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeId    – numer drogi krajowej;
 * @param[in] cityName   – wskaźnik na napis reprezentujący nazwę miasta.
 * @return Wartość @p true, jeśli droga krajowa została wydłużona.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * nazwę, nie istnieje droga krajowa o podanym numerze, nie ma miasta o podanej
 * nazwie, przez podane miasto już przechodzi droga krajowa o podanym numerze,
 * podana droga krajowa kończy się w podanym mieście, nie można jednoznacznie
 * wyznaczyć nowego fragmentu drogi krajowej lub nie udało się zaalokować
 * pamięci.
 */
bool extendRoute(Map *map, unsigned routeId, const char *cityName) {
    if (!isValidInput(map, cityName, NULL)) {
        return false;
    }

    City *city = findCityOnList(map->cities, cityName);
    if (city == NULL) {
        return false;
    }

    Route *route = findRouteOnList(map->routes, routeId);
    if (route == NULL) {
        return false;
    }

    if (findCityOnList(route->cities, city->name) != NULL) {
        return false;
    }

    return findNewRouteAfterExtend(route, city, map->cities);
}

/** @brief Usuwa odcinek drogi między dwoma różnymi miastami.
 * Usuwa odcinek drogi między dwoma miastami. Jeśli usunięcie tego odcinka drogi
 * powoduje przerwanie ciągu jakiejś drogi krajowej, to uzupełnia ją
 * istniejącymi odcinkami dróg w taki sposób, aby była najkrótsza. Jeśli jest
 * więcej niż jeden sposób takiego uzupełnienia, to dla każdego wariantu
 * wyznacza wśród dodawanych odcinków drogi ten, który był najdawniej wybudowany
 * lub remontowany i wybiera wariant z odcinkiem, który jest najmłodszy.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] cityName1  – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] cityName2  – wskaźnik na napis reprezentujący nazwę miasta.
 * @return Wartość @p true, jeśli odcinek drogi został usunięty.
 * Wartość @p false, jeśli z powodu błędu nie można usunąć tego odcinka drogi:
 * któryś z parametrów ma niepoprawną wartość, nie ma któregoś z podanych miast,
 * nie istnieje droga między podanymi miastami, nie da się jednoznacznie
 * uzupełnić przerwanego ciągu drogi krajowej lub nie udało się zaalokować
 * pamięci.
 */
bool removeRoad(Map *map, const char *cityName1, const char *cityName2) {
    if (!isValidInput(map, cityName1, cityName2)) {
        return false;
    }

    City *city1 = findCityOnList(map->cities, cityName1);
    City *city2 = findCityOnList(map->cities, cityName2);
    if (city1 == NULL || city2 == NULL) {
        return false;
    }

    if (!setRoadIsDeletedTo(city1, city2, true)) {
        return false;
    }

    ListIterator *iterator = map->routes->begin;
    while (iterator != map->routes->end) {
        if (!findNewRouteAfterRemovingRoad(iterator->data, city1, city2,
                                           map->cities)) {
            while (iterator != map->routes->begin) {
                iterator = iterator->previous;
                undoFindNewRouteAfterRemovingRoad(iterator->data, city1, city2);
            }
            setRoadIsDeletedTo(city1, city2, false);
            return false;
        }
        iterator = iterator->next;
    }

    removeRoadModule(city1, city2);

    return true;
}

/** @brief Udostępnia informacje o drodze krajowej.
 * Zwraca wskaźnik na napis, który zawiera informacje o drodze krajowej. Alokuje
 * pamięć na ten napis. Zwraca pusty napis, jeśli nie istnieje droga krajowa
 * o podanym numerze. Zaalokowaną pamięć trzeba zwolnić za pomocą funkcji free.
 * Informacje wypisywane są w formacie:
 * numer drogi krajowej;nazwa miasta;długość odcinka drogi;rok budowy lub
 * ostatniego remontu;nazwa miasta;długość odcinka drogi;rok budowy lub
 * ostatniego remontu;nazwa miasta;…;nazwa miasta.
 * Kolejność miast na liście jest taka, aby miasta @p city1 i @p city2, podane
 * w wywołaniu funkcji @ref newRoute, które utworzyło tę drogę krajową, zostały
 * wypisane w tej kolejności.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeId    – numer drogi krajowej.
 * @return Wskaźnik na napis lub NULL, gdy nie udało się zaalokować pamięci.
 */
char const* getRouteDescription(Map *map, unsigned routeId) {
    char *emptyString = malloc(sizeof(char));
    if (emptyString == NULL) {
        return NULL;
    }
    *emptyString = 0;

    if (map == NULL) {
        return emptyString;
    }

    Route *route = findRouteOnList(map->routes, routeId);
    if (route == NULL) {
        return emptyString;
    }

    free(emptyString);

    return descriptionRouteModule(route);
}
