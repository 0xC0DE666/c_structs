#include <stdlib.h>
#include <stdio.h>

#include "lib.h"

int read_line(char buffer[], int size) {
  int i, c;
  for (i = 0; i < size - 1 && (c = getchar()) != '\n' && c != EOF; ++i) {
    buffer[i] = c;
  }
  buffer[i] = '\0';
  return i;
}

int string_length(char buffer[]) {
  int i = 0;
  while (buffer[i]) {
    ++i;
  }
  return i;
}

Array* array_new(int size) {
  Array* array = malloc(sizeof(Array));

  if (array == NULL) {
    return NULL;
  }

  Element* elements = malloc(sizeof(Element) * size);

  if (elements == NULL) {
    free(array);
    return NULL;
  }

  array->size = size;
  array->count = 0;
  array->elements = elements;

  return array;
}

char* array_to_string(Array* array) {
  char* buffer = malloc(sizeof(char) *  256);

  sprintf(
    buffer,
    "size = %d\ncount = %d\n",
    array->size,
    array->count
  ); 

  return buffer;
}

int array_add(Array* array, void* value) {
  if (array->count >= array->size) {
    return 1;
  }

  array->elements[array->count].index = array->count;
  array->elements[array->count].value = value;
  array->count++;

  return 0;
}

void* array_get(Array* array, int index) {
  if (index < 0 || index >= array->count) {
    return NULL;
  }

  return array->elements[index].value;
}

void* array_remove(Array* array, int index) {
  if (index < 0 || index >= array->count) {
    return NULL;
  }

  Element* removed = (array->elements + index);

  for (int i = index; i < array->count; i++) {
    if (i < array->count - 1) {
      array->elements[i] = array->elements[i + 1];
      array->elements[i].index = i;
    } else {
      // array->elements[i] = {};
    }
  }
  array->count--;

  return removed->value;
}
