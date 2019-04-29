#ifndef ROAD_H
#define ROAD_H

#include "city.h"

#include <stdbool.h>

typedef struct Road {
    int buildYearOrLastRepairYear;
    unsigned length;
    City *destination;
    bool isDeleted;
} Road;

bool addRoadModule(City *city1, City *city2, unsigned length, int builtYear);

bool repairRoadModule(City *city1, City *city2, int repairYear);

bool setRoadIsDeletedTo(City *city1, City *city2, bool newIsDeleted);

ListIterator *findRoad(City *from, City *to);

void removeRoadModule(City *city1, City *city2);

#endif // ROAD_H
