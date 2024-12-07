#ifndef UTILS_H
#define UTILS_H

  #include "../lib/c_structs.h"

  void* as_is(void* v);
  void print_str(char* str);
  void safe_free(void** ptr);
  void need_not_free(void** ptr);

  typedef struct {
    int x;
    int y;
  } Point;


  Point* point_new(int x, int y);
  char* point_to_string(Point* const point);
  void point_double(Point* const point);
  int point_compare(Point* const a, Point* const b);

  Tree* point_tree();

#endif
