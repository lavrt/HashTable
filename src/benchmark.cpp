#include "benchmark.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <immintrin.h>

#include "std.h"

char* FillBuffer() {
    FILE* inputFile = fopen(TEXT_FILE_NAME, "rb");
    assert(inputFile);

    size_t fileSize = GetFileSize(inputFile);

    char* textBuffer = (char*)calloc(fileSize + 1, sizeof(char));
    assert(textBuffer);

    fread(textBuffer, sizeof(char), fileSize, inputFile);

    fclose(inputFile);

    return textBuffer;
}

void FillHashTable(THashTable* ht, char* textBuffer) {
    for (char* token = strtok(textBuffer, delimiters); token; token = strtok(NULL, delimiters)) {
        char* key = (char*)aligned_alloc(32, sizeof(char) * 32); // NOTE
        assert(key);
        strcpy(key, token);
        HT_Insert(ht, key);
    }
}

void RunSearchBenchmark(THashTable* ht, char* textBuffer) {
    size_t temp = 0;
    for (char* token = strtok(textBuffer, delimiters); token; token = strtok(NULL, delimiters)) {
        char* key = (char*)aligned_alloc(32, sizeof(char) * 32); // NOTE
        assert(key);
        strcpy(key, token);
        temp = *(size_t*)HT_Get(ht, key);
        // printf("%lu ", temp);
    }
}