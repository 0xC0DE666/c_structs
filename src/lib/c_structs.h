#include <stdbool.h>

#ifndef C_STRUCTS_H
#define C_STRUCTS_H

  //####################
  // SHARED
  //####################
  typedef char* (*ToStringFn)(void* const);
  typedef void (*FreeFn)(void** const);


  //####################
  // ARRAY
  //####################
  typedef struct Array {
    unsigned capacity;
    unsigned size;
    void** elements;
  } Array;

  Array* array_new(unsigned capacity);
  int array_clear(Array* const array, FreeFn const free_element);
  int array_free(Array** const array, FreeFn const free_element);

  int array_append(Array* const array, void* const element);
  int array_prepend(Array* const array, void* const element);
  int array_insert(Array* const array, int index, void* const element);

  void* array_get(Array* const array, int index);
  void* array_remove(Array* const array, int index);

  char* array_to_string(Array* const array, ToStringFn const to_string);

  bool array_index_valid(Array* const array, int index);
  bool array_has_capacity(Array* const array);


  //####################
  // MATRIX
  //####################
  typedef struct Position {
    int row;
    int column;
  } Position;

  Position position_new(unsigned row, unsigned column);
  char* position_to_string(Position* position);

  typedef struct Matrix {
    unsigned rows;
    unsigned columns;
    unsigned capacity;
    unsigned size;
    void** elements;
  } Matrix;

  Matrix* matrix_new(unsigned rows, unsigned columns);
  int matrix_clear(Matrix* const matrix, FreeFn const free_element);
  void matrix_free(Matrix** const matrix, FreeFn const free_element);

  int matrix_insert(Matrix* const matrix, Position* const position, void* const element);

  void* matrix_get(Matrix* const matrix, Position* const position);
  void* matrix_remove(Matrix* const matrix, Position* const position);

  char* matrix_to_string(Matrix* const matrix, ToStringFn to_string);

  bool matrix_position_valid(Matrix* const matrix, Position* const position);

#endif
