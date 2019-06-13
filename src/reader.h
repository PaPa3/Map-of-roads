/** @file
 * Interfejs udostępniający możliwość wczytywania ze standardowego wejścia.
 *
 * @author Paweł Pawlik <pp406289@students.mimuw.edu.pl>
 * @date 16.05.2019
 */

#ifndef READER_H
#define READER_H

#include "list.h"

#include <stdint.h>

/** @brief Wczytuje wiersz ze standardowego wejścia.
 * Wczytuje wiersz i dzieli go na słowa. Słowa to ciągi znaków oddzielone
 * znakiem ';' nie zawierające znaku o kodzie 0. Tworzy nową listę
 * i umieszcza na niej wczytane słowa. Jeśli funkcja zakończy się sukcesem
 * to @p *result wskazuję na tę liste.
 * @param[in,out] result    - wskaźnik na wskaźnik wskazujący na @p NULL.
 * @return Wartość @p 0 jeśli funkcja zakończyła się powodzenie,
 * wartość @p 1 jeśli, któreś ze słów zawiera niepoprawny znak
 * lub jest pustym słowem,
 * wartość @p 2 jeśli nie udało się zaalokować pamięci lub
 * wartość @p 3 jeśli linia do zignorowania lub
 * wartość @p EOF jeśli linia zakończyła się znakiem @p EOF.
 */
int nextLineReader(List **result);

#endif // READER_H
