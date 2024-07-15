#include <stdio.h>
#include <stdlib.h>

#include "lib.h"

/*
 * Implement a basic array.
 */

int main() {
  // print_points();
  
  Array* numbers = array_new(10);

  *(numbers->elements + 1) = (Element){};

  int val[numbers->size];
  for (int i = 0; i < numbers->size; i++) {
    val[i] = (i + 1) * 10;
    array_add(numbers, &val[i]);
  }

  Element *el0 = array_remove(numbers, 0);
  printf("%d %p\n", el0->index, el0->value);
  array_remove(numbers, 8);

  for (int i = 0; i < numbers->length; i++) {
    int v = *(int*)array_get(numbers, i);
    printf("%d\n", v);
  }
  
  return 0;
}
