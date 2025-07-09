#ifdef RUNTIME
#define _GNU_SOURCE
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

void *malloc(size_t size) {
  void *(*mallocp)(size_t size);
  char *error;

  mallocp = dlsym(RTLD_NEXT, "malloc");
  if ((error = dlerror()) != NULL) {
    fputs(error, stderr);
    exit(1);
  }

  void *ptr = mallocp(size);

  char s[128];
  sprintf(s, "malloc(%d) = %p\n", (int)size, ptr);
  write(2, s, strlen(s));
  return ptr;
}

void free(void *ptr) {
  void (*freep)(void *ptr);
  char *error;

  freep = dlsym(RTLD_NEXT, "free");
  if ((error = dlerror()) != NULL) {
    fputs(error, stderr);
    exit(1);
  }

  freep(ptr);
  char s[128];
  sprintf(s, "free(%p)\n", ptr);
  write(2, s, strlen(s));
}
#endif
