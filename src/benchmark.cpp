#include "benchmark.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <immintrin.h>

// static ------------------------------------------------------------------------------------------

static volatile size_t temp = 0; // We use volatile so that the compiler does not optimize the loop.
                                 // This is important for measuring performance, because without volatile,
                                 // the compiler can delete useless code from its point of view.

static long GetFileSize(FILE* fd);

// global ------------------------------------------------------------------------------------------

char* FillBuffer() {
    FILE* inputFile = fopen(kNameOfInputFile, "rb");
    assert(inputFile);

    size_t fileSize = GetFileSize(inputFile);

    char* textBuffer = (char*)calloc(fileSize + 1, sizeof(char));
    assert(textBuffer);

    size_t readCount = fread(textBuffer, sizeof(char), fileSize, inputFile);
    if (readCount != fileSize) {
        fclose(inputFile);
        assert(0);
    }

    fclose(inputFile);

    return textBuffer;
}

void FillHashTable(THashTable* ht, char* textBuffer) { 
    for (char* token = strtok(textBuffer, kDelimiters); token;
        token = strtok(NULL, kDelimiters)) {

        alignas(kMemoryAlignment) char key[kMaxKeyLength] = {};
        strncpy(key, token, kMaxKeyLength - 1);
        key[kMaxKeyLength - 1] = '\0';
        HT_Insert(ht, key);
    }
}

void RunSearchBenchmark(THashTable* ht, char* textBuffer) {
    for (char* token = strtok(textBuffer, kDelimiters); token;
        token = strtok(NULL, kDelimiters)) {
        
        for (size_t i = 0; i < 30; i++) {
            alignas(kMemoryAlignment) char key[kMaxKeyLength] = {};
            strncpy(key, token, kMaxKeyLength - 1);
            key[kMaxKeyLength - 1] = '\0';
            temp = HT_Get(ht, key); 
        }
    }
}

// static ------------------------------------------------------------------------------------------

static long GetFileSize(FILE* fd) {
	long fileSize = 0;

    fseek(fd, 0, SEEK_END);
    fileSize = ftell(fd);
    fseek(fd, 0, SEEK_SET);

	return fileSize;
}