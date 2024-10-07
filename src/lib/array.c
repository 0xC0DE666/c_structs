#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

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

  pthread_mutex_init(&array->lock, NULL);
  array->capacity = capacity;
  array->size = 0;
  array->elements = elements;

  for (int i = 0; i < capacity; ++i) {
    array->elements[i] = NULL;
  }

  return array;
}

int array_clear(Array* const array, FreeFn free_element) {
  pthread_mutex_lock(&array->lock);
  for (int i = 0; i < array->capacity; i++) {
    void** el = &array->elements[i];

    if (*el != NULL && free_element) {
      free_element(el);
    } else {
      *el = NULL;
    }
  }
  array->size = 0;

  pthread_mutex_unlock(&array->lock);
  return 0;
}

int array_free(Array** const array, FreeFn free_element) {
  array_clear(*array, free_element);
  pthread_mutex_lock(&(*array)->lock);

  free((*array)->elements);
  (*array)->elements = NULL;

  pthread_mutex_unlock(&(*array)->lock);
  pthread_mutex_destroy(&(*array)->lock);

  free(*array);
  *array = NULL;

  return 0;
}

int array_append(Array* const array, void* const element) {
  pthread_mutex_lock(&array->lock);
  if (!array_has_capacity(array)) {
    pthread_mutex_unlock(&array->lock);
    return 1;
  }

  array->elements[array->size] = element;
  array->size++;

  pthread_mutex_unlock(&array->lock);
  return 0;
}

int array_prepend(Array* const array, void* const element) {
  pthread_mutex_lock(&array->lock);
  if (!array_has_capacity(array)) {
    pthread_mutex_unlock(&array->lock);
    return 1;
  }

  for (int i = array->size; i >= 1; --i) {
    array->elements[i] = array->elements[i - 1];
  }

  array->elements[0] = element;
  array->size++;

  pthread_mutex_unlock(&array->lock);
  return 0;
}

int array_set(Array* const array, int index, void* const element) {
  if (!array_index_valid(array, index)) {
    pthread_mutex_unlock(&array->lock);
    return 1;
  }

  if (array->elements[index] == NULL) {
    array->size++;
  }
  array->elements[index] = element;

  pthread_mutex_unlock(&array->lock);
  return 0;
}

void* array_get(Array* const array, int index) {
  if (!array_index_valid(array, index)) {
    return NULL;
  }

  return array->elements[index];
}

void* array_remove(Array* const array, int index) {
  pthread_mutex_lock(&array->lock);
  if (!array_index_valid(array, index)) {
    pthread_mutex_unlock(&array->lock);
    return NULL;
  }

  void* removed = array->elements[index];

  for (int i = index; i < array->capacity; ++i) {
    if (i < array->capacity - 1) {
      array->elements[i] = array->elements[i + 1];
    } else {
      array->elements[i] = NULL;
    }
  }
  array->size--;

  pthread_mutex_unlock(&array->lock);
  return removed;
}

void array_for_each(Array* const array, ArrayEachFn each) {
  if (array->size == 0) {
    return;
  }

  for (int i = 0; i < array->capacity; ++i) {
    void* element = array_get(array, i);
    if (element != NULL) {
      each(element);
    }
  }
}

Array* array_map(Array* const array, ArrayMapFn map) {
  Array* mapped = array_new(array->capacity);
  if (mapped == NULL) {
    return NULL;
  }

  if (array->size == 0) {
    return mapped;
  }

  for (int i = 0; i < array->capacity; ++i) {
    void* element = array_get(array, i);
    if (element != NULL) {
      void* val = map(element);
      array_append(mapped, val);
    }
  }
  
  return mapped;
}

char* array_to_string(Array* const array, ToStringFn const to_string) {
  if (array->size == 0) {
    char* buffer = malloc(sizeof(char) * 3);
    sprintf(buffer, "[]\0");
    return buffer;
  }

  int capacity = array->capacity;
  char* elements[capacity] = {};
  int lengths[capacity] = {};
  int sum_lengths = 0;

  for (int i = 0; i < capacity; ++i) {
    void* element = array_get(array, i);
    elements[i] = element != NULL ? to_string(element) : "NULL";
    lengths[i] = strlen(elements[i]);
    sum_lengths += lengths[i];
  }

  int total_length = sum_lengths + (capacity - 1) * 2 + 2;
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

bool array_index_valid(Array* const array, int index) {
  return index >= 0 && index <= array->capacity - 1;
}

bool array_has_capacity(Array* const array) {
  return array->size < array->capacity;
}
