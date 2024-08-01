#include <stdlib.h>
#include <stdio.h>

#include "lib.h"

Array* array_new(int capacity) {
  Array* array = malloc(sizeof(Array));

  if (array == NULL) {
    return NULL;
  }

  void** elements = malloc(capacity * sizeof(void*));

  if (elements == NULL) {
    free(array);
    return NULL;
  }

  array->capacity = capacity;
  array->size = 0;
  array->elements = elements;

  for (int i = 0; i < capacity; ++i) {
    array->elements[i] = NULL;
  }

  return array;
}

void array_free(Array** array) {
  for (int i = 0; i < (*array)->capacity; ++i) {
    (*array)->elements[i] = NULL;
  }

  free((*array)->elements);
  (*array)->elements = NULL;

  free(*array);
  *array = NULL;
}

char* array_to_string(Array* array) {
  char* buffer = malloc(sizeof(char) * 256);

  sprintf(
    buffer,
    "capacity = %d\nsize = %d\n",
    array->capacity,
    array->size
  ); 

  return buffer;
}

int array_append(Array* array, void* value) {
  if (array->size >= array->capacity) {
    return 1;
  }

  array->elements[array->size] = value;
  array->size++;

  return 0;
}

void* array_get(Array* array, int index) {
  if (index < 0 || index >= array->size) {
    return NULL;
  }

  return array->elements[index];
}

void* array_remove(Array* array, int index) {
  if (index < 0 || index >= array->size) {
    return NULL;
  }

  void* removed = array->elements[index];

  for (int i = index; i < array->size; i++) {
    if (i < array->size - 1) {
      array->elements[i] = array->elements[i + 1];
    } else {
      array->elements[i] = NULL;
    }
  }
  array->size--;

  return removed;
}

int array_clear(Array* array) {
  for (int i = 0; i < array->capacity; i++) {
    array->elements[i] = NULL;
  }
  array->size = 0;

  return 0;
}
