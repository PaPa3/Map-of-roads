/** @file
 * Interfejs klasy przechowującej drogę krajową.
 *
 * @author Paweł Pawlik <pp406289@students.mimuw.edu.pl>
 * @date 29.04.2019
 */

#ifndef ROUTE_H
#define ROUTE_H

#include "city.h"
#include "list.h"

#include <stdbool.h>

/**
  * Struktura przechowująca drogę krajową.
  */
typedef struct Route {
    unsigned routeId;           ///< numer drogi krajowej
    List *cities;               ///< lista miast na drodze krajowej
    bool wasChanged;            ///< czy droga jest w trakcie modyfikacji
} Route;

/** @brief Tworzy strukturę.
 * Tworzy drogę krajową pomiędzy dwoma miastami i nadaje jej podany numer.
 * Wśród istniejących odcinków dróg wyszukuje najkrótszą drogę. Jeśli jest
 * więcej niż jeden sposób takiego wyboru, to dla każdego wariantu wyznacza
 * wśród wybranych w nim odcinków dróg ten, który był najdawniej wybudowany lub
 * remontowany i wybiera wariant z odcinkiem, który jest najmłodszy.
 * @param[in] routeId           - numer drogi krajowej;
 * @param[in] city1             - wskaźnik na pierwsze miasto;
 * @param[in] city2             - wskaźnik na drugie miasto;
 * @param[in] listOfCities      - wskażnik na wszyskie miasta na danej mapie.
 * @return Wartość @p true, jeśli droga krajowa została utworzona.
 * Wartość @p false, jeśli wystąpił błąd: nie można
 * jednoznacznie wyznaczyć drogi krajowej między podanymi miastami lub nie udało
 * się zaalokować pamięci.
 */
Route *newRouteModule(unsigned routeId, City *city1, City *city2,
                      List *listOfCities);

/** @brief Usuwa strukturę.
 * @param[in,out] route         - wskażnik na drogę krajową do usunięcia.
 */
void deleteRouteModule(Route *route);

/** @brief Poprawia drogę krajową.
 * Poprawia drogę krająwą po usunięciu odcinka drogi między dwoma miastami.
 * Jeśli usunięcie tego odcinka drogi
 * spowodowało przerwanie ciągu rogi krajowej, to uzupełnia ją
 * istniejącymi odcinkami dróg w taki sposób, aby była najkrótsza. Jeśli jest
 * więcej niż jeden sposób takiego uzupełnienia, to dla każdego wariantu
 * wyznacza wśród dodawanych odcinków drogi ten, który był najdawniej wybudowany
 * lub remontowany i wybiera wariant z odcinkiem, który jest najmłodszy.
 * @param[in,out] route         - wskaźnik drogę krajową do poprawienie;
 * @param[in] city1             - wskaźnik na pierwsze miasto;
 * @param[in] city2             - wskaźnik na drugie miasto;
 * @param listOfCities          - wskażnik na wszyskie miasta na danej mapie.
 * @return Wartość @p true jeśli udało się poprawić drogę krajową, lub @p false,
 * jeśli nie udało się zaalokować pamięci.
 */
bool findNewRouteAfterRemovingRoad(Route *route, City *city1, City *city2,
                                   List *listOfCities);

/** @brief Cofa zmiany wywołane przez @ref findNewRouteAfterRemovingRoad.
 * Cofa zmiany wywołane przez ostatnie użycie
 * @ref findNewRouteAfterRemovingRoad. Miasta (argumenty funkcji) muszą być
 * takie samejak przy ostatnim wywołaniu @ref findNewRouteAfterRemovingRoad
 * dla danej drogi krajowej.
 * @param[in,out] route         - wskaźnik drogę krajową;
 * @param[in] city1             - wskaźnik na pierwsze miasto;
 * @param[in] city2             - wskaźnik na drugie miasto.
 */
void undoFindNewRouteAfterRemovingRoad(Route *route, City *city1, City *city2);

/**
 * @brief Wydłuża drogę krajową do podanego miasta.
 * Dodaje do drogi krajowej nowe odcinki dróg do podanego miasta w taki sposób,
 * aby nowy fragment drogi krajowej był najkrótszy. Jeśli jest więcej niż jeden
 * sposób takiego wydłużenia, to dla każdego wariantu wyznacza wśród dodawanych
 * odcinków dróg ten, który był najdawniej wybudo
 * @param[in,out] route         - wskaźnik drogę krajową;
 * @param[in] city              - wskaźnik na miasto;
 * @param[in] listOfCities      - wskażnik na wszyskie miasta na danej mapie.
 * @return Wartość @p true, jeśli droga krajowa została wydłużona.
 * Wartość @p false, jeśli wystąpił błąd: nie można jednoznacznie
 * wyznaczyć nowego fragmentu drogi krajowej lub nie udało się zaalokować
 * pamięci.
 */
bool findNewRouteAfterExtend(Route *route, City *city, List *listOfCities);

/** @brief Znajduję drogę krajową na liście.
 * Znajduję drogę krajową o danym numerze na liście.
 * @param[in] list              - wskaźnik na listę dróg krajowych;
 * @param[in] routeId           - numer szukanej drogi krajowej.
 * @return Wskaźnik na szukaną drogę krajową lub NULL, jeśli na liści nie ma
 * szukanej drogi krajowej.
 */
Route *findRouteOnList(List *list, unsigned routeId);

/** @brief Udostępnia informacje o drodze krajowej.
 * Zwraca wskaźnik na napis, który zawiera informacje o drodze krajowej. Alokuje
 * pamięć na ten napis. Zaalokowaną pamięć trzeba zwolnić za pomocą funkcji free.
 * @param[in] route             - wskaźnik na drogę krajową.
 * @return Wskaźnik na napis lub NULL, gdy nie udało się zaalokować pamięci.
 */
char *descriptionRouteModule(Route *route);

#endif // ROUTE_H
