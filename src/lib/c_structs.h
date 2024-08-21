#include <stdbool.h>

#ifndef C_STRUCTS_H
#define C_STRUCTS_H

  //####################
  // SHARED
  //####################
  typedef char* (*ToStringFn)(void* const);
  typedef void (*FreeFn)(void** const);

  typedef struct Array {
    unsigned int capacity;
    unsigned int size;
    void** elements;
  } Array;


  //####################
  // ARRAY
  //####################
  Array* array_new(unsigned int capacity);
  unsigned int array_clear(Array* const array, FreeFn const free_element);
  unsigned int array_free(Array** const array, FreeFn const free_element);

  unsigned int array_append(Array* const array, void* const element);
  unsigned int array_prepend(Array* const array, void* const element);
  unsigned int array_insert(Array* const array, unsigned int index, void* const element);

  void* array_get(Array* const array, unsigned int index);
  void* array_remove(Array* const array, unsigned int index);

  char* array_to_string(Array* array, ToStringFn const to_string);

  bool array_index_valid(Array* const array, unsigned int index);
  bool array_has_capacity(Array* const array);


  //####################
  // MATRIX
  //####################
  typedef struct Position {
    unsigned int row;
    unsigned int column;
  } Position;

  Position position_new(unsigned int row, unsigned int column);
  char* position_to_string(Position* position);

  typedef struct Matrix {
    unsigned int rows;
    unsigned int columns;
    unsigned int capacity;
    unsigned int size;
    void** elements;
  } Matrix;

  Matrix* matrix_new(unsigned int rows, unsigned int columns);
  unsigned int matrix_clear(Matrix* const matrix, FreeFn const free_element);
  void matrix_free(Matrix** const matrix);

  unsigned int matrix_add(Matrix* const matrix, Position* const position, void* const element);

  void* matrix_get(Matrix* const matrix, Position* const position);
  void* matrix_remove(Matrix* const matrix, Position* const position);

  // char* matrix_to_string(Matrix* matrix);

  bool matrix_position_valid(Matrix* const matrix, Position* const position);

#endif
