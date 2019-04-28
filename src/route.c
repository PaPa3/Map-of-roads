#include "route.h"
#include "heap.h"
#include "road.h"
#include "string_builder.h"

#include <stdlib.h>
#include <assert.h>

#define INFINITY 1000000000000000ll // zmiejszyłem, może lepiej zwiększyć z powrotem

void deleteRouteStruct(Route *route) {
    assert(route);

    deleteList(route->cities, false);
    free(route);
}

int64_t compareRoutes(int64_t distance1, int64_t oldestRoute1,
                  int64_t distance2, int64_t oldestRoute2) {
    if (distance1 == distance2) {
        return oldestRoute2 - oldestRoute1;
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

bool dijkstraRoute(Route *route, City *from, City *to, City *to2,
                   List *listOfCities, int64_t knownOldestRoad) {
    assert(route);
    assert(from);
    assert(to);
    assert(listOfCities);

    Heap *heap = newHeap();
    if (heap == NULL) {
        return false;
    }

    ListIterator *iterator = listOfCities->begin;
    while (iterator != listOfCities->end) {
        ((City *)iterator->data)->distance = INFINITY;
        ((City *)iterator->data)->oldestRoadOnRoute = INFINITY;
        iterator = iterator->next;
    }

    if (route->cities != NULL) {
        iterator = route->cities->begin;
        while (iterator != route->cities->end) {
            ((City *)iterator->data)->distance = -1;
            iterator = iterator->next;
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
        deleteHeap(heap, false);
        return false;
    }

    City *ptr;
    while (heap->size != 0) {
        //debugHeap(heap);

        ptr = heap->data[1];

        if (heap->keys[1]->distance != ptr->distance) {
            popHeap(heap, false);
            continue;
        }

        if (knownOldestRoad == 0 &&
                heap->keys[1]->oldestRoad != ptr->oldestRoadOnRoute) {
            popHeap(heap, false);
            continue;
        }

        popHeap(heap, false);

        iterator = ptr->roads->begin;
        Road *road;
        while (iterator != ptr->roads->end) {
            road = iterator->data;

            if (road->isDeleted) {
                iterator = iterator->next;
                continue;
            }

            if (knownOldestRoad != 0 &&
                    knownOldestRoad > road->buildYearOrLastRepairYear) {
                iterator = iterator->next;
                continue;
            }

            int64_t distance = ptr->distance + road->length;
            int64_t oldestRoute = minInt64_t(ptr->oldestRoadOnRoute,
                                             road->buildYearOrLastRepairYear);
            int64_t c = compareRoutes(distance, oldestRoute,
                                  road->destination->distance,
                                  road->destination->oldestRoadOnRoute);

            if (knownOldestRoad != 0) {
                c = distance - road->destination->distance;
            }

            if (c < 0) {
                road->destination->distance = distance;
                road->destination->oldestRoadOnRoute = oldestRoute;
                road->destination->isRouteUnequivocal = ptr->isRouteUnequivocal;
                road->destination->previousOnRoute = ptr;

                if (!pushHeap(heap, distance, oldestRoute, road->destination)) {
                    deleteHeap(heap, false);
                    return false;
                }
            } else if (c == 0) {
                road->destination->isRouteUnequivocal = false;
            }

            iterator = iterator->next;
        }
    }

    deleteHeap(heap, false);

    return true;
}

List *findRoute(Route *route, City *from, City *to, City *to2, List *listOfCities) {
    assert(route);
    assert(from);
    assert(to);
    assert(listOfCities);

    if (!dijkstraRoute(route, from, to, to2, listOfCities, 0)) {
        return NULL;
    }

    int64_t knownOldestRoad = to->oldestRoadOnRoute;
    if (to2 != NULL) {
        if (to->distance == to2->distance) {
            knownOldestRoad = -minInt64_t(-knownOldestRoad, -to2->oldestRoadOnRoute); //max
        } else if (to->distance > to2->distance) {
            knownOldestRoad = to2->oldestRoadOnRoute;
        }
    }
    if (knownOldestRoad == INFINITY) {
        return NULL;
    }
    if (!dijkstraRoute(route, from, to, to2, listOfCities, knownOldestRoad)) {
        return NULL;
    }

    if (to2 != NULL) {
        int64_t c = compareRoutes(to->distance, to->oldestRoadOnRoute,
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
    City *ptr = to;
    while (true) {
        if (insertList(result->begin, ptr) == NULL) {
            deleteList(result, false);
            return NULL;
        }

        if (ptr == from) {
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
    result->cities = NULL;
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

    route->wasChanged = false;

    ListIterator *iterator = route->cities->begin;
    while (iterator != route->cities->end &&
           iterator->data != city1 && iterator->data != city2) {
        iterator = iterator->next;
    }

    if (iterator == route->cities->end || iterator->next == route->cities->end) {
        return true;
    }

    if (iterator->next->data != city1 && iterator->next->data != city2) {
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

    route->wasChanged = true;
    return true;
}

void undoFindNewRouteAfterRemovingRoad(Route *route, City *city1, City *city2) {
    assert(route);
    assert(city1);
    assert(city2);

    if (!route->wasChanged) {
        return;
    }

    ListIterator *iterator = route->cities->begin;
    while (iterator != route->cities->end &&
           iterator->data != city1 && iterator->data != city2) {
        iterator = iterator->next;
    }

    if (iterator == route->cities->end) {//TODO chyba do usuniecia to
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

    if (list->end->previous->data == route->cities->begin->data) {
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
        iterator = iterator->next;
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
        Road *road = findRoad(iterator->data, iterator->next->data)->data;

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
