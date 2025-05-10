#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "hashTable.h"

#include <string.h>

// static ------------------------------------------------------------------------------------------

static const char* const kNameOfInputFile = "./data/text.txt";
static const char* const kDelimiters = " \t\n\r.,;:!?()\"\'‘’";

// global ------------------------------------------------------------------------------------------

char* FillBuffer();
void FillHashTable(THashTable* ht, char* textBuffer);
void RunSearchBenchmark(THashTable* ht, char* textBuffer);

#endif // BENCHMARK_H