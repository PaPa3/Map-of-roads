#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

typedef struct List List;

typedef struct ListIterator {
    struct ListIterator *next, *previous;
    List *father;
    void *data;
} ListIterator;

typedef struct List {
    ListIterator *begin, *end;
} List;

List *newList();

ListIterator *eraseList(ListIterator *iterator, bool freeData);

ListIterator *insertList(ListIterator *iterator, void *newData);

// wyjmuje elementy z sourceList i insertuje je przed iterator
void spliceList(ListIterator *iterator, List *sourceList);

void clearList(List *List, bool freeData);

void deleteList(List *List, bool freeData);

void reverseList(List *list);

#endif // LIST_H
