#include <stdio.h>
#include <stdlib.h>

#include "lib.h"

/*
 * Implement a basic array.
 */

int main() {
  Array *array = array_new(5);
  int v = 10;
  printf("checkpoint\nn");
  array_add(array, &v);
  int *x = (int *)array->elements[0]->value;
  printf("x = %d\n", *x);
  
  return 0;
}
