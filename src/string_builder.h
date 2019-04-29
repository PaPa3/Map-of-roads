/** @file
 * Interfejs klasy przechowującej słowa.
 *
 * @author Paweł Pawlik <pp406289@students.mimuw.edu.pl>
 * @date 29.04.2019
 */

#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

#include <stdint.h>
#include <stdbool.h>

/**
  * Struktura przechowująca słowa.
  */
typedef struct StringBuilder{
    char *data;                 ///< wskaźnik na słowo
    uint32_t size;              ///< długość słowa
    uint32_t reservedMemory;    ///< wielkość zaalokowanej pamięci dla słowa
} StringBuilder;

/** @brief Tworzy strukturę.
 * @return Wskaźnik na utworzną strukturę lub NULL, jeśli nie udało się
 * zaalokować pamięci.
 */
StringBuilder *newStringBuilder();

/** @brief Usuwa strutkturę @ref StringBuilder.
 * Usuwa strukturę. Jeśli @p freeData wynosi @p true usuwa też przechowywane
 * słowo.
 * @param[in,out] string            - wskaźnik na usuwaną strukturę;
 * @param[in] freeData              - czy należy usunąć dane.
 */
void deleteStringBuilder(StringBuilder *string, bool freeData);

/** @brief Dodaje słowo do struktury.
 * Dodaje dane słowo i znak ';' na koniec tworzonego napisu.
 * @param[in,out] string            - wskaźnik na tworzony napis;
 * @param[in] ptr                   - wskaźnik na dodawane słowo.
 * @return Wartość @p true jeśli udało się dodać słow lub @p false, jeśli
 * nie udało się zaalokować pamięci.
 */
bool appendStringBuilderString(StringBuilder *string, const char *ptr);

/** @brief Dodaje liczbę do struktury.
 * Dodaje dane liczbę zrzutowaną na słowo i znak ';'
 * na koniec tworzonego napisu.
 * @param[in,out] string            - wskaźnik na tworzony napis;
 * @param[in] integer               - dodawana liczba.
 * @return Wartość @p true jeśli udało się dodać słow lub @p false, jeśli
 * nie udało się zaalokować pamięci.
 */
bool appendStringBuilderInteger(StringBuilder *string, int64_t integer);

#endif // STRING_BUILDER_H
