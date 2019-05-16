/** @file
 * Interfejs udostępniający możliwość zamiany słowa w stylu C na typ całkowity.
 *
 * @author Paweł Pawlik <pp406289@students.mimuw.edu.pl>
 * @date 16.05.2019
 */

#ifndef STRING_TO_INTEGER_H
#define STRING_TO_INTEGER_H

/** @brief Zamienia słowo na typ int.
 * @param[in] string    - wskaźnik na słowo do zamiany.
 * @return Liczba typu int jaką reprezentuje dane słowo lub @p 0 jeśli
 * słowo składniowo nie jest typem int.
 */
int stringToInt(const char *string);

/** @brief Zamienia słowo na typ unsigned.
 * @param[in] string    - wskaźnik na słowo do zamiany.
 * @return Liczba typu unsigned jaką reprezentuje dane słowo lub @p 0 jeśli
 * słowo składniowo nie jest typem unsigned.
 */
unsigned stringToUnsigned(const char *string);

#endif // STRING_TO_INTEGER_H
