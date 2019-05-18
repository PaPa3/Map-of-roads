/** @file
 * Implementacja interfejsu udostępniającego operacje na słowach.
 *
 * @author Paweł Pawlik <pp406289@students.mimuw.edu.pl>
 * @date 16.05.2019
 */

#include "string_utilities.h"

#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/** @brief Sprawdza czy napis jest poprawną liczbą dziesiętną.
 * Funkcja akceptuje liczby z zerami wiodącymi.
 * @param[in] string    - wskaźnik na napis do sprawdzenia.
 * @return Wartość @p true jeśli napis jest poprawną liczbą, wartość @p false
 * w przeciwnym wypadku.
 */
bool isStringValidDecimalNumber(const char *string) {
    if (*string == 0) {
        return false;
    }

    if (*string == '-') {
        string++;
    }

    for (uint32_t i = 0; *string; i++, string++) {
        if (*string < '0' || *string > '9') {
            return false;
        }
    }

    return true;
}

/** @brief Zamienia słowo na typ int.
 * Funkcja akceptuje liczby z zerami wiodącymi.
 * @param[in] string    - wskaźnik na słowo do zamiany.
 * @return Liczba typu int jaką reprezentuje dane słowo lub @p 0 jeśli
 * słowo składniowo nie jest typem int (w tym wypadku wartość @p errno jest
 * ustawiona na @p EILSEQ).
 */
int stringToInt(const char *string) {
    assert(string);

    if (!isStringValidDecimalNumber(string)) {
        errno = EILSEQ;
        return 0;
    }

    char *c;
    errno = 0;
    long result = strtol(string, &c, 10);
    if (errno == ERANGE || result < INT_MIN || result > INT_MAX || *c != 0) {
        errno = EILSEQ;
        return 0;
    }

    return result;
}

/** @brief Zamienia słowo na typ unsigned.
 * Funkcja akceptuje liczby z zerami wiodącymi.
 * @param[in] string    - wskaźnik na słowo do zamiany.
 * @return Liczba typu unsigned jaką reprezentuje dane słowo lub @p 0 jeśli
 * słowo składniowo nie jest typem unsigned (w tym wypadku wartość @p errno jest
 * ustawiona na @p EILSEQ).
 */
unsigned stringToUnsigned(const char *string) {
    assert(string);

    if (!isStringValidDecimalNumber(string)) {
        errno = EILSEQ;
        return 0;
    }

    char *c;
    errno = 0;
    unsigned long result = strtoul(string, &c, 10);
    if (errno == ERANGE || result > UINT_MAX || *c != 0) {
        errno = EILSEQ;
        return 0;
    }

    return result;
}

/** @brief Znajduje słowo na liście słów.
 * @param[in] begin     - wskaźnik na początek listy;
 * @param[in] end       - wskaźnik na koniec listy;
 * @param[in] string    - wskaźnik na słowo do znalezienia.
 * @return Wskaźnik na iterator zawierający szukane słowo. Jeśli lista nie
 * zawiera szukanego słowa to zwraca NULL.
 */
ListIterator *findStringOnList(ListIterator *begin, ListIterator *end,
                               const char *string) {
    assert(begin);
    assert(end);
    assert(string);

    while (begin != end) {
        if (strcmp(string, begin->data) == 0) {
            return begin;
        }

        begin = begin->next;
    }

    return NULL;
}
