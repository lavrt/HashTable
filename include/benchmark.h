#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "hashTable.h"

#include <string.h>

const char* const TEXT_FILE_NAME = "./data/text.txt";
const char* const delimiters = " \t\n\r.,;:!?()\"\'‘’";

char* FillBuffer();
void FillHashTable(THashTable* ht, char* textBuffer);
void RunSearchBenchmark(THashTable* ht, char* textBuffer);

#endif // BENCHMARK_H