#include <stdio.h>
#include <stdlib.h>

#include "lib.h"

/*
 * Implement a basic array.
 */

int main() {
  Array* array = array_new(10);
  printf("%s\n", array_to_str(array));

  for (int i = 0; i < array->size; ++i) {
    int v = 100;
    array_add(array, &v);
  }
  printf("%s\n", array_to_str(array));

  for (int i = 0; i < array->count; ++i) {
    Element el = array->elements[i];
    int x = *(int*)el.value;
    printf("%d %d\n", el.index, x);
  }

  return 0;
}
