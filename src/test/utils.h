#ifndef UTILS_H
#define UTILS_H

  void safe_free(void** ptr);
  void need_not_free(void** ptr);

  typedef struct {
    int x;
    int y;
  } Point;


  Point* point_new(int x, int y);
  char* point_to_string(Point* const point);
  void point_double(Point* const point);

  void print_str(char* str);
  void* as_is(void* v);

#endif
