#ifndef HASH_FUNCTION_H
#define HASH_FUNCTION_H

#include "hashTable.h"

#include <string.h>

extern "C" unsigned djb2_asm(const char*);

unsigned djb2(const char* key);

inline unsigned Hash(const char* key) {
    return djb2(key) % kHashTableSize;
}

#endif // HASH_FUNCTION_H