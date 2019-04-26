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

void clearList(List * List, bool freeData);

void deleteList(List * List, bool freeData);

#endif // LIST_H
