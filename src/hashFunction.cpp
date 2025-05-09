#include "hashFunction.h"

unsigned djb2(const char* key) {
    unsigned hash = 0;
    while (*key) {
        hash = (hash << 5) + hash + *key++;  
    }
    return hash;
}