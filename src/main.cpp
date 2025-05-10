#include <sched.h>
#include <assert.h>

#include "hashTable.h"
#include "benchmark.h"

// static ------------------------------------------------------------------------------------------

static int SetCpuCore(int coreIndex);

// global ------------------------------------------------------------------------------------------

int main() {
    SetCpuCore(7); 

    THashTable* ht = (THashTable*)calloc(1, sizeof(THashTable));
    assert(ht);
    HT_Create(ht);

    char* textBuffer = FillBuffer();
    FillHashTable(ht, textBuffer);
    free(textBuffer);
    textBuffer = FillBuffer();
    RunSearchBenchmark(ht, textBuffer);
    free(textBuffer);

    HT_TextDump(ht);

    return 0;
}

// static ------------------------------------------------------------------------------------------

static int SetCpuCore(int coreIndex) {
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(coreIndex, &mask);

    return sched_setaffinity(0, sizeof(mask), &mask) != -1;
}