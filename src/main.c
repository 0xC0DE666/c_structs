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
 int*** matrix =  malloc(3 * sizeof(void*)); 
 int x = 10;
 matrix[0][1] = &x;

 printf("%d\n", *matrix[0][1]);

  return 0;
}
