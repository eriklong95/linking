#include <stdio.h>
#include "vector.h"

void addvec(int *, int *, int *, int);

static int a[2] = {9, 9};
static int b[2] = {9, 9};

int main() {
  int c[2];
  addvec(a, b, c, 2);
  printf("c = [%d %d]\n", c[0], c[1]);
  return 0;
}
