#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdlib.h>

// static ------------------------------------------------------------------------------------------

static const char* const kNameOfGraphDumpFile = "./dump/dump.dot";
static const char* const kNameOfTextDumpFile = "./dump/dump.txt";

static const size_t kHashTableSize = 340;
static const size_t kMaxNodesInBucket = 100;
static const size_t kMaxKeyLength = 32;
static const size_t kMemoryAlignment = 32;

// global ------------------------------------------------------------------------------------------

struct TNode {
    alignas(kMemoryAlignment) char key[kMaxKeyLength];
    size_t value;
};

struct TBucket {
    TNode nodes[kMaxNodesInBucket];
    size_t size;
};

struct THashTable {
    TBucket buckets[kHashTableSize];
};

enum EStatus {
    Failed,
    Finished
};

void HT_Create(THashTable* ht);
EStatus HT_Insert(THashTable* ht, const char* key);
int HT_Get(THashTable* ht, const char* key);
void HT_TextDump(THashTable* ht);
float CalculateLoadFactor(const THashTable* table);

#endif // HASH_TABLE_H