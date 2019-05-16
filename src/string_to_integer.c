/** @file
 * Implementacja interfejsu udostępniającego możliwość zamiany słowa w stylu C
 * na typ całkowity.
 *
 * @author Paweł Pawlik <pp406289@students.mimuw.edu.pl>
 * @date 16.05.2019
 */


#include "string_to_integer.h"

#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <assert.h>

/** @brief Zamienia słowo na typ int.
 * @param[in] string    - wskaźnik na słowo do zamiany.
 * @return Liczba typu int jaką reprezentuje dane słowo lub @p 0 jeśli
 * słowo składniowo nie jest typem int.
 */
int stringToInt(const char *string) {
    assert(string);

    char *c;
    errno = 0;
    long result = strtol(string, &c, 10);
    if (errno == ERANGE || result < INT_MIN || result > INT_MAX) {
        return 0;
    }

    return result;
}

/** @brief Zamienia słowo na typ unsigned.
 * @param[in] string    - wskaźnik na słowo do zamiany.
 * @return Liczba typu unsigned jaką reprezentuje dane słowo lub @p 0 jeśli
 * słowo składniowo nie jest typem unsigned.
 */
unsigned stringToUnsigned(const char *string) {
    assert(string);

    char *c;
    errno = 0;
    long result = strtoul(string, &c, 10);
    if (errno == ERANGE || result > UINT_MAX) {
        return 0;
    }

    return result;
}
