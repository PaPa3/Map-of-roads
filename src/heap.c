#include "heap.h"

#include <stdlib.h>
#include <assert.h>

#define DEFAULT_HEAP_ARRAY_MEMORY_SIZE 8
///< domyślny rozmiar zaalokowanej tablicy w (@ref HeapKey)

HeapKey *newHeapKey(int64_t distance, int64_t oldestRoad) {
    HeapKey *result = malloc(sizeof(HeapKey));
    if (result == NULL) {
        return NULL;
    }

    result->distance = distance;
    result->oldestRoad = oldestRoad;

    return result;
}

void deleteHeap(Heap *heap) {
    assert(heap);

    for (uint32_t i = 1; i <= heap->size; i++) {
        free(heap->keys[i]);
    }

    free(heap->data);
    free(heap->keys);
    free(heap);
}

/** @brief Implementuje relację ważności (@ref HeapKey).
 * Sprawdza czy @p key jest ważniejszy niż @p other.
 * @param[in] key               - wskażnik na @ref HeapKey do porównania
 * @param[in] other             - wskażnik na @ref HeapKey do porównania
 * @return @p true jeśli @p key jest ważniejszy niż @p other. @p false
 * w przeciwnym przypadku.
 */
bool isHeapKeyMoreImportantThan(HeapKey *key, HeapKey *other) {
    assert(key);
    assert(other);

    if (key->distance == other->distance) {
        return key->oldestRoad > other->oldestRoad;
    }

    return key->distance < other->distance;
}

/** @brief Zwiększa zaalokowaną tablicę danej sterty.
 * Powiększa zaalokowaną tablicę (@ref keys oraz @ref data) do rozmiaru
 * @p newMemory. Nic nie robi jeśli wartość @p newMemory jest mniejsza niż
 * dotychczas zaalokowana pamięć.
 * @param[in, out] heap         - wskaźnik na stertę;
 * @param[in] newMemory         - docelowa wielkość zaalokowanej tablicy.
 * @return Wartość @p true. Jeśli nie udało się zaalokować potrzebnej pamięci
 * to zwraca @p false.
 */
bool reserveMemoryHeap(Heap *heap, uint32_t newMemory) {
    assert(heap);

    if (newMemory < heap->reservedMemory) {
        return true;
    }

    City **ptr = realloc(heap->data, sizeof(City *) * newMemory);
    if (ptr == NULL) {
        return false;
    }
    heap->data = ptr;

    HeapKey **ptr2 = realloc(heap->keys, sizeof(HeapKey *) * newMemory);
    if (ptr2 == NULL) {
        return NULL;
    }
    heap->keys = ptr2;

    heap->reservedMemory = newMemory;

    return true;
}

Heap *newHeap() {
    Heap *result = malloc(sizeof(Heap));
    if (result == NULL) {
        return NULL;
    }

    result->size = 0;
    result->reservedMemory = 0;
    result->data = NULL;
    result->keys = NULL;

    if (!reserveMemoryHeap(result, DEFAULT_HEAP_ARRAY_MEMORY_SIZE)) {
        free(result);
        return NULL;
    }

    return result;
}

/** @brief Zamienia miejscami dane w tablicy danej sterty.
 * @param[in,out] heap      - wskaźnik na stertę;
 * @param[in] position1     - pozycja w tablicy do zmiany;
 * @param[in] position2     - pozycja w tablicy do zmiany.
 */
void swapHeapData(Heap *heap, uint32_t position1, uint32_t position2) {
    assert(heap);
    assert(0 < position1 && position1 <= heap->size);
    assert(0 < position2 && position2 <= heap->size);

    HeapKey *key = heap->keys[position1];
    heap->keys[position1] = heap->keys[position2];
    heap->keys[position2] = key;

    City *city = heap->data[position1];
    heap->data[position1] = heap->data[position2];
    heap->data[position2] = city;
}

bool pushHeap(Heap *heap, int64_t distance, int64_t oldestRoad, City *city) {
    assert(heap);

    HeapKey *key = newHeapKey(distance, oldestRoad);
    if (key == NULL) {
        return false;
    }

    uint32_t position = heap->size + 1;

    if (position == heap->reservedMemory) {
        if (!reserveMemoryHeap(heap, heap->reservedMemory * 2)) {
            return false;
        }
    }

    heap->size++;
    heap->data[position] = city;
    heap->keys[position] = key;

    while (position > 1 &&
           isHeapKeyMoreImportantThan(heap->keys[position], heap->keys[position / 2])) {
        swapHeapData(heap, position, position / 2);

        position /= 2;
    }

    return true;
}

void popHeap(Heap *heap) {
    assert(heap);
    assert(heap->size);

    swapHeapData(heap, 1, heap->size);
    free(heap->keys[heap->size]);

    if (--heap->size == 0) {
        return;
    }

    uint32_t position = 1;
    while (true) {
        uint32_t childWithMostImportantKey = 0;

        if (position * 2 <= heap->size) {
            childWithMostImportantKey = position * 2;
        }

        if (position * 2 + 1 <= heap->size &&
                isHeapKeyMoreImportantThan(heap->keys[position * 2 + 1],
                                     heap->keys[childWithMostImportantKey]) ) {
            childWithMostImportantKey = position * 2 + 1;
        }

        if (childWithMostImportantKey != 0 &&
                isHeapKeyMoreImportantThan(heap->keys[childWithMostImportantKey],
                                     heap->keys[position])) {
            swapHeapData(heap, position, childWithMostImportantKey);
            position = childWithMostImportantKey;
        } else {
            break;
        }
    }
}
