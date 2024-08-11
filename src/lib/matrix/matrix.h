#include <stdbool.h>

#ifndef MATRIX_H
#define MATRIX_H

  typedef struct Position {
    int row;
    int column;
  } Position;

  Position position_new(int row, int column);

  typedef struct Matrix {
    int rows;
    int columns;
    int capacity;
    int size;
    void** elements;
  } Matrix;

  Matrix* matrix_new(int rows, int columns);
  void matrix_free(Matrix** const matrix);
  // char* matrix_to_string(Matrix* matrix);
  int matrix_add(Matrix* const matrix, Position* const position, void* const value);
  void* matrix_get(Matrix* const matrix, Position* const position);
  void* matrix_remove(Matrix* const matrix, Position* const position);
  int matrix_clear(Matrix* const matrix);
  bool matrix_position_valid(Matrix* const matrix, Position* const position);

#endif
