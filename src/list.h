/** @file
 * Interfejs klasy przechowującej listę.
 *
 * @author Paweł Pawlik <pp406289@students.mimuw.edu.pl>
 * @date 29.04.2019
 */

#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stdint.h>

typedef struct List List;

/**
 * Struktura przechowująca iterator listy będąca jednocześnie węzłem listy.
 */
typedef struct ListIterator {
    struct ListIterator *next;      ///< wskaźnik na kolejny węzeł listy
    struct ListIterator *previous;  ///< wskaźnik na poprzedni węzeł listy
    List *father;                   ///< wskaźnik na listę do której należy węzeł
    void *data;                     ///< wskaźnik na dane przechowywane przez węzeł
} ListIterator;

/**
 * Struktura przechowująca iterator listę.
 * Lista jest reprezentowana przez pierwszy i ostatni węzeł. Ostatni węzeł
 * jest strażnikiem (nie przechowuję żadnych danych). Dla pustej listy początek
 * jest równy końcowi.
 */
typedef struct List {
    ListIterator *begin;            ///< wskaźnik na początek listy
    ListIterator *end;              ///< wskaźnik na koniec listy
} List;

/** @brief Tworzy strukturę.
 * Tworzy pustą listę.
 * @return Wskaźnik na utworzoną listę lub NULL, jeśli nie udało się zaalokować
 * pamięci.
 */
List *newList();

/** @brief Czyści listę.
 * Czyści listę (ustawię listę na pustą). Usuwa węzły list.
 * Jeśli @p freeData wynosi @p true to
 * wywołuje funkcję free() na danych przechowywanych przez węzły listy.
 * @param[in,out] list              - wskaźnik na listę;
 * @param[in] freeData              - czy należy usunąć dane.
 */
void clearList(List *list, bool freeData);

/** @brief Usuwa listę.
 * Usuwa listę wraz z jej węzłami. Jeśli @p freeData wynosi @p true to
 * wywołuje funkcję free() na danych przechowywanych przez węzły listy.
 * @param[in,out] list              - wskaźnik na listę;
 * @param[in] freeData              - czy należy usunąć dane.
 */
void deleteList(List *list, bool freeData);

/** @brief Usuwa węzeł listy.
 * Usuwa węzeł listy oraz poprawia listę, do której należy węzeł tak, aby
 * dalej była poprawną listą. Jeśli @p freeData wynosi @p true to
 * wywołuje funkcję free() na danych przechowywanych przez węzeł.
 * @param[in,out] iterator          - wskaźnik na węzeł;
 * @param[in] freeData              - czy należy usunąć dane.
 * @return Wskaźnik na węzeł, który był następnikiem usuwanego węzła.
 */
ListIterator *eraseList(ListIterator *iterator, bool freeData);

/** @brief Wstawia dane do listy.
 * Wstawia dane do listy w miejsce poprzedzające dany węzeł.
 * @param[in,out] iterator          - wskaźnik na węzeł;
 * @param[in] newData               - wskaźnik na dane do wstawienia.
 * @return Wskaźnik na nowo powstały węzeł zawierający wstawiane dane lub NULL,
 * jeśli nie udało się zaalokować pamięci.
 */
ListIterator *insertList(ListIterator *iterator, void *newData);

/** @brief Przenosi listę do innej listy.
 * Przenosi listę i wstawia ją przed dany węzeł innej listy. W wyniku operacji
 * przenoszona lista staję się pustą listą.
 * @param[in,out] iterator          - wskaźnik na węzeł;
 * @param[in,out] sourceList        - wskaźnik na listę do przeniesienia.
 */
void spliceList(ListIterator *iterator, List *sourceList);

/** @brief Odwraca listę.
 * Odwraca kolejność elementów na liście.
 * @param[in,out] list              - wskaźnik na listę.
 */
void reverseList(List *list);

/** @brief Ostatni element listy.
 * Zwraca wskaźnik na dane ostatniego węzła na liście.
 * @param[in] list                  - wskaźnik na listę.
 * @return Wskaźnik na szukane dane.
 */
void *backList(List *list);

/** @brief Rzomiar listy.
 * Zwraca rozmiar (ilość węzłów przechowywujących dane) listy.
 * @param list                      - wskaźnik na listę.
 * @return Rozmiar listy.
 */
uint32_t sizeList(List *list);

#endif // LIST_H
