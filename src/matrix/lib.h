#include <stdbool.h>

#ifndef LIB_H
#define LIB_H

  typedef struct Element {
    int index;
    void* value;
  } Element;

  typedef struct Matrix {
    int capacity;
    int rows;
    int columns;
    int length;
    Element** elements;
  } Matrix;

  Matrix* matrix_new(int capacity);
  char* matrix_to_string(Matrix* matrix);
  int matrix_add(Matrix* matrix, void* value);
  void* matrix_get(Matrix* matrix, int index);
  void* matrix_remove(Matrix* matrix, int index);
  int matrix_clear(Matrix* matrix);
  void matrix_free(Matrix** matrix);

#endif
