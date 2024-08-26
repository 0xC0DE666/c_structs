#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

Point* point_new(int x, int y) {
  Point* point = malloc(sizeof(Point));

  if (point == NULL) {
    return NULL;
  }
  point->x = x;
  point->y = y;

  return point;
}

void point_free(Point** const point) {
  free(*point);
  *point = NULL;
}

char* point_to_str(Point* point) {
  char* buffer = malloc(sizeof(char) * 5);
 
  sprintf(
    buffer,
    "(%d, %d)",
    point->x,
    point->y
  ); 
 
  return buffer;
}

void inc(int* v) {
  *v += 1;
}
