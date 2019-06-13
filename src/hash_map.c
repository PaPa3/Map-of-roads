/** @file
 * Implementacja interfejsu klasy przechowującej haszmapę miast (@ref City).
 * Haszmapa jest zbudowana na drzewie binarnym.
 *
 * @author Paweł Pawlik <pp406289@students.mimuw.edu.pl>
 * @date 13.06.2019
 */

#include "hash_map.h"
#include "list.h"
#include "city.h"

#include <stdlib.h>
#include <assert.h>

/**
 * Struktura przechowująca haszmapę miast (@ref City).
 */
typedef struct HashMap {
    HashMap *left;      ///< wskaźnik na lewego syna (bit o wartości @p 0)
    HashMap *right;     ///< wskaźnik na lewego syna (bit o wartości @p 1)
    List *data;         ///< wskaźnik na listę miast (@ref City)
} HashMap;

/** @brief Tworzy strukturę.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
HashMap *newHashMap() {
    HashMap *result = malloc(sizeof(HashMap));
    if (result == NULL) {
        return NULL;
    }

    result->data = newList();
    if (result->data == NULL) {
        free(result);
        return NULL;
    }

    result->left = NULL;
    result->right = NULL;

    return result;
}

/** @brief Usuwa Strukutrę.
 * Usuwa strukturę wskazywaną przez @p hashMap.
 * @param[in,out] hashMap   - wskaźnik na usuwaną strukturę.
 */
void deleteHashMap(HashMap *hashMap) {
    if (hashMap == NULL) {
        return;
    }

    deleteList(hashMap->data, false);
    deleteHashMap(hashMap->left);
    deleteHashMap(hashMap->right);

    free(hashMap);
}

/** @brief Znajduje wierzchołek odpowiadający danemu haszowi.
 * Znajduje wierzchołek odpowiadający danemu haszowi. Jeśli trzeba to dodaje
 * nowe wierzchołki do drzewa haszmapy.
 * @param[in,out] hashMap   - wskażnik na haszmapę;
 * @param[in] hash          - szukany hasz.
 * @return Wskaźnik na szukany wierzchołek lub NULL jeśli nie udało się
 * zaalokować pamięci.
 */
HashMap *findNodeHashMap(HashMap *hashMap, uint32_t hash) {
    assert(hashMap);

    while (hash) {
        if (hash % 2) {
            if (hashMap->right == NULL) {
                hashMap->right = newHashMap();
                if (hashMap->right == NULL) {
                    return NULL;
                }
            }

            hashMap = hashMap->right;
        } else {
            if (hashMap->left == NULL) {
                hashMap->left = newHashMap();
                if (hashMap->left == NULL) {
                    return NULL;
                }
            }

            hashMap = hashMap->left;
        }

        hash /= 2;
    }

    return hashMap;
}

/** @brief Wstawia strukturę @ref City na haszmapę.
 * Wstawia strukturę @ref City na haszmapę. Jeśli trzeba to dodaje
 * nowe wierzchołki do drzewa haszmapy.
 * @param[in,out] hashMap   - wskaźnik na haszmapę;
 * @param[in] city          - wskaźnik na dodawane miasto.
 * @return Wartość @p true lub @p false jeśli nie udało się zaalkować pamięci.
 */
bool insertHashMap(HashMap *hashMap, City *city) {
    assert(hashMap);

    hashMap = findNodeHashMap(hashMap, city->hashName);
    if (hashMap == NULL) {
        return false;
    }

    insertList(hashMap->data->end, city);
    return true;
}

/** @brief Znajduję miasto na haszmapie.
 * Znajduję miasto (@ref City) na haszmapie.
 * @param[in] hashMap       - wskaźnik na haszmapę;
 * @param[in] cityName      - wskaźnik na nazwę szukanego miasta.
 * @return Wskażnik na szukane miasto lub NULL jeśli dane miasto nie znajduje
 * się na haszmapie.
 */
City *findCityOnHashMap(HashMap *hashMap, const char *cityName) {
    assert(hashMap);
    assert(cityName);

    hashMap = findNodeHashMap(hashMap, hashString(cityName));
    if (hashMap == NULL) {
        return NULL;
    }

    return findCityOnList(hashMap->data, cityName);
}

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
                                         const char *cityName) {
    assert(hashMap);
    assert(listOfCities);
    assert(cityName);

    City *result = findCityOnHashMap(hashMap, cityName);
    if (result != NULL) {
        return result;
    }

    result = newCity(cityName);
    if (insertList(listOfCities->begin, result) == NULL) {
        deleteCity(result);
        return NULL;
    }

    if (!insertHashMap(hashMap, result)) {
        eraseList(listOfCities->begin, false);
        deleteCity(result);
        return NULL;
    }

    return result;
}
