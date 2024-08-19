#include <stdbool.h>

#ifndef ARRAY_H
#define ARRAY_H

  typedef struct Array {
    int capacity;
    int size;
    void** elements;
  } Array;

  typedef void (*FreeFn)(void** const);

  Array* array_new(int capacity);
  int array_clear(Array* const array, FreeFn const free_value);
  int array_free(Array** const array, FreeFn const free_value);

  // char* array_to_string(Array* array);
  int array_append(Array* const array, void* const value);
  int array_prepend(Array* const array, void* const value);
  int array_insert(Array* const array, int index, void* const value);

  void* array_get(Array* const array, int index);
  void* array_remove(Array* const array, int index);

  bool array_index_valid(Array* const array, int index);
  bool array_has_capacity(Array* const array);

#endif
