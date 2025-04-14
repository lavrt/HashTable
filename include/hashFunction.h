#ifndef HASH_FUNCTION_H
#define HASH_FUNCTION_H

#include "hashTable.h"

#include <string.h>

unsigned MurmurHash2(const char* key, unsigned len);

inline unsigned Hash(const char* key, size_t tableSize) {
    return MurmurHash2(key, strlen(key)) % tableSize;
}

#endif // HASH_FUNCTION_H