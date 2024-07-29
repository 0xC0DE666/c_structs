#include <stdbool.h>

#ifndef LIB_H
#define LIB_H

  typedef struct Matrix {
    int capacity;
    int rows;
    int columns;
    void** elements;
  } Matrix;

  Matrix* matrix_new(int rows, int columns);
  void matrix_free(Matrix** matrix);
  char* matrix_to_string(Matrix* matrix);
  int matrix_add(Matrix* matrix, void* value);
  void* matrix_get(Matrix* matrix, int index);
  void* matrix_remove(Matrix* matrix, int index);
  int matrix_clear(Matrix* matrix);

#endif
