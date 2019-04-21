#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

typedef struct List List;

typedef struct ListIterator ListIterator;

List *newList(void *(*dataCopy)(const void *), void (*dataDelete)(void *));

List *listCopy(List *list);

ListIterator *listBegin(List *list);

ListIterator *listEnd(List *list);

bool listIteratorIsBegin(ListIterator *iterator);

bool listIteratorIsEnd(ListIterator *iterator);

ListIterator *listIteratorNext(ListIterator *listIterator);

ListIterator *listIteratorPrevious(ListIterator *listIterator);

const void *listIteratorData(ListIterator *listIterator);

ListIterator *listIteratorSetData(ListIterator *iterator, const void *newData);

ListIterator *listIteratorInsert(ListIterator *iterator, const void *newData);

ListIterator *listPushBack(List *list, const void *newData);

ListIterator *listPushFront(List *list, const void *newData);

ListIterator *listIteratorErase(ListIterator *iterator);

bool listPopBack(List *list);

bool listPopFront(List *list);

void listDelete(List *list);

bool listClear(List *list);

#endif // LIST_H
