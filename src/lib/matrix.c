#include <stdlib.h>
#include <stdio.h>

#include "c_structs.h"

Position position_new(unsigned int row, unsigned int column) {
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

Matrix* matrix_new(unsigned int rows, unsigned int columns) {
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

  matrix->capacity = rows * columns;
  matrix->rows = rows;
  matrix->columns = columns;
  matrix->elements = ptr_rows;

  return matrix;
}

unsigned int matrix_clear(Matrix* const matrix, FreeFn const free_element) {
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

  return 0;
}

void matrix_free(Matrix** const matrix, FreeFn const free_element) {
  matrix_clear(*matrix, free_element);

  free((*matrix)->elements[0]);
  (*matrix)->elements[0] = NULL;

  free((*matrix)->elements);
  (*matrix)->elements = NULL;

  free(*matrix);
  *matrix = NULL;
}

// char* matrix_to_string(Matrix* matrix) {
//   char* buffer = malloc(sizeof(char) * 256);
// 
//   sprintf(
//     buffer,
//     "capacity = %d\nlength = %d\n",
//     matrix->capacity,
//     matrix->length
//   ); 
// 
//   return buffer;
// }
// 

unsigned int matrix_add(Matrix* const matrix, Position* const position, void* const value) {
  if (matrix->size >= matrix->capacity) {
    return 1;
  }

  if (matrix_position_valid(matrix, position) == false) {
    return 1;
  }

  void** row = matrix->elements[position->row];
  row[position->column] = value;

  matrix->size++;

  return 0;
}

void* matrix_get(Matrix* const matrix, Position* const position) {
  if (matrix_position_valid(matrix, position) == false) {
    return NULL;
  }

  void** row = matrix->elements[position->row];
  return row[position->column];
}

void* matrix_remove(Matrix* const matrix, Position* const position) {
  if (matrix_position_valid(matrix, position) == false) {
    return NULL;
  }

  void** row = matrix->elements[position->row];
  void* removed = row[position->column];

  row[position->column] = NULL;
  matrix->size--;

  return removed;
}


bool matrix_position_valid(Matrix* const matrix, Position* const position) {
  if (position->row < 0 || position->row >= matrix->rows || position->column < 0 || position->column >= matrix->columns) {
    return false;
  }
  return true;
}
