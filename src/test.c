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
  Array *array = array_new(10);
  cr_assert_eq(array->size, 10);
  cr_assert_eq(array->length, 0);
  cr_assert_eq(array->elements == NULL, false);
}

// ####################
// array_add
// ####################
Test(array_add, _1) {
  Array *array = array_new(5);

  for (int i = 0; i < array->size; ++i) {
    int values[array->size];
    values[i] = (i + 1) * 10;
    array_add(array, &values[i]);

    cr_assert_eq(array->length, i + 1);
    cr_assert_eq(*(int*)array->elements[i].value, (i + 1) * 10);
  }
}

Test(array_add, _2) {
  Array *array = array_new(5);

  for (int i = 0; i < array->size; ++i) {
    char *values[] = {"one", "two", "three", "four", "five"};
    array_add(array, values[i]);

    cr_assert_eq(array->length, i + 1);
    char *str = (char*)array->elements[i].value;
    cr_assert_eq(strcmp(str, values[i]), 0);
  }
}

Test(array_add, _3) {
  typedef struct {
    int x;
    int y;
  } Point;

  Array *array = array_new(5);

  for (int i = 0; i < array->size; ++i) {
    Point values[array->size];
    values[i].x = i;
    values[i].x = (i + 1) * 10;
    array_add(array, &values[i]);

    cr_assert_eq(array->length, i + 1);
    Point p = *(Point*)array->elements[i].value;
    cr_assert_eq(p.x, values[i].x);
    cr_assert_eq(p.y, values[i].y);
  }
}

Test(array_add, _4) {
  Array *array = array_new(5);

  for (int i = 0; i < 10; ++i) {
    int values[10];
    values[i] = (i + 1) * 10;
    int res = array_add(array, &values[i]);

    if (i >= 5) {
      cr_assert_eq(res, 1);
      cr_assert_eq(array->length, 5);
    }
  }
}

// ####################
// array_get
// ####################
Test(array_get, _1) {
  Array *array = array_new(5);

  for (int i = 0; i < array->size; ++i) {
    int values[array->size];
    values[i] = (i + 1) * 10;
    array_add(array, &values[i]);

    cr_assert_eq(array->length, i + 1);
    cr_assert_eq(*(int*)array_get(array, i), (i + 1) * 10);
  }
}

Test(array_get, _2) {
  Array *array = array_new(5);

  for (int i = 0; i < array->size; ++i) {
    char *values[] = {"one", "two", "three", "four", "five"};
    array_add(array, values[i]);

    cr_assert_eq(array->length, i + 1);
    char *str = (char*)array_get(array, i);
    cr_assert_eq(strcmp(str, values[i]), 0);
  }
}

Test(array_get, _3) {
  typedef struct {
    int x;
    int y;
  } Point;

  Array *array = array_new(5);

  for (int i = 0; i < array->size; ++i) {
    Point values[array->size];
    values[i].x = i;
    values[i].x = (i + 1) * 10;
    array_add(array, &values[i]);

    cr_assert_eq(array->length, i + 1);
    Point p = *(Point*)array_get(array, i);
    cr_assert_eq(p.x, values[i].x);
    cr_assert_eq(p.y, values[i].y);
  }
}

Test(array_get, _4) {
  Array *array = array_new(5);

  for (int i = 0; i < 10; ++i) {
    int values[10];
    values[i] = (i + 1) * 10;
    int res = array_add(array, &values[i]);

    if (i >= 5) {
      cr_assert_eq(res, 1);
      cr_assert_eq(array_get(array, i), NULL);
      cr_assert_eq(array->length, 5);
    }
  }
}
