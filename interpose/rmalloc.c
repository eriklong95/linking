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
  ssize_t result = write(2, s, strlen(s));
  if (result == -1) {
    fputs("write error", stderr);
    exit(1);
  }
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
  
  ssize_t result = write(2, s, strlen(s));
  if (result == -1) {
    fputs("write error", stderr);
    exit(1);
  }
}
