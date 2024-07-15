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

Array *array_new(int capacity) {
  Array *array = malloc(sizeof(Array));

  if (array == NULL) {
    return NULL;
  }

  Element **elements = malloc(capacity * sizeof(Element *));

  if (elements == NULL) {
    free(array);
    return NULL;
  }

  array->capacity = capacity;
  array->length = 0;
  array->elements = elements;

  return array;
}

char *array_to_string(Array *array) {
  char *buffer = malloc(sizeof(char) *  256);

  sprintf(
    buffer,
    "capacity = %d\nlength = %d\n",
    array->capacity,
    array->length
  ); 

  return buffer;
}

int array_add(Array *array, void *value) {
  if (array->length >= array->capacity) {
    return 1;
  }

  Element *e = malloc(sizeof(Element));

  if (e == NULL) {
    return 1;
  }

  e->index = array->length;
  e->value = value;

  array->elements[array->length] = e;
  array->length++;

  return 0;
}

void *array_get(Array *array, int index) {
  if (index < 0 || index >= array->length) {
    return NULL;
  }

  return array->elements[index]->value;
}

// void *array_remove(Array *array, int index) {
//   if (index < 0 || index >= array->length) {
//     return NULL;
//   }
// 
//   Element *removed = (array->elements + index);
// 
//   for (int i = index; i < array->length; i++) {
//     if (i < array->length - 1) {
//       array->elements[i] = array->elements[i + 1];
//       array->elements[i].index = i;
//     } else {
//       array->elements[i] = (Element){};
//     }
//   }
//   array->length--;
// 
//   return removed->value;
// }
