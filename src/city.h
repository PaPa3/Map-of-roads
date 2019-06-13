/** @file
 * Interfejs klasy przechowującej miasto w mapie dróg krajowych.
 *
 * @author Paweł Pawlik <pp406289@students.mimuw.edu.pl>
 * @date 29.04.2019
 */

#ifndef CITY_H
#define CITY_H

#include "list.h"

#include <stdint.h>
#include <stdbool.h>

/**
 * Struktura przechowująca miasto w mapie dróg krajowych. Dodatkowo
 * trzyma zmienne używane przez algorytm dijkstry do liczenia najkrótszej
 * drogi od pewnego ustalonego miasta.
 */
typedef struct City {
    char *name;                     ///< nazwa miasta
    uint32_t hashName;              ///< hasz nazwy miasta
    List *roads;                    ///< list dróg wychodzących z miasta

    /** @name Zmienne użwane przez algorytm dijkstry.
     * Zmienne używane przez algorytm dijkstry do liczenia najkrótszej
     * drogi od pewnego ustalonego miasta.
     */
    ///@{
    int64_t distance;               ///< długość najkrótszej drogi
    int64_t oldestRoadOnRoute;      ///< optymalny wiek najkrótszej drogi
    bool isRouteUnequivocal;        ///< jednoznaczność najkrótszej drogi
    struct City *previousOnRoute;   ///< poprzednie miasto na najkrótszej drodze
    ///@}
} City;

/** @brief Tworzy strukturę.
 * Tworzy strukturę z miastem o nazwie @p name oraz liczy hasz
 * słowa @p name.
 * @param[in] name              - wskaźnik na nazwę miasta.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
City *newCity(const char *name);

/** @brief Usuwa Strukutrę.
 * Usuwa strukturę wskazywaną przez @p city.
 * @param[in] city              - wskaźnik na usuwaną strukturę.
 */
void deleteCity(City *city);

/** @brief Sprawdza czy słowo jest poprawną nazwą miasta.
 * Sprawdza czy @p string jest poprawną nazwą miasta. Poprawna nazwa
 * miasta jest niepustym napisem w stylu C niezawierającym kodów
 * od 0 do 31 ani średnika i zakończonym zerem.
 * @param[in] string            - wskaźnik na słowo do sprawdzenia.
 * @return Wartość @p true jeśli słowo jest poprawną nazwą miasta.
 * Wartość @p false w przeciwnym przypadku.
 */
bool isStringValidCityName(const char *string);

/** @brief Znajduje miasto na liście miast.
 * Znajduję strukturę @ref City o nazwie @p name na liście miast.
 * @param[in] listOfCities      - lista struktur @ref City;
 * @param[in] name              - wskaźnik na słowo będące nazwą szukanego miasta.
 * @return Wskaźnik na szukaną strukturę lub NULL jeśli szukane miasto
 * nie znajduję się na liście.
 */
City *findCityOnList(List *listOfCities, const char *name);

/** @brief Liczy hasz słowa.
 * Liczy hasz słowa @p string.
 * @param[in] string            - wskaźnik na słowo.
 * @return Hasz słowa.
 */
uint32_t hashString(const char *string);

#endif // CITY_H
