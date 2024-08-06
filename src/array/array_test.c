#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "array.h"

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

  array_free(&array);
}

// ####################
// array_free
// ####################
Test(array_free, _1) {
  Array* array = array_new(5);
  int values[array->capacity];

  for (int i = 0; i < array->capacity; ++i) {
    values[i] = (i + 1) * 10;
    array_append(array, &values[i]);
    cr_assert_eq(array->size, i + 1);
  }

  array_free(&array);

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

  array_free(&array);
}

Test(array_append, _2) {
  Array* array = array_new(5);
  int values[array->capacity];

  for (int i = 0; i < array->capacity; ++i) {
    values[i] = (i + 1) * 10;
    array_append(array, &values[i]);

    int* n = (int*) array->elements[i];
    cr_assert_eq(array->size, i + 1);
    cr_assert_eq(*n, values[i]);
  }

  array_free(&array);
}

Test(array_append, _3) {
  Array* array = array_new(5);
  char* values[] = {"one", "two", "three", "four", "five"};

  for (int i = 0; i < array->capacity; ++i) {
    array_append(array, values[i]);

    char* str = (char*) array->elements[i];
    cr_assert_eq(array->size, i + 1);
    cr_assert_eq(strcmp(str, values[i]), 0);
  }

  array_free(&array);
}

Test(array_append, _4) {
  typedef struct {
    int x;
    int y;
  } Point;

  Array* array = array_new(5);
  Point values[array->capacity];

  for (int i = 0; i < array->capacity; ++i) {
    values[i] = (Point) {i, i + 2};
    array_append(array, &values[i]);

    Point* p = (Point*) array->elements[i];
    cr_assert_eq(array->size, i + 1);
    cr_assert_eq(p->x, values[i].x);
    cr_assert_eq(p->y, values[i].y);
  }

  array_free(&array);
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

  array_free(&array);
}

Test(array_prepend, _2) {
  Array* array = array_new(5);
  int values[array->capacity];

  for (int i = 0; i < array->capacity; ++i) {
    values[i] = (i + 1) * 10;
    array_prepend(array, &values[i]);

    int* n = (int*) array->elements[0];
    cr_assert_eq(array->size, i + 1);
    cr_assert_eq(*n, values[i]);
  }

  int a = array->size - 1, b = 0;
  for (int i = 0; i < array->size; ++i) {
    int* n = (int*) array->elements[a];
    cr_assert_eq(*n, values[b]);
    --a; ++b;
  }
  
  array_free(&array);
}

Test(array_prepend, _3) {
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
  
  array_free(&array);
}

Test(array_prepend, _4) {
  typedef struct {
    int x;
    int y;
  } Point;

  Array* array = array_new(5);
  Point values[array->capacity];

  for (int i = 0; i < array->capacity; ++i) {
    values[i] = (Point) {i, i + 2};
    array_prepend(array, &values[i]);

    Point* p = (Point*) array->elements[0];
    cr_assert_eq(array->size, i + 1);
    cr_assert_eq(p->x, values[i].x);
    cr_assert_eq(p->y, values[i].y);
  }

  int a = array->size - 1, b = 0;
  for (int i = 0; i < array->size; ++i) {
    Point* p = (Point*) array->elements[a];
    cr_assert_eq(p->x, values[b].x);
    cr_assert_eq(p->y, values[b].y);
    --a; ++b;
  }
  
  array_free(&array);
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

  array_free(&array);
}

Test(array_insert, _2) {
  Array* array = array_new(5);
  int values[array->capacity];

  int idx = 1;
  int total_inserts = 4;
  for (int i = 0; i < total_inserts; ++i) {
    values[i] = (i + 1) * 10;
    array_insert(array, idx, &values[i]);

    int* n = (int*) array->elements[idx];
    cr_assert_eq(array->size, i + 1);
    cr_assert_eq(*n, values[i]);
  }

  int a = idx, b = total_inserts - 1;
  for (int i = 0; i < total_inserts; ++i) {
    int* n = (int*) array->elements[a];
    cr_assert_eq(*n, values[b]);
    ++a; --b;
  }

  array_free(&array);
}

Test(array_insert, _3) {
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

  array_free(&array);
}

Test(array_insert, _4) {
  typedef struct {
    int x;
    int y;
  } Point;

  Array* array = array_new(5);
  Point values[array->capacity];

  int idx = 1;
  int total_inserts = 4;
  for (int i = 0; i < total_inserts; ++i) {
    values[i] = (Point) {i, i + 2};
    array_insert(array, idx, &values[i]);

    Point* p = (Point*) array->elements[idx];
    cr_assert_eq(array->size, i + 1);
    cr_assert_eq(p->x, values[i].x);
    cr_assert_eq(p->y, values[i].y);
  }

  int a = idx, b = total_inserts - 1;
  for (int i = 0; i < total_inserts; ++i) {
    Point* p = (Point*) array->elements[a];
    cr_assert_eq(p->x, values[b].x);
    cr_assert_eq(p->y, values[b].y);
    ++a; --b;
  }

  array_free(&array);
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

    if (i >= 5) {
      cr_assert_eq(res, 1);
      cr_assert_eq(array->size, 5);
      cr_assert_eq(array_get(array, i), NULL);
    }
  }

  array_free(&array);
}

Test(array_get, _2) {
  Array* array = array_new(5);
    int values[array->capacity];

  for (int i = 0; i < array->capacity; ++i) {
    values[i] = (i + 1) * 10;
    array_append(array, &values[i]);

    cr_assert_eq(array->size, i + 1);
    int* n = (int*) array_get(array, i);
    cr_assert_eq(*n, (i + 1) * 10);
  }

  array_free(&array);
}

Test(array_get, _3) {
  Array* array = array_new(5);
  char* values[] = {"one", "two", "three", "four", "five"};

  for (int i = 0; i < array->capacity; ++i) {
    array_append(array, values[i]);

    cr_assert_eq(array->size, i + 1);
    char* str = (char*) array_get(array, i);
    cr_assert_eq(strcmp(str, values[i]), 0);
  }

  array_free(&array);
}

Test(array_get, _4) {
  typedef struct {
    int x;
    int y;
  } Point;

  Array* array = array_new(5);

  for (int i = 0; i < array->capacity; ++i) {
    Point values[array->capacity];
    values[i] = (Point) {i, i + 2};
    array_append(array, &values[i]);

    cr_assert_eq(array->size, i + 1);
    Point* p = (Point*) array_get(array, i);
    cr_assert_eq(p->x, values[i].x);
    cr_assert_eq(p->y, values[i].y);
  }

  array_free(&array);
}


// ####################
// array_remove
// ####################
Test(array_remove, _1) {
  Array* array = array_new(5);
  int values[array->capacity * 2];

  for (int i = 0; i < array->capacity * 2; ++i) {
    values[i] = (i + 1) * 10;
    array_append(array, &values[i]);
  }

  for (int i = array->capacity - 1; i >= 0; --i) {
    if (i >= array->capacity) {
      void* val = array_remove(array, i);
      cr_assert_eq(val, NULL);
      cr_assert_eq(array->size, 5);
    }
  }

  array_free(&array);
}

Test(array_remove, _2) {
  Array* array = array_new(5);
  int values[array->capacity];

  for (int i = 0; i < array->capacity; ++i) {
    values[i] = (i + 1) * 10;
    array_append(array, &values[i]);
    cr_assert_eq(array->size, i + 1);
  }

  int x = 1;
  for (int i = array->capacity - 1; i >= 0; --i) {
    int* n = (int*) array_remove(array, i);
    cr_assert_eq(array_get(array, i), NULL);
    cr_assert_eq(*n, (i + 1) * 10);
    cr_assert_eq(array->size, array->capacity - x++);
  }

  array_free(&array);
}

Test(array_remove, _3) {
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

  array_free(&array);
}

Test(array_remove, _4) {
  typedef struct {
    int x;
    int y;
  } Point;

  Array* array = array_new(5);
  Point values[array->capacity];

  for (int i = 0; i < array->capacity; ++i) {
    values[i] = (Point) {i, i + 2};
    array_append(array, &values[i]);
    cr_assert_eq(array->size, i + 1);
  }

  int x = 1;
  for (int i = array->capacity - 1; i >= 0; --i) {
    Point* p = (Point*) array_remove(array, i);
    cr_assert_eq(array_get(array, i), NULL);
    cr_assert_eq(p->x, values[i].x);
    cr_assert_eq(p->y, values[i].y);
    cr_assert_eq(array->size, array->capacity - x++);
  }

  array_free(&array);
}

// ####################
// array_clear
// ####################
Test(array_clear, _1) {
  Array* array = array_new(5);
  int values[array->capacity];

  for (int i = 0; i < array->capacity; ++i) {
    values[i] = (i + 1) * 10;
    array_append(array, &values[i]);
    cr_assert_eq(array->size, i + 1);
  }

  array_clear(array);

  cr_assert_eq(array->size, 0);
  for (int i = 0; i < array->capacity; ++i) {
    cr_assert_eq(array->elements[i], NULL);
  }

  int idx = 0;
  array_append(array, &values[idx]);
  int* v = array_get(array, idx);

  cr_assert_eq(*v, values[idx]);

  array_free(&array);
}

// ####################
// array_index_valid
// ####################
Test(array_index_valid, _1) {
  Array* array = array_new(5);
  int index = 5;

  bool result = array_index_valid(array, index);
  cr_assert_eq(result, false);

  array_free(&array);
}

Test(array_index_valid, _2) {
  Array* array = array_new(5);
  int index = 3;

  bool result = array_index_valid(array, index);
  cr_assert_eq(result, true);

  array_free(&array);
}

// ####################
// array_has_capacity
// ####################
Test(array_has_capacity, _1) {
  Array* array = array_new(1);
  int x = 10;
  array_append(array, &x);

  bool result = array_has_capacity(array);
  cr_assert_eq(result, false);

  array_free(&array);
}

Test(array_has_capacity, _2) {
  Array* array = array_new(1);

  bool result = array_has_capacity(array);
  cr_assert_eq(result, true);

  array_free(&array);
}
