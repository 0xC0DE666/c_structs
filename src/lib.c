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

Array *array_new(int size) {
  Array *array = malloc(sizeof(Array));
  //printf("*array memory allocated\n");
  array->values = malloc(sizeof(int) * size);
  //printf("*array->values memory allocated\n");

  array->size = size;
  array->count = 0;
  array->values = NULL;
  //printf("*array initialised\n");

  return array;
}
