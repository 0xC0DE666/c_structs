#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "lib.h"

// ####################
// string_length
// ####################
Test(string_length, pass) {
  int result = string_length("");
  cr_assert_eq(result, 0);

  result = string_length("a");
  cr_assert_eq(result, 1);

  result = string_length("abc");
  cr_assert_eq(result, 3);
}

// ####################
// array_new
// ####################
Test(array_new, _1) {
  Array* array = array_new(10);
  cr_assert_eq(array->size, 10);
  cr_assert_eq(array->count, 0);
  cr_assert_eq(array->elements == NULL, false);
}

// ####################
// array_add
// ####################
Test(array_add, _1) {
  Array* array = array_new(5);

  for (int i = 0; i < array->size; ++i) {
    int values[array->size];
    values[i] = (i + 1) * 10;
    array_add(array, &values[i]);

    cr_assert_eq(array->count, i + 1);
    cr_assert_eq(*(int*)array->elements[i].value, (i + 1) * 10);
  }
}

Test(array_add, _2) {
  Array* array = array_new(5);

  for (int i = 0; i < array->size; ++i) {
    char values[] = {'a', 'b', 'c', 'd', 'e'};
    array_add(array, &values[i]);

    cr_assert_eq(array->count, i + 1);
    cr_assert_eq(*(char*)array->elements[i].value, values[i]);
  }
}

Test(array_add, _3) {
  typedef struct {
    int x;
    int y;
  } Point;

  Array* array = array_new(5);

  for (int i = 0; i < array->size; ++i) {
    Point values[array->size];
    values[i].x = i;
    values[i].x = (i + 1) * 10;
    array_add(array, &values[i]);

    cr_assert_eq(array->count, i + 1);
    Point p = *(Point*)array->elements[i].value;
    cr_assert_eq(p.x, values[i].x);
    cr_assert_eq(p.y, values[i].y);
  }
}
