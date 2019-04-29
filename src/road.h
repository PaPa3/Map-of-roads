/** @file
 * Interfejs klasy przechowującej odcinek drogowy.
 *
 * @author Paweł Pawlik <pp406289@students.mimuw.edu.pl>
 * @date 29.04.2019
 */

#ifndef ROAD_H
#define ROAD_H

#include "city.h"

#include <stdbool.h>

/**
  * Struktura przechowująca odcinek drogowy.
  */
typedef struct Road {
    int buildYearOrLastRepairYear;  ///< rok budowy lub rok ostatniego remontu
    unsigned length;                ///< długość drogi
    City *destination;              ///< miasto do którego prowadzi droga
    bool isDeleted;                 ///< @p true <=> droga jest w trakcie usuwania
} Road;

/** @brief Dodaje drogę między dwoma miastami.
 * @param[in,out] city1             - wskaźnik na pierwsze miasto;
 * @param[in,out] city2             - wskaźnik na drugie miasto;
 * @param[in] length                - długość drogi;
 * @param[in] builtYear             - rok budowy drogi.
 * @return Wartość @p true jeśli dodano drogę lub @ false, jeśli nie udało się
 * zaalokować pamięci.
 */
bool addRoadModule(City *city1, City *city2, unsigned length, int builtYear);

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
bool repairRoadModule(City *city1, City *city2, int repairYear);

/** @brief Ustawia parametr @ref Road.isDeleted danej drogi.
 * Ustawia parametr @ref Road.isDeleted drogi pomiędzy danymi miastami.
 * @param[in,out] city1             - wskaźnik na pierwsze miasto;
 * @param[in,out] city2             - wskaźnik na drugie miasto;
 * @param[in] newIsDeleted          - parametr do ustawienia.
 * @return Wartość @p true udało się ustawić parametr lub @ false, jeśli
 * nie istnieje droga pomiędzy miastami.
 */
bool setRoadIsDeletedTo(City *city1, City *city2, bool newIsDeleted);

/** @brief Znajduje drogę.
 * Znajduje drogę z miasta do innego miasta.
 * @param[in] from                  - wskaźnik na początek odcinka drogowego;
 * @param[in] to                    - wskaźnik na koniec odcinka drogowego.
 * @return Wskaźnik na węzeł listy, który zawiera szukaną drogę lub NULL,
 * jeśli nie istnieje szukana droga.
 */
ListIterator *findRoadModule(City *from, City *to);

/** @brief Usuwa drogę.
 * Usuwa drogę pomiędzy danymi miastami.
 * @param[in,out] city1             - wskaźnik na pierwsze miasto;
 * @param[in,out] city2             - wskaźnik na drugie miasto.
 */
void removeRoadModule(City *city1, City *city2);

#endif // ROAD_H
