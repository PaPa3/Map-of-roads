#ifndef SET_H
#define SET_H

#include "treap.h"

typedef struct Set Set;

typedef struct Treap SetIterator;

Set *newSet(int (*compare)(const void *, const void *));

SetIterator *beginSet(Set *set);

SetIterator *nextSetIterator(SetIterator *setIterator);

SetIterator *findSet(Set *set, void *data);

bool insertSet(Set *set, void *data);

void eraseSet(Set *set, void *data, bool freeData);

void deleteSet(Set *set, bool freeData);

#endif // SET_H
