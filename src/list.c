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

void insertListIterator(ListIterator *iterator, ListIterator *newIterator) {
    assert(iterator);
    assert(newIterator);

    if (iterator->previous != iterator) { // jesli iterator != poczatek listy
        newIterator->previous = iterator->previous;
        iterator->previous->next = newIterator;
    } else {
        newIterator->previous = newIterator;
        iterator->father->begin = newIterator;
    }
    newIterator->next = iterator;
    iterator->previous = newIterator;
    newIterator->father = iterator->father;
}

ListIterator *insertList(ListIterator *iterator, void *newData) {
    assert(iterator);

    ListIterator *result = newListIterator(iterator->father);
    if (result == NULL) {
        return NULL;
    }

    result->data = newData;
    insertListIterator(iterator, result);

    return result;
}

void spliceList(ListIterator *iterator, List *sourceList) {
    assert(iterator);
    assert(sourceList);

    ListIterator *ptr = sourceList->begin;
    while (ptr != sourceList->end) {
        ptr = ptr->next;
        insertListIterator(iterator, ptr->previous);
    }

    sourceList->begin = sourceList->end;
    sourceList->end->previous = sourceList->end;
}

void clearList(List *List, bool freeData) {
    assert(List);

    while (List->begin != List->end) {
        eraseList(List->begin, freeData);
    }
}

void deleteList(List *List, bool freeData) {
    assert(List);

    clearList(List, freeData);
    free(List->end);
    free(List);
}

void swapListIteratorPointers(ListIterator **iterator1, ListIterator **iterator2) {
    assert(iterator1);
    assert(iterator2);

    ListIterator *ptr = *iterator1;
    *iterator1 = *iterator2;
    *iterator2 = ptr;
}

void reverseList(List *list) {
    assert(list);

    if (list->begin == list->end) {
        return;
    }

    ListIterator *iterator = list->begin;
    while (iterator->next != iterator) {
        swapListIteratorPointers(&iterator->previous, &iterator->next);
        iterator = iterator->previous;
    }

    iterator = list->begin;
    list->begin->next = list->end;
    list->begin = list->end->previous;
    list->begin->previous = list->begin;
    list->end->previous = iterator;
}
