#include <stdbool.h>
//#include <stdio.h>
//#include <stdlib.h>

#include <string.h>

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "../lib/c_structs.h"
#include "utils.h"

// ####################
// position_new
// ####################
Test(position_new, _1) {
  Position p = position_new(3, 3);
  cr_assert_eq(p.row, 3);
  cr_assert_eq(p.column, 3);
}


// ####################
// position_to_string
// ####################
Test(position_to_string, _1) {
  Position a = position_new(1, 2);
  Position b = position_new(2, 8);

  char* str_a = position_to_string(&a);
  char* str_b = position_to_string(&b);

  cr_assert_eq(strcmp(str_a, "(1, 2)"), 0);
  cr_assert_eq(strcmp(str_b, "(2, 8)"), 0);

  ptr_free((void*) &str_a);
  ptr_free((void*) &str_b);
}


// ####################
// matrix_new
// ####################
Test(matrix_new, _1) {
  int rows = 5, columns = 5;
  Matrix* matrix = matrix_new(rows, columns);

  cr_assert_eq(matrix->capacity, rows * columns);
  cr_assert_eq(matrix->rows, rows);
  cr_assert_eq(matrix->columns, columns);
  cr_assert_eq(matrix->elements == NULL, false);

  for (int r = 0; r < matrix->rows; ++r) {
    void** row = matrix->elements[r];
    for (int c = 0; c < matrix->columns; ++c) {
      cr_assert_eq(row[c], NULL);
    }
  }

  matrix_free(&matrix, NULL);
}


// ####################
// matrix_clear
// ####################
Test(matrix_clear, _1) {
  Matrix* matrix = matrix_new(5, 6);

  int sze = 0;
  for (int r = 0; r < matrix->rows; ++r) {
    for (int c = 0; c < matrix->columns; ++c) {
      Position p = {r, c};
      int res = matrix_set(matrix, &p, point_new(r, c));
      res == 0 ? ++sze : 0;
      cr_assert_eq(matrix->size, sze);
      cr_assert_eq(matrix_get(matrix, &p) != NULL, true);
    }
  }
  cr_assert_eq(matrix->size, matrix->capacity);

  matrix_clear(matrix, (FreeFn) point_free);
  cr_assert_eq(matrix != NULL, true);
  cr_assert_eq(matrix->size, 0);

  for (int r = 0; r < matrix->rows; ++r) {
    void** row = matrix->elements[r];
    for (int c = 0; c < matrix->columns; ++c) {
      cr_assert_eq(row[c], NULL);
    }
  }

  matrix_free(&matrix, NULL);
}


// ####################
// matrix_free
// ####################
Test(matrix_free, _1) {
  Matrix* matrix = matrix_new(5, 5);

  for (int r = 0; r < matrix->rows; ++r) {
    for (int c = 0; c < matrix->columns; ++c) {
      Position p = position_new(r, c);
      matrix_set(matrix, &p, point_new(r, c));
      cr_assert_eq(matrix_get(matrix, &p) != NULL, true);
    }
  }
  cr_assert_eq(matrix->size, matrix->capacity);

  matrix_free(&matrix, (FreeFn) point_free);
  cr_assert_eq(matrix, NULL);
}


// ####################
// matrix_set
// ####################
Test(matrix_set, _1) {
  Matrix* matrix = matrix_new(5, 5);

  int sze = 0;
  for (int r = -5; r < matrix->rows * 2; ++r) {
    for (int c = -5; c < matrix->columns * 2; ++c) {
      Position p = {r, c};
      int res = matrix_set(matrix, &p, point_new(r, c));
      res == 0 ? ++sze : 0;

      if (matrix_position_valid(matrix, &p)) {
        cr_assert_eq(res, 0);
        cr_assert_eq(matrix->size, sze);
        Point* pt = matrix_get(matrix, &p);
        cr_assert_eq(pt != NULL, true);
        cr_assert_eq(pt->x, r);
        cr_assert_eq(pt->y, c);
      }

      if (!matrix_position_valid(matrix, &p)) {
        cr_assert_eq(res, 1);
        cr_assert_eq(matrix->size, sze);
        cr_assert_eq(matrix_get(matrix, &p), NULL);
      }
    }
  }

  Point* points[matrix->capacity] = {};
  int idx = 0;
  
  for (int r = 0; r < matrix->rows; ++r) {
    for (int c = 0; c < matrix->columns; ++c) {
      Position p = {r, c};
      points[idx] = matrix_get(matrix, &p);
      ++idx;
      int res = matrix_set(matrix, &p, point_new((r + 1) * 2, (c + 1) * 2));

      cr_assert_eq(res, 0);
      cr_assert_eq(matrix->size, matrix->capacity);
      Point* pt = matrix_get(matrix, &p);
      cr_assert_eq(pt != NULL, true);
      cr_assert_eq(pt->x, (r + 1) * 2);
      cr_assert_eq(pt->y, (c + 1) * 2);
    }
  }

  for (int i = 0; i < matrix->capacity; ++i) {
    point_free(&points[i]);
  }
  matrix_free(&matrix, (FreeFn) point_free);
}


// ####################
// matrix_get
// ####################
Test(matrix_get, _1) {
  Matrix* matrix = matrix_new(5, 5);

  int sze = 0;
  for (int r = -5; r < matrix->rows * 2; ++r) {
    for (int c = -5; c < matrix->columns * 2; ++c) {
      Position p = {r, c};
      int res = matrix_set(matrix, &p, point_new(r, c));
      res == 0 ? ++sze : 0;

      if (matrix_position_valid(matrix, &p)) {
        cr_assert_eq(res, 0);
        cr_assert_eq(matrix->size, sze);
        Point* pt = matrix_get(matrix, &p);
        cr_assert_eq(pt != NULL, true);
        cr_assert_eq(pt->x, r);
        cr_assert_eq(pt->y, c);
      }

      if (!matrix_position_valid(matrix, &p)) {
        cr_assert_eq(res, 1);
        cr_assert_eq(matrix->size, sze);
        cr_assert_eq(matrix_get(matrix, &p), NULL);
      }
    }
  }
  cr_assert_eq(sze, matrix->capacity);

  matrix_free(&matrix, (FreeFn) point_free);
}


// ####################
// matrix_remove
// ####################
Test(matrix_remove, _1) {
  Matrix* matrix = matrix_new(5, 2);

  int sze = 0;
  for (int r = -5; r < matrix->rows * 2; ++r) {
    for (int c = -2; c < matrix->columns * 2; ++c) {
      Position p = {r, c};
      int res = matrix_set(matrix, &p, point_new(r, c));
      res == 0 ? ++sze : 0;

      cr_assert_eq(matrix->size, sze);

      if (matrix_position_valid(matrix, &p)) {
        Point* pt = matrix_remove(matrix, &p);
        pt != NULL ? --sze : 0;
        cr_assert_eq(pt != NULL, true);
        cr_assert_eq(pt->x, r);
        cr_assert_eq(pt->y, c);
        cr_assert_eq(matrix_get(matrix, &p), NULL);
        cr_assert_eq(matrix->size, sze);
        point_free(&pt);
      }

      if (!matrix_position_valid(matrix, &p)) {
        Point* pt = matrix_remove(matrix, &p);
        cr_assert_eq(pt == NULL, true);
      }
    }
  }

  matrix_free(&matrix, (FreeFn) point_free);
}


// ####################
// matrix_for_each
// ####################
Test(matrix_for_each, _1) {
  Matrix* matrix = matrix_new(3, 3);

  for (int r = 0; r < matrix->rows; ++r) {
    for (int c = 0; c < matrix->columns; ++c) {
      Position pos = {r, c};
      matrix_set(matrix, &pos, point_new(r, c));
    }
  }

  matrix_for_each(matrix, (MatrixEachFn) point_double);

  for (int r = 0; r < matrix->rows; ++r) {
    for (int c = 0; c < matrix->columns; ++c) {
      Position pos = {r, c};
      Point* p = matrix_get(matrix, &pos);
      cr_assert_eq(p->x, r * 2);
      cr_assert_eq(p->y, c * 2);
    }
  }

  matrix_free(&matrix, (FreeFn) point_free);
}

// ####################
// matrix_map
// ####################
Test(matrix_map, _1) {
  Matrix* matrix = matrix_new(3, 3);
  Matrix* empty = matrix_map(matrix, (MatrixMapFn) NULL);

  cr_assert_eq(empty != NULL, true);
  cr_assert_eq(empty->size, 0);

  matrix_free(&matrix, NULL);
  matrix_free(&empty, NULL);
}

Test(matrix_map, _2) {
  Matrix* points = matrix_new(3, 3);
  
  for (int r = 0; r < points->rows; ++r) {
    for (int c = 0; c < points->columns; ++c) {
      Position pos = {r, c};
      matrix_set(points, &pos, point_new(r, c));
    }
  }

  Matrix* strings = matrix_map(points, (MatrixMapFn) point_to_string);

  cr_assert_eq(strings->capacity, points->capacity);
  cr_assert_eq(strings->size, points->size);
  cr_assert_eq(strings->elements != NULL, true);

  for (int r = 0; r < points->rows; ++r) {
    for (int c = 0; c < points->columns; ++c) {
      Position pos = {r, c};
      char* result = matrix_get(strings, &pos);
      char* expected = point_to_string(matrix_get(points, &pos));
      cr_assert_eq(strcmp(result, expected), 0);
    }
  }

  matrix_free(&points, (FreeFn) point_free);
  matrix_free(&strings, (FreeFn) ptr_free);
}


// ####################
// matrix_to_string
// ####################
Test(matrix_to_string, empty) {
  Matrix* matrix = matrix_new(2, 2);

  char* result = matrix_to_string(matrix, (ToStringFn) position_to_string);
  char* expected = "[]";
  cr_assert_eq(strcmp(result, expected), 0);

  free(result);
  matrix_free(&matrix, NULL);
}

Test(matrix_to_string, single_element) {
  Matrix* matrix = matrix_new(1, 1);
  Position pos = position_new(0, 0);

  matrix_set(matrix, &pos, &pos);

  char* result = matrix_to_string(matrix, (ToStringFn) position_to_string);
  char* expected = "[(0, 0)]\n";
  cr_assert_eq(strcmp(result, expected), 0);

  free(result);
  matrix_free(&matrix, NULL);
}

Test(matrix_to_string, single_row) {
  Matrix* matrix = matrix_new(1, 2);

  for (int r = 0; r < matrix->rows; ++r) {
    for (int c = 0; c < matrix->columns; ++c) {
      Position pos = position_new(r, c);
      matrix_set(matrix, &pos, point_new(r, c));
    }
  }

  char* result = matrix_to_string(matrix, (ToStringFn) point_to_string);
  char* expected = "[(0, 0), (0, 1)]\n";
  cr_assert_eq(strcmp(result, expected), 0);

  free(result);
  matrix_free(&matrix, (FreeFn) point_free);
}

Test(matrix_to_string, single_column) {
  Matrix* matrix = matrix_new(2, 1);

  for (int r = 0; r < matrix->rows; ++r) {
    for (int c = 0; c < matrix->columns; ++c) {
      Position pos = position_new(r, c);
      matrix_set(matrix, &pos, point_new(r, c));
    }
  }

  char* result = matrix_to_string(matrix, (ToStringFn) point_to_string);
  char* expected = "[(0, 0)]\n[(1, 0)]\n";
  cr_assert_eq(strcmp(result, expected), 0);

  free(result);
  matrix_free(&matrix, (FreeFn) point_free);
}

Test(matrix_to_string, multi) {
  Matrix* matrix = matrix_new(2, 2);

  int i = 1;
  for (int r = 0; r < matrix->rows; ++r) {
    for (int c = 0; c < matrix->columns; ++c) {
      Position pos = position_new(r, c);
      matrix_set(matrix, &pos, i % 2 == 0 ? point_new(r, c) : NULL);
      ++i;
    }
  }

  char* result = matrix_to_string(matrix, (ToStringFn) position_to_string);
  char* expected = "[NULL, (0, 1)]\n[NULL, (1, 1)]\n";
  cr_assert_eq(strcmp(result, expected), 0);

  free(result);
  matrix_free(&matrix, (FreeFn) point_free);
}


// ####################
// matrix_position_valid
// ####################
Test(matrix_position_valid, _1) {
  Matrix* matrix = matrix_new(3, 3);

  for (int r = -3; r < matrix->rows * 2; ++r) {
    for (int c = -3; c < matrix->columns * 2; ++c) {
      Position p = {r, c};
      bool result = matrix_position_valid(matrix, &p);
      bool expected =
        r >= 0 && c >= 0 && r < matrix->rows && c < matrix->columns ? true : false;
      cr_assert_eq(result, expected);
    }
  }

  matrix_free(&matrix, NULL);
}

// ####################
// matrix_has_capacity
// ####################
Test(matrix_has_capacity, _1) {
  Matrix* matrix = matrix_new(3, 3);

  for (int r = 0; r < matrix->rows * 2; ++r) {
    for (int c = 0; c < matrix->columns * 2; ++c) {
      bool result = matrix_has_capacity(matrix);
      bool expected = matrix->size < matrix->capacity ? true : false;
      cr_assert_eq(result, expected);
      Position p = {r, c};
      matrix_set(matrix, &p, point_new(r, c));
    }
  }
  cr_assert_eq(matrix->size, matrix->capacity);

  matrix_free(&matrix, (FreeFn) point_free);
}
