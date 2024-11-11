#include <stdbool.h>
#include <string.h>
//#include <stdio.h>
//#include <stdlib.h>

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

  safe_free((void*) &str_a);
  safe_free((void*) &str_b);
}

// ####################
// grid_position_valid
// ####################
Test(grid_position_valid, _1) {
  Grid* grid = grid_new(3, 3).ok;

  for (int r = -3; r < grid->rows * 2; ++r) {
    for (int c = -3; c < grid->columns * 2; ++c) {
      Position p = {r, c};
      bool result = grid_position_valid(grid, &p);
      bool expected =
        r >= 0 && c >= 0 && r < grid->rows && c < grid->columns ? true : false;
      cr_assert_eq(result, expected);
    }
  }

  grid_free(&grid, NULL);
}

// ####################
// grid_has_capacity
// ####################
Test(grid_has_capacity, _1) {
  Grid* grid = grid_new(3, 3).ok;

  for (int r = 0; r < grid->rows * 2; ++r) {
    for (int c = 0; c < grid->columns * 2; ++c) {
      bool result = grid_has_capacity(grid);
      bool expected = grid->size < grid->capacity ? true : false;
      cr_assert_eq(result, expected);
      Position p = {r, c};
      grid_set(grid, &p, point_new(r, c));
    }
  }
  cr_assert_eq(grid->size, grid->capacity);

  grid_free(&grid, (FreeFn) point_free);
}

// ####################
// grid_new
// ####################
Test(grid_new, _1) {
  int rows = 5, columns = 5;
  Grid* grid = grid_new(rows, columns).ok;

  cr_assert_eq(grid->capacity, rows * columns);
  cr_assert_eq(grid->rows, rows);
  cr_assert_eq(grid->columns, columns);
  cr_assert_eq(grid->elements == NULL, false);

  for (int r = 0; r < grid->rows; ++r) {
    void** row = grid->elements + r * columns;
    for (int c = 0; c < grid->columns; ++c) {
      cr_assert_eq(row[c], NULL);
    }
  }

  grid_free(&grid, NULL);
}

// ####################
// grid_clear
// ####################
Test(grid_clear, _1) {
  Grid* grid = grid_new(5, 6).ok;

  int sze = 0;
  for (int r = 0; r < grid->rows; ++r) {
    for (int c = 0; c < grid->columns; ++c) {
      Position p = {r, c};
      int res = grid_set(grid, &p, point_new(r, c));
      res == 0 ? ++sze : 0;
      cr_assert_eq(grid->size, sze);
      cr_assert_eq(grid_get(grid, &p).ok != NULL, true);
    }
  }
  cr_assert_eq(grid->size, grid->capacity);

  grid_clear(grid, (FreeFn) point_free);
  cr_assert_eq(grid != NULL, true);
  cr_assert_eq(grid->size, 0);

  for (int r = 0; r < grid->rows; ++r) {
    void** row = grid->elements + r * grid->columns;
    for (int c = 0; c < grid->columns; ++c) {
      cr_assert_eq(row[c], NULL);
    }
  }

  grid_free(&grid, NULL);
}

// ####################
// grid_free
// ####################
Test(grid_free, _1) {
  Grid* grid = grid_new(5, 5).ok;

  for (int r = 0; r < grid->rows; ++r) {
    for (int c = 0; c < grid->columns; ++c) {
      Position p = position_new(r, c);
      grid_set(grid, &p, point_new(r, c));
      cr_assert_eq(grid_get(grid, &p).ok != NULL, true);
    }
  }
  cr_assert_eq(grid->size, grid->capacity);

  grid_free(&grid, (FreeFn) point_free);
  cr_assert_eq(grid, NULL);
}

// ####################
// grid_set
// ####################
Test(grid_set, _1) {
  Grid* grid = grid_new(5, 5).ok;

  int sze = 0;
  for (int r = -5; r < grid->rows * 2; ++r) {
    for (int c = -5; c < grid->columns * 2; ++c) {
      Position p = {r, c};
      int res = grid_set(grid, &p, point_new(r, c));
      res == 0 ? ++sze : 0;

      if (grid_position_valid(grid, &p)) {
        cr_assert_eq(res, 0);
        cr_assert_eq(grid->size, sze);
        Point* pt = grid_get(grid, &p).ok;
        cr_assert_eq(pt != NULL, true);
        cr_assert_eq(pt->x, r);
        cr_assert_eq(pt->y, c);
      }

      if (!grid_position_valid(grid, &p)) {
        cr_assert_eq(res, 1);
        cr_assert_eq(grid->size, sze);
        Result res = grid_get(grid, &p);
        Error* err = res.error;
        cr_assert_eq(res.ok, NULL);
        cr_assert_eq(err->code, ERR_CODE_GENERAL);
        cr_assert_eq(strcmp(err->message, ERR_MSG_INVALID_POSITION), 0);
        error_free(&err);
      }
    }
  }

  Point* points[grid->capacity] = {};
  int idx = 0;
  
  for (int r = 0; r < grid->rows; ++r) {
    for (int c = 0; c < grid->columns; ++c) {
      Position p = {r, c};
      points[idx] = grid_get(grid, &p).ok;
      ++idx;
      int res = grid_set(grid, &p, point_new((r + 1) * 2, (c + 1) * 2));

      cr_assert_eq(res, 0);
      cr_assert_eq(grid->size, grid->capacity);
      Point* pt = grid_get(grid, &p).ok;
      cr_assert_eq(pt != NULL, true);
      cr_assert_eq(pt->x, (r + 1) * 2);
      cr_assert_eq(pt->y, (c + 1) * 2);
    }
  }

  for (int i = 0; i < grid->capacity; ++i) {
    point_free(&points[i]);
  }
  grid_free(&grid, (FreeFn) point_free);
}

// ####################
// grid_get
// ####################
Test(grid_get, _1) {
  Grid* grid = grid_new(5, 5).ok;

  int sze = 0;
  for (int r = -5; r < grid->rows * 2; ++r) {
    for (int c = -5; c < grid->columns * 2; ++c) {
      Position p = {r, c};
      int res = grid_set(grid, &p, point_new(r, c));
      res == 0 ? ++sze : 0;

      if (grid_position_valid(grid, &p)) {
        cr_assert_eq(res, 0);
        cr_assert_eq(grid->size, sze);
        Point* pt = grid_get(grid, &p).ok;
        cr_assert_eq(pt != NULL, true);
        cr_assert_eq(pt->x, r);
        cr_assert_eq(pt->y, c);
      }

      if (!grid_position_valid(grid, &p)) {
        cr_assert_eq(res, 1);
        cr_assert_eq(grid->size, sze);
        Result res = grid_get(grid, &p);
        Error* err = res.error;
        cr_assert_eq(res.ok, NULL);
        cr_assert_eq(err->code, ERR_CODE_GENERAL);
        cr_assert_eq(strcmp(err->message, ERR_MSG_INVALID_POSITION), 0);
        error_free(&err);
      }
    }
  }
  cr_assert_eq(sze, grid->capacity);

  grid_free(&grid, (FreeFn) point_free);
}

// ####################
// grid_remove
// ####################
Test(grid_remove, _1) {
  Grid* grid = grid_new(5, 2).ok;

  int sze = 0;
  for (int r = -5; r < grid->rows * 2; ++r) {
    for (int c = -2; c < grid->columns * 2; ++c) {
      Position p = {r, c};
      int res = grid_set(grid, &p, point_new(r, c));
      res == 0 ? ++sze : 0;

      cr_assert_eq(grid->size, sze);

      if (grid_position_valid(grid, &p)) {
        Point* pt = grid_remove(grid, &p).ok;
        pt != NULL ? --sze : 0;
        cr_assert_eq(pt != NULL, true);
        cr_assert_eq(pt->x, r);
        cr_assert_eq(pt->y, c);
        cr_assert_eq(grid_get(grid, &p).ok, NULL);
        cr_assert_eq(grid->size, sze);
        point_free(&pt);
      }

      if (!grid_position_valid(grid, &p)) {
        cr_assert_eq(res, 1);
        cr_assert_eq(grid->size, sze);
        Result res = grid_get(grid, &p);
        Error* err = res.error;
        cr_assert_eq(res.ok, NULL);
        cr_assert_eq(err->code, ERR_CODE_GENERAL);
        cr_assert_eq(strcmp(err->message, ERR_MSG_INVALID_POSITION), 0);
        error_free(&err);
      }
    }
  }

  grid_free(&grid, (FreeFn) point_free);
}

// ####################
// grid_for_each
// ####################
Test(grid_for_each, _1) {
  Grid* grid = grid_new(3, 3).ok;
  int e = grid_for_each(grid, (ArrayEachFn) point_double);
  cr_assert_eq(e, 0);

  for (int r = 0; r < grid->rows; ++r) {
    for (int c = 0; c < grid->columns; ++c) {
      Position pos = {r, c};
      grid_set(grid, &pos, point_new(r, c));
    }
  }

  e = grid_for_each(grid, (GridEachFn) point_double);
  cr_assert_eq(e, 0);

  for (int r = 0; r < grid->rows; ++r) {
    for (int c = 0; c < grid->columns; ++c) {
      Position pos = {r, c};
      Point* p = grid_get(grid, &pos).ok;
      cr_assert_eq(p->x, r * 2);
      cr_assert_eq(p->y, c * 2);
    }
  }

  grid_free(&grid, (FreeFn) point_free);
}

// ####################
// grid_map
// ####################
Test(grid_map, _1) {
  Grid* points = grid_new(3, 3).ok;
  Grid* empty = grid_map(points, (GridMapFn) NULL).ok;

  cr_assert_eq(empty != NULL, true);
  cr_assert_eq(empty->size, 0);
  grid_free(&empty, NULL);

  
  for (int r = 0; r < points->rows; ++r) {
    for (int c = 0; c < points->columns; ++c) {
      Position pos = {r, c};
      grid_set(points, &pos, point_new(r, c));
    }
  }

  Grid* strings = grid_map(points, (GridMapFn) point_to_string).ok;

  cr_assert_eq(strings->capacity, points->capacity);
  cr_assert_eq(strings->size, points->size);
  cr_assert_eq(strings->elements != NULL, true);

  for (int r = 0; r < points->rows; ++r) {
    for (int c = 0; c < points->columns; ++c) {
      Position pos = {r, c};
      char* result = grid_get(strings, &pos).ok;
      char* expected = point_to_string(grid_get(points, &pos).ok);
      cr_assert_eq(strcmp(result, expected), 0);
    }
  }

  grid_free(&points, (FreeFn) point_free);
  grid_free(&strings, (FreeFn) safe_free);
}

// ####################
// grid_to_string
// ####################
Test(grid_to_string, empty) {
  Grid* grid = grid_new(2, 2).ok;

  char* result = grid_to_string(grid, (ToStringFn) position_to_string).ok;
  char* expected = "[]";
  cr_assert_eq(strcmp(result, expected), 0);

  free(result);
  grid_free(&grid, NULL);
}

Test(grid_to_string, single_element) {
  Grid* grid = grid_new(1, 1).ok;
  Position pos = position_new(0, 0);

  grid_set(grid, &pos, &pos);

  char* result = grid_to_string(grid, (ToStringFn) position_to_string).ok;
  char* expected = "[(0, 0)]\n";
  cr_assert_eq(strcmp(result, expected), 0);

  free(result);
  grid_free(&grid, NULL);
}

Test(grid_to_string, single_row) {
  Grid* grid = grid_new(1, 2).ok;

  for (int r = 0; r < grid->rows; ++r) {
    for (int c = 0; c < grid->columns; ++c) {
      Position pos = position_new(r, c);
      grid_set(grid, &pos, point_new(r, c));
    }
  }

  char* result = grid_to_string(grid, (ToStringFn) point_to_string).ok;
  char* expected = "[(0, 0), (0, 1)]\n";
  cr_assert_eq(strcmp(result, expected), 0);

  free(result);
  grid_free(&grid, (FreeFn) point_free);
}

Test(grid_to_string, single_column) {
  Grid* grid = grid_new(2, 1).ok;

  for (int r = 0; r < grid->rows; ++r) {
    for (int c = 0; c < grid->columns; ++c) {
      Position pos = position_new(r, c);
      grid_set(grid, &pos, point_new(r, c));
    }
  }

  char* result = grid_to_string(grid, (ToStringFn) point_to_string).ok;
  char* expected = "[(0, 0)]\n[(1, 0)]\n";
  cr_assert_eq(strcmp(result, expected), 0);

  free(result);
  grid_free(&grid, (FreeFn) point_free);
}

Test(grid_to_string, multi) {
  Grid* grid = grid_new(2, 2).ok;

  int i = 1;
  for (int r = 0; r < grid->rows; ++r) {
    for (int c = 0; c < grid->columns; ++c) {
      Position pos = position_new(r, c);
      grid_set(grid, &pos, i % 2 == 0 ? point_new(r, c) : NULL);
      ++i;
    }
  }

  char* result = grid_to_string(grid, (ToStringFn) position_to_string).ok;
  char* expected = "[NULL, (0, 1)]\n[NULL, (1, 1)]\n";
  cr_assert_eq(strcmp(result, expected), 0);

  free(result);
  grid_free(&grid, (FreeFn) point_free);
}
