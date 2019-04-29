#include "string_builder.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <inttypes.h>
#include <string.h>

#define DEFAULT_STRING_BUILDER_MEMORY_SIZE 8
#define INTEGER_LENGTH 22

bool resizeStringBuilder(StringBuilder *ptr, uint32_t newMemorySize) {
    assert(ptr);

    if (newMemorySize < ptr->reservedMemory) {
        return true;
    }

    char *string = realloc(ptr->data, newMemorySize * sizeof(char));
    if (string == NULL) {
        return false;
    }
    ptr->data = string;

    ptr->reservedMemory = newMemorySize;

    return true;
}

StringBuilder *newStringBuilder() {
    StringBuilder *result = malloc(sizeof(StringBuilder));
    if (result == NULL) {
        return NULL;
    }

    result->size = 0;
    result->data = NULL;
    result->reservedMemory = 0;
    if (!resizeStringBuilder(result, DEFAULT_STRING_BUILDER_MEMORY_SIZE)) {
        free(result);
        return NULL;
    }

    return result;
}

void deleteStringBuilder(StringBuilder *string, bool freeData) {
    if (freeData) {
        free(string->data);
    }
    free(string);
}

bool appendStringBuilderString(StringBuilder *string, char *ptr) {
    assert(string);

    while (strlen(ptr) + 2 + string->size >= string->reservedMemory) {
        if (!resizeStringBuilder(string, string->reservedMemory * 2)) {
            return false;
        }
    }

    int x = sprintf(&string->data[string->size], "%s;", ptr);
    string->size += x;

    return true;
}

bool appendStringBuilderInteger(StringBuilder *string, int64_t integer) {
    assert(string);

    while (INTEGER_LENGTH + string->size >= string->reservedMemory) {
        if (!resizeStringBuilder(string, string->reservedMemory * 2)) {
            return false;
        }
    }

    int x = sprintf(&string->data[string->size], "%" PRId64 ";", integer);
    string->size += x;

    return true;
}
