#include "heap.h"

#include <stdlib.h>
#include <assert.h>

#define DEFAULT_HEAP_ARRAY_MEMORY_SIZE 8

typedef struct HeapKey {
    int64_t distance;
    int64_t oldestRoad;
} HeapKey;

HeapKey *newHeapKey(int64_t distance, int64_t oldestRoad) {
    HeapKey *result = malloc(sizeof(HeapKey));
    if (result == NULL) {
        return NULL;
    }

    result->distance = distance;
    result->oldestRoad = oldestRoad;

    return result;
}

void deleteHeap(Heap *heap, bool freeData) {
    assert(heap);

    for (uint32_t i = 1; i <= heap->size; i++) {
        free(heap->keys[i]);
        if (freeData && heap->data[i] != NULL) {
            free(heap->data[i]);
        }
    }

    free(heap->data);
    free(heap->keys);
    free(heap);
}

bool isHeapKeySmallerThan(HeapKey *key, HeapKey *other) {
    if (key->distance == other->distance) {
        return key->oldestRoad < other->oldestRoad;
    }

    return key->distance < other->distance;
}

bool reserveMemoryHeap(Heap *heap, uint32_t newMemory) {
    assert(heap);

    if (newMemory < heap->reservedMemory) {
        return true;
    }

    void **ptr = realloc(heap->data, sizeof(void *) * newMemory);
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

    if (!reserveMemoryHeap(result, DEFAULT_HEAP_ARRAY_MEMORY_SIZE)) {
        free(result);
        return NULL;
    }

    return result;
}

void swapHeapData(Heap *heap, uint32_t position1, uint32_t position2) {
    assert(heap);
    assert(0 < position1 <= heap->size);
    assert(0 < position2 <= heap->size);

    HeapKey *key = heap->keys[position1];
    heap->keys[position1] = heap->keys[position2];
    heap->keys[position2] = key;

    void *pointer = heap->data[position1];
    heap->data[position1] = heap->data[position2];
    heap->data[position2] = pointer;
}

bool pushHeap(Heap *heap, int64_t distance, int64_t oldestRoad, void *data) {
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
    heap->data[position] = data;
    heap->keys[position] = key;

    while (position > 1 &&
           isHeapKeySmallerThan(heap->keys[position], heap->keys[position / 2])) {
           //heap->keys[position] < heap->keys[position / 2]) {
        swapHeapData(heap, position, position / 2);

        position /= 2;
    }

    return true;
}

void *popHeap(Heap *heap, bool freeData) {
    assert(heap);
    assert(heap->size);

    void *result = heap->data[1];

    swapHeapData(heap, 1, heap->size);
    if (freeData && heap->data[heap->size] != NULL) {
        free(heap->data[heap->size]);
    }

    if (--heap->size == 0) {
        return result;
    }

    uint32_t position = 1;
    while (true) {
        uint32_t childWithSmallestKey = 0;

        if (position * 2 <= heap->size) {
            childWithSmallestKey = position * 2;
        }

        if (position * 2 + 1 <= heap->size &&
                isHeapKeySmallerThan(heap->keys[position * 2 + 1],
                                     heap->keys[childWithSmallestKey]) ) {
                //heap->keys[position * 2 + 1] < heap->keys[childWithSmallestKey]) {
            childWithSmallestKey = position * 2 + 1;
        }

        if (childWithSmallestKey != 0 &&
                isHeapKeySmallerThan(heap->keys[childWithSmallestKey],
                                     heap->keys[position])) {
                //heap->keys[position] > heap->keys[childWithSmallestKey]) {
            swapHeapData(heap, position, childWithSmallestKey);
            position = childWithSmallestKey;
        } else {
            break;
        }
    }

    return result;
}
