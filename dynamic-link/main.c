#include <stdio.h>

void addvec(int *, int *, int *, int);

static int x[2] = {1, 2};
static int y[2] = {3, 4};
static int z[2];

int main() {
  addvec(x, y, z, 2);
  printf("z = [%d %d]\n", z[0], z[1]);
  return 0;
}
