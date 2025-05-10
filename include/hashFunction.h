#ifndef HASH_FUNCTION_H
#define HASH_FUNCTION_H

#include "hashTable.h"

#include <string.h>
#include <stdint.h>

uint32_t crc32(const char* key, size_t length);

inline unsigned Hash(const char* key) {
    return crc32(key, strlen(key)) % kHashTableSize;
}

#endif // HASH_FUNCTION_H