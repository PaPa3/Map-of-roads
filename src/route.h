#ifndef ROUTE_H
#define ROUTE_H

#include "city.h"
#include "list.h"

#include <stdbool.h>

typedef struct Route {
    unsigned routeId;
    List *cities;
    bool wasChanged;
} Route;

Route *newRouteStruct(unsigned routeId, City *city1, City *city2,
                      List *listOfCities);

void deleteRouteStruct(Route *route);

bool findNewRouteAfterRemovingRoad(Route *route, City *city1, City *city2,
                                   List *listOfCities);
void undoFindNewRouteAfterRemovingRoad(Route *route, City *city1, City *city2);

bool findNewRouteAfterExtend(Route *route, City *city, List *listOfCities);

Route *findRouteOnList(List *list, unsigned routeId);

char *descriptionRoute(Route *route);

#endif // ROUTE_H
