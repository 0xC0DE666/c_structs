#include <stdio.h>
#include <stdlib.h>

#include "lib.h"

/*
 * Implement a basic array.
 */

int main() {
  Array *array = array_new(10);

  for (int i = 0; i < array->size + 100; ++i) {
    array_add(array, i + 1);
  }
  printf("%s\n", array_to_str(array));

  for (int i = 0; i < array->count; ++i) {
    printf("%d\n", array->values[i]);
  }

  return 0;
}
