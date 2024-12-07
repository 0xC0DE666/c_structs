#include <stdlib.h>

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "utils.h"

// ####################
// safe_free
// ####################
Test(safe_free, _1) {
  Point* point = point_new(5, 1);

  cr_assert_eq(point != NULL, true);

  safe_free((void**) &point);

  cr_assert_eq(point, NULL);
}

Test(safe_free, _2) {
  char* str = malloc(sizeof(char) * 10);

  cr_assert_eq(str != NULL, true);

  safe_free((void**) &str);

  cr_assert_eq(str, NULL);
}

// ####################
// point_new
// ####################
Test(point_new, _1) {
  Point* point = point_new(0, 1);

  cr_assert_eq(point->x, 0);
  cr_assert_eq(point->y, 1);

  safe_free((void**) &point);
}

// ####################
// point_to_str
// ####################
Test(point_to_str, _1) {
  Point* point = point_new(3, 2);
  
  char* result = point_to_string(point);
  char* expected = "(3, 2)";

  cr_assert_eq(strcmp(result, expected), 0);

  free(result);
  safe_free((void**) &point);
}

// ####################
// point_double
// ####################
Test(point_double, _1) {
  Point* point = point_new(3, 2);
  
  point_double(point);

  cr_assert_eq(point->x, 6);
  cr_assert_eq(point->y, 4);

  safe_free((void**) &point);
}

// ####################
// point_compare
// ####################
Test(point_compare, _1) {
  Point* a = point_new(0, 0);
  Point* b = point_new(1, 1);

  // a before b
  int v = point_compare(a, b);
  cr_assert_eq(v <= -1, true);

  a->x = 1; a->y = 1;
  b->x = 1; b->y = 1;

  // a equal b
  v = point_compare(a, b);
  cr_assert_eq(v == 0, true);

  a->x = 2; a->y = 2;
  b->x = 1; b->y = 1;
  // b before a
  v = point_compare(a, b);
  cr_assert_eq(v >= 1, true);


  safe_free((void**) &a);
  safe_free((void**) &b);
}

// ####################
// point_tree
// ####################
Test(point_tree, _1) {
  Tree* tree = point_tree();

  Point* p = (Point*) tree->root->value;
  cr_assert_eq(p->x, 1);
  cr_assert_eq(p->y, 1);

  p = (Point*) tree->root->left_child->value;
  cr_assert_eq(p->x, 0);
  cr_assert_eq(p->y, 0);

  p = (Point*) tree->root->right_child->value;
  cr_assert_eq(p->x, 2);
  cr_assert_eq(p->y, 2);


  p = (Point*) tree->root->left_child->right_child->value;
  cr_assert_eq(p->x, 3);
  cr_assert_eq(p->y, 3);

  p = (Point*) tree->root->right_child->left_child->value;
  cr_assert_eq(p->x, 1);
  cr_assert_eq(p->y, 1);

  tree_free(&tree, (FnFree) safe_free);
}
