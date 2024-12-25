#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "../lib/c_structs.h"

/*
 * Generic data structures implemnted in C.
 */

void grid_fun() {
 void** grid =  malloc(3 * sizeof(void*)); 
 for (unsigned int i = 0; i < 3; ++i) {
   grid[i] = malloc(3 * sizeof(void*));
 }

 int i = 1;
 for (unsigned int r = 0; r < 3; ++r) {
   char** row = grid[r];
   for (unsigned int c = 0; c < 3; ++c) {
     char* str = malloc(32 * sizeof(char));
     sprintf(str, "Eat %d cupcake(s)!", i);
     row[c] = str;
     ++i;
   }
 }

 for (unsigned int r = 0; r < 3; ++r) {
   char** row = grid[r];
   for (unsigned int c = 0; c < 3; ++c) {
     printf("%s\n", row[c]);
     free(row[c]);
   }
   free(grid[r]);
 }
}

void array_fun() {
  Array* arr = array_new(5).ok;
  int values[arr->capacity];

  for (unsigned int i = 0; i < arr->capacity - 1; ++i) {
    values[i] = i + 1;
    array_append(arr, &values[i]);
  }
  values[4] = 10;
  array_set(arr, 10, &values[4]);

  for (unsigned int i = 0; i < arr->size; ++i) {
    int* x = (int*) array_get(arr, i).ok;
    printf("%d %d\n", i, *x);
  }
}

void old_code() {
  Grid* trix = grid_new(3, 3).ok;
  Position values[9];

  int i = 0;
  for (unsigned int r = 0; r < 3; ++r) {
    void** row = trix->elements[r];
    printf("row %d -> %p\n", r, row);

    for (unsigned int c = 0; c < 3; ++c) {
      values[i] = position_new(r, c);
      grid_set(trix, &values[i], &values[i]);

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

  char* str = array_to_string(array, (FnToString) position_to_string).ok;
  printf("%s\n", str);
}

void grid_to_string_test() {
  Grid* grid = grid_new(2, 2).ok;
  Position pos[grid->rows][grid->columns] = {};

  for (unsigned int r = 0; r < grid->rows; ++r) {
    for (unsigned int c = 0; c < grid->columns; ++c) {
      pos[r][c] = position_new(r, c); 
      grid_set(grid, &pos[r][c], &pos[r][c]);
      printf("%u %u\n", r, c);
    }
  }

  char* str = grid_to_string(grid, (FnToString) position_to_string).ok;
  printf("%s", str);
}

void add2(int* v) {
  *v += 2;
}

void print(int* v) {
  printf("%d\n", *v);
}

void prev() {
  printf("mutex + node = %ld\n", sizeof(pthread_mutex_t) + sizeof(ListNode));
  printf("node = %ld\n", sizeof(ListNode));
  printf("list = %ld\n", sizeof(List));
  Array* arr = array_new(3).ok;
  int values[arr->capacity] = {};
  
  for (unsigned int i = 0; i < arr->capacity; ++i) {
    values[i] = i + 1;
    array_append(arr, &values[i]);
  }

  array_for_each(arr, (FnArrayEach) print);
  printf("\n");
  array_for_each(arr, (FnArrayEach) add2);
  array_for_each(arr, (FnArrayEach) print);
}

int main() {
  // prev();
  if (-1) {
    printf("-1\n");
  }
  if (0) {
    printf("0\n");
  }
  if (1) {
    printf("1\n");
  }
  return 0;
}
