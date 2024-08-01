#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./array/lib.h"
#include "./matrix/lib.h"

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
     sprintf(str, "Eat %d dick(s)! Nigga!", i);
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

int main() {
  Array* arr = array_new(5);
  int values[arr->capacity];

  for (int i = 0; i < arr->capacity - 1; ++i) {
    values[i] = i + 1;
    array_append(arr, &values[i]);
  }
  values[4] = 10;
  array_insert(arr, 10, &values[4]);

  for (int i = 0; i < arr->size; ++i) {
    int* x = (int*) array_get(arr, i);
    printf("%d %d\n", i, *x);
  }

  return 0;
}
