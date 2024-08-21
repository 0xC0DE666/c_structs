#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "c_structs.h"

Array* array_new(unsigned int capacity) {
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

unsigned int array_clear(Array* const array, FreeFn free_element) {
  for (int i = 0; i < array->capacity; i++) {
    void** ptr = &array->elements[i];
    if (*ptr != NULL && free_element) {
      free_element(ptr);
    } else {
      array->elements[i] = NULL;
    }
  }
  array->size = 0;

  return 0;
}

unsigned int array_free(Array** const array, FreeFn free_element) {
  array_clear(*array, free_element);

  free((*array)->elements);
  (*array)->elements = NULL;

  free(*array);
  *array = NULL;

  return 0;
}

unsigned int array_append(Array* const array, void* const element) {
  if (!array_has_capacity(array)) {
    return 1;
  }

  array->elements[array->size] = element;
  array->size++;

  return 0;
}

unsigned int array_prepend(Array* const array, void* const element) {
  if (!array_has_capacity(array)) {
    return 1;
  }

  for (int i = array->size; i >= 1; --i) {
    array->elements[i] = array->elements[i - 1];
  }

  array->elements[0] = element;
  array->size++;

  return 0;
}

unsigned int array_insert(Array* const array, unsigned int index, void* const element) {
  if (!array_has_capacity(array)) {
    return 1;
  }

  if (!array_index_valid(array, index)) {
    return 1;
  }

  bool open_spot = false;
  for (int i = index; i < array->capacity; ++i) {
    if (array->elements[i] == NULL) {
      open_spot = true;
      break;
    }
  }

  if (!open_spot) {
    return 1;
  }

  for (int i = array->capacity - 1; i >= index; --i) {
    array->elements[i] = array->elements[i - 1];
  }
  array->elements[index] = element;
  array->size++;

  return 0;
}

void* array_get(Array* const array, unsigned int index) {
  if (!array_index_valid(array, index)) {
    return NULL;
  }

  return array->elements[index];
}

void* array_remove(Array* const array, unsigned int index) {
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

char* array_to_string(Array* const array, ToStringFn const to_string) {
  if (array->size == 0) {
    char* buffer = malloc(sizeof(char) * 3);
    sprintf(buffer, "[]\0");
    return buffer;
  }

  unsigned int capacity = array->capacity;
  char* elements[capacity] = {};
  unsigned int lengths[capacity] = {};
  unsigned int sum_lengths = 0;

  for (int i = 0; i < capacity; ++i) {
    void* element = array_get(array, i);
    elements[i] = element != NULL ? to_string(element) : "NULL";
    lengths[i] = strlen(elements[i]);
    sum_lengths += lengths[i];
  }

  unsigned int total_length = sum_lengths + (capacity - 1) * 2 + 2;
  char* buffer = malloc(sizeof(char) * total_length);

  if (buffer == NULL) {
    return NULL;
  }

  sprintf(buffer, "[");
  for (int i = 0; i < capacity; ++i) {
    strcat(buffer, elements[i]);
    if (i < capacity - 1) {
      strcat(buffer, ", ");
    }
  }
  strcat(buffer, "]\0");

  return buffer;
}

bool array_index_valid(Array* const array, unsigned int index) {
  return index >= 0 && index <= array->capacity - 1;
}

bool array_has_capacity(Array* const array) {
  return array->size < array->capacity;
}
