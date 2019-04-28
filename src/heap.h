#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>
#include <stdbool.h>

typedef struct HeapKey {
    int64_t distance;
    int64_t oldestRoad;
} HeapKey;


typedef struct Heap {
    void **data;
    HeapKey **keys;
    uint32_t size;
    uint32_t reservedMemory;
} Heap;

Heap *newHeap();

void deleteHeap(Heap *heap, bool freeData);

bool pushHeap(Heap *heap, int64_t distance, int64_t oldestRoad, void *data);

// zwraca top
void *popHeap(Heap *heap, bool freeData);

#endif // HEAP_H
