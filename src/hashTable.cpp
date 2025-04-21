#include "hashTable.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "hashFunction.h"

THashTable* HT_Create() {
    THashTable* ht = (THashTable*)calloc(1, sizeof(THashTable));
    assert(ht);

    ht->buckets = (TNode**)calloc(TABLE_SIZE, sizeof(TNode*));
    assert(ht->buckets);

    ht->size = TABLE_SIZE;
    ht->count = 0;

    return ht;
}

void HT_Destroy(THashTable* ht) {
    for (size_t i = 0; i < ht->size; i++) {
        TNode* current = ht->buckets[i];
        while (current) {
            TNode* temp = current;
            current = current->next;
            free(temp->value);
            free(temp->key);
            free(temp);
        }
    }

    free(ht->buckets);
    free(ht);
}

void HT_Insert(THashTable* ht, const char* key) {
    unsigned index = Hash(key, ht->size);
    TNode* current = ht->buckets[index];

    while (current) {
        if (!strcmp(current->key, key)) {
            ++*(size_t*)(current->value);
            return;
        }
        current = current->next;
    }

    TNode* newNode = (TNode*)calloc(1, sizeof(TNode));
    assert(newNode);

    size_t* tempValue = (size_t*)calloc(1, sizeof(size_t));
    assert(tempValue);

    *(tempValue) = 1;

    newNode->key = strdup(key);
    newNode->value = tempValue;
    newNode->next = ht->buckets[index];
    ht->buckets[index] = newNode;
    ht->count++;
}

void* HT_Get(THashTable* ht, const char* key) {
    unsigned index = Hash(key, ht->size);
    TNode* current = ht->buckets[index];

    while (current) {
        if (!strcmp(current->key, key)) {
            return current->value;
        }
        current = current->next;
    }

    return NULL;
}

EStatus HT_Remove(THashTable* ht, const char* key) {
    unsigned index = Hash(key, ht->size);
    TNode* current = ht->buckets[index];
    TNode* prev = NULL;

    while (current) {
        if (!strcmp(current->key, key)) {
            if (!prev) {
                ht->buckets[index] = current->next;
            } else {
                prev->next = current->next;
            }

            free(current->value);
            free(current->key);
            free(current);
            ht->count--;

            return Finished;
        }

        prev = current;
        current = current->next;
    }

    return Failed;
}

void HT_GraphDump(THashTable* ht) {
    FILE* dumpFile = fopen(NAME_OF_GRAPH_DUMP_FILE, "wb");
    assert(dumpFile);

    fprintf(dumpFile,
        "digraph HashTable {\n"
        "    rankdir=\"LR\";\n" 
        "    node [shape=record, style=filled, fillcolor=lightblue];\n\n"
        "    hash_table [label=\""
    );

    for (size_t i = 0; i < ht->size; i++) {
        fprintf(dumpFile, "<%lu>%lu", i, i);
        if (i == ht->size - 1) {
            fprintf(dumpFile, "\", shape=record, fillcolor=lightgrey];\n\n");
            break;
        }
        fprintf(dumpFile, "|");
    }
    
    fprintf(dumpFile, "    node [shape=box];\n\n");

    for (size_t i = 0; i < ht->size; i++) {
        fprintf(dumpFile,
            "    subgraph cluster_%lu {\n"
            "        label=\"bucket %lu\";\n",
            i, i
        );

        TNode* current = ht->buckets[i];
        for (size_t j = 0; current; j++) {
            fprintf(dumpFile,
                "        node_%lu_%lu [label=\"\\\"%s\\\" : %lu\"];\n",
                i, j, current->key, *((size_t*)current->value)
            );
            current = current->next;
        }

        current = ht->buckets[i];
        fprintf(dumpFile, "       ");
        for (size_t j = 0; current; j++) {
            fprintf(dumpFile, " node_%lu_%lu", i, j);
            if (current->next) {
                fprintf(dumpFile, " ->");
            } else {
                fprintf(dumpFile, ";\n    }\n");
            }
            current = current->next;
        }
    }
    fprintf(dumpFile, "}\n");

    // dot -Tpng ./dump/dump.dot -o graph.png

    fclose(dumpFile);
}

void HT_TextDump(THashTable* ht) {
    FILE* dumpFile = fopen(NAME_OF_TEXT_DUMP_FILE, "wb");
    assert(dumpFile);

    fprintf(
        dumpFile,
        "hash table: [%p]\n"
        "number of buckets: %lu\n"
        "total number of items: %lu\n"
        "load factor: %.lf\n\n",
        ht, ht->size, ht->count, (double)ht->count / ht->size
    );

    for (size_t i = 0; i < ht->size; i++) {
        TNode* current = ht->buckets[i];
        fprintf(dumpFile, "[%p] ", current);

        while (current) {
            fprintf(dumpFile, "{ %s | %d } ", current->key, *((int*)current->value));
            current = current->next;
        }

        fprintf(dumpFile, "\n");
    }

    fclose(dumpFile);
}