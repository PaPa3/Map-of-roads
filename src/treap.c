#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <assert.h>
#include <stdlib.h>
#include "treap.h"

typedef struct Treap {
    struct Treap *leftSon, *rightSon, *father;
    uint32_t priority;
    uint32_t numberOfTreapsThatPointOnMe;
    void *data;
    bool isRoot;
} Treap;

Treap *newTreap() {
    Treap *ptr = malloc(sizeof(Treap));
    if (ptr == NULL) {
        return NULL;
    }

    ptr->leftSon = NULL;
    ptr->rightSon = NULL;
    ptr->father = NULL;
    ptr->data = NULL;
    ptr->priority = rand();
    ptr->numberOfTreapsThatPointOnMe = 0;
    ptr->isRoot = true;
    return ptr;
}

Treap *newTreapWithFather(Treap *father) {
    Treap *ptr = newTreap();
    if (ptr == NULL) {
        return NULL;
    }
    ptr->father = father;
    ptr->numberOfTreapsThatPointOnMe = 1;
    ptr->isRoot = false;
    return ptr;
}

void increasePointingTreapsByOne(Treap *ptr) {
    assert(ptr != NULL);
    ptr->numberOfTreapsThatPointOnMe++;
    ptr->isRoot = false;
}

void reducePointingTreapsByOne(Treap *ptr, bool makePtrRoot) {
    if (ptr == NULL) {
        return;
    }

    ptr->numberOfTreapsThatPointOnMe--;
    if (ptr->numberOfTreapsThatPointOnMe == 0) {
        if (!makePtrRoot) {
            reducePointingTreapsByOne(ptr->leftSon, false);
            reducePointingTreapsByOne(ptr->rightSon, false);
            free(ptr);
        } else {
            ptr->isRoot = true;
        }
    }
}

Treap *attach(Treap *father, Treap *leftSon, Treap *rightSon) {
    assert(father != NULL);
    father->leftSon = leftSon;
    father->rightSon = rightSon;
    increasePointingTreapsByOne(leftSon);
    increasePointingTreapsByOne(rightSon);
    return father;
}

bool copyMyChildTreapIfNecessary(Treap *me, Treap *child) {
    assert(me != NULL);
    assert(me->leftSon == child || me->rightSon == child);

    if (child == NULL) {
        return true;
    }

    if (child->father == me) {
        return true;
    } else {
        Treap *ptr = newTreap();
        if (ptr == NULL) {
            return false;
        }

        *ptr = *child;
        if (me->leftSon == child) {
            me->leftSon = ptr;
        } else {
            me->rightSon = ptr;
        }
        ptr->father = me;
        ptr->numberOfTreapsThatPointOnMe = 1;
        reducePointingTreapsByOne(child, false);
        return true;
    }
}

Treap *merge(Treap *left, Treap *right) {
    assert(left != NULL || right != NULL);
    if (left == NULL) {
        return right;
    }
    if (right == NULL) {
        return left;
    }

    if (left->priority > right->priority) {
        if (!copyMyChildTreapIfNecessary(left, left->rightSon)) {
            return NULL;
        }
        Treap *ptr = merge(left->rightSon, right);
        if (ptr == NULL) {
            return NULL;
        }
        return attach(left, left->leftSon, ptr);
    } else {
        if (!copyMyChildTreapIfNecessary(right, right->leftSon)) {
            return false;
        }
        Treap *ptr = merge(left, right->leftSon);
        if (ptr == NULL) {
            return NULL;
        }
        return attach(right, ptr, right->rightSon);
    }
}

typedef struct PairTreap {
    Treap *first, *second;
} PairTreap;

PairTreap *newPairTreap(Treap *first, Treap *second) {
    PairTreap *ptr = malloc(sizeof(PairTreap));
    if (ptr == NULL) {
        return NULL;
    }

    ptr->first = first;
    ptr->second = second;
    return ptr;
}

PairTreap *split(Treap *ptr, void *splitPoint,
                 int(*compare)(const void *, const void *)) {
    if (ptr == NULL) {
        return newPairTreap(NULL, NULL);
    }

    int c = compare(ptr->data, splitPoint);
    if (c == 0) {
        Treap *first = ptr->leftSon;
        return newPairTreap(first, attach(ptr, NULL, ptr->rightSon));
    } else if (c < 0) {
        if (!copyMyChildTreapIfNecessary(ptr, ptr->rightSon)) {
            return NULL;
        }

        PairTreap *result = split(ptr->rightSon, splitPoint, compare);
        if (result == NULL) {
            return NULL;
        }

        return newPairTreap(attach(ptr, ptr->leftSon, result->first), result->second);
    } else {
        if (!copyMyChildTreapIfNecessary(ptr, ptr->leftSon)) {
            return NULL;
        }

        PairTreap *result = split(ptr->leftSon, splitPoint, compare);
        if (result == NULL) {
            return NULL;
        }

        return newPairTreap(result->first, attach(ptr, result->second, ptr->rightSon));
    }
}
