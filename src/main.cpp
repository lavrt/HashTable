#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sched.h>

#include "hashTable.h"
#include "benchmark.h"

static int SetCpuCore(int coreIndex);

int main() {
    SetCpuCore(7); 

    THashTable* ht = HT_Create();
    
    char* textBuffer = FillBuffer();
    FillHashTable(ht, textBuffer);
    free(textBuffer);

    textBuffer = FillBuffer();
    RunSearchBenchmark(ht, textBuffer);
    free(textBuffer);

    HT_TextDump(ht);
    HT_Destroy(ht);

    return 0;
}

static int SetCpuCore(int coreIndex) {
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(coreIndex, &mask);

    return (sched_setaffinity(0, sizeof(mask), &mask) == -1) ? 0 : 1;
}