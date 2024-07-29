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
  int rows = 3, columns = 3;
  Matrix* matrix = matrix_new(rows, columns);

  cr_assert_eq(matrix->capacity, rows * columns);
  cr_assert_eq(matrix->rows, rows);
  cr_assert_eq(matrix->columns, columns);
  cr_assert_eq(matrix->elements == NULL, false);

  matrix_free(&matrix);
}

Test(matrix_new, _2) {
  int rows = 5, columns = 5;
  Matrix* matrix = matrix_new(rows, columns);

  cr_assert_eq(matrix->capacity, rows * columns);
  cr_assert_eq(matrix->rows, rows);
  cr_assert_eq(matrix->columns, columns);
  cr_assert_eq(matrix->elements == NULL, false);

  for (int r = 0; r < rows; ++r) {
    void** row = matrix->elements[r];
    for (int c = 0; c < columns; ++c) {
      cr_assert_eq(row[c], NULL);
    }
  }

  matrix_free(&matrix);
}


// ####################
// matrix_free
// ####################
Test(matrix_free, _1) {
  Matrix* matrix = matrix_new(5, 5);
  int values[matrix->capacity];

  int i = 0;
  for (int r = 0; r < matrix->rows; ++r) {
    for (int c = 0; c < matrix->columns; ++c) {
      values[i] = (i + 1) * 10;
      Position p = {r, c};
      matrix_add(matrix, &p, &values[i]);
      cr_assert_eq(matrix->size, i + 1);
      ++i;
    }
  }

  matrix_free(&matrix);

  cr_assert_eq(matrix, NULL);
}

// ####################
// matrix_add
// ####################
Test(matrix_add, _1) {
  Matrix* matrix = matrix_new(5, 2);
  int values[matrix->capacity];

  int i = 0;
  for (int r = 0; r < matrix->rows; ++r) {
    for (int c = 0; c < matrix->columns; ++c) {
      values[i] = (i + 1) * 10;
      Position p = {r, c};
      matrix_add(matrix, &p, &values[i]);

      void** row = matrix->elements[r];
      int* v = (int*) row[c];
      cr_assert_eq(matrix->size, i + 1);
      cr_assert_eq(*v, (i + 1) * 10);
      ++i;
    }
  }

  matrix_free(&matrix);
}

Test(matrix_add, _2) {
  Matrix* matrix = matrix_new(1, 5);
  char* values[] = {"one", "two", "three", "four", "five"};

  int i = 0;
  for (int r = 0; r < matrix->rows; ++r) {
    for (int c = 0; c < matrix->columns; ++c) {
      Position p = {r, c};
      int res = matrix_add(matrix, &p, values[i]);

      void** row = matrix->elements[r];
      char* v = (char*) row[c];
      cr_assert_eq(matrix->size, i + 1);
      cr_assert_eq(strcmp(v, values[i]), 0);
      ++i;
    }
  }

  matrix_free(&matrix);
}

Test(matrix_add, _3) {
  typedef struct {
    int x;
    int y;
  } Point;

  Matrix* matrix = matrix_new(4, 4);
  Point values[matrix->capacity];

  int i = 0;
  for (int r = 0; r < matrix->rows; ++r) {
    for (int c = 0; c < matrix->columns; ++c) {
      values[i].x = i;
      values[i].x = (i + 1) * 10;
      Position p = {r, c};
      int res = matrix_add(matrix, &p, &values[i]);

      void** row = matrix->elements[r];
      Point* v = (Point*) row[c];
      cr_assert_eq(matrix->size, i + 1);
      cr_assert_eq(v->x, values[i].x);
      cr_assert_eq(v->y, values[i].y);
      ++i;
    }
  }

  matrix_free(&matrix);
}

Test(matrix_add, _4) {
  Matrix* matrix = matrix_new(5, 5);
  int values[matrix->capacity * 2];

  int i = 0;
  for (int r = 0; r < matrix->capacity * 2; ++r) {
    for (int c = 0; c < matrix->capacity * 2; ++c) {
      values[i] = (i + 1) * 10;
      Position p = {r, c};
      int res = matrix_add(matrix, &p, &values[i]);

      if (i >= 26) {
        cr_assert_eq(res, 1);
        cr_assert_eq(matrix->size, 25);
      }
    }
  }

  matrix_free(&matrix);
}

// // ####################
// // matrix_get
// // ####################
// Test(matrix_get, _1) {
//   Matrix* matrix = matrix_new(5);
//     int values[matrix->capacity];
// 
//   for (int i = 0; i < matrix->capacity; ++i) {
//     values[i] = (i + 1) * 10;
//     matrix_add(matrix, &values[i]);
// 
//     cr_assert_eq(matrix->length, i + 1);
//     int* n = (int*)matrix_get(matrix, i);
//     cr_assert_eq(*n, (i + 1) * 10);
//   }
// 
//   matrix_free(&matrix);
// }
// 
// Test(matrix_get, _2) {
//   Matrix* matrix = matrix_new(5);
//   char* values[] = {"one", "two", "three", "four", "five"};
// 
//   for (int i = 0; i < matrix->capacity; ++i) {
//     matrix_add(matrix, values[i]);
// 
//     cr_assert_eq(matrix->length, i + 1);
//     char* str = (char*)matrix_get(matrix, i);
//     cr_assert_eq(strcmp(str, values[i]), 0);
//   }
// 
//   matrix_free(&matrix);
// }
// 
// Test(matrix_get, _3) {
//   typedef struct {
//     int x;
//     int y;
//   } Point;
// 
//   Matrix* matrix = matrix_new(5);
// 
//   for (int i = 0; i < matrix->capacity; ++i) {
//     Point values[matrix->capacity];
//     values[i].x = i;
//     values[i].x = (i + 1) * 10;
//     matrix_add(matrix, &values[i]);
// 
//     cr_assert_eq(matrix->length, i + 1);
//     Point* p = (Point*)matrix_get(matrix, i);
//     cr_assert_eq(p->x, values[i].x);
//     cr_assert_eq(p->y, values[i].y);
//   }
// 
//   matrix_free(&matrix);
// }
// 
// Test(matrix_get, _4) {
//   Matrix* matrix = matrix_new(5);
//   int values[matrix->capacity * 2];
// 
//   for (int i = 0; i < matrix->capacity * 2; ++i) {
//     values[i] = (i + 1) * 10;
//     int res = matrix_add(matrix, &values[i]);
// 
//     if (i >= 5) {
//       cr_assert_eq(res, 1);
//       cr_assert_eq(matrix_get(matrix, i), NULL);
//       cr_assert_eq(matrix->length, 5);
//     }
//   }
// 
//   matrix_free(&matrix);
// }
// 
// // ####################
// // matrix_remove
// // ####################
// Test(matrix_remove, _1) {
//   Matrix* matrix = matrix_new(5);
//   int values[matrix->capacity];
// 
//   for (int i = 0; i < matrix->capacity; ++i) {
//     values[i] = (i + 1) * 10;
//     matrix_add(matrix, &values[i]);
//     cr_assert_eq(matrix->length, i + 1);
//   }
// 
//   int x = 1;
//   for (int i = matrix->capacity - 1; i >= 0; --i) {
//     int* n = (int*)matrix_remove(matrix, i);
//     cr_assert_eq(matrix_get(matrix, i), NULL);
//     cr_assert_eq(*n, (i + 1) * 10);
//     cr_assert_eq(matrix->length, matrix->capacity - x++);
//   }
// 
//   matrix_free(&matrix);
// }
// 
// Test(matrix_remove, _2) {
//   Matrix* matrix = matrix_new(5);
//   char* values[] = {"one", "two", "three", "four", "five"};
// 
//   for (int i = 0; i < matrix->capacity; ++i) {
//     matrix_add(matrix, values[i]);
//     cr_assert_eq(matrix->length, i + 1);
//   }
// 
//   int x = 1;
//   for (int i = matrix->capacity - 1; i >= 0; --i) {
//     char* str = (char*)matrix_remove(matrix, i);
//     cr_assert_eq(matrix_get(matrix, i), NULL);
//     cr_assert_eq(strcmp(str, values[i]), 0);
//     cr_assert_eq(matrix->length, matrix->capacity - x++);
//   }
// 
//   matrix_free(&matrix);
// }
// 
// Test(matrix_remove, _3) {
//   typedef struct {
//     int x;
//     int y;
//   } Point;
// 
//   Matrix* matrix = matrix_new(5);
//   Point values[matrix->capacity];
// 
//   for (int i = 0; i < matrix->capacity; ++i) {
//     values[i].x = i;
//     values[i].x = (i + 1) * 10;
//     matrix_add(matrix, &values[i]);
//     cr_assert_eq(matrix->length, i + 1);
//   }
// 
//   int x = 1;
//   for (int i = matrix->capacity - 1; i >= 0; --i) {
//     Point* p = (Point*)matrix_remove(matrix, i);
//     cr_assert_eq(matrix_get(matrix, i), NULL);
//     cr_assert_eq(p->x, values[i].x);
//     cr_assert_eq(p->y, values[i].y);
//     cr_assert_eq(matrix->length, matrix->capacity - x++);
//   }
// 
//   matrix_free(&matrix);
// }
// 
// Test(matrix_remove, _4) {
//   Matrix* matrix = matrix_new(5);
//   int values[matrix->capacity * 2];
// 
//   for (int i = 0; i < matrix->capacity * 2; ++i) {
//     values[i] = (i + 1) * 10;
//     matrix_add(matrix, &values[i]);
//   }
// 
//   for (int i = matrix->capacity - 1; i >= 0; --i) {
//     if (i >= 5) {
//       void* val = matrix_remove(matrix, i);
//       cr_assert_eq(val, NULL);
//       cr_assert_eq(matrix->length, 5);
//     }
//   }
// 
//   matrix_free(&matrix);
// }
// 
// // ####################
// // matrix_clear
// // ####################
// Test(matrix_clear, _1) {
//   Matrix* matrix = matrix_new(5);
//   int values[matrix->capacity];
// 
//   for (int i = 0; i < matrix->capacity; ++i) {
//     values[i] = (i + 1) * 10;
//     matrix_add(matrix, &values[i]);
//     cr_assert_eq(matrix->length, i + 1);
//   }
// 
//   matrix_clear(matrix);
// 
//   cr_assert_eq(matrix->length, 0);
//   for (int i = 0; i < matrix->capacity; ++i) {
//     cr_assert_eq(matrix->elements[i], NULL);
//   }
// 
//   int idx = 0;
//   matrix_add(matrix, &values[idx]);
//   int* v = matrix_get(matrix, idx);
// 
//   cr_assert_eq(*v, values[idx]);
// 
//   matrix_free(&matrix);
// }

