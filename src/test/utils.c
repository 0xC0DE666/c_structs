#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

void safe_free(void** ptr) {
  if (ptr == NULL || *ptr == NULL) return;

  free(*ptr);
  *ptr = NULL;
}

void need_not_free(void** ptr) {}

Point* point_new(int x, int y) {
  Point* point = malloc(sizeof(Point));

  if (point == NULL) {
    return NULL;
  }
  point->x = x;
  point->y = y;

  return point;
}

char* point_to_string(Point* point) {
  char* buffer = malloc(sizeof(char) * 14);
 
  sprintf(
    buffer,
    "(%d, %d)",
    point->x,
    point->y
  ); 
 
  return buffer;
}

void point_double(Point* point) {
  point->x = point->x * 2;
  point->y = point->y * 2;
}

void print_string(char* str) {
  printf("%s", str);
}

void* nothing(void* v) {
  return v;
}
