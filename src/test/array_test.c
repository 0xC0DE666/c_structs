#include <stdbool.h>
//#include <stdio.h>
//#include <stdlib.h>

#include <string.h>

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "../lib/c_structs.h"
#include "utils.h"

// ####################
// array_new
// ####################
Test(array_new, _1) {
  Array* array = array_new(10);

  cr_assert_eq(array->capacity, 10);
  cr_assert_eq(array->size, 0);
  cr_assert_eq(array->elements == NULL, false);

  for (int i = 0; i < array->size; ++i) {
    cr_assert_eq(array->elements[i], NULL);
  }

  array_free(&array, NULL);
}

// ####################
// array_clear
// ####################
Test(array_clear, _1) {
  Array* array = array_new(5);

  for (int i = 0; i < array->capacity; ++i) {
    array_append(array, point_new(i, i + 1));
    cr_assert_eq(array->size, i + 1);
  }

  array_clear(array, (FreeFn) point_free);

  cr_assert_eq(array->size, 0);
  for (int i = 0; i < array->capacity; ++i) {
    cr_assert_eq(array->elements[i], NULL);
  }

  array_append(array, point_new(0, 0));
  Point* a = array_get(array, 0);
  Point* b = array->elements[0];

  cr_assert_eq(array->size, 1);
  cr_assert_eq(a->x, b->x);
  cr_assert_eq(a->y, b->y);

  array_free(&array, (FreeFn) point_free);
}

// ####################
// array_free
// ####################
Test(array_free, _1) {
  Array* array = array_new(5);

  for (int i = 0; i < array->capacity; ++i) {
    array_append(array, point_new(i, i + 1));
    cr_assert_eq(array->size, i + 1);
  }

  array_free(&array, (FreeFn) point_free);
  cr_assert_eq(array, NULL);
}

// ####################
// array_append
// ####################
Test(array_append, _1) {
  Array* array = array_new(5);
  int values[array->capacity * 2];

  for (int i = 0; i < array->capacity * 2; ++i) {
    values[i] = (i + 1) * 10;
    int res = array_append(array, &values[i]);

    if (i < array->capacity) {
      int* n = (int*) array->elements[i];
      cr_assert_eq(array->size, i + 1);
      cr_assert_eq(*n, values[i]);
    }

    if (i >= array->capacity) {
      cr_assert_eq(res, 1);
      cr_assert_eq(array->size, array->capacity);
    }
  }

  array_free(&array, NULL);
}

Test(array_append, _2) {
  Array* array = array_new(5);
  char* values[] = {"one", "two", "three", "four", "five"};

  for (int i = 0; i < array->capacity; ++i) {
    array_append(array, values[i]);

    char* str = (char*) array->elements[i];
    cr_assert_eq(array->size, i + 1);
    cr_assert_eq(strcmp(str, values[i]), 0);
  }

  array_free(&array, NULL);
}


// ####################
// array_prepend
// ####################
Test(array_prepend, _1) {
  Array* array = array_new(5);
  int values[array->capacity * 2];

  for (int i = 0; i < array->capacity * 2; ++i) {
    values[i] = (i + 1) * 10;
    int res = array_prepend(array, &values[i]);


    if (i < array->capacity) {
      int* n = (int*) array->elements[0];
      cr_assert_eq(array->size, i + 1);
      cr_assert_eq(*n, values[i]);
    }

    if (i >= array->capacity) {
      cr_assert_eq(res, 1);
      cr_assert_eq(array->size, array->capacity);
    }
  }

  int a = array->size - 1, b = 0;
  for (int i = 0; i < array->size; ++i) {
    int* n = (int*) array->elements[a];
    cr_assert_eq(*n, values[b]);
    --a; ++b;
  }

  array_free(&array, NULL);
}

Test(array_prepend, _2) {
  Array* array = array_new(5);
  char* values[] = {"one", "two", "three", "four", "five"};

  for (int i = 0; i < array->capacity; ++i) {
    array_prepend(array, values[i]);

    char* str = (char*) array->elements[0];
    cr_assert_eq(array->size, i + 1);
    cr_assert_eq(strcmp(str, values[i]), 0);
  }

  int a = array->size - 1, b = 0;
  for (int i = 0; i < array->size; ++i) {
    char* str = (char*) array->elements[a];
    cr_assert_eq(strcmp(str, values[b]), 0);
    --a; ++b;
  }
  
  array_free(&array, NULL);
}


// ####################
// array_insert
// ####################
Test(array_insert, _1) {
  Array* array = array_new(5);
  int values[array->capacity * 2];

  int idx = 2;
  int total_inserts = 3;
  for (int i = 0; i < array->capacity * 2; ++i) {
    values[i] = (i + 1) * 10;
    int res = array_insert(array, idx, &values[i]);


    if (i < total_inserts) {
      int* n = (int*) array->elements[idx];
      cr_assert_eq(array->size, i + 1);
      cr_assert_eq(*n, values[i]);
    }

    if (i >= array->capacity) {
      cr_assert_eq(res, 1);
      cr_assert_eq(array->size, total_inserts);
    }
  }

  int a = idx, b = idx;
  for (int i = 0; i < total_inserts; ++i) {
    int* n = (int*) array->elements[a];
    cr_assert_eq(*n, values[b]);
    ++a; --b;
  }

  array_free(&array, NULL);
}

Test(array_insert, _2) {
  Array* array = array_new(5);
  char* values[] = {"one", "two", "three", "four", "five"};

  int idx = 1;
  int total_inserts = 4;
  for (int i = 0; i < total_inserts; ++i) {
    array_insert(array, idx, values[i]);

    char* str = (char*) array->elements[idx];
    cr_assert_eq(array->size, i + 1);
    cr_assert_eq(strcmp(str, values[i]), 0);
  }

  int a = idx, b = total_inserts - 1;
  for (int i = 0; i < total_inserts; ++i) {
    char* str = (char*) array->elements[a];
    cr_assert_eq(strcmp(str, values[b]), 0);
    ++a; --b;
  }

  array_free(&array, NULL);
}


// ####################
// array_set
// ####################
Test(array_set, _1) {
  Array* array = array_new(5);
  int values[array->capacity * 2];

  int res;
  for (int i = 0; i < array->capacity * 2; ++i) {
    values[i] = (i + 1) * 10;
    res = array_set(array, i, &values[i]);


    if (i < array->size) {
      int* n = (int*) array->elements[i];
      cr_assert_eq(array->size, i + 1);
      cr_assert_eq(*n, values[i]);
    }

    if (i >= array->capacity) {
      cr_assert_eq(res, 1);
      cr_assert_eq(array->size, array->capacity);
    }
  }
  int i = 0;
  values[i] = -10;
  array_set(array, i, &values[i]);

  cr_assert_eq(array->size, array->capacity);
  int* v = array_get(array, i);
  cr_assert_eq(*v, values[i]);

  array_free(&array, NULL);
}


// ####################
// array_get
// ####################
Test(array_get, _1) {
  Array* array = array_new(5);
  int values[array->capacity * 2];

  for (int i = 0; i < array->capacity * 2; ++i) {
    values[i] = (i + 1) * 10;
    int res = array_append(array, &values[i]);

    if (i < array->size) {
      int* v = (int*) array_get(array, i);
      cr_assert_eq(res, 0);
      cr_assert_eq(array->size, i + 1);
      cr_assert_eq(*v, values[i]);
    }

    if (i >= 5) {
      cr_assert_eq(res, 1);
      cr_assert_eq(array->size, 5);
      cr_assert_eq(array_get(array, i), NULL);
    }
  }

  array_free(&array, NULL);
}

Test(array_get, _2) {
  Array* array = array_new(5);
  char* values[] = {"one", "two", "three", "four", "five"};

  for (int i = 0; i < array->capacity; ++i) {
    array_append(array, values[i]);

    cr_assert_eq(array->size, i + 1);
    char* str = (char*) array_get(array, i);
    cr_assert_eq(strcmp(str, values[i]), 0);
  }

  array_free(&array, NULL);
}


// ####################
// array_remove
// ####################
Test(array_remove, _1) {
  Array* array = array_new(5);
  int values[array->capacity];

  for (int i = 0; i < array->capacity; ++i) {
    values[i] = (i + 1) * 10;
    array_append(array, &values[i]);
    cr_assert_eq(array->size, i + 1);
  }

  for (int i = -5; i <= array->capacity * 2; ++i) {
    if (array_index_valid(array, i)) {
      int* v = (int*) array_remove(array, 0);
      cr_assert_eq(*v, values[i]);
      cr_assert_eq(array->size, array->capacity - (i + 1));
    }

    if (!array_index_valid(array, i)) {
      void* v = array_remove(array, i);
      cr_assert_eq(v, NULL);
      if (i < 0) {
        cr_assert_eq(array->size, 5);
      } else {
        cr_assert_eq(array->size, 0);
      }
    }
  }

  array_free(&array, NULL);
}

Test(array_remove, _2) {
  Array* array = array_new(5);
  char* values[] = {"one", "two", "three", "four", "five"};

  for (int i = 0; i < array->capacity; ++i) {
    array_append(array, values[i]);
    cr_assert_eq(array->size, i + 1);
  }

  int x = 1;
  for (int i = array->capacity - 1; i >= 0; --i) {
    char* str = (char*) array_remove(array, i);
    cr_assert_eq(array_get(array, i), NULL);
    cr_assert_eq(strcmp(str, values[i]), 0);
    cr_assert_eq(array->size, array->capacity - x++);
  }

  array_free(&array, NULL);
}


// ####################
// array_for_each
// ####################
Test(array_for_each, _1) {
  Array* array = array_new(5);
  
  for (int i = 0; i < array->capacity; ++i) {
    array_append(array, point_new(i, i));
  }

  array_for_each(array, (ArrayEachFn) point_double);
  
  for (int i = 0; i < array->capacity; ++i) {
    Point* p = array_get(array, i);
    cr_assert_eq(p->x, i * 2);
    cr_assert_eq(p->y, i * 2);
  }

  array_free(&array, (FreeFn) point_free);
}

// ####################
// array_map
// ####################
Test(array_map, _1) {
  Array* array = array_new(5);
  Array* empty = array_map(array, (ArrayMapFn) NULL);

  cr_assert_eq(empty->size, 0);

  array_free(&array, NULL);
  array_free(&empty, NULL);
}

Test(array_map, _2) {
  Array* points = array_new(5);
  
  for (int i = 0; i < points->capacity; ++i) {
    array_append(points, point_new(i, i));
  }

  Array* point_strs = array_map(points, (ArrayMapFn) point_to_string);

  cr_assert_eq(point_strs->capacity, points->capacity);
  cr_assert_eq(point_strs->size, points->size);
  cr_assert_eq(point_strs->elements != NULL, true);
  
  for (int i = 0; i < points->capacity; ++i) {
    char* result = (char*) array_get(point_strs, i);
    char* expected = point_to_string(array_get(points, i));
    cr_assert_eq(strcmp(result, expected), 0);
  }

  array_free(&points, (FreeFn) point_free);
  array_free(&point_strs, (FreeFn) ptr_free);
}


// ####################
// array_to_string
// ####################
Test(array_to_string, _1) {
  Array* array = array_new(2);

  char* result = array_to_string(array, (ToStringFn) NULL);
  char* expected = "[]";
  cr_assert_eq(strcmp(result, expected), 0);

  free(result);
  array_free(&array, NULL);
}

Test(array_to_string, _2) {
  Array* array = array_new(3);

  array_append(array, point_new(0, 0));
  array_insert(array, 2, point_new(1, 1));

  char* result = array_to_string(array, (ToStringFn) point_to_string);
  char* expected = "[(0, 0), NULL, (1, 1)]";
  cr_assert_eq(strcmp(result, expected), 0);

  free(result);
  array_free(&array, NULL);
}


// ####################
// array_index_valid
// ####################
Test(array_index_valid, _1) {
  Array* array = array_new(5);

  for (int i = -5; i < array->capacity * 2; ++i) {
    bool result = array_index_valid(array, i);
    bool expected = i < 0 ? false : i >= array->capacity ? false : true;
    cr_assert_eq(result, expected);
  }

  array_free(&array, NULL);
}

// ####################
// array_has_capacity
// ####################
Test(array_has_capacity, _1) {
  Array* array = array_new(5);
  char* values[] = {"one", "two", "three", "four", "five"};

  for (int i = 0; i < array->capacity * 2; ++i) {
    bool result = array_has_capacity(array);
    bool expected = i < array->capacity ? true : false;
    cr_assert_eq(result, expected);
    array_append(array, values[i]);
  }

  array_free(&array, NULL);
}
