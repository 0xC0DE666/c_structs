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
Test(position_new, _j) {
  Position position = position_new(3, 3);

  cr_assert_eq(position.row, 3);
  cr_assert_eq(position.column, 3);
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
  int values[matrix->capacity];

  int i = 0;
  for (int r = 0; r < matrix->rows; ++r) {
    for (int c = 0; c < matrix->columns; ++c) {
      values[i] = (i + 1) * 10;
      Position p = {r, c};
      matrix_insert(matrix, &p, &values[i]);
      cr_assert_eq(matrix->size, i + 1);
      ++i;
    }
  }

  matrix_clear(matrix, NULL);
  cr_assert_eq(matrix->size, 0);

  for (int r = 0; r < matrix->rows; ++r) {
    void** row = matrix->elements[r];
    for (int c = 0; c < matrix->columns; ++c) {
      cr_assert_eq(row[c], NULL);
    }
  }

  Position p = {0, 0};
  int idx = 0;
  matrix_insert(matrix, &p, &values[idx]);
  int* v = matrix_get(matrix, &p);

  cr_assert_eq(*v, values[idx]);

  matrix_free(&matrix, NULL);
}

Test(matrix_clear, _2) {
  Matrix* matrix = matrix_new(5, 6);

  int i = 0;
  for (int r = 0; r < matrix->rows; ++r) {
    for (int c = 0; c < matrix->columns; ++c) {
      Position p = {r, c};
      matrix_insert(matrix, &p, point_new(r, c));
      cr_assert_eq(matrix->size, i + 1);
      ++i;
    }
  }

  matrix_clear(matrix, (FreeFn) point_free);
  cr_assert_eq(matrix->size, 0);

  for (int r = 0; r < matrix->rows; ++r) {
    void** row = matrix->elements[r];
    for (int c = 0; c < matrix->columns; ++c) {
      cr_assert_eq(row[c], NULL);
    }
  }

  Position p = {0, 0};
  matrix_insert(matrix, &p, &p);
  Position* v = matrix_get(matrix, &p);

  cr_assert_eq(v->row, p.row);
  cr_assert_eq(v->column, p.column);

  matrix_free(&matrix, NULL);
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
      matrix_insert(matrix, &p, &values[i]);
      cr_assert_eq(matrix_get(matrix, &p) != NULL, true);
      ++i;
    }
  }

  matrix_free(&matrix, NULL);
  cr_assert_eq(matrix, NULL);
}

Test(matrix_free, _2) {
  Matrix* matrix = matrix_new(5, 5);

  for (int r = 0; r < matrix->rows; ++r) {
    for (int c = 0; c < matrix->columns; ++c) {
      Position p = position_new(r, c);
      matrix_insert(matrix, &p, point_new(r, c));
      cr_assert_eq(matrix_get(matrix, &p) != NULL, true);
    }
  }

  matrix_free(&matrix, (FreeFn) point_free);
  cr_assert_eq(matrix, NULL);
}


// ####################
// matrix_insert
// ####################
Test(matrix_insert, _1) {
  Matrix* matrix = matrix_new(5, 5);
  int values[matrix->capacity * 2];

  int i = 0;
  for (int r = 0; r < matrix->capacity * 2; ++r) {
    for (int c = 0; c < matrix->capacity * 2; ++c) {
      values[i] = (i + 1) * 10;
      Position p = {r, c};
      int res = matrix_insert(matrix, &p, &values[i]);

      if (i >= matrix->capacity) {
        cr_assert_eq(res, 1);
        cr_assert_eq(matrix->size, matrix->capacity);
        cr_assert_eq(matrix_get(matrix, &p), NULL);
      }
    }
  }

  matrix_free(&matrix, NULL);
}

Test(matrix_insert, _2) {
  Matrix* matrix = matrix_new(5, 2);
  int values[matrix->capacity];

  int i = 0;
  for (int r = 0; r < matrix->rows; ++r) {
    for (int c = 0; c < matrix->columns; ++c) {
      values[i] = (i + 1) * 10;
      Position p = {r, c};
      matrix_insert(matrix, &p, &values[i]);

      void** row = matrix->elements[r];
      int* v = (int*) row[c];
      cr_assert_eq(matrix->size, i + 1);
      cr_assert_eq(*v, (i + 1) * 10);
      ++i;
    }
  }

  matrix_free(&matrix, NULL);
}

Test(matrix_insert, _3) {
  Matrix* matrix = matrix_new(1, 5);
  char* values[] = {"one", "two", "three", "four", "five"};

  int i = 0;
  for (int r = 0; r < matrix->rows; ++r) {
    for (int c = 0; c < matrix->columns; ++c) {
      Position p = {r, c};
      matrix_insert(matrix, &p, values[i]);

      void** row = matrix->elements[r];
      char* v = (char*) row[c];
      cr_assert_eq(matrix->size, i + 1);
      cr_assert_eq(strcmp(v, values[i]), 0);
      ++i;
    }
  }

  matrix_free(&matrix, NULL);
}

Test(matrix_insert, _4) {
  Matrix* matrix = matrix_new(4, 4);
  Point values[matrix->capacity];

  int i = 0;
  for (int r = 0; r < matrix->rows; ++r) {
    for (int c = 0; c < matrix->columns; ++c) {
      values[i].x = i;
      values[i].x = (i + 1) * 10;
      Position p = {r, c};
      matrix_insert(matrix, &p, &values[i]);

      void** row = matrix->elements[r];
      Point* v = (Point*) row[c];
      cr_assert_eq(matrix->size, i + 1);
      cr_assert_eq(v->x, values[i].x);
      cr_assert_eq(v->y, values[i].y);
      ++i;
    }
  }

  matrix_free(&matrix, NULL);
}


// ####################
// matrix_get
// ####################
Test(matrix_get, _1) {
  Matrix* matrix = matrix_new(5, 5);
  int values[matrix->capacity * 2];

  int i = 0;
  for (int r = 0; r < matrix->capacity * 2; ++r) {
    for (int c = 0; c < matrix->capacity * 2; ++c) {
      values[i] = (i + 1) * 10;
      Position p = {r, c};
      int res = matrix_insert(matrix, &p, &values[i]);

      if (i >= 26) {
        cr_assert_eq(res, 1);
        cr_assert_eq(matrix->size, 25);
      }
    }
  }

  matrix_free(&matrix, NULL);
}

Test(matrix_get, _2) {
  Matrix* matrix = matrix_new(5, 2);
  int values[matrix->capacity];

  int i = 0;
  for (int r = 0; r < matrix->rows; ++r) {
    for (int c = 0; c < matrix->columns; ++c) {
      values[i] = (i + 1) * 10;
      Position p = {r, c};
      matrix_insert(matrix, &p, &values[i]);

      int* v = (int*) matrix_get(matrix, &p);
      cr_assert_eq(matrix->size, i + 1);
      cr_assert_eq(*v, (i + 1) * 10);
      ++i;
    }
  }

  matrix_free(&matrix, NULL);
}

Test(matrix_get, _3) {
  Matrix* matrix = matrix_new(1, 5);
  char* values[] = {"one", "two", "three", "four", "five"};

  int i = 0;
  for (int r = 0; r < matrix->rows; ++r) {
    for (int c = 0; c < matrix->columns; ++c) {
      Position p = {r, c};
      matrix_insert(matrix, &p, values[i]);

      char* v = (char*) matrix_get(matrix, &p);
      cr_assert_eq(matrix->size, i + 1);
      cr_assert_eq(strcmp(v, values[i]), 0);
      ++i;
    }
  }

  matrix_free(&matrix, NULL);
}

Test(matrix_get, _4) {
  Matrix* matrix = matrix_new(4, 4);
  Point values[matrix->capacity];

  int i = 0;
  for (int r = 0; r < matrix->rows; ++r) {
    for (int c = 0; c < matrix->columns; ++c) {
      values[i].x = i;
      values[i].x = (i + 1) * 10;
      Position p = {r, c};
      matrix_insert(matrix, &p, &values[i]);

      Point* v = (Point*) matrix_get(matrix, &p);
      cr_assert_eq(matrix->size, i + 1);
      cr_assert_eq(v->x, values[i].x);
      cr_assert_eq(v->y, values[i].y);
      ++i;
    }
  }

  matrix_free(&matrix, NULL);
}


// ####################
// matrix_remove
// ####################
Test(matrix_remove, _1) {
  Matrix* matrix = matrix_new(5, 2);
  int values[matrix->capacity];

  int i = 1;
  for (int r = 0; r < matrix->rows; ++r) {
    for (int c = 0; c < matrix->columns; ++c) {
      values[i] = i * 10;
      Position p = {r, c};

      matrix_insert(matrix, &p, &values[i]);
      cr_assert_eq(matrix->size, i);
      ++i;
    }
  }

  i = 1;
  for (int r = 0; r < matrix->rows; ++r) {
    for (int c = 0; c < matrix->columns; ++c) {
      Position p = {r, c};
      int* n = (int*) matrix_remove(matrix, &p);

      cr_assert_eq(matrix_get(matrix, &p), NULL);
      cr_assert_eq(*n, i * 10);
      cr_assert_eq(matrix->size, matrix->capacity - i);
      ++i;
    }
  }

  matrix_free(&matrix, NULL);
}

Test(matrix_remove, _2) {
  Matrix* matrix = matrix_new(1, 5);
  char* values[] = {"one", "two", "three", "four", "five"};

  int i = 0;
  for (int r = 0; r < matrix->rows; ++r) {
    for (int c = 0; c < matrix->columns; ++c) {
      Position p = {r, c};

      matrix_insert(matrix, &p, values[i]);
      cr_assert_eq(matrix->size, i + 1);
      ++i;
    }
  }

  i = 0;
  for (int r = 0; r < matrix->rows; ++r) {
    for (int c = 0; c < matrix->columns; ++c) {
      Position p = {r, c};
      char* n = (char*) matrix_remove(matrix, &p);

      cr_assert_eq(matrix_get(matrix, &p), NULL);
      cr_assert_eq(strcmp(n, values[i]), 0);
      cr_assert_eq(matrix->size, matrix->capacity - (i + 1));
      ++i;
    }
  }

  matrix_free(&matrix, NULL);
}

Test(matrix_remove, _3) {
  Matrix* matrix = matrix_new(5, 3);
  Point values[matrix->capacity];

  int i = 0;
  for (int r = 0; r < matrix->rows; ++r) {
    for (int c = 0; c < matrix->columns; ++c) {
      Position p = {r, c};
      values[i].x = i;
      values[i].x = (i + 1) * 10;
      matrix_insert(matrix, &p, &values[i]);
      cr_assert_eq(matrix->size, i + 1);
      ++i;
    }
  }

  i = 0;
  for (int r = 0; r < matrix->rows; ++r) {
    for (int c = 0; c < matrix->columns; ++c) {
      Position p = {r, c};
      Point* pt = (Point*) matrix_remove(matrix, &p);

      cr_assert_eq(matrix_get(matrix, &p), NULL);
      cr_assert_eq(pt->x, values[i].x);
      cr_assert_eq(pt->y, values[i].y);
      cr_assert_eq(matrix->size, matrix->capacity - (i + 1));
      ++i;
    }
  }

  matrix_free(&matrix, NULL);
}

Test(matrix_remove, _4) {
  Matrix* matrix = matrix_new(5, 2);
  int values[matrix->capacity * 2];

  int i = 0;
  for (int r = 0; r < matrix->capacity * 2; ++r) {
    for (int c = 0; c < matrix->capacity * 2; ++c) {
      values[i] = (i + 1) * 10;
      Position p = {r, c};
      matrix_insert(matrix, &p, &values[i]);
    }
  }

  for (int r = 0; r < matrix->capacity * 2; ++r) {
    for (int c = 0; c < matrix->capacity * 2; ++c) {
      if (i >= 11) {
        Position p = {r, c};
        void* val = matrix_remove(matrix, &p);
        cr_assert_eq(val, NULL);
        cr_assert_eq(matrix->size, 5);
      }
    }
  }

  matrix_free(&matrix, NULL);
}


// ####################
// matrix_for_each
// ####################
Test(matrix_for_each, _1) {
  Matrix* matrix = matrix_new(3, 3);

  for (int r = 0; r < matrix->rows; ++r) {
    for (int c = 0; c < matrix->columns; ++c) {
      Position pos = {r, c};
      matrix_insert(matrix, &pos, point_new(r, c));
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
// matrix_to_string
// ####################
Test(matrix_to_string, _1) {
  Matrix* matrix = matrix_new(2, 2);
  Position pos[matrix->rows][matrix->columns] = {};

  char* result = matrix_to_string(matrix, (ToStringFn) position_to_string);
  char* expected = "[]";
  cr_assert_eq(strcmp(result, expected), 0);

  free(result);
  matrix_free(&matrix, NULL);
}

Test(matrix_to_string, _2) {
  Matrix* matrix = matrix_new(2, 2);
  Position pos[matrix->rows][matrix->columns] = {};

  int i = 1;
  for (int r = 0; r < matrix->rows; ++r) {
    for (int c = 0; c < matrix->columns; ++c) {
      pos[r][c] = position_new(r, c); 
      matrix_insert(matrix, &pos[r][c], i % 2 == 0 ? &pos[r][c] : NULL);
      ++i;
    }
  }

  char* result = matrix_to_string(matrix, (ToStringFn) position_to_string);
  char* expected = "[NULL, (0, 1)]\n[NULL, (1, 1)]\n";
  cr_assert_eq(strcmp(result, expected), 0);

  free(result);
  matrix_free(&matrix, NULL);
}


// ####################
// matrix_position_valid
// ####################
Test(matrix_position_valid, _1) {
  Matrix* matrix = matrix_new(5, 6);
  Position position = position_new(6, 8);

  bool result = matrix_position_valid(matrix, &position);
  cr_assert_eq(result, false);

  matrix_free(&matrix, NULL);
}

Test(matrix_position_valid, _2) {
  Matrix* matrix = matrix_new(5, 6);
  Position position = position_new(1, 1);

  bool result = matrix_position_valid(matrix, &position);
  cr_assert_eq(result, true);

  matrix_free(&matrix, NULL);
}
