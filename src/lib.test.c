#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <limits.h>

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "lib.h"

// ####################
// array_new
// ####################
Test(array_new, _1) {
  Array* array = array_new(10);

  cr_assert_eq(array->capacity, 10);
  cr_assert_eq(array->length, 0);
  cr_assert_eq(array->elements == NULL, false);

  array_free(&array);
}

// ####################
// array_add
// ####################
Test(array_add, _1) {
  Array* array = array_new(5);
  int values[array->capacity];

  for (int i = 0; i < array->capacity; ++i) {
    values[i] = (i + 1) * 10;
    array_add(array, &values[i]);

    int* n = (int*)array->elements[i]->value;
    cr_assert_eq(array->length, i + 1);
    cr_assert_eq(*n, (i + 1) * 10);
  }

  array_free(&array);
}

Test(array_add, _2) {
  Array* array = array_new(5);
  char* values[] = {"one", "two", "three", "four", "five"};

  for (int i = 0; i < array->capacity; ++i) {
    array_add(array, values[i]);

    char* str = (char*)array->elements[i]->value;
    cr_assert_eq(array->length, i + 1);
    cr_assert_eq(strcmp(str, values[i]), 0);
  }

  array_free(&array);
}

Test(array_add, _3) {
  typedef struct {
    int x;
    int y;
  } Point;

  Array* array = array_new(5);
  Point values[array->capacity];

  for (int i = 0; i < array->capacity; ++i) {
    values[i].x = i;
    values[i].x = (i + 1) * 10;
    array_add(array, &values[i]);

    Point* p = (Point*)array->elements[i]->value;
    cr_assert_eq(array->length, i + 1);
    cr_assert_eq(p->x, values[i].x);
    cr_assert_eq(p->y, values[i].y);
  }

  array_free(&array);
}

Test(array_add, _4) {
  Array* array = array_new(5);
  int values[array->capacity * 2];

  for (int i = 0; i < array->capacity * 2; ++i) {
    values[i] = (i + 1) * 10;
    int res = array_add(array, &values[i]);

    if (i >= 5) {
      cr_assert_eq(res, 1);
      cr_assert_eq(array->length, 5);
    }
  }

  array_free(&array);
}

// ####################
// array_get
// ####################
Test(array_get, _1) {
  Array* array = array_new(5);
    int values[array->capacity];

  for (int i = 0; i < array->capacity; ++i) {
    values[i] = (i + 1) * 10;
    array_add(array, &values[i]);

    cr_assert_eq(array->length, i + 1);
    int* n = (int*)array_get(array, i);
    cr_assert_eq(*n, (i + 1) * 10);
  }

  array_free(&array);
}

Test(array_get, _2) {
  Array* array = array_new(5);
  char* values[] = {"one", "two", "three", "four", "five"};

  for (int i = 0; i < array->capacity; ++i) {
    array_add(array, values[i]);

    cr_assert_eq(array->length, i + 1);
    char* str = (char*)array_get(array, i);
    cr_assert_eq(strcmp(str, values[i]), 0);
  }

  array_free(&array);
}

Test(array_get, _3) {
  typedef struct {
    int x;
    int y;
  } Point;

  Array* array = array_new(5);

  for (int i = 0; i < array->capacity; ++i) {
    Point values[array->capacity];
    values[i].x = i;
    values[i].x = (i + 1) * 10;
    array_add(array, &values[i]);

    cr_assert_eq(array->length, i + 1);
    Point* p = (Point*)array_get(array, i);
    cr_assert_eq(p->x, values[i].x);
    cr_assert_eq(p->y, values[i].y);
  }

  array_free(&array);
}

Test(array_get, _4) {
  Array* array = array_new(5);
  int values[array->capacity * 2];

  for (int i = 0; i < array->capacity * 2; ++i) {
    values[i] = (i + 1) * 10;
    int res = array_add(array, &values[i]);

    if (i >= 5) {
      cr_assert_eq(res, 1);
      cr_assert_eq(array_get(array, i), NULL);
      cr_assert_eq(array->length, 5);
    }
  }

  array_free(&array);
}

// ####################
// array_remove
// ####################
Test(array_remove, _1) {
  Array* array = array_new(5);
  int values[array->capacity];

  for (int i = 0; i < array->capacity; ++i) {
    values[i] = (i + 1) * 10;
    array_add(array, &values[i]);
    cr_assert_eq(array->length, i + 1);
  }

  int x = 1;
  for (int i = array->capacity - 1; i >= 0; --i) {
    int* n = (int*)array_remove(array, i);
    cr_assert_eq(array_get(array, i), NULL);
    cr_assert_eq(*n, (i + 1) * 10);
    cr_assert_eq(array->length, array->capacity - x++);
  }

  array_free(&array);
}

Test(array_remove, _2) {
  Array* array = array_new(5);
  char* values[] = {"one", "two", "three", "four", "five"};

  for (int i = 0; i < array->capacity; ++i) {
    array_add(array, values[i]);
    cr_assert_eq(array->length, i + 1);
  }

  int x = 1;
  for (int i = array->capacity - 1; i >= 0; --i) {
    char* str = (char*)array_remove(array, i);
    cr_assert_eq(array_get(array, i), NULL);
    cr_assert_eq(strcmp(str, values[i]), 0);
    cr_assert_eq(array->length, array->capacity - x++);
  }

  array_free(&array);
}

Test(array_remove, _3) {
  typedef struct {
    int x;
    int y;
  } Point;

  Array* array = array_new(5);
  Point values[array->capacity];

  for (int i = 0; i < array->capacity; ++i) {
    values[i].x = i;
    values[i].x = (i + 1) * 10;
    array_add(array, &values[i]);
    cr_assert_eq(array->length, i + 1);
  }

  int x = 1;
  for (int i = array->capacity - 1; i >= 0; --i) {
    Point* p = (Point*)array_remove(array, i);
    cr_assert_eq(array_get(array, i), NULL);
    cr_assert_eq(p->x, values[i].x);
    cr_assert_eq(p->y, values[i].y);
    cr_assert_eq(array->length, array->capacity - x++);
  }

  array_free(&array);
}

Test(array_remove, _4) {
  Array* array = array_new(5);
  int values[array->capacity * 2];

  for (int i = 0; i < array->capacity * 2; ++i) {
    values[i] = (i + 1) * 10;
    array_add(array, &values[i]);
  }

  for (int i = array->capacity - 1; i >= 0; --i) {
    if (i >= 5) {
      void* val = array_remove(array, i);
      cr_assert_eq(val, NULL);
      cr_assert_eq(array->length, 5);
    }
  }

  array_free(&array);
}

// ####################
// array_clear
// ####################
Test(array_clear, _1) {
  Array* array = array_new(5);
  int values[array->capacity];

  for (int i = 0; i < array->capacity; ++i) {
    values[i] = (i + 1) * 10;
    array_add(array, &values[i]);
    cr_assert_eq(array->length, i + 1);
  }

  array_clear(array);

  cr_assert_eq(array->length, 0);
  for (int i = 0; i < array->capacity; ++i) {
    cr_assert_eq(array->elements[i], NULL);
  }

  int idx = 0;
  array_add(array, &values[idx]);
  int* v = array_get(array, idx);

  cr_assert_eq(*v, values[idx]);

  array_free(&array);
}

// ####################
// array_free
// ####################
Test(array_free, _1) {
  Array* array = array_new(5);
  int values[array->capacity];

  for (int i = 0; i < array->capacity; ++i) {
    values[i] = (i + 1) * 10;
    array_add(array, &values[i]);
    cr_assert_eq(array->length, i + 1);
  }

  array_free(&array);

  cr_assert_eq(array, NULL);
}
