#include "route.h"
#include "heap.h"
#include "road.h"
#include "string_builder.h"

#include <stdlib.h>
#include <assert.h>

#define INFINITY 1000000000000000000ll

void deleteRouteStruct(Route *route) {
    assert(route);

    deleteList(route->cities, false);
    free(route);
}

int compareRoutes(int64_t distance1, int64_t oldestRoute1,
                  int64_t distance2, int64_t oldestRoute2) {
    if (distance1 == distance2) {
        return oldestRoute1 - oldestRoute2;
    }

    return distance1 - distance2;
}

int64_t minInt64_t(int64_t x, int64_t y) {
    if (x < y) {
        return x;
    } else {
        return y;
    }
}

List *findRoute(Route *route, City *from, City *to, City *to2, List *listOfCities) {
    assert(route);
    assert(from);
    assert(to);
    assert(listOfCities);

    Heap *heap = newHeap();
    if (heap == NULL) {
        return NULL;
    }

    ListIterator *iterator = listOfCities->begin;
    while (iterator != listOfCities->end) {
        ((City *)iterator->data)->distance = INFINITY;
    }

    if (route->cities != NULL) {
        iterator = route->cities->begin;
        while (iterator != route->cities->end) {
            ((City *)iterator->data)->distance = -1;
        }
    }

    from->distance = 0;
    from->oldestRoadOnRoute = INFINITY;
    from->isRouteUnequivocal = true;
    to->distance = INFINITY;
    if (to2 != NULL) {
        to2->distance = INFINITY;
    }
    if (!pushHeap(heap, 0, INFINITY, from)) {
        return NULL;
    }

    City *ptr;
    while (heap->size != 0) {
        ptr = popHeap(heap, false);

        iterator = ptr->roads->begin;
        Road *road;
        while (iterator != ptr->roads->end) {
            road = iterator->data;

            if (road->isDeleted) {
                continue;
            }

            int64_t distance = ptr->distance + road->length;
            int64_t oldestRoute = minInt64_t(ptr->oldestRoadOnRoute,
                                             road->buildYearOrLastRepairYear);
            int c = compareRoutes(distance, oldestRoute,
                                  road->destination->distance,
                                  road->destination->oldestRoadOnRoute);

            if (c <= 0) {
                road->destination->distance = distance;
                road->destination->isRouteUnequivocal = oldestRoute;
                road->destination->isRouteUnequivocal = ptr->isRouteUnequivocal;
                road->destination->previousOnRoute = ptr;
                if (c == 0) {
                    road->destination->isRouteUnequivocal = false;
                }

                if (!pushHeap(heap, distance, oldestRoute, road->destination)) {
                    deleteHeap(heap, false);
                    return NULL;
                }
            }
        }
    }

    deleteHeap(heap, false);
    if (to2 != NULL) {
        int c = compareRoutes(to->distance, to->oldestRoadOnRoute,
                              to2->distance, to2->oldestRoadOnRoute);
        if (c == 0) {
            return NULL;
        } else if (c > 0) {
            to = to2;
        }
    }
    if (to->distance == INFINITY || !to->isRouteUnequivocal) {
        return NULL;
    }

    List *result = newList();
    ptr = to;
    while (true) {
        if (insertList(result->begin, ptr) == NULL) {
            deleteList(result, false);
            return NULL;
        }

        if (ptr->previousOnRoute == from) {
            break;
        }

        ptr = ptr->previousOnRoute;
    }

    return result;
}

Route *newRouteStruct(unsigned routeId, City *city1, City *city2,
                      List *listOfCities) {
    Route *result = malloc(sizeof(Route));
    if (result == NULL) {
        return NULL;
    }

    result->routeId = routeId;
    result->cities = findRoute(result, city1, city2, NULL, listOfCities);
    if (result->cities == NULL) {
        free(result);
        return NULL;
    }

    return result;
}

bool findNewRouteAfterRemovingRoad(Route *route, City *city1, City *city2,
                                   List *listOfCities) {
    assert(route);
    assert(city1);
    assert(city2);
    assert(listOfCities);

    ListIterator *iterator = route->cities->begin;
    while (iterator != route->cities->end &&
           iterator->data != city1 && iterator->data != city2) {
        iterator = iterator->next;
    }

    if (iterator == route->cities->end) {
        return true;
    }

    List *list;
    if (iterator->data == city1) {
        list = findRoute(route, city1, city2, NULL, listOfCities);
    } else {
        list = findRoute(route, city2, city1, NULL, listOfCities);
    }

    if (list == NULL) {
        return false;
    }

    eraseList(list->begin, false);
    eraseList(list->end->previous, false);
    spliceList(iterator->next, list);
    deleteList(list, false);

    return true;
}

void undoFindNewRouteAfterRemovingRoad(Route *route, City *city1, City *city2) {
    assert(route);
    assert(city1);
    assert(city2);

    ListIterator *iterator = route->cities->begin;
    while (iterator != route->cities->end &&
           iterator->data != city1 && iterator->data != city2) {
        iterator = iterator->next;
    }

    if (iterator == route->cities->end) {
        return;
    }

    iterator = iterator->next;
    while (iterator->data != city1 && iterator->data != city2) {
        iterator = iterator->next;
        eraseList(iterator->previous, false);
    }
}

bool findNewRouteAfterExtend(Route *route, City *city, List *listOfCities) {
    assert(route);
    assert(city);
    assert(listOfCities);

    List *list = findRoute(route, city, route->cities->begin->data,
                           route->cities->end->previous->data, listOfCities);
    if (list == NULL) {
        return false;
    }

    if (list->end->previous == route->cities->begin) {
        eraseList(list->end->previous, false);
        spliceList(route->cities->begin, list);
        deleteList(list, false);
    } else {
        eraseList(list->end->previous, false);
        reverseList(list);
        spliceList(route->cities->end, list);
        deleteList(list, false);
    }

    return true;
}

Route *findRouteOnList(List *list, unsigned routeId) {
    assert(list);

    ListIterator *iterator = list->begin;
    while (iterator != list->end) {
        if (((Route *)iterator->data)->routeId == routeId) {
            return iterator->data;
        }
    }

    return NULL;
}

char *descriptionRoute(Route *route) {
    assert(route);

    StringBuilder *result = newStringBuilder();
    if (result == NULL) {
        return NULL;
    }

    if (!appendStringBuilderInteger(result, route->routeId)) {
        deleteStringBuilder(result, true);
        return NULL;
    }

    ListIterator *iterator = route->cities->begin;
    City *city = iterator->data;
    if (!appendStringBuilderString(result, city->name)) {
        deleteStringBuilder(result, true);
        return NULL;
    }

    while (iterator != route->cities->end->previous) {
        Road *road = findRoad(iterator->data, iterator->next->data);

        if (!appendStringBuilderInteger(result, road->length)) {
            deleteStringBuilder(result, true);
            return NULL;
        }

        if (!appendStringBuilderInteger(result, road->buildYearOrLastRepairYear)) {
            deleteStringBuilder(result, true);
            return NULL;
        }

        iterator = iterator->next;
        city = iterator->data;

        if (!appendStringBuilderString(result, city->name)) {
            deleteStringBuilder(result, true);
            return NULL;
        }
    }

    result->data[result->size - 1] = 0;
    char *ptr = result->data;
    deleteStringBuilder(result, false);

    return ptr;
}
