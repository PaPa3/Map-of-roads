#include "map.h"
#include "city.h"
#include "road.h"
#include "list.h"
#include "road.h"
#include "route.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Map {
    List *cities;
    List *routes;
} Map;

Map *newMap() {
    Map *result = malloc(sizeof(Map));
    if (result == NULL) {
        return NULL;
    }

    result->cities = newList();
    if (result->cities == NULL) {
        free(result);
        return NULL;
    }

    result->routes = newList();
    if (result->routes == NULL) {
        deleteList(result->cities, false);
        free(result);
        return NULL;
    }

    return result;
}

void deleteMap(Map *map) {
    if (map == NULL) {
        return;
    }

    ListIterator *ptr = map->cities->begin;
    while (ptr != map->cities->end) {
        deleteCity(ptr->data);
        ptr = ptr->next;
    }
    deleteList(map->cities, false);

    ptr = map->routes->begin;
    while (ptr != map->routes->end) {
        deleteRouteStruct(ptr->data);
        ptr = ptr->next;
    }
    deleteList(map->routes, false);
}

bool isValidInput(Map *map, const char *cityName1, const char *cityName2) {
    if (map == NULL) {
        return false;
    }

    if (cityName1 != NULL && !isStringValidCityName(cityName1)) {
        return false;
    }

    if (cityName2 != NULL && !isStringValidCityName(cityName2)) {
        return false;
    }

    if (cityName1 != NULL && cityName2 != NULL &&
            strcmp(cityName1, cityName2) == 0) {
        return false;
    }

    return true;
}

bool addRoad(Map *map, const char *cityName1, const char *cityName2,
             unsigned length, int builtYear) {
    isValidInput(map, cityName1, cityName2);

    City *city1 = findCityInsertIfNecessary(map->cities, cityName1);
    City *city2 = findCityInsertIfNecessary(map->cities, cityName2);
    if (city1 == NULL || city2 == NULL) {
        return false;
    }

    return addRoadModule(city1, city2, length, builtYear);
}

bool repairRoad(Map *map, const char *cityName1, const char *cityName2, int repairYear) {
    isValidInput(map, cityName1, cityName2);

    City *city1 = findCityOnList(map->cities, cityName1);
    City *city2 = findCityOnList(map->cities, cityName2);
    if (city1 == NULL || city2 == NULL) {
        return false;
    }

    return repairRoadModule(city1, city2, repairYear);
}

bool newRoute(Map *map, unsigned routeId,
              const char *cityName1, const char *cityName2) {
    isValidInput(map, cityName1, cityName2);

    City *city1 = findCityOnList(map->cities, cityName1);
    City *city2 = findCityOnList(map->cities, cityName2);
    if (city1 == NULL || city2 == NULL) {
        return false;
    }

    if (findRouteOnList(map->routes, routeId) != NULL) {
        return false;
    }

    Route *route = newRouteStruct(routeId, city1, city2, map->cities);
    if (route == NULL) {
        return false;
    }

    insertList(map->routes->end, route);
    return true;
}

bool extendRoute(Map *map, unsigned routeId, const char *cityName) {
    isValidInput(map, cityName, NULL);

    City *city = findCityOnList(map->cities, cityName);
    if (city == NULL) {
        return false;
    }

    Route *route = findRouteOnList(map->routes, routeId);
    if (route == NULL) {
        return false;
    }

    return findNewRouteAfterExtend(route, city, map->cities);
}

bool removeRoad(Map *map, const char *cityName1, const char *cityName2) {
    isValidInput(map, cityName1, cityName2);

    City *city1 = findCityOnList(map->cities, cityName1);
    City *city2 = findCityOnList(map->cities, cityName2);
    if (city1 == NULL || city2 == NULL) {
        return false;
    }

    if (!setRoadIsDeletedTo(city1, city2, true)) {
        return false;
    }

    ListIterator *iterator = map->routes->begin;
    while (iterator != map->routes->end) {
        if (!findNewRouteAfterRemovingRoad(iterator->data, city1, city2,
                                           map->cities)) {
            while (iterator != map->routes->begin) {
                iterator = iterator->previous;
                undoFindNewRouteAfterRemovingRoad(iterator->data, city1, city2);
            }
            return false;
        }
    }

    return true;
}

char const* getRouteDescription(Map *map, unsigned routeId) {
    char *emptyString = malloc(sizeof(char));
    if (emptyString == NULL) {
        return NULL;
    }
    *emptyString = 0;

    if (map == NULL) {
        return emptyString;
    }

    Route *route = findRouteOnList(map->routes, routeId);
    if (route == NULL) {
        return emptyString;
    }

    return descriptionRoute(route);
}
