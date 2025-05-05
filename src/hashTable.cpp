#include "hashTable.h"

#include <immintrin.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "hashFunction.h"
#include "strcpy.h"

// static ------------------------------------------------------------------------------------------

static int FastStrcmp(const char* s1, const char* s2);
static int AllocateNode(THashTable* ht);
static void FreeNode(THashTable* ht, int index);
static size_t CountUsedBuckets(const THashTable* ht);

// global ------------------------------------------------------------------------------------------

void HT_Create(THashTable* ht) {
    ht->freeHead = 0;
    ht->size = 0;

    for (size_t i = 0; i < kHashSize; i++) {
        ht->buckets[i] = -1;
    }

    for (size_t i = 0; i < kMaxNodes - 1; i++) {
        ht->nodes[i].next = i + 1;
    }
    ht->nodes[kMaxNodes - 1].next = -1;
}

EStatus HT_Insert(THashTable* ht, const char* key) {
    unsigned hashValue = Hash(key);
    int current = ht->buckets[hashValue];

    while (current != -1) {
        if (!FastStrcmp(ht->nodes[current].key, key)) {
            ht->nodes[current].value++;
            return Finished;
        }
        current = ht->nodes[current].next;
    }

    int index = AllocateNode(ht);
    if (index == -1) {
        return Failed;
    }

    strcpy(ht->nodes[index].key, key);
    ht->nodes[index].value = 1;
    ht->nodes[index].next = ht->buckets[hashValue];
    ht->buckets[hashValue] = index;
    ht->size++;

    return Finished;
}

size_t HT_Get(THashTable* ht, const char* key) {
    unsigned hashValue = Hash(key);
    int current = ht->buckets[hashValue];

    while (current != -1) {
        if (!FastStrcmp(ht->nodes[current].key, key)) {
            return ht->nodes[current].value;
        }
        current = ht->nodes[current].next;
    }

    return 0;
}

EStatus HT_Remove(THashTable* ht, const char* key) {
    unsigned hashValue = Hash(key);
    int current = ht->buckets[hashValue];
    int prev = -1;

    while (current != -1) {
        if (FastStrcmp(ht->nodes[current].key, key)) {
            prev = current;
            current = ht->nodes[current].next;
            continue;
        }

        if (prev == -1) {
            ht->buckets[hashValue] = ht->nodes[current].next;
        } else {
            ht->nodes[prev].next = ht->nodes[current].next;
        }

        FreeNode(ht, current);
        ht->size--;
        return Finished;
    }

    return Failed;
}

// void HT_GraphDump(THashTable* ht) {
//     FILE* dumpFile = fopen(kNameOfGraphDumpFile, "wb");
//     assert(dumpFile);

//     fprintf(dumpFile,
//         "digraph HashTable {\n"
//         "    rankdir=\"LR\";\n" 
//         "    node [shape=record, style=filled, fillcolor=lightblue];\n\n"
//         "    hash_table [label=\""
//     );

//     for (size_t i = 0; i < ht->size; i++) {
//         fprintf(dumpFile, "<%lu>%lu", i, i);
//         if (i == ht->size - 1) {
//             fprintf(dumpFile, "\", shape=record, fillcolor=lightgrey];\n\n");
//             break;
//         }
//         fprintf(dumpFile, "|");
//     }
    
//     fprintf(dumpFile, "    node [shape=box];\n\n");

//     for (size_t i = 0; i < ht->size; i++) {
//         fprintf(dumpFile,
//             "    subgraph cluster_%lu {\n"
//             "        label=\"bucket %lu\";\n",
//             i, i
//         );

//         TNode* current = ht->buckets[i];
//         for (size_t j = 0; current; j++) {
//             fprintf(dumpFile,
//                 "        node_%lu_%lu [label=\"\\\"%s\\\" : %lu\"];\n",
//                 i, j, current->key, *((size_t*)current->value)
//             );
//             current = current->next;
//         }

//         current = ht->buckets[i];
//         fprintf(dumpFile, "       ");
//         for (size_t j = 0; current; j++) {
//             fprintf(dumpFile, " node_%lu_%lu", i, j);
//             if (current->next) {
//                 fprintf(dumpFile, " ->");
//             } else {
//                 fprintf(dumpFile, ";\n    }\n");
//             }
//             current = current->next;
//         }
//     }
//     fprintf(dumpFile, "}\n");

//     // dot -Tpng ./dump/dump.dot -o graph.png

//     fclose(dumpFile);
// }

void HT_TextDump(THashTable* ht) {
    FILE* dumpFile = fopen(kNameOfTextDumpFile, "wb");
    assert(dumpFile);

    fprintf(
        dumpFile,
        "hash table: [%p]\n"
        "number of buckets: %lu/%lu\n"
        "total number of items: %lu/%lu\n"
        "load factor: %.lf\n\n",
        ht, CountUsedBuckets(ht), kHashSize, ht->size, kMaxNodes, (double)ht->size / CountUsedBuckets(ht)
    );

    for (size_t i = 0; i < ht->size; i++) {
        int current = ht->buckets[i];
        if (current == ht->freeHead) {
            fclose(dumpFile);
            return;
        }

        fprintf(dumpFile, "[%p] ", ht->nodes + current);
        while (current != -1) {
            fprintf(dumpFile, "{ %s | %d } ", ht->nodes[current].key, ht->nodes[current].value);
            current = ht->nodes[current].next;
        }
        fprintf(dumpFile, "\n");
    }
}

// static ------------------------------------------------------------------------------------------

static int FastStrcmp(const char* s1, const char* s2) {
    const __m256i vec1 = _mm256_load_si256((const __m256i*)s1);
    const __m256i vec2 = _mm256_load_si256((const __m256i*)s2);
    const __m256i res = _mm256_xor_si256(vec1, vec2);
    return !_mm256_testz_si256(res, res);
}

static int AllocateNode(THashTable* ht) {
    if (ht->freeHead == -1) {
        return -1;
    }
    int index = ht->freeHead;
    ht->freeHead = ht->nodes[index].next;
    return index;
}

static void FreeNode(THashTable* ht, int index) {
    ht->nodes[index].next = ht->freeHead;
    ht->freeHead = index;
}

static size_t CountUsedBuckets(const THashTable* ht) {
    size_t used = 0;
    for (size_t i = 0; i < kHashSize; i++) {
        used += ht->buckets[i] != -1;
    }
    return used;
}