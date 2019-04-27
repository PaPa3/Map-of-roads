#ifndef CITY_H
#define CITY_H

#include "list.h"

#include <stdint.h>
#include <stdbool.h>

typedef struct City {
    char *name;
    uint32_t hashName;
    List *roads;
    int64_t distance;
    int64_t oldestRoadOnRoute;
    bool isRouteUnequivocal;
    struct City *previousOnRoute;
} City;

//uint32_t hashString(const char *string);

//City *newCity(const char *name);

void deleteCity(City *city);

bool isStringValidCityName(const char *string);

City *findCityOnList(List *listOfCities, const char *name);

City *findCityInsertIfNecessary(List *listOfCities, const char *name);

//bool isEqualCity(City *city1, City *city2);

#endif // CITY_H
