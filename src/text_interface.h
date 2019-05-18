/** @file
 * Interfejs udostępniający interfejs tekstowy mapy dróg.
 *
 * @author Paweł Pawlik <pp406289@students.mimuw.edu.pl>
 * @date 16.05.2019
 */

#ifndef TEXT_INTERFACE_H
#define TEXT_INTERFACE_H

#include "string_builder.h"
#include "map.h"

/** @brief Obsługuje pojedynczy wiersz wejścia.
 * Czyta pojedynczy wiersz i wywołuję odpowiednie operacje na danej mapie.
 * Jeśli wiersz jest postaci:
 *  - numer drogi krajowej;nazwa miasta;długość odcinka drogi;rok budowy lub
 *  - ostatniego remontu;nazwa miasta;długość odcinka drogi;rok budowy lub
 *  - ostatniego remontu;nazwa miasta;…;nazwa miasta - to dodaje drogę
 *      o takim przebiegu do mapy;
 *  - addRoad;city1;city2;length;builtYear - to wywołuje funkcję @ref addRoad
 *      z podanymi parametrami;
 *  - repairRoad;city1;city2;repairYear - to wywołuje funckję @ref repairRoad
 *      z podanymi parametrami;
 *  - getRouteDescription;routeId - to wywołuje funkcję @ref getRouteDescription
 *      z podanymi parametrem. Jeśli wynik działania tej funkcji jest inny niż
 *      NULL, to wypisuje na standardowe wyjście jedną linię z wynikiem
 *      działania tej funkcji.
 *
 * Jeśli polecenie jest niepoprawne składniowo lub jego wykonanie
 * zakończyło się błędem, czyli odpowiednia funkcja zakończyła się wynikiem
 * @p false lub @p NULL, to wypisuje na standardowe wyjście diagnostyczne
 * jednoliniowy komunikat: ERROR @p lineNumber.
 * @param[in,out] map       - wskźnik na mapę;
 * @param[in] lineNumber    - numer aktualnie obsługiwanej linii wejścia.
 * @return Wartość @p 0 jeśli operacja zakończyła się sukcesem,
 * wartość @p 2 jeśli nie udało się zaalokować pamięci lub
 * wartość @p EOF jeśli wiersz zakończył się znakiem @p EOF.
 */
int nextCommandTextInterface(Map *map, uint32_t lineNumber);

#endif // TEXT_INTERFACE_H
