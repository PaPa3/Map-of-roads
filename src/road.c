#include "road.h"

#include <stdlib.h>
#include <assert.h>

Road *newRoad(City *destination, unsigned length, int builtYear) {
    assert(destination);

    Road *result = malloc(sizeof(Road));
    if (result == NULL) {
        return NULL;
    }

    result->buildYearOrLastRepairYear = builtYear;
    result->length = length;
    result->destination = destination;
    result->isDeleted = false;

    return result;
}

bool addRoadFromTo(City *from, City *to, unsigned length, int builtYear) {
    assert(from);
    assert(to);

    Road *road = newRoad(to, length, builtYear);
    if (road == NULL) {
        return false;
    }

    if (insertList(from->roads->begin, road) == NULL) {
        free(road);
        return false;
    }

    return true;
}

bool addRoadModule(City *city1, City *city2, unsigned length, int builtYear) {
    assert(city1);
    assert(city2);

    ListIterator *iterator = city1->roads->begin;
    while (iterator != city1->roads->begin) {
        if (((Road *)iterator->data)->destination == city2) {
            return false;
        }
    }

    if (!addRoadFromTo(city1, city2, length, builtYear)) {
        return false;
    }

    if (!addRoadFromTo(city2, city1, length, builtYear)) {
        eraseList(city1->roads->begin, true);
        return false;
    }

    return true;
}

Road *findRoad(City *from, City *to) {
    assert(from);
    assert(to);

    ListIterator *iterator = from->roads->begin;
    while (iterator != from->roads->end) {
        Road *road = iterator->data;
        if (road->destination == to) {
            return road;
        }
        iterator = iterator->next;
    }

    return NULL;
}

bool repairRoadModule(City *city1, City *city2, int repairYear) {
    assert(city1);
    assert(city2);

    Road *road = findRoad(city1, city2);
    if (road == NULL || road->buildYearOrLastRepairYear > repairYear) {
        return false;
    }

    road->buildYearOrLastRepairYear = repairYear;
    findRoad(city2, city1)->buildYearOrLastRepairYear = repairYear;

    return true;
}

bool setRoadIsDeletedTo(City *city1, City *city2, bool newIsDeleted) {
    assert(city1);
    assert(city2);

    Road *road = findRoad(city1, city2);
    if (road == NULL) {
        return false;
    }

    road->isDeleted = newIsDeleted;
    findRoad(city2, city1)->isDeleted = newIsDeleted;

    return true;
}
