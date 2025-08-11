#include <stdio.h>
#include <malloc.h>

void *cmalloc(size_t size) {
  void *ptr = malloc(size);
  printf("malloc(%d) = %p\n", (int)size, ptr);
  return ptr;
}

void cfree(void *ptr) {
  free(ptr);
  printf("free(%p)\n", ptr);
}
