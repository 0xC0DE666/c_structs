#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "c_structs.h"

bool array_index_valid(Array* const array, int index) {
  return index >= 0 && index <= array->capacity - 1;
}

bool array_has_capacity(Array* const array) {
  return array->size < array->capacity;
}

Result array_new(int capacity) {
  Array* array = malloc(sizeof(Array) + capacity * sizeof(void*));

  if (array == NULL) {
    return fail(1, ERR_MALLOC_FAILED);
  }

  int e = pthread_rwlock_init(&array->lock, NULL);
  if (e) {
    free(array);
    return fail(e, ERR_RWLOCK_INIT_FAILED);
  }
  array->capacity = capacity;
  array->size = 0;

  for (int i = 0; i < capacity; ++i) {
    array->elements[i] = NULL;
  }

  return success(array);
}

int array_clear(Array* const array, FreeFn free_element) {
  int e = pthread_rwlock_trywrlock(&array->lock);
  if (e) return e;

  for (int i = 0; i < array->capacity; i++) {
    void** el = array->elements + i;

    if (*el != NULL && free_element) {
      free_element(el);
    } else {
      *el = NULL;
    }
  }
  array->size = 0;

  e = pthread_rwlock_unlock(&array->lock);
  if (e) return e;

  return 0;
}

int array_free(Array** const array, FreeFn free_element) {
  array_clear(*array, free_element);

  int e = pthread_rwlock_destroy(&(*array)->lock);
  if (e) return e;

  free(*array);
  *array = NULL;

  return 0;
}

int array_append(Array* const array, void* const element) {
  int e = pthread_rwlock_trywrlock(&array->lock);
  if (e) return e;

  if (!array_has_capacity(array)) {
    e = pthread_rwlock_unlock(&array->lock);
    if (e) return e;

    return 1;
  }

  array->elements[array->size] = element;
  array->size++;

  e = pthread_rwlock_unlock(&array->lock);
  if (e) return e;

  return 0;
}

int array_prepend(Array* const array, void* const element) {
  int e = pthread_rwlock_trywrlock(&array->lock);
  if (e) return e;

  if (!array_has_capacity(array)) {
    e = pthread_rwlock_unlock(&array->lock);
    if (e) return e;

    return 1;
  }

  for (int i = array->size; i >= 1; --i) {
    array->elements[i] = array->elements[i - 1];
  }

  array->elements[0] = element;
  array->size++;

  e = pthread_rwlock_unlock(&array->lock);
  if (e) return e;

  return 0;
}

int array_set(Array* const array, int index, void* const element) {
  int e = pthread_rwlock_trywrlock(&array->lock);
  if (e) return e;

  if (!array_index_valid(array, index)) {
    e = pthread_rwlock_unlock(&array->lock);
    if (e) return e;

    return 1;
  }

  if (array->elements[index] == NULL) {
    array->size++;
  }
  array->elements[index] = element;

  e = pthread_rwlock_unlock(&array->lock);
  if (e) return e;

  return 0;
}

Result array_get(Array* const array, int index) {
  int e = pthread_rwlock_tryrdlock(&array->lock);
  if (e) return fail(e, ERR_RDLOCK_FAILED);

  if (!array_index_valid(array, index)) {
    e = pthread_rwlock_unlock(&array->lock);
    if (e) return fail(e, ERR_RWLOCK_UNLOCK_FAILED);

    return fail(1, ERR_INDEX_OUT_OF_BOUNDS);
  }

  e = pthread_rwlock_unlock(&array->lock);
  if (e) return fail(e, ERR_RWLOCK_UNLOCK_FAILED);

  return success(array->elements[index]);
}

Result array_remove(Array* const array, int index) {
  int e = pthread_rwlock_trywrlock(&array->lock);
  if (e) return fail(e, ERR_WRLOCK_FAILED); 

  if (!array_index_valid(array, index)) {
    e = pthread_rwlock_unlock(&array->lock);
    if (e) return fail(e, ERR_RWLOCK_UNLOCK_FAILED);

    return fail(1, ERR_INDEX_OUT_OF_BOUNDS);
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

  e = pthread_rwlock_unlock(&array->lock);
  if (e) return fail(e, ERR_RWLOCK_UNLOCK_FAILED);

  return success(removed);
}

int array_for_each(Array* const array, ArrayEachFn each) {
  int e = pthread_rwlock_tryrdlock(&array->lock);
  if (e) return e;

  if (array->size == 0) {
    e = pthread_rwlock_unlock(&array->lock);
    if (e) return e;

    return 0;
  }

  for (int i = 0; i < array->capacity; ++i) {
    void* element = array_get(array, i).ok;
    if (element != NULL) {
      each(element);
    }
  }

  e = pthread_rwlock_unlock(&array->lock);
  if (e) return e;

  return 0;
}

Result array_map(Array* const array, ArrayMapFn map) {
  int e = pthread_rwlock_trywrlock(&array->lock);
  if (e) return fail(e, ERR_WRLOCK_FAILED);

  Array* mapped = array_new(array->capacity).ok;
  if (mapped == NULL) {
    e = pthread_rwlock_unlock(&array->lock);
    if (e) return fail(e, ERR_RWLOCK_UNLOCK_FAILED);

    return fail(1, ERR_MALLOC_FAILED);
  }

  if (array->size == 0) {
    e = pthread_rwlock_unlock(&array->lock);
    if (e) return fail(e, ERR_RWLOCK_UNLOCK_FAILED);

    return success(mapped);
  }

  for (int i = 0; i < array->capacity; ++i) {
    void* element = array->elements[i];
    if (element) {
      array_append(mapped, map(element));
    }
  }
  

  e = pthread_rwlock_unlock(&array->lock);
  if (e) return fail(e, ERR_RWLOCK_UNLOCK_FAILED);

  return success(mapped);
}

Result array_to_string(Array* const array, ToStringFn const to_string) {
  int e = pthread_rwlock_trywrlock(&array->lock);
  if (e) return fail(e, ERR_WRLOCK_FAILED);

  if (array->size == 0) {
    char* buffer = malloc(sizeof(char) * 3);
    sprintf(buffer, "[]\0");

    e = pthread_rwlock_unlock(&array->lock);
    if (e) return fail(e, ERR_RWLOCK_UNLOCK_FAILED);

    return success(buffer);
  }

  int capacity = array->capacity;
  char* elements[capacity] = {};
  int lengths[capacity] = {};
  int sum_lengths = 0;

  for (int i = 0; i < capacity; ++i) {
    void* element = array->elements[i];
    elements[i] = element != NULL ? to_string(element) : "NULL";
    lengths[i] = strlen(elements[i]);
    sum_lengths += lengths[i];
  }

  int total_length = sum_lengths + (capacity - 1) * 2 + 2;
  char* buffer = malloc(sizeof(char) * total_length);

  if (buffer == NULL) {
    e = pthread_rwlock_unlock(&array->lock);
    if (e) return fail(e, ERR_RWLOCK_UNLOCK_FAILED);

    return fail(1, ERR_MALLOC_FAILED);
  }

  sprintf(buffer, "[");
  for (int i = 0; i < capacity; ++i) {
    strcat(buffer, elements[i]);
    if (i < capacity - 1) {
      strcat(buffer, ", ");
    }
  }
  strcat(buffer, "]\0");

  e = pthread_rwlock_unlock(&array->lock);
  if (e) return fail(e, ERR_RWLOCK_UNLOCK_FAILED);

  return success(buffer);
}
