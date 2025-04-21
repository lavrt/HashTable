#include "benchmark.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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
        HT_Insert(ht, token);
    }
}

void RunSearchBenchmark(THashTable* ht, char* textBuffer) {
    size_t temp = 0;
    for (char* token = strtok(textBuffer, delimiters); token; token = strtok(NULL, delimiters)) {
        temp = *(size_t*)HT_Get(ht, token);
        // printf("%lu ", temp);
    }
}