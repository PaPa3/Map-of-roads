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

/** @brief Sortuje listę słów algorytmem merge sort.
 * @param[in,out] begin - wskaźnik na początek listy;
 * @param[in,out] end   - wskaźnik na koniec listy.
 * @return Wskaźnik na początek posortowanej listy.
 */
ListIterator *mergeSortListOfStrings(ListIterator *begin, ListIterator *end) {
    assert(begin);
    assert(end);
    assert(begin->father == end->father);

    uint32_t size = 0;
    ListIterator *iterator = begin;

    while (iterator != end) {
        size++;
        iterator = iterator->next;
    }

    if (size <= 1) {
        return begin;
    }

    ListIterator *middle = begin;
    for (uint32_t i = 0; i < size / 2; i++) {
        middle = middle->next;
    }

    begin = mergeSortListOfStrings(begin, middle);
    middle = mergeSortListOfStrings(middle, end);

    /* Teraz łączymy dwie posortawane połowy listy. Sposób jest nietrywialny
     * by uniknąć niepotrzebnych malloców. */
    end->previous = middle->previous;
    middle->previous->next = end;
    ListIterator *it1 = begin;
    ListIterator *it2 = middle;
    ListIterator *result;
    if (strcmp(it1->data, it2->data) < 0) {
        result = it1;
    } else {
        result = it2;
    }

    for (uint32_t i = 0; i < (size + 1) / 2; i++) {
        while (it1 != end && strcmp(it1->data, it2->data) < 0) {
            it1 = it1->next;
        }
        iterator = it2->next;
        insertListIterator(it1, it2);
        it2 = iterator;
    }

    return result;
}

/** @brief Sortuje listę słów.
 * @param[in,out] list  - wskaźnik na listę słów.
 */
void sortListOfStrings(List *list) {
    mergeSortListOfStrings(list->begin, list->end);
}
