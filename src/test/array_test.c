#include <stdbool.h>
#include <string.h>
//#include <stdio.h>
//#include <stdlib.h>

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "../lib/c_structs.h"
#include "utils.h"

// ####################
// array_index_valid
// ####################
Test(array_index_valid, _1) {
  Array* array = array_new(5).ok;

  for (int i = -5; i < array->capacity * 2; ++i) {
    bool result = array_index_valid(array, i);
    bool expected = i >= 0 && i < array->capacity ? true : false;
    cr_assert_eq(result, expected);
  }

  int e = array_free(&array, (FreeFn) need_not_free);
  cr_assert_eq(e, 0);
}

// ####################
// array_has_capacity
// ####################
Test(array_has_capacity, _1) {
  Array* array = array_new(5).ok;
  char* values[] = {"one", "two", "three", "four", "five"};

  for (int i = 0; i < array->capacity * 2; ++i) {
    bool result = array_has_capacity(array);
    bool expected = i < array->capacity ? true : false;
    cr_assert_eq(result, expected);
    array_append(array, values[i]);
  }

  int e = array_free(&array, (FreeFn) need_not_free);
  cr_assert_eq(e, 0);
}

// ####################
// array_new
// ####################
Test(array_new, _1) {
  Array* array = array_new(10).ok;

  cr_assert_eq(array->capacity, 10);
  cr_assert_eq(array->size, 0);
  cr_assert_eq(array->elements == NULL, false);

  for (int i = 0; i < array->size; ++i) {
    cr_assert_eq(array->elements[i], NULL);
  }

  int e = array_free(&array, (FreeFn) need_not_free);
  cr_assert_eq(e, 0);
}

// ####################
// array_clear
// ####################
Test(array_clear, _1) {
  Array* array = array_new(5).ok;

  for (int i = 0; i < array->capacity; ++i) {
    array_append(array, point_new(i, i + 1));
    cr_assert_eq(array->size, i + 1);
    cr_assert_eq(array_get(array, i).ok != NULL, true);
  }
  cr_assert_eq(array->size, array->capacity);

  array_clear(array, (FreeFn) safe_free);
  cr_assert_eq(array != NULL, true);
  cr_assert_eq(array->size, 0);

  for (int i = 0; i < array->capacity; ++i) {
    cr_assert_eq(array->elements[i], NULL);
  }

  int e = array_free(&array, (FreeFn) safe_free);
  cr_assert_eq(e, 0);
}

// ####################
// array_free
// ####################
Test(array_free, _1) {
  Array* array = array_new(5).ok;

  for (int i = 0; i < array->capacity; ++i) {
    array_append(array, point_new(i, i + 1));
    cr_assert_eq(array->size, i + 1);
  }
  cr_assert_eq(array->size, array->capacity);

  int e = array_free(&array, (FreeFn) safe_free);
  cr_assert_eq(e, 0);
  cr_assert_eq(array, NULL);
}

// ####################
// array_append
// ####################
Test(array_append, _1) {
  Array* array = array_new(5).ok;

  for (int i = 0; i < array->capacity * 2; ++i) {
    int res = array_append(array, point_new(i, i));

    if (i < array->capacity) {
      cr_assert_eq(res, 0);
      cr_assert_eq(array->size, i + 1);
      Point* p = array->elements[i];
      cr_assert_eq(p != NULL, true);
      cr_assert_eq(p->x, i);
      cr_assert_eq(p->y, i);
    }

    if (i >= array->capacity) {
      cr_assert_eq(res, ERR_CODE_GENERAL);
      cr_assert_eq(array->size, array->capacity);
    }
  }
  cr_assert_eq(array->size, array->capacity);

  int e = array_free(&array, (FreeFn) safe_free);
  cr_assert_eq(e, 0);
}

// ####################
// array_prepend
// ####################
Test(array_prepend, _1) {
  Array* array = array_new(5).ok;

  for (int i = 0; i < array->capacity * 2; ++i) {
    int res = array_prepend(array, point_new(i, i));

    if (i < array->capacity) {
      cr_assert_eq(res, 0);
      cr_assert_eq(array->size, i + 1);
      Point* p = array->elements[0];
      cr_assert_eq(p != NULL, true);
      cr_assert_eq(p->x, i);
      cr_assert_eq(p->y, i);
    }

    if (i >= array->capacity) {
      cr_assert_eq(res, ERR_CODE_GENERAL);
      cr_assert_eq(array->size, array->capacity);
    }
  }
  cr_assert_eq(array->size, array->capacity);

  int e = array_free(&array, (FreeFn) safe_free);
  cr_assert_eq(e, 0);
}

// ####################
// array_set
// ####################
Test(array_set, _1) {
  Array* array = array_new(5).ok;

  unsigned int sze = 0;
  for (unsigned int i = 0; i < array->capacity * 2; ++i) {
    int res = array_set(array, i, point_new(i, i));
    res == 0 ? ++sze : 0;

    if (array_index_valid(array, i)) {
      Point* pt = array_get(array, i).ok;
      cr_assert_eq(res, 0);
      cr_assert_eq(array->size, sze);
      cr_assert_eq(pt != NULL, true);
      cr_assert_eq(pt->x, i);
      cr_assert_eq(pt->y, i);
    }

    if (!array_index_valid(array, i)) {
      cr_assert_eq(res, ERR_CODE_GENERAL);
      cr_assert_eq(array->size, sze);
      Result res = array_get(array, i);
      void* ok = res.ok;
      Error err = res.error;
      cr_assert_eq(ok, NULL);
      cr_assert_eq(err.code, ERR_CODE_GENERAL);
      cr_assert_eq(strcmp(err.message, ERR_MSG_INVALID_INDEX), 0);
    }
  }
  cr_assert_eq(sze, array->capacity);

  Point* points[array->capacity] = {};

  for (int i = 0; i < array->capacity; ++i) {
    points[i] = array_get(array, i).ok;
    int res = array_set(array, i, point_new((i + 1) * 2, (i + 1) * 2));

    Point* pt = array_get(array, i).ok;
    cr_assert_eq(res, 0);
    cr_assert_eq(array->size, array->capacity);
    cr_assert_eq(pt != NULL, true);
    cr_assert_eq(pt->x, (i + 1) * 2);
    cr_assert_eq(pt->y, (i + 1) * 2);
  }

  for (int i = 0; i < array->capacity; ++i) {
    safe_free((void**) &points[i]);
  }
  int e = array_free(&array, (FreeFn) safe_free);
  cr_assert_eq(e, 0);
}

// ####################
// array_get
// ####################
Test(array_get, _1) {
  Array* array = array_new(5).ok;

  unsigned int sze = 0;
  for (unsigned int i = 0; i < array->capacity * 2; ++i) {
    int res = array_set(array, i, point_new(i, i));
    res == 0 ? ++sze : 0;

    if (array_index_valid(array, i)) {
      cr_assert_eq(res, 0);
      cr_assert_eq(array->size, sze);
      Point* p = array_get(array, i).ok;
      cr_assert_eq(p != NULL, true);
      cr_assert_eq(p->x, i);
      cr_assert_eq(p->y, i);
    }

    if (!array_index_valid(array, i)) {
      cr_assert_eq(res, ERR_CODE_GENERAL);
      cr_assert_eq(array->size, sze);
      Result res = array_get(array, i);
      void* ok = res.ok;
      Error err = res.error;
      cr_assert_eq(ok, NULL);
      cr_assert_eq(err.code, ERR_CODE_GENERAL);
      cr_assert_eq(strcmp(err.message, ERR_MSG_INVALID_INDEX), 0);
    }
  }
  cr_assert_eq(sze, array->capacity);

  int e = array_free(&array, (FreeFn) safe_free);
  cr_assert_eq(e, 0);
}

// ####################
// array_remove
// ####################
Test(array_remove, _1) {
  Array* array = array_new(5).ok;

  for (int i = 0; i < array->capacity; ++i) {
    int res = array_append(array, point_new(i, i));
    cr_assert_eq(res, 0);
    cr_assert_eq(array->size, i + 1);
  }

  for (int i = -5; i <= array->capacity * 2; ++i) {
    if (array_index_valid(array, i)) {
      Point* p = array_remove(array, 0).ok;
      cr_assert_eq(p != NULL, true);
      cr_assert_eq(p->x, i);
      cr_assert_eq(p->y, i);
      cr_assert_eq(array->size, array->capacity - (i + 1));
    }

    if (!array_index_valid(array, i)) {
      Result res = array_remove(array, i);
      void* ok = res.ok;
      Error err = res.error;
      cr_assert_eq(ok, NULL);
      cr_assert_eq(err.code, ERR_CODE_GENERAL);
      cr_assert_eq(strcmp(err.message, ERR_MSG_INVALID_INDEX), 0);
    }
  }

  int e = array_free(&array, (FreeFn) safe_free);
  cr_assert_eq(e, 0);
}

// ####################
// array_for_each
// ####################
Test(array_for_each, _1) {
  Array* array = array_new(5).ok;
  int e = array_for_each(array, (ArrayEachFn) point_double);
  cr_assert_eq(e, 0);
  
  for (int i = 0; i < array->capacity; ++i) {
    array_append(array, point_new(i, i));
  }

  e = array_for_each(array, (ArrayEachFn) point_double);
  cr_assert_eq(e, 0);
  
  for (int i = 0; i < array->capacity; ++i) {
    Point* p = array_get(array, i).ok;
    cr_assert_eq(p->x, i * 2);
    cr_assert_eq(p->y, i * 2);
  }

  e = array_free(&array, (FreeFn) safe_free);
  cr_assert_eq(e, 0);
}

// ####################
// array_map
// ####################
Test(array_map, _2) {
  Array* points = array_new(5).ok;
  Array* empty = array_map(points, (ArrayMapFn) as_is).ok;

  cr_assert_eq(empty != NULL, true);
  cr_assert_eq(empty->size, 0);
  array_free(&empty, NULL);

  
  for (int i = 0; i < points->capacity; ++i) {
    int e = array_append(points, point_new(i, i));
    cr_assert_eq(e, 0);
  }

  Array* strings = array_map(points, (ArrayMapFn) point_to_string).ok;

  cr_assert_eq(strings->capacity, points->capacity);
  cr_assert_eq(strings->size, points->size);
  cr_assert_eq(strings->elements != NULL, true);
  
  for (int i = 0; i < points->capacity; ++i) {
    char* result = (char*) array_get(strings, i).ok;
    char* expected = point_to_string(array_get(points, i).ok);
    cr_assert_eq(strcmp(result, expected), 0);
  }

  int e = array_free(&points, (FreeFn) safe_free);
  cr_assert_eq(e, 0);
  e = array_free(&strings, (FreeFn) safe_free);
  cr_assert_eq(e, 0);
}

// ####################
// array_to_string
// ####################
Test(array_to_string, _1) {
  Array* array = array_new(3).ok;

  char* result = array_to_string(array, (ToStringFn) point_to_string).ok;
  char* expected = "[]";
  cr_assert_eq(strcmp(result, expected), 0);
  safe_free((void**) &result);

  array_append(array, point_new(0, 0));
  array_set(array, 2, point_new(1, 1));

  result = array_to_string(array, (ToStringFn) point_to_string).ok;
  expected = "[(0, 0), NULL, (1, 1)]";
  cr_assert_eq(strcmp(result, expected), 0);

  safe_free((void**) &result);
  int e = array_free(&array, (FreeFn) safe_free);
  cr_assert_eq(e, 0);
}
