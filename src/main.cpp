#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "hashTable.h"
#include "std.h"

const char* const TEXT_FILE_NAME = "text.txt";

int main() {
    THashTable* ht = HT_create();

    FILE* inputFile = fopen(TEXT_FILE_NAME, "rb");
    assert(inputFile);

    size_t fileSize = GetFileSize(inputFile);

    char* textBuffer = (char*)calloc(fileSize + 1, sizeof(char));
    assert(textBuffer);

    fread(textBuffer, sizeof(char), fileSize, inputFile);

    fclose(inputFile);
    
    int temp = 0;
    for (char* token = strtok(textBuffer, " "); token; token = strtok(NULL, " ")) {
        HT_insert(ht, token, &temp);
    }

    HT_dump(ht);
    HT_destroy(ht);
    
    return 0;
}