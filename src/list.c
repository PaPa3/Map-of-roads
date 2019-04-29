/** @file
 * Implementacja interfejsu klasy przechowującej listę.
 *
 * @author Paweł Pawlik <pp406289@students.mimuw.edu.pl>
 * @date 29.04.2019
 */

#include "list.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

/** @brief Tworzy strukturę.
 * Towrzy pusty węzeł, który będzie należał do listy wskazywanej przez @p father.
 * @param[in] father                - wskażnik na listę.
 * @return Wskaźnik na utowrzoną strukturę lub NULL, jeśli nie udało się
 * zaalokować pamięci.
 */
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

/** @brief Towrzy strukturę.
 * Tworzy pustą listę.
 * @return Wskaźnik na utworzoną listę lub NULL, jeśli nie udało się zaalokować
 * pamięci.
 */
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

/** @brief Usuwa węzeł listy.
 * Usuwa węzeł listy oraz poprawia listę, do której należy węzeł tak, aby
 * dalej była poprawną listą. Jeśli @p freeData wynosi @p true to
 * wywołuje funkcję free() na danych przechowywanych przez węzeł.
 * @param[in,out] iterator          - wskaźnik na węzeł;
 * @param[in] freeData              - czy należy usunąć dane.
 * @return Wskaźnik na węzeł, który był następnikiem usuwanego węzła.
 */
ListIterator *eraseList(ListIterator *iterator, bool freeData) {
    assert(iterator);
    assert(iterator->next != iterator); // Nie można usunąć końca listy.

    if (iterator->previous == iterator) { // Jesli iterator == początek listy.
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

/** @brief Czyści listę.
 * Czyści listę (ustawię listę na pustą). Usuwa węzły list.
 * Jeśli @p freeData wynosi @p true to
 * wywołuje funkcję free() na danych przechowywanych przez węzły listy.
 * @param[in,out] list              - wskaźnik na listę;
 * @param[in] freeData              - czy należy usunąć dane.
 */
void clearList(List *list, bool freeData) {
    assert(list);

    while (list->begin != list->end) {
        eraseList(list->begin, freeData);
    }
}

/** @brief Usuwa listę.
 * Usuwa listę wraz z jej węzłami. Jeśli @p freeData wynosi @p true to
 * wywołuje funkcję free() na danych przechowywanych przez węzły listy.
 * @param[in,out] list              - wskaźnik na listę;
 * @param[in] freeData              - czy należy usunąć dane.
 */
void deleteList(List *list, bool freeData) {
    assert(list);

    clearList(list, freeData);
    free(list->end);
    free(list);
}

/** @brief Wstawia węzeł do listy.
 * Wstawie węzeł @p newIterator przed węzeł @p iterator.
 * @param[in,out] iterator          - wskaźnik na węzeł przed, który wstawić;
 * @param[in,out] newIterator       - wskażnik na wstawiany węzeł.
 */
void insertListIterator(ListIterator *iterator, ListIterator *newIterator) {
    assert(iterator);
    assert(newIterator);

    if (iterator->previous != iterator) { // Jesli iterator != początek listy.
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

/** @brief Wstawia dane do listy.
 * Wstawia dane do listy w miejsce poprzedzające dany węzeł.
 * @param[in,out] iterator          - wskaźnik na węzeł;
 * @param[in] newData               - wskaźnik na dane do wstawienia.
 * @return Wskaźnik na nowo powstały węzeł zawierający wstawiane dane lub NULL,
 * jeśli nie udało się zaalokować pamięci.
 */
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

/** @brief Przenosi listę do innej listy.
 * Przenosi listę i wstawia ją przed dany węzeł innej listy. W wyniku operacji
 * przenoszona lista staję się pustą listą.
 * @param[in,out] iterator          - wskaźnik na węzeł;
 * @param[in,out] sourceList        - wskażnik na listę do przeniesienia.
 */
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

/** @brief Zamienia miejscami dwa wskaźniki.
 * Zamienia miejscami dwa wskaźniki na @ref ListIterator.
 * @param[in,out] iterator1         - wskaźnik na pierwszy wskaźnik do zamiany;
 * @param[in,out] iterator2         - wskaźnik na drugi wskaźnik do zamiany.
 */
void swapListIteratorPointers(ListIterator **iterator1, ListIterator **iterator2) {
    assert(iterator1);
    assert(iterator2);

    ListIterator *ptr = *iterator1;
    *iterator1 = *iterator2;
    *iterator2 = ptr;
}

/** @brief Odwraca listę.
 * Odwraca kolejność elementów na liście.
 * @param[in,out] list              - wskaźnik na listę.
 */
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

/** @brief Ostatni element listy.
 * Zwraca wskaźnik na dane ostatniego węzła na liście.
 * @param[in] list                  - wskaźnik na listę.
 * @return Wskaźnik na szukane dane.
 */
void *backList(List *list) {
    assert(list);
    assert(list->begin != list->end); // Lista nie może być pusta.

    return list->end->previous->data;
}
