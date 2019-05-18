/** @file
 * Interfejs udostępniający operacje na słowach.
 *
 * @author Paweł Pawlik <pp406289@students.mimuw.edu.pl>
 * @date 16.05.2019
 */

#ifndef STRING_TO_INTEGER_H
#define STRING_TO_INTEGER_H

#include "list.h"

#include <errno.h>

/** @brief Zamienia słowo na typ int.
 * Funkcja akceptuje liczby z zerami wiodącymi.
 * @param[in] string    - wskaźnik na słowo do zamiany.
 * @return Liczba typu int jaką reprezentuje dane słowo lub @p 0 jeśli
 * słowo składniowo nie jest typem int (w tym wypadku wartość @p errno jest
 * ustawiona na @p EILSEQ).
 */
int stringToInt(const char *string);

/** @brief Zamienia słowo na typ unsigned.
 * Funkcja akceptuje liczby z zerami wiodącymi.
 * @param[in] string    - wskaźnik na słowo do zamiany.
 * @return Liczba typu unsigned jaką reprezentuje dane słowo lub @p 0 jeśli
 * słowo składniowo nie jest typem unsigned (w tym wypadku wartość @p errno jest
 * ustawiona na @p EILSEQ).
 */
unsigned stringToUnsigned(const char *string);

/** @brief Znajduje słowo na liście słów.
 * @param[in] begin     - wskaźnik na początek listy;
 * @param[in] end       - wskaźnik na koniec listy;
 * @param[in] string    - wskaźnik na słowo do znalezienia.
 * @return Wskaźnik na iterator zawierający szukane słowo. Jeśli lista nie
 * zawiera szukanego słowa to zwraca NULL.
 */
ListIterator *findStringOnList(ListIterator *begin, ListIterator *end,
                               const char *string);

#endif // STRING_TO_INTEGER_H
