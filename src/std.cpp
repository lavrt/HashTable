#include "std.h"

#include <assert.h>

size_t GetFileSize(FILE* fd) {
	size_t fileSize = 0;

    fseek(fd, 0, SEEK_END);
    fileSize = ftello(fd);
    fseek(fd, 0, SEEK_SET);

	return fileSize;
}