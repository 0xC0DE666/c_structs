#include <stdio.h>
#include <stdlib.h>

#include "lib.h"

/*
 * Implement a basic array.
 */

int main() {
  Array *array = array_new(10);
  printf("array addr = %p\n", array);
  printf("size addr = %p\n", &array->size);
  printf("count addr = %p\n", &array->count);
  printf("values addr = %p\n", &array->values);
  

  int *values = array->values;
  values[0] = 99;
  printf("%d\n", values[0]);
  return 0;
}


