#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdlib.h>

const size_t TABLE_SIZE = 512;

struct TNode {
    char* key;
    void* value;
    TNode* next;
};

struct THashTable {
    TNode** buckets;
    size_t size;
    size_t count;
};

enum EStatus {
    Failed,
    Finished
};

THashTable* HT_create();
void HT_destroy(THashTable* ht);
void HT_insert(THashTable* ht, const char* key, void* value);
void* HT_get(THashTable* ht, const char* key);
EStatus HT_remove(THashTable* ht, const char* key);

#endif // HASH_TABLE_H