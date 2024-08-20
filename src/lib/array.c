#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "c_structs.h"

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

int array_clear(Array* const array, FreeFn free_element) {
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

int array_free(Array** const array, FreeFn free_element) {
  array_clear(*array, free_element);

  free((*array)->elements);
  (*array)->elements = NULL;

  free(*array);
  *array = NULL;

  return 0;
}

// 1. try malloc char** for size of array
// 2. for each element in array call to_string on element and assign to pointer above
// 3. for each string get strlen and add to total
// 4. try malloc for total strlen
// 5. concat everything
// 6. return char*

char* array_to_string(Array* const array, ToStringFn const to_string) {
  int capacity = array->capacity;
  char* elements[capacity] = {};
  int lengths[capacity] = {};
  int sum_lengths = 0;


  for (int i = 0; i < capacity; ++i) {
    void* element = array_get(array, i);
    if (element != NULL) {
      elements[i] = to_string(element);
    } else {
      elements[i] = "";
    }
    lengths[i] = strlen(elements[i]);
    sum_lengths += lengths[i];
  }

  // sum_lengths + capacity * 3 + 2 brackets + 2 newl + 1 null
  int total_chars = sum_lengths + (3 * capacity) + 5;
  printf("total_chars = %d\n", total_chars);
  char* buffer = malloc(sizeof(char) * total_chars);

  if (buffer == NULL) {
    return NULL;
  }

  strcat(buffer, "[\n");
  for (int i = 0; i < capacity; ++i) {
    strcat(buffer, "\t");
    strcat(buffer, elements[i]);
    strcat(buffer, ",\n");
  }
  strcat(buffer, "]\n\0");

  return buffer;
}

int array_append(Array* const array, void* const element) {
  if (!array_has_capacity(array)) {
    return 1;
  }

  array->elements[array->size] = element;
  array->size++;

  return 0;
}

int array_prepend(Array* const array, void* const element) {
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

int array_insert(Array* const array, int index, void* const element) {
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

void* array_get(Array* const array, int index) {
  if (!array_index_valid(array, index)) {
    return NULL;
  }

  return array->elements[index];
}

void* array_remove(Array* const array, int index) {
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

bool array_index_valid(Array* const array, int index) {
  return index >= 0 && index <= array->capacity - 1;
}

bool array_has_capacity(Array* const array) {
  return array->size < array->capacity;
}
