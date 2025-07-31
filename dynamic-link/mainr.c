#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int x[2] = {1, 2};
int y[2] = {3, 4};
int z[2];

int main(int argc, char *argv[]) {
  void *handle;
  void (*fn)(int *, int *, int *, int);
  char *error;

  handle = dlopen("./libvector.so", RTLD_LAZY);
  if (!handle) {
    fprintf(stderr, "%s\n", dlerror());
    exit(1);
  }

  char *fn_name = argv[1];

  fn = dlsym(handle, fn_name);
  if ((error = dlerror()) != NULL) {
    fprintf(stderr, "%s\n", error);
    exit(1);
  }

  printf("Address of function %s is %p\n", fn_name, fn);

  fn(x, y, z, 2);
  printf("z = [%d %d]\n", z[0], z[1]);

  int *addcnt = dlsym(handle, "addcnt");

  printf("addcnt = %d\n", *addcnt);

  if (dlclose(handle) < 0) {
    fprintf(stderr, "%s\n", dlerror());
    exit(1);
  }

  return 0;
}
