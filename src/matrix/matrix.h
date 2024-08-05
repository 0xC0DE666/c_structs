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
  void matrix_free(Matrix** matrix);
  // char* matrix_to_string(Matrix* matrix);
  int matrix_add(Matrix* matrix, Position* position, void* value);
  void* matrix_get(Matrix* matrix, Position* position);
  void* matrix_remove(Matrix* matrix, Position* position);
  int matrix_clear(Matrix* matrix);
  bool matrix_position_valid(Matrix* matrix, Position* position);

#endif
