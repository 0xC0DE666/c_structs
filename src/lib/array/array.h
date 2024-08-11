#include <stdbool.h>

#ifndef ARRAY_H
#define ARRAY_H

  typedef struct Array {
    int capacity;
    int size;
    void** elements;
  } Array;

  Array* array_new(int capacity);
  void array_free(Array** const array);
  // char* array_to_string(Array* array);
  int array_append(Array* const array, void* const  value);
  int array_prepend(Array* const array, void* const value);
  int array_insert(Array* const array, int index, void* const value);

  void* array_get(Array* const array, int index);
  void* array_remove(Array* const array, int index);
  int array_clear(Array* const array);
  bool array_index_valid(Array* const array, int index);
  bool array_has_capacity(Array* const array);

#endif
