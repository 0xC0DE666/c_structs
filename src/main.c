#include <stdio.h>
#include <stdlib.h>

#include "lib.h"

/*
 * Implement a basic array.
 */

typedef struct {
  int x;
  int y;
} Point;

void print_points() {
  Point* values = malloc(10 * sizeof(Point));
  Point p = {0};
  printf("%d %d\n\n", p.x, p.y);

  Point points[10] = {};
  for (int i = 0; i < 10; i++) {
    Point* p = (Point*)(values + i);
    (*p).x = i + 1;
    (*p).y = i + 10;
    *((Point*)(values + i)) = *p;
  }

  *(values + 0) = *(values + 1);
  
  for (int i = 0; i < 10; i++) {
    Point* p = (Point*)(values + i);
    printf("%d %d\n", (*p).x, (*p).y);
  }
}

int main() {
  // print_points();
  
  Array* numbers = array_new(10);

  int val[numbers->size];
  for (int i = 0; i < numbers->size; i++) {
    val[i] = (i + 1) * 10;
    array_add(numbers, &val[i]);
  }

  array_remove(numbers, 0);

  for (int i = 0; i < numbers->count; i++) {
    int v = *(int*)array_get(numbers, i);
    printf("%d\n", v);
  }
  
  return 0;
}
