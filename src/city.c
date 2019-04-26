#include "city.h"

#include <stdlib.h>

uint32_t hashString(char *string) {
    uint64_t result = 0;

    while (*string) {
        result = (result * 257 + *string) % 1000000007;
    }

    return result;
}

City *newCity(char *name) {
    City *result = malloc(sizeof(City));
    if (result == NULL) {
        return NULL;
    }

    result->roads = newList();
    if (result->roads == NULL) {
        free(result);
        return NULL;
    }

    result->name = name;
    result->hashName = hashString(name);
    result->distance = -1;

    return result;
}
