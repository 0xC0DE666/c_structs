#ifndef UTILS_H
#define UTILS_H

  typedef struct {
    int x;
    int y;
  } Point;


  Point* point_new(int x, int y);
  void point_free(Point** const point);
  char* point_to_str(Point* const point);

#endif
