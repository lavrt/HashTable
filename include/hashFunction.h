#ifndef HASH_FUNCTION_H
#define HASH_FUNCTION_H

#include "hashTable.h"

#include <string.h>

unsigned MurmurHash2(const char* key, unsigned len);

inline unsigned Hash(const char* key) {
    return MurmurHash2(key, strlen(key)) % kHashSize;
}

#endif // HASH_FUNCTION_H