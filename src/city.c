/** @file
 * Implementacja interfejsu klasy przechowującej miasto w mapie dróg krajowych.
 *
 * @author Paweł Pawlik <pp406289@students.mimuw.edu.pl>
 * @date 29.04.2019
 */

#include "city.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <string.h>

/** @brief Sprawdza czy słowo jest poprawną nazwą miasta.
 * Sprawdza czy @p string jest poprawną nazwą miasta. Poprawna nazwa
 * miasta jest niepustym napisem w stylu C niezawierającym kodów
 * od 0 do 31 ani średnika i zakończonym zerem.
 * @param[in] string            - wskaźnik na słowo do sprawdzenia.
 * @return Wartość @p true jeśli słowo jest poprawną nazwą miasta.
 * Wartość @p false w przeciwnym przypadku.
 */
bool isStringValidCityName(const char *string) {
    if (string == NULL || *string == 0) {
        return false;
    }

    while (*string) {
        if ((0 <= *string && *string <= 31) || *string == ';') {
            return false;
        }
        string++;
    }

    return true;
}

/** @brief Liczy hasz słowa.
 * Liczy hasz słowa @p string.
 * @param[in] string            - wskażnik na słowo.
 * @return Hasz słowa.
 */
uint32_t hashString(const char *string) {
    assert(string);

    uint64_t result = 0;

    while (*string) {
        result = (result * 2333 + 1000 + *string) % 1000000007;
        string++;
    }

    return result;
}

/** @brief Tworzy strukturę.
 * Tworzy strukturę z miastem o nazwie @p name oraz liczy hasz
 * słowa @p name.
 * @param[in] name              - wskażnik na nazwę miasta.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
City *newCity(const char *name) {
    assert(isStringValidCityName(name));

    char *nameCopy = malloc(sizeof(char) * (strlen(name) + 1));
    if (nameCopy == NULL) {
        return NULL;
    }
    strcpy(nameCopy, name);

    City *result = malloc(sizeof(City));
    if (result == NULL) {
        free(nameCopy);
        return NULL;
    }

    result->roads = newList();
    if (result->roads == NULL) {
        free(nameCopy);
        free(result);
        return NULL;
    }

    result->name = nameCopy;
    result->hashName = hashString(nameCopy);
    result->distance = -1;

    return result;
}

/** @brief Usuwa Strukutrę.
 * Usuwa strukturę wskazywaną przez @p city.
 * @param[in] city              - wskaźnik na usuwaną strukturę.
 */
void deleteCity(City *city) {
    assert(city);

    free(city->name);
    deleteList(city->roads, true);
    free(city);
}

/** @brief Znajduje miasto na liście miast.
 * Znajduję strukturę @ref City o nazwie @p name na liście miast.
 * @param[in] listOfCities      - lista struktur @ref City;
 * @param[in] name              - wskażnik na słowo będące nazwą szukanego miasta.
 * @return Wskażnik na szukaną strukturę lub NULL jeśli szukane miasto
 * nie znajduję się na liście.
 */
City *findCityOnList(List *listOfCities, const char *name) {
    assert(listOfCities);
    assert(name);

    uint32_t hashName = hashString(name);
    ListIterator *iterator = listOfCities->begin;
    City *city;

    while (iterator != listOfCities->end) {
        city = iterator->data;
        if (city->hashName == hashName && strcmp(city->name, name) == 0) {
            break;
        }

        iterator = iterator->next;
    }

    if (iterator == listOfCities->end) {
        return NULL;
    }

    return iterator->data;
}

/** @brief Znajduje miasto na liście miast (tworzy nowe jeśli takiego nie ma).
 * Znajduję strukturę @ref City o nazwie @p name na liście miast. Jeśli
 * na liście nie ma szukanego miasta to tworzy nowe o nazwie @p name oraz
 * dodaje je na listę
 * @param[in,out] listOfCities   - lista struktur @ref City;
 * @param[in] name               - wskażnik na słowo będące nazwą szukanego miasta.
 * @return Wskażnik na szukaną strukturę.
 */
City *findCityInsertIfNecessary(List *listOfCities, const char *name) {
    assert(listOfCities);
    assert(name);

    City *result = findCityOnList(listOfCities, name);
    if (result == NULL) {
        City *city = newCity(name);
        if (city == NULL) {
            return NULL;
        }

        ListIterator *iterator = insertList(listOfCities->end, city);
        if (iterator == NULL) {
            deleteCity(city);
            return NULL;
        }

        return backList(listOfCities);
    }

    return result;
}
