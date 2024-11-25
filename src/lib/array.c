#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "c_structs.h"

bool array_index_valid(const Array* array, unsigned int index) {
  return index <= array->capacity - 1;
}

bool array_has_capacity(const Array* array) {
  return array->size < array->capacity;
}

Result array_new(unsigned int capacity) {
  Array* array = malloc(sizeof(Array) + capacity * sizeof(void*));

  if (array == NULL) {
    return result_std_error();
  }

  int e = pthread_rwlock_init(&array->lock, NULL);
  if (e) {
    free(array);
    return result_std_error();
  }
  array->capacity = capacity;
  array->size = (unsigned int) 0;

  for (unsigned int i = 0; i < capacity; ++i) {
    array->elements[i] = NULL;
  }

  return result_ok(array);
}

int array_clear(Array* const array, const FreeFn free_element) {
  int e = pthread_rwlock_trywrlock(&array->lock);
  if (e) return e;

  for (unsigned int i = 0; i < array->capacity; i++) {
    void** el = array->elements + i;

    if (*el != NULL) {
      free_element(el);
    } else {
      *el = NULL;
    }
  }
  array->size = 0;

  e = pthread_rwlock_unlock(&array->lock);
  if (e) return e;

  return SUC_CODE_GENERAL;
}

int array_free(Array** const array, FreeFn free_element) {
  array_clear(*array, free_element);

  int e = pthread_rwlock_destroy(&(*array)->lock);
  if (e) return e;

  free(*array);
  *array = NULL;

  return SUC_CODE_GENERAL;
}

int array_append(Array* const array, void* const element) {
  int e = pthread_rwlock_trywrlock(&array->lock);
  if (e) return e;

  if (!array_has_capacity(array)) {
    e = pthread_rwlock_unlock(&array->lock);
    if (e) return e;

    return ERR_CODE_GENERAL;
  }

  array->elements[array->size] = element;
  array->size++;

  e = pthread_rwlock_unlock(&array->lock);
  if (e) return e;

  return SUC_CODE_GENERAL;
}

int array_prepend(Array* const array, void* const element) {
  int e = pthread_rwlock_trywrlock(&array->lock);
  if (e) return e;

  if (!array_has_capacity(array)) {
    e = pthread_rwlock_unlock(&array->lock);
    if (e) return e;

    return ERR_CODE_GENERAL;
  }

  for (unsigned int i = array->size; i >= 1; --i) {
    array->elements[i] = array->elements[i - 1];
  }

  array->elements[0] = element;
  array->size++;

  e = pthread_rwlock_unlock(&array->lock);
  if (e) return e;

  return SUC_CODE_GENERAL;
}

int array_set(Array* const array, unsigned int index, void* const element) {
  int e = pthread_rwlock_trywrlock(&array->lock);
  if (e) return e;

  if (!array_index_valid(array, index)) {
    e = pthread_rwlock_unlock(&array->lock);
    if (e) return e;

    return ERR_CODE_GENERAL;
  }

  if (array->elements[index] == NULL) {
    array->size++;
  }
  array->elements[index] = element;

  e = pthread_rwlock_unlock(&array->lock);
  if (e) return e;

  return SUC_CODE_GENERAL;
}

Result array_get(Array* const array, unsigned int index) {
  int e = pthread_rwlock_tryrdlock(&array->lock);
  if (e) return result_std_error();

  if (!array_index_valid(array, index)) {
    e = pthread_rwlock_unlock(&array->lock);
    if (e) return result_std_error();

    return result_error(ERR_CODE_GENERAL, ERR_MSG_ARRAY_INDEX_INVALID);
  }

  e = pthread_rwlock_unlock(&array->lock);
  if (e) return result_std_error();

  return result_ok(array->elements[index]);
}

Result array_remove(Array* const array, unsigned int index) {
  int e = pthread_rwlock_trywrlock(&array->lock);
  if (e) return result_std_error();

  if (!array_index_valid(array, index)) {
    e = pthread_rwlock_unlock(&array->lock);
    if (e) return result_std_error();

    return result_error(ERR_CODE_GENERAL, ERR_MSG_ARRAY_INDEX_INVALID);
  }

  void* removed = array->elements[index];

  for (unsigned int i = index; i < array->capacity; ++i) {
    if (i < array->capacity - 1) {
      array->elements[i] = array->elements[i + 1];
    } else {
      array->elements[i] = NULL;
    }
  }
  array->size--;

  e = pthread_rwlock_unlock(&array->lock);
  if (e) return result_std_error();

  return result_ok(removed);
}

int array_for_each(Array* const array, ArrayEachFn each) {
  int e = pthread_rwlock_tryrdlock(&array->lock);
  if (e) return e;

  if (array->size == 0) {
    e = pthread_rwlock_unlock(&array->lock);
    if (e) return e;

    return 0;
  }

  for (unsigned int i = 0; i < array->capacity; ++i) {
    Result res = array_get(array, i);
    if (res.error.code != SUC_CODE_GENERAL) {
      fprintf(stderr, res.error.message);
      return res.error.code;
    }

    void* element = res.ok;
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
  if (e) return result_std_error();

  Result res = array_new(array->capacity);
  if (res.error.code != SUC_CODE_GENERAL) {
    return res;
  }

  Array* mapped = res.ok;
  if (mapped == NULL) {
    e = pthread_rwlock_unlock(&array->lock);
    if (e) return result_std_error();

    return result_std_error();
  }

  if (array->size == 0) {
    e = pthread_rwlock_unlock(&array->lock);
    if (e) return result_std_error();

    return result_ok(mapped);
  }

  for (unsigned int i = 0; i < array->capacity; ++i) {
    void* element = array->elements[i];
    if (element) {
      array_append(mapped, map(element));
    }
  }
  

  e = pthread_rwlock_unlock(&array->lock);
  if (e) return result_std_error();

  return result_ok(mapped);
}

Result array_to_string(Array* const array, ToStringFn const to_string) {
  int e = pthread_rwlock_trywrlock(&array->lock);
  if (e) return result_std_error();

  if (array->size == 0) {
    char* buffer = malloc(sizeof(char) * 3);
    sprintf(buffer, "[]");

    e = pthread_rwlock_unlock(&array->lock);
    if (e) {
      free(buffer);
      return result_std_error();
    };

    return result_ok(buffer);
  }

  unsigned int capacity = array->capacity;
  char* elements[capacity] = {};
  unsigned int lengths[capacity] = {};
  unsigned int sum_lengths = 0;

  for (unsigned int i = 0; i < capacity; ++i) {
    void* element = array->elements[i];
    elements[i] = element != NULL ? to_string(element) : "NULL";
    lengths[i] = strlen(elements[i]);
    sum_lengths += lengths[i];
  }

  unsigned int total_length = sum_lengths + (capacity * 2) + 4;
  char* buffer = malloc(sizeof(char) * total_length);

  if (buffer == NULL) {
    e = pthread_rwlock_unlock(&array->lock);
    if (e) return result_std_error();

    return result_std_error();
  }

  sprintf(buffer, "[");
  for (unsigned int i = 0; i < capacity; ++i) {
    strcat(buffer, elements[i]);
    if (i < capacity - 1) {
      strcat(buffer, ", ");
    }
  }
  strcat(buffer, "]\0");

  e = pthread_rwlock_unlock(&array->lock);
  if (e) {
    free(buffer);
    return result_std_error();
  }

  return result_ok(buffer);
}
