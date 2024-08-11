#include <stdbool.h>

#ifndef ARRAY_H
#define ARRAY_H

  typedef struct Array {
    int capacity;
    int size;
    void** elements;
  } Array;

  Array* array_new(int capacity);
  void array_free(Array** array);
  // char* array_to_string(Array* array);
  int array_append(Array* array, void* value);
  int array_prepend(Array* array, void* value);
  int array_insert(Array* array, int index, void* value);

  void* array_get(Array* array, int index);
  void* array_remove(Array* array, int index);
  int array_clear(Array* array);
  bool array_index_valid(Array* array, int index);
  bool array_has_capacity(Array* array);

#endif
