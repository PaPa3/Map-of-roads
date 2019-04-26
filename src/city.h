#ifndef CITY_H
#define CITY_H

#include "list.h"

#include <stdint.h>

typedef struct City {
    char *name;
    uint32_t hashName;
    List *roads;
    int64_t distance;
} City;

uint32_t hashString(char *string);

City *newCity(char *name);

#endif // CITY_H
