#include <stdbool.h>

#ifndef C_STRUCTS_H
#define C_STRUCTS_H

  //####################
  // SHARED
  //####################
  typedef char* (*ToStringFn)(void* const);
  typedef void (*FreeFn)(void** const);

  typedef struct Array {
    int capacity;
    int size;
    void** elements;
  } Array;


  //####################
  // ARRAY
  //####################
  Array* array_new(int capacity);
  int array_clear(Array* const array, FreeFn const free_element);
  int array_free(Array** const array, FreeFn const free_element);

  char* array_to_string(Array* array, ToStringFn const to_string);
  int array_append(Array* const array, void* const element);
  int array_prepend(Array* const array, void* const element);
  int array_insert(Array* const array, int index, void* const element);

  void* array_get(Array* const array, int index);
  void* array_remove(Array* const array, int index);

  bool array_index_valid(Array* const array, int index);
  bool array_has_capacity(Array* const array);


  //####################
  // MATRIX
  //####################
  typedef struct Position {
    int row;
    int column;
  } Position;

  Position position_new(int row, int column);
  char* position_to_string(Position* position);

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
  int matrix_add(Matrix* const matrix, Position* const position, void* const element);
  void* matrix_get(Matrix* const matrix, Position* const position);
  void* matrix_remove(Matrix* const matrix, Position* const position);
  int matrix_clear(Matrix* const matrix);
  bool matrix_position_valid(Matrix* const matrix, Position* const position);

#endif
