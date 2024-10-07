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

Matrix* matrix_new(int rows, int columns) {
  Matrix* matrix = malloc(sizeof(Matrix));

  if (matrix == NULL) {
    return NULL;
  }

  void** ptr_rows =  malloc(rows * sizeof(void*));

  if (ptr_rows == NULL) {
    free(matrix);
    return NULL;
  }

  void** ptr_columns = malloc(rows * columns * sizeof(void*));

  if (ptr_columns == NULL) {
    free(matrix);
    free(ptr_rows);
    return NULL;
  }

  for (int r = 0; r < rows; ++r) {
    ptr_rows[r] = ptr_columns + (r * columns);

    void** row = ptr_rows[r];
    for (int c = 0; c < columns; ++c) {
      row[c] = NULL;
    }
  }

  pthread_mutex_init(&matrix->lock, NULL);
  matrix->size = 0;
  matrix->capacity = rows * columns;
  matrix->rows = rows;
  matrix->columns = columns;
  matrix->elements = ptr_rows;

  return matrix;
}

int matrix_clear(Matrix* const matrix, FreeFn const free_element) {
  pthread_mutex_lock(&matrix->lock);
  for (int r = 0; r < matrix->rows; r++) {
    void** row = matrix->elements[r];

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

  pthread_mutex_unlock(&matrix->lock);
  return 0;
}

int matrix_free(Matrix** const matrix, FreeFn const free_element) {
  matrix_clear(*matrix, free_element);
  pthread_mutex_lock(&(*matrix)->lock);

  free((*matrix)->elements[0]);
  (*matrix)->elements[0] = NULL;

  free((*matrix)->elements);
  (*matrix)->elements = NULL;

  pthread_mutex_unlock(&(*matrix)->lock);
  pthread_mutex_destroy(&(*matrix)->lock);

  free(*matrix);
  *matrix = NULL;

  return 0;
}


int matrix_set(Matrix* const matrix, Position* const position, void* const value) {
  pthread_mutex_lock(&matrix->lock);

  if (!matrix_position_valid(matrix, position)) {
    pthread_mutex_unlock(&matrix->lock);
    return 1;
  }

  void** row = matrix->elements[position->row];

  if (row[position->column] == NULL) {
    matrix->size++;
  }
  row[position->column] = value;


  pthread_mutex_unlock(&matrix->lock);
  return 0;
}

void* matrix_get(Matrix* const matrix, Position* const position) {
  if (!matrix_position_valid(matrix, position)) {
    return NULL;
  }

  void** row = matrix->elements[position->row];
  return row[position->column];
}

void* matrix_remove(Matrix* const matrix, Position* const position) {
  pthread_mutex_lock(&matrix->lock);
  if (!matrix_position_valid(matrix, position)) {
    pthread_mutex_unlock(&matrix->lock);
    return NULL;
  }

  void** row = matrix->elements[position->row];
  void* removed = row[position->column];

  row[position->column] = NULL;
  matrix->size--;

  pthread_mutex_unlock(&matrix->lock);
  return removed;
}


void matrix_for_each(Matrix* const matrix, MatrixEachFn const each) {
  if (matrix->size == 0) {
    return;
  }

  for (int r = 0; r < matrix->rows; ++r) {
    for (int c = 0; c < matrix->columns; ++c) {
      Position p = position_new(r, c);
      void* element = matrix_get(matrix, &p);
      if (element != NULL) {
        each(element);
      }
    }
  }
}

Matrix* matrix_map(Matrix* const matrix, MatrixMapFn const map) {
  Matrix* mapped = matrix_new(matrix->rows, matrix->columns);
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
      void* element = matrix_get(matrix, &pos);
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
      void* element = matrix_get(matrix, &p);
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


bool matrix_position_valid(Matrix* const matrix, Position* const position) {
  bool valid_row = position->row >= 0 && position->row < matrix->rows;
  bool valid_col = position->column >= 0 && position->column < matrix->columns;
  return valid_row && valid_col;
}

bool matrix_has_capacity(Matrix* const matrix) {
  return matrix->size < matrix->capacity;
}
