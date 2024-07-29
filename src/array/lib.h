#include <stdbool.h>

#ifndef LIB_H
#define LIB_H

  typedef struct Element {
    int index;
    void* value;
  } Element;

  typedef struct Array {
    int capacity;
    int length;
    Element** elements;
  } Array;

  Array* array_new(int capacity);
  char* array_to_string(Array* array);
  int array_add(Array* array, void* value);
  void* array_get(Array* array, int index);
  void* array_remove(Array* array, int index);
  int array_clear(Array* array);
  void array_free(Array** array);

#endif
