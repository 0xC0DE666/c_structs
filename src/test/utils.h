#ifndef UTILS_H
#define UTILS_H

  void free_ptr(void** ptr);

  typedef struct {
    int x;
    int y;
  } Point;


  Point* point_new(int x, int y);
  void point_free(Point** const point);
  char* point_to_str(Point* const point);
  void point_double(Point* const point);

#endif
