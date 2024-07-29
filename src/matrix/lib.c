#include <stdlib.h>
#include <stdio.h>

#include "lib.h"

Matrix* matrix_new(int capacity) {
  Matrix* matrix = malloc(sizeof(Matrix));

  if (matrix == NULL) {
    return NULL;
  }

  Element** elements = malloc(capacity * sizeof(Element*));

  if (elements == NULL) {
    free(matrix);
    return NULL;
  }

  matrix->capacity = capacity;
  matrix->length = 0;
  matrix->elements = elements;

  for (int i = 0; i < capacity; ++i) {
    matrix->elements[i] = NULL;
  }

  return matrix;
}

char* matrix_to_string(Matrix* matrix) {
  char* buffer = malloc(sizeof(char) * 256);

  sprintf(
    buffer,
    "capacity = %d\nlength = %d\n",
    matrix->capacity,
    matrix->length
  ); 

  return buffer;
}

int matrix_add(Matrix* matrix, void* value) {
  if (matrix->length >= matrix->capacity) {
    return 1;
  }

  Element* e = malloc(sizeof(Element));

  if (e == NULL) {
    return 1;
  }

  e->index = matrix->length;
  e->value = value;

  matrix->elements[matrix->length] = e;
  matrix->length++;

  return 0;
}

void* matrix_get(Matrix* matrix, int index) {
  if (index < 0 || index >= matrix->length) {
    return NULL;
  }

  return matrix->elements[index]->value;
}

void* matrix_remove(Matrix* matrix, int index) {
  if (index < 0 || index >= matrix->length) {
    return NULL;
  }

  Element* removed = matrix->elements[index];

  for (int i = index; i < matrix->length; i++) {
    if (i < matrix->length - 1) {
      matrix->elements[i] = matrix->elements[i + 1];
      matrix->elements[i]->index = i;
    } else {
      matrix->elements[i] = NULL;
    }
  }
  matrix->length--;

  return removed->value;
}

int matrix_clear(Matrix* matrix) {
  for (int i = 0; i < matrix->capacity; i++) {
    if (matrix->elements[i] != NULL) {
      free(matrix->elements[i]);
      matrix->elements[i] = NULL;
    }
  }
  matrix->length = 0;

  return 0;
}

void matrix_free(Matrix** matrix) {
  for (int i = 0; i < (*matrix)->capacity; ++i) {
    if ((*matrix)->elements[i] == NULL) {
      continue;
    }
    free((*matrix)->elements[i]);
    (*matrix)->elements[i] = NULL;
  }

  free((*matrix)->elements);
  (*matrix)->elements = NULL;

  free(*matrix);
  *matrix = NULL;
}
