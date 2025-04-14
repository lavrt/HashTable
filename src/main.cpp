#include <stdio.h>

#include "hashTable.h"

int main() {
    THashTable* ht = HT_create();
    
    int a = 54;
    int b = 48;
    HT_insert(ht, "apple", &a);
    HT_insert(ht, "orange", &b);
    
    printf("apple: %d\n", *(int*)HT_get(ht, "apple"));
    printf("orange: %d\n", *(int*)HT_get(ht, "orange"));
    
    // HT_remove(ht, "apple");
    printf("apple after remove: %s\n", HT_get(ht, "apple") ? "exists" : "not found");

    HT_dump(ht);
    
    HT_destroy(ht);
    
    return 0;
}