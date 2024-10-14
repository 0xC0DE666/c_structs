#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "c_structs.h"

Position position_new(int row, int column) {
  return (Position) {row, column};
}

char* position_to_string(Position* position) {
  char* buffer = malloc(14);

  sprintf(
    buffer,
    "(%d, %d)",
    position->row,
    position->column
  );

  return buffer;
}

bool matrix_position_valid(Matrix* const matrix, Position* const position) {
  bool valid_row = position->row >= 0 && position->row < matrix->rows;
  bool valid_col = position->column >= 0 && position->column < matrix->columns;
  return valid_row && valid_col;
}

bool matrix_has_capacity(Matrix* const matrix) {
  return matrix->size < matrix->capacity;
}

Result matrix_new(int rows, int columns) {
  Matrix* matrix = malloc(sizeof(Matrix) + rows * columns * sizeof(void*));

  if (matrix == NULL) {
    return fail(1, ERR_MALLOC_FAILED);
  }

  int e = pthread_rwlock_init(&matrix->lock, NULL);
  if (e) {
    free(matrix);
    return fail(e, ERR_RWLOCK_INIT_FAILED);
  }
  matrix->rows = rows;
  matrix->columns = columns;
  matrix->capacity = rows * columns;
  matrix->size = 0;

  for (int r = 0; r < rows; ++r) {
    void** row  = matrix->elements + r * columns;
    for (int c = 0; c < columns; ++c) {
      row[c] = NULL;
    }
  }

  return success(matrix);
}

int matrix_clear(Matrix* const matrix, FreeFn const free_element) {
  int e = pthread_rwlock_trywrlock(&matrix->lock);
  if (e) return e;

  for (int r = 0; r < matrix->rows; r++) {
    void** row = matrix->elements + r * matrix->columns;

    for (int c = 0; c < matrix->columns; c++) {
      void** el = &row[c];

      if (*el != NULL && free_element) {
        free_element(el);
      } else {
        *el = NULL;
      }
    }
  }
  matrix->size = 0;

  e = pthread_rwlock_unlock(&matrix->lock);
  if (e) return e;

  return 0;
}

int matrix_free(Matrix** const matrix, FreeFn const free_element) {
  matrix_clear(*matrix, free_element);

  int e = pthread_rwlock_destroy(&(*matrix)->lock);
  if (e) return e;

  free(*matrix);
  *matrix = NULL;

  return 0;
}


int matrix_set(Matrix* const matrix, Position* const position, void* const value) {
  int e = pthread_rwlock_trywrlock(&matrix->lock);
  if (e) return e;

  if (!matrix_position_valid(matrix, position)) {
    e = pthread_rwlock_unlock(&matrix->lock);
    if (e) return e;

    return 1;
  }

  void** row = matrix->elements + position->row * matrix->columns;

  if (row[position->column] == NULL) {
    matrix->size++;
  }
  row[position->column] = value;

  e = pthread_rwlock_unlock(&matrix->lock);
  if (e) return e;

  return 0;
}

Result matrix_get(Matrix* const matrix, Position* const position) {
  int e = pthread_rwlock_tryrdlock(&matrix->lock);
  if (e) return fail(e, ERR_RDLOCK_FAILED);

  if (!matrix_position_valid(matrix, position)) {
    e = pthread_rwlock_unlock(&matrix->lock);
    if (e) return fail(e, ERR_RWLOCK_UNLOCK_FAILED);

    return fail(1, ERR_INVALID_POSITION);
  }

  e = pthread_rwlock_unlock(&matrix->lock);
  if (e) return fail(e, ERR_RWLOCK_UNLOCK_FAILED);

  void** row = matrix->elements + position->row * matrix->columns;
  return success(row[position->column]);
}

Result matrix_remove(Matrix* const matrix, Position* const position) {
  int e = pthread_rwlock_trywrlock(&matrix->lock);
  if (e) return fail(e, ERR_WRLOCK_FAILED); 

  if (!matrix_position_valid(matrix, position)) {
    e = pthread_rwlock_unlock(&matrix->lock);
    if (e) return fail(e, ERR_RWLOCK_UNLOCK_FAILED);

    return fail(1, ERR_INVALID_POSITION);
  }

  void** row = matrix->elements + position->row * matrix->columns;
  void* removed = row[position->column];

  row[position->column] = NULL;
  matrix->size--;

  e = pthread_rwlock_unlock(&matrix->lock);
  if (e) return fail(e, ERR_RWLOCK_UNLOCK_FAILED);

  return success(removed);
}


void matrix_for_each(Matrix* const matrix, MatrixEachFn const each) {
  if (matrix->size == 0) {
    return;
  }

  for (int r = 0; r < matrix->rows; ++r) {
    for (int c = 0; c < matrix->columns; ++c) {
      Position p = position_new(r, c);
      void* element = matrix_get(matrix, &p).ok;
      if (element != NULL) {
        each(element);
      }
    }
  }
}

Matrix* matrix_map(Matrix* const matrix, MatrixMapFn const map) {
  Matrix* mapped = matrix_new(matrix->rows, matrix->columns).ok;
  if (mapped == NULL) {
    return NULL;
  }

  if (matrix->size == 0) {
    return mapped;
  }

  Position pos;
  for (int r = 0; r < matrix->rows; ++r) {
    for (int c = 0; c < matrix->columns; ++c) {
      pos = position_new(r, c);
      void* element = matrix_get(matrix, &pos).ok;
      if (element != NULL) {
        void* val = map(element);
        matrix_set(mapped, &pos, val);
      }
    }
  }

  return mapped;
}

char* matrix_to_string(Matrix* const matrix, ToStringFn const to_string) {
  if (matrix->size == 0) {
    char* buffer = malloc(sizeof(char) * 3);
    sprintf(buffer, "[]\0");
    return buffer;
  }

  int rows = matrix->rows;
  int columns = matrix->columns;
  int capacity = matrix->capacity;
  char* elements[rows][columns] = {};
  int lengths[rows][columns] = {};
  int sum_lengths = 0;

  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < columns; ++c) {
      Position p = position_new(r, c);
      void* element = matrix_get(matrix, &p).ok;
      bool not_null = element != NULL;
      elements[r][c] = not_null ? to_string(element) : "NULL";
      lengths[r][c] = not_null ? strlen(elements[r][c]) : strlen("NULL");
      sum_lengths += lengths[r][c];
    }
  }
  // rows: (2) brackets  + (1) newline
  // capacity: + (2) comma space
  int total_length = sum_lengths + (rows * 3) + (capacity * 2) - rows;
  char* buffer = malloc(sizeof(char) * total_length);

  if (buffer == NULL) {
    return NULL;
  }

  sprintf(buffer, "[");
  for (int r = 0; r < rows; ++r) {
    if (r > 0) {
      strcat(buffer, "[");
    }
    for (int c = 0; c < columns; ++c) {
      strcat(buffer, elements[r][c]);
      if (c < columns - 1) {
        strcat(buffer, ", ");
      }
    }
    strcat(buffer, "]\n\0");
  }

  return buffer;
}
