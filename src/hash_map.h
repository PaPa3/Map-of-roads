/** @file
 * Interfejs klasy przechowującej haszmapę miast (@ref City).
 * Haszmapa jest zbudowana na drzewie binarnym.
 *
 * @author Paweł Pawlik <pp406289@students.mimuw.edu.pl>
 * @date 13.06.2019
 */

#ifndef HASH_MAP_H
#define HASH_MAP_H

#include "city.h"

#include <stdbool.h>

/**
 * Struktura przechowująca haszmapę miast (@ref City).
 */
typedef struct HashMap HashMap;

/** @brief Tworzy strukturę.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
HashMap *newHashMap();

/** @brief Usuwa Strukutrę.
 * Usuwa strukturę wskazywaną przez @p hashMap.
 * @param[in,out] hashMap   - wskaźnik na usuwaną strukturę.
 */
void deleteHashMap(HashMap *hashMap);

/** @brief Znajduję miasto na haszmapie.
 * Znajduję miasto (@ref City) na haszmapie.
 * @param[in] hashMap       - wskaźnik na haszmapę;
 * @param[in] cityName      - wskaźnik na nazwę szukanego miasta.
 * @return Wskażnik na szukane miasto lub NULL jeśli dane miasto nie znajduje
 * się na haszmapie.
 */
City *findCityOnHashMap(HashMap *hashMap, const char *cityName);

/** @brief Znajduję miasto na haszmapie, jeśli go nie ma to je dodaje.
 * Znajduję miasto (@ref City) na haszmapie, jeśli go nie ma to dodaje to
 * miasto na haszmapę @p hashMap oraz na listę @p listOfCities.
 * @param[in,out] hashMap   - wskaźnik na haszmapę;
 * @param[in,out] listOfCities - wskaźnik na listę miast;
 * @param[in] cityName      - wskaźnik na nazwę szukanego miasta.
 * @return Wskażnik na szukane miasto lub NULL jeśli nie udało się zaalkować
 * pamięci.
 */
City *findCityOnHashMapInsertIfNecessary(HashMap *hashMap, List *listOfCities,
                                         const char *cityName);

#endif // HASH_MAP_H
