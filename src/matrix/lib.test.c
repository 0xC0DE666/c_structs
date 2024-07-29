#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <limits.h>

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "lib.h"

// ####################
// matrix_new
// ####################
Test(matrix_new, _1) {
  Matrix* matrix = matrix_new(10);

  cr_assert_eq(matrix->capacity, 10);
  cr_assert_eq(matrix->length, 0);
  cr_assert_eq(matrix->elements == NULL, false);

  matrix_free(&matrix);
}

// ####################
// matrix_add
// ####################
Test(matrix_add, _1) {
  Matrix* matrix = matrix_new(5);
  int values[matrix->capacity];

  for (int i = 0; i < matrix->capacity; ++i) {
    values[i] = (i + 1) * 10;
    matrix_add(matrix, &values[i]);

    int* n = (int*)matrix->elements[i]->value;
    cr_assert_eq(matrix->length, i + 1);
    cr_assert_eq(*n, (i + 1) * 10);
  }

  matrix_free(&matrix);
}

Test(matrix_add, _2) {
  Matrix* matrix = matrix_new(5);
  char* values[] = {"one", "two", "three", "four", "five"};

  for (int i = 0; i < matrix->capacity; ++i) {
    matrix_add(matrix, values[i]);

    char* str = (char*)matrix->elements[i]->value;
    cr_assert_eq(matrix->length, i + 1);
    cr_assert_eq(strcmp(str, values[i]), 0);
  }

  matrix_free(&matrix);
}

Test(matrix_add, _3) {
  typedef struct {
    int x;
    int y;
  } Point;

  Matrix* matrix = matrix_new(5);
  Point values[matrix->capacity];

  for (int i = 0; i < matrix->capacity; ++i) {
    values[i].x = i;
    values[i].x = (i + 1) * 10;
    matrix_add(matrix, &values[i]);

    Point* p = (Point*)matrix->elements[i]->value;
    cr_assert_eq(matrix->length, i + 1);
    cr_assert_eq(p->x, values[i].x);
    cr_assert_eq(p->y, values[i].y);
  }

  matrix_free(&matrix);
}

Test(matrix_add, _4) {
  Matrix* matrix = matrix_new(5);
  int values[matrix->capacity * 2];

  for (int i = 0; i < matrix->capacity * 2; ++i) {
    values[i] = (i + 1) * 10;
    int res = matrix_add(matrix, &values[i]);

    if (i >= 5) {
      cr_assert_eq(res, 1);
      cr_assert_eq(matrix->length, 5);
    }
  }

  matrix_free(&matrix);
}

// ####################
// matrix_get
// ####################
Test(matrix_get, _1) {
  Matrix* matrix = matrix_new(5);
    int values[matrix->capacity];

  for (int i = 0; i < matrix->capacity; ++i) {
    values[i] = (i + 1) * 10;
    matrix_add(matrix, &values[i]);

    cr_assert_eq(matrix->length, i + 1);
    int* n = (int*)matrix_get(matrix, i);
    cr_assert_eq(*n, (i + 1) * 10);
  }

  matrix_free(&matrix);
}

Test(matrix_get, _2) {
  Matrix* matrix = matrix_new(5);
  char* values[] = {"one", "two", "three", "four", "five"};

  for (int i = 0; i < matrix->capacity; ++i) {
    matrix_add(matrix, values[i]);

    cr_assert_eq(matrix->length, i + 1);
    char* str = (char*)matrix_get(matrix, i);
    cr_assert_eq(strcmp(str, values[i]), 0);
  }

  matrix_free(&matrix);
}

Test(matrix_get, _3) {
  typedef struct {
    int x;
    int y;
  } Point;

  Matrix* matrix = matrix_new(5);

  for (int i = 0; i < matrix->capacity; ++i) {
    Point values[matrix->capacity];
    values[i].x = i;
    values[i].x = (i + 1) * 10;
    matrix_add(matrix, &values[i]);

    cr_assert_eq(matrix->length, i + 1);
    Point* p = (Point*)matrix_get(matrix, i);
    cr_assert_eq(p->x, values[i].x);
    cr_assert_eq(p->y, values[i].y);
  }

  matrix_free(&matrix);
}

Test(matrix_get, _4) {
  Matrix* matrix = matrix_new(5);
  int values[matrix->capacity * 2];

  for (int i = 0; i < matrix->capacity * 2; ++i) {
    values[i] = (i + 1) * 10;
    int res = matrix_add(matrix, &values[i]);

    if (i >= 5) {
      cr_assert_eq(res, 1);
      cr_assert_eq(matrix_get(matrix, i), NULL);
      cr_assert_eq(matrix->length, 5);
    }
  }

  matrix_free(&matrix);
}

// ####################
// matrix_remove
// ####################
Test(matrix_remove, _1) {
  Matrix* matrix = matrix_new(5);
  int values[matrix->capacity];

  for (int i = 0; i < matrix->capacity; ++i) {
    values[i] = (i + 1) * 10;
    matrix_add(matrix, &values[i]);
    cr_assert_eq(matrix->length, i + 1);
  }

  int x = 1;
  for (int i = matrix->capacity - 1; i >= 0; --i) {
    int* n = (int*)matrix_remove(matrix, i);
    cr_assert_eq(matrix_get(matrix, i), NULL);
    cr_assert_eq(*n, (i + 1) * 10);
    cr_assert_eq(matrix->length, matrix->capacity - x++);
  }

  matrix_free(&matrix);
}

Test(matrix_remove, _2) {
  Matrix* matrix = matrix_new(5);
  char* values[] = {"one", "two", "three", "four", "five"};

  for (int i = 0; i < matrix->capacity; ++i) {
    matrix_add(matrix, values[i]);
    cr_assert_eq(matrix->length, i + 1);
  }

  int x = 1;
  for (int i = matrix->capacity - 1; i >= 0; --i) {
    char* str = (char*)matrix_remove(matrix, i);
    cr_assert_eq(matrix_get(matrix, i), NULL);
    cr_assert_eq(strcmp(str, values[i]), 0);
    cr_assert_eq(matrix->length, matrix->capacity - x++);
  }

  matrix_free(&matrix);
}

Test(matrix_remove, _3) {
  typedef struct {
    int x;
    int y;
  } Point;

  Matrix* matrix = matrix_new(5);
  Point values[matrix->capacity];

  for (int i = 0; i < matrix->capacity; ++i) {
    values[i].x = i;
    values[i].x = (i + 1) * 10;
    matrix_add(matrix, &values[i]);
    cr_assert_eq(matrix->length, i + 1);
  }

  int x = 1;
  for (int i = matrix->capacity - 1; i >= 0; --i) {
    Point* p = (Point*)matrix_remove(matrix, i);
    cr_assert_eq(matrix_get(matrix, i), NULL);
    cr_assert_eq(p->x, values[i].x);
    cr_assert_eq(p->y, values[i].y);
    cr_assert_eq(matrix->length, matrix->capacity - x++);
  }

  matrix_free(&matrix);
}

Test(matrix_remove, _4) {
  Matrix* matrix = matrix_new(5);
  int values[matrix->capacity * 2];

  for (int i = 0; i < matrix->capacity * 2; ++i) {
    values[i] = (i + 1) * 10;
    matrix_add(matrix, &values[i]);
  }

  for (int i = matrix->capacity - 1; i >= 0; --i) {
    if (i >= 5) {
      void* val = matrix_remove(matrix, i);
      cr_assert_eq(val, NULL);
      cr_assert_eq(matrix->length, 5);
    }
  }

  matrix_free(&matrix);
}

// ####################
// matrix_clear
// ####################
Test(matrix_clear, _1) {
  Matrix* matrix = matrix_new(5);
  int values[matrix->capacity];

  for (int i = 0; i < matrix->capacity; ++i) {
    values[i] = (i + 1) * 10;
    matrix_add(matrix, &values[i]);
    cr_assert_eq(matrix->length, i + 1);
  }

  matrix_clear(matrix);

  cr_assert_eq(matrix->length, 0);
  for (int i = 0; i < matrix->capacity; ++i) {
    cr_assert_eq(matrix->elements[i], NULL);
  }

  int idx = 0;
  matrix_add(matrix, &values[idx]);
  int* v = matrix_get(matrix, idx);

  cr_assert_eq(*v, values[idx]);

  matrix_free(&matrix);
}

// ####################
// matrix_free
// ####################
Test(matrix_free, _1) {
  Matrix* matrix = matrix_new(5);
  int values[matrix->capacity];

  for (int i = 0; i < matrix->capacity; ++i) {
    values[i] = (i + 1) * 10;
    matrix_add(matrix, &values[i]);
    cr_assert_eq(matrix->length, i + 1);
  }

  matrix_free(&matrix);

  cr_assert_eq(matrix, NULL);
}
