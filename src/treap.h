#ifndef TREAP_H
#define TREAP_H

#include <stdint.h>
#include <stdbool.h>

typedef struct Treap {
    struct Treap *leftSon, *rightSon, *father;
    uint32_t priority;
    void *data;
} Treap;

Treap *newTreap(void *data);

Treap *attachTreap(Treap *father, Treap *leftSon, Treap *rightSon);

Treap *mergeTreap(Treap *left, Treap *right);

void splitTreap(Treap *ptr, Treap **leftResult, Treap **rightResult,
                void *splitPoint, int(*compare)(const void *, const void *));

Treap *firstElementTreap(Treap *treap);

void deleteTreap(Treap *treap, bool freeData);

#endif // TREAP_H
