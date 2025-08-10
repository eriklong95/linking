#include <stdlib.h>

int main() {
  int *p = malloc(32);
  *p = 1;
  free(p);
}
