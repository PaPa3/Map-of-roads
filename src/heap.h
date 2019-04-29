/** @file
 * Interfejs klasy przechowującej stertę (kolejkę priorytetową).
 *
 * @author Paweł Pawlik <pp406289@students.mimuw.edu.pl>
 * @date 29.04.2019
 */

#ifndef HEAP_H
#define HEAP_H

#include "city.h"

#include <stdint.h>
#include <stdbool.h>

/**
 * Struktura przechuwuje klucze według, których sortowane są dane na stercie.
 * Struktura posiada relację (ważność) będącą porządkiem liniowym.
 * Ważniejszy jest klucz z mniejszą odległością. Jeśli dane dwie struktury
 * @ref HeapKey mają taką samą odległość to ważniejsza jest ta, której
 * najdawniej wybudowany odcinek jest najmłodszy.
 */
typedef struct HeapKey {
    int64_t distance;           ///< odległość
    int64_t oldestRoad;         ///< najdawniej wybudowany odcinek
} HeapKey;

/**
 * Struktura przechuwująca stertę.
 * Pole sterty składa się z klucza (@ref HeapKey) oraz wskaźnika na (@ref City).
 * Na wierzchu sterty znajdują się dane dla których klucz (@ref HeapKey) jest
 * najważniejszy. Wierzch sterty jest indeksowany numerem @p 1.
 */
typedef struct Heap {
    City **data;                ///< tablica wskaźników na (@ref City)
    HeapKey **keys;             ///< tablica kluczy
    uint32_t size;              ///< liczba elementów znajdujących się na stercie
    uint32_t reservedMemory;    ///< długość aktualnie zaalokowanej tablicy
                                ///  (@ref data) oraz (@ref keys)
} Heap;

/** @brief Tworzy nową strukturę.
 * Tworzy pustą stertę.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Heap *newHeap();

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p heap. Funkcja nie usuwa struktur
 * wskazywanych przez @ref Heap.data.
 * @param[in] heap              – wskaźnik na usuwaną strukturę.
 */
void deleteHeap(Heap *heap);

/** @brief Dodaje element na stertę.
 * Dodaje miasto (@ref City) na stertę z kluczem (@ref HeapKey).
 * @param[in,out] heap          - wskaźnik na stertę;
 * @param[in] distance          - odległość klucza;
 * @param[in] oldestRoad        - najdawniej wybudowany odcinek klucza;
 * @param[in] city              - wskaźnik na dodawane miasto.
 * @return Zwraca @p true. Jeśli nie udało się zaalokować potrzebnej pamięci
 * to zwraca @p false.
 */
bool pushHeap(Heap *heap, int64_t distance, int64_t oldestRoad, City *city);

/** @brief Usuwa wierzch sterty.
 * Usuwa wierzch sterty. Funkcja nie usuwa struktury
 * wskazywanej przez @ref Heap.data.
 * @param[in,out] heap          - wskaźnik na stertę.
 */
void popHeap(Heap *heap);

#endif // HEAP_H
