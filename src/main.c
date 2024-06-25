#include <stdio.h>
#include <stdlib.h>

#include "lib.h"

/*
 * Implement a basic array.
 */

int main() {
  // Array* array = array_new(10);
  // printf("init start\n");
  // Element* s = array->elements;
  // for (int i = 0; i < 10; i++) {
  //   s->index = i;
  //   int v = (i + 1) * 10;
  //   s->value = &v;
  //   ++s;
  // }
  // printf("init end\n");

  // printf("print start\n");
  // s = array->elements;
  // for (int i = 1; i <= 10; i++) {
  //   printf("%d %d\n", s->index, *(int*)s->value);
  //   ++s;
  // }
  // printf("print end\n");

  Array* array = array_new(10);
  printf("%s\n", array_to_str(array));

  for (int i = 0; i < array->size; ++i) {
    int v[10];
    v[i] = (i + 1) * 10;
    array_add(array, &v[i]);
  }
  printf("%s\n", array_to_str(array));

  for (int i = 0; i < array->count; ++i) {
    Element el = array->elements[i];
    // int x = *(int*)array->elements[i].value;
    printf("%d %p %d\n", el.index, el.value, *(int*)el.value);
  }

  // for (int i = 0; i < 10; i++) {
  //   int x[10];
  //   printf("%p\n", &x[i]);
  // }

  return 0;
}
