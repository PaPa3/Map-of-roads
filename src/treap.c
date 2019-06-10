#include "treap.h"

#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <assert.h>
#include <stdlib.h>

Treap *newTreap(void *data) {
    Treap *ptr = malloc(sizeof(Treap));
    if (ptr == NULL) {
        return NULL;
    }

    ptr->leftSon = NULL;
    ptr->rightSon = NULL;
    ptr->father = NULL;
    ptr->data = data;
    ptr->priority = rand();
    return ptr;
}

Treap *attachTreap(Treap *father, Treap *leftSon, Treap *rightSon) {
    assert(father != NULL);
    father->leftSon = leftSon;
    father->rightSon = rightSon;

    if (leftSon) {
        leftSon->father = father;
    }
    if (rightSon) {
        rightSon->father = father;
    }
    return father;
}

Treap *mergeTreap(Treap *left, Treap *right) {
    assert(left != NULL || right != NULL);

    if (left == NULL) {
        return right;
    }

    if (right == NULL) {
        return left;
    }

    if (left->priority > right->priority) {
        Treap *ptr = mergeTreap(left->rightSon, right);
        return attachTreap(left, left->leftSon, ptr);
    } else {
        Treap *ptr = mergeTreap(left, right->leftSon);
        return attachTreap(right, ptr, right->rightSon);
    }
}

void splitTreap(Treap *ptr, Treap **leftResult, Treap **rightResult,
                void *splitPoint, int(*compare)(const void *, const void *)) {
    assert(*leftResult == NULL);
    assert(*rightResult == NULL);

    if (ptr == NULL) {
        return;
    }

    int c = compare(ptr->data, splitPoint);
    if (c == 0) {
        *leftResult = ptr->leftSon;
        *rightResult = ptr;
        attachTreap(*rightResult, NULL, (*rightResult)->rightSon);
    } else if (c < 0) {
        splitTreap(ptr->rightSon, leftResult, rightResult, splitPoint, compare);
        *leftResult = attachTreap(ptr, ptr->leftSon, *leftResult);
    } else {
        splitTreap(ptr->leftSon, leftResult, rightResult, splitPoint, compare);
        *rightResult = attachTreap(ptr, *rightResult, ptr->rightSon);
    }

    if (*leftResult) {
        (*leftResult)->father = NULL;
    }

    if (*rightResult) {
        (*rightResult)->father = NULL;
    }
}

Treap *firstElementTreap(Treap *treap) {
    if (treap == NULL) {
        return NULL;
    }

    while (treap->leftSon) {
        treap = treap->leftSon;
    }

    return treap;
}

void deleteTreap(Treap *treap, bool freeData) {
    if (treap == NULL) {
        return;
    }

    deleteTreap(treap->leftSon, freeData);
    deleteTreap(treap->rightSon, freeData);

    if (freeData) {
        free(treap->data);
    }
    free(treap);
}
