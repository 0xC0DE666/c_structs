#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "c_structs.h"

Position position_new(unsigned int row, unsigned int column) {
  return (Position) {row, column};
}

char* position_to_string(Position* position) {
  char* buffer = malloc(14);

  if (buffer == NULL) {
    return NULL;
  }

  sprintf(
    buffer,
    "(%d, %d)",
    position->row,
    position->column
  );

  return buffer;
}

bool grid_position_valid(Grid* const grid, Position* const position) {
  bool valid_row = position->row < grid->rows;
  bool valid_col = position->column < grid->columns;
  return valid_row && valid_col;
}

bool grid_has_capacity(Grid* const grid) {
  return grid->size < grid->capacity;
}

Result grid_new(unsigned int rows, unsigned int columns) {
  Grid* grid = malloc(sizeof(Grid) + rows * columns * sizeof(void*));

  if (grid == NULL) {
    return result_std_error();
  }

  int e = pthread_rwlock_init(&grid->lock, NULL);
  if (e) {
    free(grid);
    return result_std_error();
  }
  grid->rows = rows;
  grid->columns = columns;
  grid->capacity = rows * columns;
  grid->size = 0;

  for (unsigned int r = 0; r < rows; ++r) {
    void** row  = grid->elements + r * columns;
    for (unsigned int c = 0; c < columns; ++c) {
      row[c] = NULL;
    }
  }

  return result_ok(grid);
}

int grid_clear(Grid* const grid, FreeFn const free_element) {
  int e = pthread_rwlock_trywrlock(&grid->lock);
  if (e) return e;

  for (unsigned int r = 0; r < grid->rows; r++) {
    void** row = grid->elements + r * grid->columns;

    for (unsigned int c = 0; c < grid->columns; c++) {
      void** el = &row[c];

      if (*el != NULL && free_element) {
        free_element(el);
      } else {
        *el = NULL;
      }
    }
  }
  grid->size = 0;

  e = pthread_rwlock_unlock(&grid->lock);
  if (e) return e;

  return 0;
}

int grid_free(Grid** const grid, FreeFn const free_element) {
  grid_clear(*grid, free_element);

  int e = pthread_rwlock_destroy(&(*grid)->lock);
  if (e) return e;

  free(*grid);
  *grid = NULL;

  return 0;
}


int grid_set(Grid* const grid, Position* const position, void* const value) {
  int e = pthread_rwlock_trywrlock(&grid->lock);
  if (e) return e;

  if (!grid_position_valid(grid, position)) {
    e = pthread_rwlock_unlock(&grid->lock);
    if (e) return e;

    return 1;
  }

  void** row = grid->elements + position->row * grid->columns;

  if (row[position->column] == NULL) {
    grid->size++;
  }
  row[position->column] = value;

  e = pthread_rwlock_unlock(&grid->lock);
  if (e) return e;

  return 0;
}

Result grid_get(Grid* const grid, Position* const position) {
  int e = pthread_rwlock_tryrdlock(&grid->lock);
  if (e) return result_std_error();

  if (!grid_position_valid(grid, position)) {
    e = pthread_rwlock_unlock(&grid->lock);
    if (e) return result_std_error();

    return result_error(ERR_CODE_GENERAL, ERR_MSG_GRID_POSITION_INVALID);
  }

  e = pthread_rwlock_unlock(&grid->lock);
  if (e) return result_std_error();

  void** row = grid->elements + position->row * grid->columns;
  return result_ok(row[position->column]);
}

Result grid_remove(Grid* const grid, Position* const position) {
  int e = pthread_rwlock_trywrlock(&grid->lock);
  if (e) return result_std_error();

  if (!grid_position_valid(grid, position)) {
    e = pthread_rwlock_unlock(&grid->lock);
    if (e) return result_std_error();

    return result_error(ERR_CODE_GENERAL, ERR_MSG_GRID_POSITION_INVALID);
  }

  void** row = grid->elements + position->row * grid->columns;
  void* removed = row[position->column];

  row[position->column] = NULL;
  grid->size--;

  e = pthread_rwlock_unlock(&grid->lock);
  if (e) return result_std_error();

  return result_ok(removed);
}


int grid_for_each(Grid* const grid, GridEachFn const each) {
  int e = pthread_rwlock_tryrdlock(&grid->lock);
  if (e) return e;

  if (grid->size == 0) {
    e = pthread_rwlock_unlock(&grid->lock);
    if (e) return e;

    return 0;
  }

  for (unsigned int r = 0; r < grid->rows; ++r) {
    for (unsigned int c = 0; c < grid->columns; ++c) {
      Position p = position_new(r, c);
      Result res = grid_get(grid, &p);
      if (res.error.code != SUC_CODE_GENERAL) {
        fprintf(stderr, res.error.message);
        return res.error.code;
      }

      void* element = res.ok;
      if (element != NULL) {
        each(element);
      }
    }
  }

  e = pthread_rwlock_unlock(&grid->lock);
  if (e) return e;

  return 0;
}

Result grid_map(Grid* const grid, GridMapFn const map) {
  int e = pthread_rwlock_trywrlock(&grid->lock);
  if (e) return result_std_error();

  Result res = grid_new(grid->rows, grid->columns);
  if (res.error.code != SUC_CODE_GENERAL) {
    return res;
  }

  Grid* mapped = res.ok;
  if (mapped == NULL) {
    e = pthread_rwlock_unlock(&grid->lock);
    if (e) return result_std_error();

    return result_std_error();
  }

  if (grid->size == 0) {
    e = pthread_rwlock_unlock(&grid->lock);
    if (e) return result_std_error();

    return result_ok(mapped);
  }

  Position pos;
  for (unsigned int r = 0; r < grid->rows; ++r) {
    void** row = grid->elements + r * grid->columns;
    for (unsigned int c = 0; c < grid->columns; ++c) {
      pos = position_new(r, c);
      void* element = row[c];
      if (element != NULL) {
        void* val = map(element);
        grid_set(mapped, &pos, val);
      }
    }
  }

  e = pthread_rwlock_unlock(&grid->lock);
  if (e) return result_std_error();

  return result_ok(mapped);
}

Result grid_to_string(Grid* const grid, ToStringFn const to_string) {
  int e = pthread_rwlock_trywrlock(&grid->lock);
  if (e) return result_std_error();

  if (grid->size == 0) {
    char* buffer = malloc(sizeof(char) * 3);
    sprintf(buffer, "[]");

    e = pthread_rwlock_unlock(&grid->lock);
    if (e) {
      free(buffer);
      return result_std_error();
    };

    return result_ok(buffer);
  }

  unsigned int rows = grid->rows;
  unsigned int columns = grid->columns;
  unsigned int capacity = grid->capacity;
  char* elements[rows][columns] = {};
  unsigned int lengths[rows][columns] = {};
  unsigned int sum_lengths = 0;

  for (unsigned int r = 0; r < rows; ++r) {
    void** row = grid->elements + r * grid->columns;
    for (unsigned int c = 0; c < columns; ++c) {
      void* element = row[c];
      bool not_null = element != NULL;
      elements[r][c] = not_null ? to_string(element) : "NULL";
      lengths[r][c] = not_null ? strlen(elements[r][c]) : strlen("NULL");
      sum_lengths += lengths[r][c];
    }
  }
  // rows: (2) brackets  + (1) newline
  // capacity: + (2) comma space
  unsigned int total_length = sum_lengths + (rows * 3) + (capacity * 2) - rows;
  char* buffer = malloc(sizeof(char) * total_length);

  if (buffer == NULL) {
    e = pthread_rwlock_unlock(&grid->lock);
    if (e) return result_std_error();

    return result_std_error();
  }

  sprintf(buffer, "[");
  for (unsigned int r = 0; r < rows; ++r) {
    if (r > 0) {
      strcat(buffer, "[");
    }
    for (unsigned int c = 0; c < columns; ++c) {
      strcat(buffer, elements[r][c]);
      if (c < columns - 1) {
        strcat(buffer, ", ");
      }
    }
    strcat(buffer, "]\n\0");
  }

  e = pthread_rwlock_unlock(&grid->lock);
  if (e) {
    free(buffer);
    return result_std_error();
  }

  return result_ok(buffer);
}
