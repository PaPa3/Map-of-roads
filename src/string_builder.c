/** @file
 * Implementacja interfejsu klasy przechowującej słowa.
 *
 * @author Paweł Pawlik <pp406289@students.mimuw.edu.pl>
 * @date 29.04.2019
 */

#include "string_builder.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <inttypes.h>
#include <string.h>

#define DEFAULT_STRING_BUILDER_MEMORY_SIZE 8
///< domyślny rozmiar zaalokowanego miejsca na napis w (@ref StringBuilder)
#define INTEGER_LENGTH 22
///< pewne górne ograniczenie na długość liczby całkowitej jako słowa

/** @brief Zwiększa zaalokowane miejsce na napis.
 * Powiększa zaalokowaną miejsce na napis do rozmiaru
 * @p newMemorySize. Nic nie robi jeśli wartość @p newMemorySize
 * jest mniejsza niż  dotychczas zaalokowana pamięć.
 * @param[in,out] ptr               - wskaźnik na strukturę @ref StringBuilder;
 * @param[in] newMemorySize         - docelowa wielkość zaalokowanej tablicy.
 * @return Wartość @p true. Jeśli nie udało się zaalokować potrzebnej pamięci
 * to zwraca @p false.
 */
bool resizeStringBuilder(StringBuilder *ptr, uint32_t newMemorySize) {
    assert(ptr);

    if (newMemorySize < ptr->reservedMemory) {
        return true;
    }

    char *string = realloc(ptr->data, newMemorySize * sizeof(char));
    if (string == NULL) {
        return false;
    }

    ptr->data = string;
    ptr->reservedMemory = newMemorySize;

    return true;
}

/** @brief Tworzy strukturę.
 * @return Wskaźnik na utworzoną strukturę lub NULL, jeśli nie udało się
 * zaalokować pamięci.
 */
StringBuilder *newStringBuilder() {
    StringBuilder *result = malloc(sizeof(StringBuilder));
    if (result == NULL) {
        return NULL;
    }

    result->size = 0;
    result->data = NULL;
    result->reservedMemory = 0;
    if (!resizeStringBuilder(result, DEFAULT_STRING_BUILDER_MEMORY_SIZE)) {
        free(result);
        return NULL;
    }

    return result;
}

/** @brief Usuwa strukturę @ref StringBuilder.
 * Usuwa strukturę. Jeśli @p freeData wynosi @p true usuwa też przechowywane
 * słowo.
 * @param[in,out] string            - wskaźnik na usuwaną strukturę;
 * @param[in] freeData              - czy należy usunąć dane.
 */
void deleteStringBuilder(StringBuilder *string, bool freeData) {
    if (freeData) {
        free(string->data);
    }
    free(string);
}

/** @brief Dodaje słowo do struktury.
 * Dodaje dane słowo i znak ';' na koniec tworzonego napisu.
 * @param[in,out] string            - wskaźnik na tworzony napis;
 * @param[in] ptr                   - wskaźnik na dodawane słowo.
 * @return Wartość @p true jeśli udało się dodać słowo lub @p false, jeśli
 * nie udało się zaalokować pamięci.
 */
bool appendStringBuilderString(StringBuilder *string, const char *ptr) {
    assert(string);

    while (strlen(ptr) + 2 + string->size >= string->reservedMemory) {
        if (!resizeStringBuilder(string, string->reservedMemory * 2)) {
            return false;
        }
    }

    int x = sprintf(&string->data[string->size], "%s;", ptr);
    string->size += x;

    return true;
}

/** @brief Dodaje liczbę do struktury.
 * Dodaje dane liczbę zrzutowaną na słowo i znak ';'
 * na koniec tworzonego napisu.
 * @param[in,out] string            - wskaźnik na tworzony napis;
 * @param[in] integer               - dodawana liczba.
 * @return Wartość @p true jeśli udało się dodać słow lub @p false, jeśli
 * nie udało się zaalokować pamięci.
 */
bool appendStringBuilderInteger(StringBuilder *string, int64_t integer) {
    assert(string);

    while (INTEGER_LENGTH + string->size >= string->reservedMemory) {
        if (!resizeStringBuilder(string, string->reservedMemory * 2)) {
            return false;
        }
    }

    int x = sprintf(&string->data[string->size], "%" PRId64 ";", integer);
    string->size += x;

    return true;
}
