#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

typedef struct LocalList LocalList;

typedef struct LocalListIterator {
    struct LocalListIterator *next, *previous;
    LocalList *father;
    void *data;
} LocalListIterator;

typedef struct LocalList {
    LocalListIterator *begin, *end;
    uint32_t howManyListsPointOnMe;
    void *(*dataCopy)(const void *);
    void (*dataDelete)(void *);
} LocalList;

typedef struct List {
    LocalList *localList;
} List;

typedef struct ListIterator {
    LocalListIterator *iterator;
    List *father;
} ListIterator;

LocalListIterator *newLocalListIterator(LocalList *father) {
    assert(father != NULL);

    LocalListIterator *ptr = malloc(sizeof(LocalListIterator));
    if (ptr == NULL) {
        return NULL;
    }

    ptr->father = father;
    ptr->next = ptr;
    ptr->previous = ptr;
    ptr->data = NULL;
    return ptr;
}

LocalList *newLocalList(void *(*dataCopy)(const void *), void (*dataDelete)(void *)) {
    LocalList *ptr = malloc(sizeof(LocalList));
    if (ptr == NULL) {
        return NULL;
    }

    LocalListIterator *iterator = newLocalListIterator(ptr);
    if (iterator == NULL) {
        return NULL;
    }

    ptr->dataCopy = dataCopy;
    ptr->dataDelete = dataDelete;
    ptr->begin = iterator;
    ptr->end = iterator;
    ptr->howManyListsPointOnMe = 0;
    return ptr;
}

List *newList(void *(*dataCopy)(const void *), void (*dataDelete)(void *)) {
    List *ptr = malloc(sizeof(List));
    if (ptr == NULL) {
        return NULL;
    }

    LocalList *localList = newLocalList(dataCopy, dataDelete);
    if (localList == NULL) {
        return NULL;
    }

    ptr->localList = localList;
    localList->howManyListsPointOnMe = 1;
    return ptr;
}

List *listCopy(List *list) {
    assert(list != NULL);

    List *result = malloc(sizeof(List));
    if (result == NULL) {
        return NULL;
    }

    *result = *list;
    result->localList->howManyListsPointOnMe++;
    return result;
}

LocalListIterator *localListIteratorCopy(LocalListIterator *locallListIterator) {
    assert(locallListIterator != NULL);

    LocalListIterator *ptr = malloc(sizeof(LocalListIterator));
    if (ptr == NULL) {
        return NULL;
    }

    *ptr = *locallListIterator;
    if (ptr->data != NULL) {
        ptr->data = ptr->father->dataCopy(ptr->data);
        if (ptr->data == NULL) {
            free(ptr);
            return NULL;
        }
    }
    return ptr;
}

LocalListIterator *localListErase(LocalListIterator *iterator) {
    assert(iterator != NULL);
    assert(iterator->next != iterator);


    if (iterator->previous == iterator) { // iterator == początek listy
        iterator->next->previous = iterator->next;
        iterator->father->begin = iterator->next;
    } else {
        iterator->previous->next = iterator->next;
        iterator->next->previous = iterator->previous;
    }

    LocalListIterator *result = iterator->next;
    iterator->father->dataDelete(iterator->data);
    free(iterator);
    return result;
}

void localListPopFront(LocalList *localList) {
    assert(localList != NULL);

    localListErase(localList->begin);
    /*assert(localList != NULL);
    assert(localList->begin != localList->end);

    localList->begin = localList->begin->next;
    free(localList->begin->previous);
    localList->begin->previous = localList->begin;*/
}

void localListClear(LocalList * localList) {
    assert(localList != NULL);

    while (localList->begin != localList->end) {
        localListPopFront(localList);
    }
}

void localListDelete(LocalList * localList) {
    assert(localList != NULL);

    localListClear(localList);
    free(localList->begin);
    free(localList);
}

void listDelete(List *list) {
    assert(list != NULL);

    list->localList->howManyListsPointOnMe--;
    if (list->localList->howManyListsPointOnMe == 0) {
        localListDelete(list->localList);
    }

    free(list);
}

bool listClear(List *list) {
    assert(list != NULL);

    LocalList *ptr = newLocalList(list->localList->dataCopy, list->localList->dataDelete);
    if (ptr == NULL) {
        return false;
    }

    list->localList->howManyListsPointOnMe--;
    if (list->localList->howManyListsPointOnMe == 0) {
        localListDelete(list->localList);
    }

    list->localList = ptr;
    ptr->howManyListsPointOnMe = 1;
    return true;
}

LocalList *localListCopy(LocalList *localList) {
    assert(localList != NULL);

    LocalList *result = malloc(sizeof(LocalList));
    if (result == NULL) {
        return NULL;
    }

    *result = *localList;
    LocalListIterator *ptr = localListIteratorCopy(result->begin);
    if (ptr == NULL) {
        free(result);
        return NULL;
    }
    ptr->previous = ptr;
    result->begin = ptr;
    ptr->father = result;

    if (localList->begin == localList->end) { // jesli pusta lista
        ptr->next = ptr;
        result->end = ptr;
        return result;
    }

    LocalListIterator *iterator = result->begin;
    bool finishWhile = false;
    while (!finishWhile) {
        LocalListIterator *next = localListIteratorCopy(iterator->next);
        if (next == NULL) {
            while (result->begin != iterator) {
                localListPopFront(result);
            }
            free(result);
            return NULL;
        }

        if (iterator->next->next == iterator->next) { //koniec listy
            finishWhile = true;
        }

        next->father = result;
        iterator->next = next;
        next->previous = iterator;

        iterator = next;
    }

    result->end = iterator;
    iterator->next = iterator;

    return result;
}

LocalListIterator *localListIteratorInsert(LocalListIterator *iterator, const void *newData) {
    assert(iterator != NULL);
    assert(newData != NULL);

    LocalListIterator *ptr = newLocalListIterator(iterator->father);
    if (ptr == NULL) {
        return NULL;
    }

    ptr->data = ptr->father->dataCopy(newData);
    if (ptr->data == NULL) {
        free(ptr);
        return NULL;
    }

    if (iterator->previous != iterator) { // jesli iterator != poczatek listy
        ptr->previous = iterator->previous;
        iterator->previous->next = ptr;
    } else {
        iterator->father->begin = ptr;
    }
    ptr->next = iterator;
    iterator->previous = ptr;
    return ptr;
}

ListIterator *newListIterator(LocalListIterator *iterator, List *father) {
    ListIterator *ptr = malloc(sizeof(ListIterator));
    if (ptr == NULL) {
        return NULL;
    }

    ptr->iterator = iterator;
    ptr->father = father;
    return ptr;
}

ListIterator *listBegin(List *list) {
    assert(list != NULL);

    ListIterator *ptr = newListIterator(list->localList->begin, list);
    return ptr;
}

ListIterator *listEnd(List *list) {
    assert(list != NULL);

    ListIterator *ptr = newListIterator(list->localList->end, list);
    return ptr;
}

bool listIteratorIsBegin(ListIterator *iterator) {
    assert(iterator != NULL);

    return iterator->father->localList->begin == iterator->iterator;
}

bool listIteratorIsEnd(ListIterator *iterator) {
    assert(iterator != NULL);

    return iterator->father->localList->end == iterator->iterator;
}

ListIterator *listIteratorNext(ListIterator *listIterator) {
    assert(listIterator != NULL);

    listIterator->iterator = listIterator->iterator->next;
    return listIterator;
}

ListIterator *listIteratorPrevious(ListIterator *listIterator) {
    assert(listIterator != NULL);

    listIterator->iterator = listIterator->iterator->previous;
    return listIterator;
}

const void *listIteratorData(ListIterator *listIterator) {
    assert(listIterator != NULL);
    return listIterator->iterator->data;
}

ListIterator *listCopyIfNecessary(ListIterator *listIterator) {
    assert(listIterator != NULL);

    if (listIterator->father->localList->howManyListsPointOnMe == 1) {
        return listIterator;
    }

    uint32_t listIteratorPosition = 0;
    LocalListIterator *begin = listIterator->father->localList->begin;
    while (listIterator->iterator != begin) {
        listIteratorPosition++;
        listIterator = listIteratorPrevious(listIterator);
    }

    LocalList *localList = localListCopy(listIterator->father->localList);
    if (localList == NULL) {
        return NULL;
    }

    localList->howManyListsPointOnMe = 1;
    listIterator->father->localList->howManyListsPointOnMe--;

    listIterator->father->localList = localList;

    listIterator->iterator = listIterator->father->localList->begin;
    while (listIteratorPosition) {
        listIteratorPosition--;
        listIterator = listIteratorNext(listIterator);
    }

    return listIterator;
}

ListIterator *listIteratorSetData(ListIterator *iterator, const void *newData) {
    assert(iterator != NULL);
    assert(newData != NULL);

    iterator = listCopyIfNecessary(iterator);
    if (iterator == NULL) {
        return NULL;
    }

    void *data = iterator->father->localList->dataCopy(newData);
    if (data == NULL) {
        return NULL;
    }

    iterator->father->localList->dataDelete(iterator->iterator->data);
    iterator->iterator->data = data;
    return iterator;
}

ListIterator *listIteratorInsert(ListIterator *iterator, const void *newData) {
    assert(iterator != NULL);

    iterator = listCopyIfNecessary(iterator);
    if (iterator == NULL) {
        return NULL;
    }

    LocalListIterator *ptr = localListIteratorInsert(iterator->iterator, newData);
    if (ptr == NULL) {
        return NULL;
    }

    ListIterator *result = newListIterator(ptr, iterator->father);
    if (result == NULL) {
        localListErase(ptr);
        return NULL;
    }

    return result;
}

ListIterator *listPushBack(List *list, const void *newData) {
    assert(list != NULL);

    ListIterator *end = listEnd(list);
    if (end == NULL) {
        return NULL;
    }

    ListIterator *result = listIteratorInsert(end, newData);
    free(end);
    return result;
}

ListIterator *listPushFront(List *list, const void *newData) {
    assert(list != NULL);

    ListIterator *begin = listBegin(list);
    if (begin == NULL) {
        return NULL;
    }

    ListIterator *result = listIteratorInsert(begin, newData);
    free(begin);
    return result;
}

ListIterator *listIteratorErase(ListIterator *iterator) {
    assert(iterator != NULL);

    iterator = listCopyIfNecessary(iterator);
    if (iterator == NULL) {
        return NULL;
    }

    iterator->iterator = localListErase(iterator->iterator);
    if (iterator->iterator == NULL) {
        return NULL;
    }

    return iterator;
}

bool listPopBack(List *list) {
    assert(list != NULL);

    ListIterator *back = listEnd(list);
    if (back == NULL) {
        return NULL;
    }

    back = listIteratorPrevious(back);

    if (back == NULL) {
        return NULL;
    }

    return listIteratorErase(back) != NULL;
}

bool listPopFront(List *list) {
    assert(list != NULL);

    ListIterator *begin = listBegin(list);
    if (begin == NULL) {
        return NULL;
    }

    return listIteratorErase(begin) != NULL;
}

