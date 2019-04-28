#include "city.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <string.h>

uint32_t hashString(const char *string) {
    uint64_t result = 0;

    while (*string) {
        result = (result * 257 + *string) % 1000000007;
        string++;
    }

    return result;
}

City *newCity(const char *name) {
    char *nameCopy = malloc(sizeof(char) * (strlen(name) + 1));
    if (nameCopy == NULL) {
        return NULL;
    }
    strcpy(nameCopy, name);

    City *result = malloc(sizeof(City));
    if (result == NULL) {
        free(nameCopy);
        return NULL;
    }

    result->roads = newList();
    if (result->roads == NULL) {
        free(nameCopy);
        free(result);
        return NULL;
    }

    result->name = nameCopy;
    result->hashName = hashString(nameCopy);
    result->distance = -1;

    return result;
}

void deleteCity(City *city) {
    assert(city);

    free(city->name);
    deleteList(city->roads, true);
    free(city);
}


bool isStringValidCityName(const char *string) {
    if (string == NULL || *string == 0) {
        return false;
    }

    while (*string) {
        if ((0 <= *string && *string <= 31) || *string == ';') {
            return false;
        }
        string++;
    }

    return true;
}

City *findCityOnList(List *listOfCities, const char *name) {
    assert(listOfCities);

    uint32_t hashName = hashString(name);
    ListIterator *iterator = listOfCities->begin;
    City *city;

    while (iterator != listOfCities->end) {
        city = iterator->data;
        if (city->hashName == hashName && strcmp(city->name, name) == 0) {
            break;
        }

        iterator = iterator->next;
    }

    if (iterator == listOfCities->end) {
        return NULL;
    }

    return iterator->data;
}

City *findCityInsertIfNecessary(List *listOfCities, const char *name) {
    assert(listOfCities);

    City *result = findCityOnList(listOfCities, name);
    if (result == NULL) {
        City *city = newCity(name);
        if (city == NULL) {
            return NULL;
        }

        ListIterator *iterator = insertList(listOfCities->end, city);
        if (iterator == NULL) {
            deleteCity(city);
            return NULL;
        }

        return listOfCities->end->previous->data;
    }

    return result;
}
