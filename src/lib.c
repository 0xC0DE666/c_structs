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
  //printf("*array memory allocated\n");
  Element* elements = malloc(sizeof(Element) * size);
  //printf("*array->values memory allocated\n");

  array->size = size;
  array->count = 0;
  array->elements = elements;
  //printf("*array initialised\n");

  return array;
}

char* array_to_str(Array* array) {
  char* buffer = malloc(sizeof(char)*  256);
  sprintf(
    buffer,
    "size = %d\ncount = %d\n",
    array->size,
    array->count
  ); 
  return buffer;
}

int array_add(Array* array, void* value) {
  if (array->count < array->size) {
    Element new = {array->count, value};
    array->elements[array->count] = new;
    array->count++;
    return 0;
  }
  return 1;
}
