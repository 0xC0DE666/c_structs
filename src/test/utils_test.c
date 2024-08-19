#include <stdlib.h>

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "utils.h"


// ####################
// point_new
// ####################
Test(point_new, _1) {
  Point* point = point_new(0, 1);

  cr_assert_eq(point->x, 0);
  cr_assert_eq(point->y, 1);

  point_free(&point);
}

// ####################
// point_free
// ####################
Test(point_free, _1) {
  Point* point = point_new(5, 1);

  point_free(&point);

  cr_assert_eq(point, NULL);
}

// ####################
// point_to_str
// ####################
Test(point_to_str, _1) {
  Point* point = point_new(3, 2);
  
  char* result = point_to_str(point);
  char* expected = "(3, 2)";

  cr_assert_eq(strcmp(result, expected), 0);

  free(result);
  point_free(&point);
}
