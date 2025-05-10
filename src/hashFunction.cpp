#include "hashFunction.h"

uint32_t crc32(const char* key, size_t length) {
    uint32_t hash = 0x0BA0BAB0;
    size_t i = 0;

    for (; i + 2 <= length; i += 2) {
        uint16_t word = *((const uint16_t*)(key + i)); 
        
        asm volatile (
            "crc32w %1, %0"
            : "+r" (hash)
            : "rm" (word)
        );
    }

    if (i < length) {
        asm volatile (
            "crc32b %1, %0"
            : "+r" (hash)
            : "rm" (key[i])
        );
    }

    return hash;
}
