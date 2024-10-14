#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "./lib/c_structs.h"

/*
 * Generic data structures implemnted in C.
 */

void matrix_fun() {
 void** matrix =  malloc(3 * sizeof(void*)); 
 for (int i = 0; i < 3; ++i) {
   matrix[i] = malloc(3 * sizeof(void*));
 }

 int i = 1;
 for (int r = 0; r < 3; ++r) {
   char** row = matrix[r];
   for (int c = 0; c < 3; ++c) {
     char* str = malloc(32 * sizeof(char));
     sprintf(str, "Eat %d cupcake(s)!", i);
     row[c] = str;
     ++i;
   }
 }

 for (int r = 0; r < 3; ++r) {
   char** row = matrix[r];
   for (int c = 0; c < 3; ++c) {
     printf("%s\n", row[c]);
     free(row[c]);
   }
   free(matrix[r]);
 }
}

void array_fun() {
  Array* arr = array_new(5).ok;
  int values[arr->capacity];

  for (int i = 0; i < arr->capacity - 1; ++i) {
    values[i] = i + 1;
    array_append(arr, &values[i]);
  }
  values[4] = 10;
  array_set(arr, 10, &values[4]);

  for (int i = 0; i < arr->size; ++i) {
    int* x = (int*) array_get(arr, i).ok;
    printf("%d %d\n", i, *x);
  }
}

void old_code() {
  Matrix* trix = matrix_new(3, 3);
  Position values[9];

  int i = 0;
  for (int r = 0; r < 3; ++r) {
    void** row = trix->elements[r];
    printf("row %d -> %p\n", r, row);

    for (int c = 0; c < 3; ++c) {
      values[i] = position_new(r, c);
      matrix_set(trix, &values[i], &values[i]);

      void** col = row[c];
      printf("elements[%d][%d] -> %p\n", r, c, col);
      ++i;
    }
    printf("\n");
  }
}

void array_to_string_test() {
  Array* array = array_new(4).ok;
  Position p1 = position_new(0, 0);
  Position p2 = position_new(0, 1);

  array_append(array, &p1);
  array_set(array, 2, &p2);

  char* str = array_to_string(array, (ToStringFn) position_to_string);
  printf("%s\n", str);
}

void matrix_to_string_test() {
  Matrix* matrix = matrix_new(2, 2);
  Position pos[matrix->rows][matrix->columns] = {};

  for (int r = 0; r < matrix->rows; ++r) {
    for (int c = 0; c < matrix->columns; ++c) {
      pos[r][c] = position_new(r, c); 
      matrix_set(matrix, &pos[r][c], &pos[r][c]);
      printf("%u %u\n", r, c);
    }
  }

  char* str = matrix_to_string(matrix, (ToStringFn) position_to_string);
  printf("%s", str);
}

void add2(int* v) {
  *v += 2;
}

void print(int* v) {
  printf("%d\n", *v);
}

void prev() {
  printf("mutex + node = %ld\n", sizeof(pthread_mutex_t) + sizeof(Node));
  printf("node = %ld\n", sizeof(Node));
  printf("linked_list = %ld\n", sizeof(LinkedList));
  Array* arr = array_new(3).ok;
  int values[arr->capacity] = {};
  
  for (int i = 0; i < arr->capacity; ++i) {
    values[i] = i + 1;
    array_append(arr, &values[i]);
  }

  array_for_each(arr, (ArrayEachFn) print);
  printf("\n");
  array_for_each(arr, (ArrayEachFn) add2);
  array_for_each(arr, (ArrayEachFn) print);
}

int main() {
  prev();
  return 0;
}
