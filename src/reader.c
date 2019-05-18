/** @file
 * Implementacja interfejsu udostępniającego możliwość wczytywania
 * ze standardowego wejścia.
 *
 * @author Paweł Pawlik <pp406289@students.mimuw.edu.pl>
 * @date 16.05.2019
 */

#include "reader.h"
#include "string_builder.h"

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

/** @brief Czyta pojedyncze słowo ze standarowego wejścia.
 * Wczytuje słowo. Słowa to ciągi znaków oddzielone
 * znakiem ';' lub '\n' nie zawierające znaków o kodach [0, 31].
 * Jeśli funkcja zakończy się sukcesem
 * to @p *result wskazuję napis w stylu C zawierający wczytane słowo
 * @param[in,out] result    - wskaźnik na wskaźnik wskazujący na @p NULL.
 * @return Wartość @p 1 jeśli słowo zawiera niepoprawny znak,
 * wartość @p 2 jeśli nie udało się zaalokować pamięci lub
 * znak jaki wystąpił po danym słowie zrzutowany na typ int.
 */
int nextStringReader(char **result) {
    assert(result);
    assert(*result == NULL);

    StringBuilder *string = newStringBuilder();
    if (string == NULL) {
        return 2;
    }

    char c;
    while (scanf("%c", &c) == 1) {
        if (c == ';' || c == '\n') {
            *result = string->data;
            deleteStringBuilder(string, false);
            return c;
        }

        if (0 <= c && c <= 31) {
            deleteStringBuilder(string, true);
            return 1;
        }

        if (!appendStringBuilderChar(string, c)) {
            deleteStringBuilder(string, true);
            return 2;
        }
    }

    *result = string->data;
    deleteStringBuilder(string, false);
    return EOF;
}

/** @brief Ignoruje cały wiersz z wejścia.
 * Ignoruję wszystkie znaki do napotkania @p EOF lub znaku '\n'.
 */
void ignoreLineReader()  {
    char c;
    while (scanf("%c", &c) == 1 && c != '\n');
}

/** @brief Wczytuje wiersz ze standardowego wejścia.
 * Wczytuje wiersz i dzieli go na słowa. Słowa to ciągi znaków oddzielone
 * znakiem ';' nie zawierające znaków o kodach [0, 31]. Tworzy nową listę
 * i umieszcza na niej wczytane słowa. Jeśli funkcja zakończy się sukcesem
 * to @p *result wskazuję na tę liste.
 * @param[in,out] result    - wskaźnik na wskaźnik wskazujący na @p NULL.
 * @return Wartość @p 0 jeśli funkcja zakończyła się powodzenie,
 * wartość @p 1 jeśli, któreś ze słów zawiera niepoprawny znak,
 * wartość @p 2 jeśli nie udało się zaalokować pamięci lub
 * wartość @p EOF jeśli linia zakończyła się znakiem @p EOF.
 */
int nextLineReader(List **result) {
    assert(result);
    assert(*result == NULL);

    *result = newList();
    if (*result == NULL) {
        return 2;
    }

    int x;
    do {
        char *ptr = NULL;
        x = nextStringReader(&ptr);

        if (x != 1 && x != 2 && insertList((*result)->end, ptr) == NULL) {
            x = 2;
        }

        if (x == 1 || x == 2) {
            deleteList(*result, true);
            *result = NULL;

            if (x == 1) {
                ignoreLineReader();
                return 1;
            } else {
                return 2;
            }
        }
    } while (x == ';');

    if (x == EOF) {
        return EOF;
    }

    return 0;
}
