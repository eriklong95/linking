#include <stdlib.h>

#define malloc(size) cmalloc(size)
#define free(ptr) cfree(ptr)

void *cmalloc(size_t size);
void cfree(void *ptr);
