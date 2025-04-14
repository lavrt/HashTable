#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "hashTable.h"
#include "std.h"

const char* const TEXT_FILE_NAME = "./data/text.txt";
const char* delimiters = " \t\n\r.,;:!?()\"\'‘’";

int main() {
    THashTable* ht = HT_Create();

    FILE* inputFile = fopen(TEXT_FILE_NAME, "rb");
    assert(inputFile);

    size_t fileSize = GetFileSize(inputFile);

    char* textBuffer = (char*)calloc(fileSize + 1, sizeof(char));
    assert(textBuffer);

    fread(textBuffer, sizeof(char), fileSize, inputFile);

    fclose(inputFile);
    
    int temp = 0;
    for (char* token = strtok(textBuffer, delimiters); token; token = strtok(NULL, delimiters)) {
        HT_Insert(ht, token, &temp);
    }

    HT_TextDump(ht);
    HT_Destroy(ht);
    
    return 0;
}