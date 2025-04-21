#include "std.h"

#include <assert.h>
#include <immintrin.h>

size_t GetFileSize(FILE* fd) {
	size_t fileSize = 0;

    fseek(fd, 0, SEEK_END);
    fileSize = ftello(fd);
    fseek(fd, 0, SEEK_SET);

	return fileSize;
}

int FastStrcmp(const char* s1, const char* s2) {
    __m256i vec1 = _mm256_load_si256((__m256i*)s1);
    __m256i vec2 = _mm256_load_si256((__m256i*)s2);
    __m256i cmp = _mm256_cmpeq_epi8(vec1, vec2);
    unsigned mask = _mm256_movemask_epi8(cmp);
    return (mask == 0xFFFFFFFF) ? 0 : 1;
}