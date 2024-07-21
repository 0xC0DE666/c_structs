#include <stdio.h>
#include <stdlib.h>

#include "lib.h"

/*
 * Implement a basic array.
 */

int main() {
  Array *array = array_new(10);
  int numbers[] = {1,2,3};
  array_add(array, &numbers[0]);
  array_add(array, &numbers[1]);
  array_add(array, &numbers[2]);
  printf("%p\n", &array);
  array_free(&array);
  printf("%p\n", array);
  // printf("%p\n", array);

  return 0;
}
