#include "benchmark.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <immintrin.h>

// static ------------------------------------------------------------------------------------------

static const size_t kMemoryAlignment = 32;

static size_t GetFileSize(FILE* fd);

// global ------------------------------------------------------------------------------------------

char* FillBuffer() {
    FILE* inputFile = fopen(kNameOfInputFile, "rb");
    assert(inputFile);

    size_t fileSize = GetFileSize(inputFile);

    char* textBuffer = (char*)calloc(fileSize + 1, sizeof(char));
    assert(textBuffer);

    fread(textBuffer, sizeof(char), fileSize, inputFile);

    fclose(inputFile);

    return textBuffer;
}

void FillHashTable(THashTable* ht, char* textBuffer) {
    for (char* token = strtok(textBuffer, kdelimiters); token; token = strtok(NULL, kdelimiters)) {
        char* key = (char*)aligned_alloc(kMemoryAlignment, sizeof(char) * kMaxKeyLength); 
        assert(key);
        strcpy(key, token);
        HT_Insert(ht, key);
    }
}

void RunSearchBenchmark(THashTable* ht, char* textBuffer) {
    size_t temp = 0;
    for (char* token = strtok(textBuffer, kdelimiters); token; token = strtok(NULL, kdelimiters)) {
        char* key = (char*)aligned_alloc(kMemoryAlignment, sizeof(char) * kMaxKeyLength); 
        assert(key);
        strcpy(key, token);
        temp = *(size_t*)HT_Get(ht, key);
        // printf("%lu ", temp);
    }
}

// static ------------------------------------------------------------------------------------------

static size_t GetFileSize(FILE* fd) {
	size_t fileSize = 0;

    fseek(fd, 0, SEEK_END);
    fileSize = ftello(fd);
    fseek(fd, 0, SEEK_SET);

	return fileSize;
}