#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdlib.h>

const size_t TABLE_SIZE = 340;

const char* const NAME_OF_GRAPH_DUMP_FILE = "./dump/dump.dot";
const char* const NAME_OF_TEXT_DUMP_FILE = "./dump/dump.txt";

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

THashTable* HT_Create();
void HT_Destroy(THashTable* ht);
void HT_Insert(THashTable* ht, const char* key, void* value);
void* HT_Get(THashTable* ht, const char* key);
EStatus HT_Remove(THashTable* ht, const char* key);
void HT_GraphDump(THashTable* ht);
void HT_TextDump(THashTable* ht);

#endif // HASH_TABLE_H