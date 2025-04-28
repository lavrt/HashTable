#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdlib.h>

const char* const kNameOfGraphDumpFile = "./dump/dump.dot";
const char* const kNameOfTextDumpFile = "./dump/dump.txt";

const size_t kHashSize = 340;
const size_t kMaxNodes = 8192;
const size_t kMaxKeyLength = 32;

struct TNode {
    char key[kMaxKeyLength];
    size_t value;
    int next;
};

struct THashTable {
    TNode nodes[kMaxNodes];
    int freeHead;
    int buckets[kHashSize];
    size_t size;
};

enum EStatus {
    Failed,
    Finished
};

void HT_Create(THashTable* ht);
EStatus HT_Insert(THashTable* ht, const char* key);
size_t HT_Get(THashTable* ht, const char* key);
EStatus HT_Remove(THashTable* ht, const char* key);
void HT_GraphDump(THashTable* ht);
void HT_TextDump(THashTable* ht);

#endif // HASH_TABLE_H