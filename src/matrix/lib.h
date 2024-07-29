#include <stdbool.h>

#ifndef LIB_H
#define LIB_H

  typedef struct {
    int row;
    int column;
  } Position;

  typedef struct Matrix {
    int rows;
    int columns;
    int capacity;
    int size;
    void** elements;
  } Matrix;

  Matrix* matrix_new(int rows, int columns);
  void matrix_free(Matrix** matrix);
  char* matrix_to_string(Matrix* matrix);
  int matrix_add(Matrix* matrix, Position* position, void* value);
  void* matrix_get(Matrix* matrix, Position* position);
  void* matrix_remove(Matrix* matrix, Position* position);
  int matrix_clear(Matrix* matrix);

#endif
