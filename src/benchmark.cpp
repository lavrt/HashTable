#include "benchmark.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <immintrin.h>

#include "strcpy.h"

// static ------------------------------------------------------------------------------------------

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
    for (char* token = strtok(textBuffer, kdelimiters); token;
        token = strtok(NULL, kdelimiters)) {

        alignas(kMemoryAlignment) char key[kMaxKeyLength] = {};
        strcpy(key, token);
        HT_Insert(ht, key);
    }
}

void RunSearchBenchmark(THashTable* ht, char* textBuffer) {
    size_t temp = 0;
    for (int i = 0; i < 14; i++) {
        for (char* token = strtok(textBuffer, kdelimiters); token;
            token = strtok(NULL, kdelimiters)) {

            alignas(kMemoryAlignment) char key[kMaxKeyLength] = {};
            strcpy(key, token);
            temp = HT_Get(ht, key); 
            HT_Remove(ht, key);
            HT_Insert(ht, key);
        }
        free(textBuffer);
        textBuffer = FillBuffer();
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