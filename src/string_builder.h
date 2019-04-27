#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

#include <stdint.h>
#include <stdbool.h>

typedef struct StringBuilder{
    char *data;
    uint32_t size;
    uint32_t reservedMemory;
} StringBuilder;

StringBuilder *newStringBuilder();

void deleteStringBuilder(StringBuilder *string, bool freeData);

bool appendStringBuilderString(StringBuilder *string, char *ptr);

bool appendStringBuilderInteger(StringBuilder *string, int64_t integer);

#endif // STRING_BUILDER_H
