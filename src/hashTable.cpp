#include "hashTable.h"

#include <immintrin.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "hashFunction.h"

// static ------------------------------------------------------------------------------------------

static int FastStrcmp(const char* s1, const char* s2);

// global ------------------------------------------------------------------------------------------

void HT_Create(THashTable* ht) {
    for (size_t i = 0; i < kHashTableSize; i++) {
        ht->buckets[i].size = 0;
    }
}

EStatus HT_Insert(THashTable* ht, const char* key) {
    unsigned index = Hash(key);
    TBucket* bucket = &ht->buckets[index];

    for (size_t i = 0; i < bucket->size; i++) {
        if (!FastStrcmp(bucket->nodes[i].key, key)) {
            bucket->nodes[i].value++;
            return Finished;
        }
    }

    if (bucket->size >= kMaxNodesInBucket) {
        return Failed;
    }

    strncpy(bucket->nodes[bucket->size].key, key, kMaxKeyLength);
    bucket->nodes[bucket->size].value = 1;
    bucket->size++;

    return Finished;
}

int HT_Get(THashTable* ht, const char* key) {
    unsigned index = Hash(key);
    TBucket* bucket = &ht->buckets[index];

    for (size_t i = 0; i < bucket->size; i++) {
        if (!FastStrcmp(bucket->nodes[i].key, key)) {
            return bucket->nodes[i].value;
        }
    }

    return -1;
}

void HT_TextDump(THashTable* ht) {
    FILE* dumpFile = fopen(kNameOfTextDumpFile, "wb");
    assert(dumpFile);

    fprintf(dumpFile, "Map\nloadfactor: %.f\n\n", CalculateLoadFactor(ht));

    for (size_t i = 0; i < kHashTableSize; i++) {
        TBucket* bucket = &ht->buckets[i];
        if (!bucket->size) {
            continue;
        }

        fprintf(dumpFile, "Bucket %lu:\n", i);
        for (size_t j = 0; j < bucket->size; j++) {
            fprintf(dumpFile, "  '%s' -- %zu\n", bucket->nodes[j].key, bucket->nodes[j].value);
        }
    }

    fclose(dumpFile);
}

float CalculateLoadFactor(const THashTable* table) {
    size_t total_elements = 0;
    size_t non_empty_buckets = 0;

    for (size_t i = 0; i < kHashTableSize; i++) {
        if (table->buckets[i].size > 0) {
            total_elements += table->buckets[i].size;
            non_empty_buckets++;
        }
    }

    return (non_empty_buckets) ? ((float)total_elements / non_empty_buckets) : 0;
}


// static ------------------------------------------------------------------------------------------

static int FastStrcmp(const char* s1, const char* s2) {
    const __m256i vec1 = _mm256_load_si256((const __m256i*)s1);
    const __m256i vec2 = _mm256_load_si256((const __m256i*)s2);
    const __m256i res = _mm256_xor_si256(vec1, vec2);
    return !_mm256_testz_si256(res, res);
}
