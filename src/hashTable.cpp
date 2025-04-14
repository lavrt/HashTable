#include "hashTable.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "hashFunction.h"

THashTable* HT_create() {
    THashTable* ht = (THashTable*)calloc(1, sizeof(THashTable));
    assert(ht);

    ht->buckets = (TNode**)calloc(TABLE_SIZE, sizeof(TNode*));
    assert(ht->buckets);

    ht->size = TABLE_SIZE;
    ht->count = 0;

    return ht;
}

void HT_destroy(THashTable* ht) {
    for (size_t i = 0; i < ht->size; i++) {
        TNode* current = ht->buckets[i];
        while (current) {
            TNode* temp = current;
            current = current->next;
            free(temp->key);
            free(temp);
        }
    }

    free(ht->buckets);
    free(ht);
}

void HT_insert(THashTable* ht, const char* key, void* value) {
    unsigned index = Hash(key, ht->size);
    TNode* current = ht->buckets[index];

    while (current) {
        if (!strcmp(current->key, key)) {
            current->value = value;
            return;
        }
        current = current->next;
    }

    TNode* newNode = (TNode*)calloc(1, sizeof(TNode));
    assert(newNode);

    newNode->key = strdup(key);
    newNode->value = value;
    newNode->next = ht->buckets[index];
    ht->buckets[index] = newNode;
    ht->count++;
}

void* HT_get(THashTable* ht, const char* key) {
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

EStatus HT_remove(THashTable* ht, const char* key) {
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

void HT_dump(THashTable* ht) {
    FILE* dumpFile = fopen(DUMP_FILE_NAME, "wb");
    assert(dumpFile);

    fprintf(
        dumpFile,
        "hash table: [%p]\n"
        "number of buckets: %lu\n"
        "total number of items: %lu\n\n",
        ht, ht->size, ht->count
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