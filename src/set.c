#include "set.h"
#include "treap.h"

#include <assert.h>
#include <stdlib.h>

typedef struct Set {
    Treap *root;
    int (*compare)(const void *, const void *);
} Set;

Set *newSet(int (*compare)(const void *, const void *)) {
    assert(compare);

    Set *result = malloc(sizeof(Set));
    if (result == NULL) {
        return NULL;
    }

    result->root = NULL;
    result->compare = compare;

    return result;
}

SetIterator *beginSet(Set *set) {
    assert(set);

    return firstElementTreap(set->root);
}

SetIterator *nextSetIterator(SetIterator *setIterator) {
    assert(setIterator);

    if (setIterator->rightSon != NULL) {
        return firstElementTreap(setIterator->rightSon);
    }

    SetIterator *previous;
    do {
        previous = setIterator;
        setIterator = setIterator->father;
    } while (setIterator != NULL && setIterator->rightSon == previous);

    return setIterator;
}

SetIterator *findSet(Set *set, void *data) {
    assert(set);

    Treap *left = NULL;
    Treap *right = NULL;

    splitTreap(set->root, &left, &right, data, set->compare);
    SetIterator *result = firstElementTreap(right);
    set->root = mergeTreap(left, right);

    return result;
}

bool insertSet(Set *set, void *data) {
    Treap *treap = newTreap(data);
    if (treap == NULL) {
        return false;
    }

    Treap *left = NULL;
    Treap *right = NULL;

    splitTreap(set->root, &left, &right, data, set->compare);

    Treap *begin = firstElementTreap(right);
    if (begin == NULL || set->compare(data, begin->data) != 0) {
        right = mergeTreap(treap, right);
    }

    set->root = mergeTreap(left, right);

    return true;
}

void eraseSet(Set *set, void *data, bool freeData) {
    assert(set);

    Treap *left = NULL;
    Treap *right = NULL;

    splitTreap(set->root, &left, &right, data, set->compare);

    Treap *begin = firstElementTreap(right);
    if (begin != NULL && set->compare(data, begin->data) == 0) {
        if (begin->father == NULL) {
            right = begin->rightSon;
            if (right != NULL) {
                right->father = NULL;
            }
        } else {
            attachTreap(begin->father, begin->rightSon, begin->father->rightSon);
        }
        begin->rightSon = NULL;
        deleteTreap(begin, freeData);
    }

    set->root = mergeTreap(left, right);
}

void deleteSet(Set *set, bool freeData) {
    assert(set);

    deleteTreap(set->root, freeData);
}
