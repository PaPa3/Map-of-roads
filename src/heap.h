#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>
#include <stdbool.h>

typedef struct Heap {
    void **data;
    int64_t *keys;
    uint32_t size;
    uint32_t reservedMemory;
} Heap;

Heap *newHeap();

bool pushHeap(Heap *heap, int64_t key, void *data);

void *popHeap(Heap *heap, bool freeData);

#endif // HEAP_H
