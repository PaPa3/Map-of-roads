#include "list.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

ListIterator *newListIterator(List *father) {
    assert(father);

    ListIterator *result = malloc(sizeof(ListIterator));
    if (result == NULL) {
        return NULL;
    }

    result->father = father;
    result->next = result;
    result->previous = result;
    result->data = NULL;

    return result;
}

List *newList() {
    List *result = malloc(sizeof(List));
    if (result == NULL) {
        return NULL;
    }

    ListIterator *iterator = newListIterator(result);
    if (iterator == NULL) {
        free(result);
        return NULL;
    }

    result->begin = iterator;
    result->end = iterator;

    return result;
}

ListIterator *eraseList(ListIterator *iterator, bool freeData) {
    assert(iterator);
    assert(iterator->next != iterator); // nie można usunąć końca

    if (iterator->previous == iterator) { // iterator == początek listy
        iterator->next->previous = iterator->next;
        iterator->father->begin = iterator->next;
    } else {
        iterator->previous->next = iterator->next;
        iterator->next->previous = iterator->previous;
    }

    ListIterator *result = iterator->next;

    if (freeData && iterator->data != NULL) {
        free(iterator->data);
    }
    free(iterator);

    return result;
}

ListIterator *insertList(ListIterator *iterator, void *newData) {
    assert(iterator);

    ListIterator *result = newListIterator(iterator->father);
    if (result == NULL) {
        return NULL;
    }

    result->data = newData;

    if (iterator->previous != iterator) { // jesli iterator != poczatek listy
        result->previous = iterator->previous;
        iterator->previous->next = result;
    } else {
        iterator->father->begin = result;
    }
    result->next = iterator;
    iterator->previous = result;

    return result;
}

void clearList(List * List, bool freeData) {
    assert(List);

    while (List->begin != List->end) {
        eraseList(List->begin, freeData);
    }
}

void deleteList(List * List, bool freeData) {
    assert(List);

    clearList(List, freeData);
    free(List->end);
    free(List);
}
