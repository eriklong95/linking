#include <stdio.h>

void swap();

extern int *bufp0;

int buf[2] = {1, 2};

int main() {

  printf("%d", *bufp0);
  swap();
  return 0;
}
