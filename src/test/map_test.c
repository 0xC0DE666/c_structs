#include <stdbool.h>
//#include <stdio.h>
//#include <stdlib.h>

#include <string.h>

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "../lib/c_structs.h"
#include "utils.h"

// ####################
// map_node_new
// ####################
Test(map_node_new, _1) {
  MapNode* node = map_node_new(point_new(0, 1)).ok;

  cr_assert_eq(node != NULL, true);
  cr_assert_eq(node->value != NULL, true);
  cr_assert_eq(node->left_child, NULL);
  cr_assert_eq(node->right_child, NULL);

  Point* p = node->value;
  cr_assert_eq(p->x, 0);
  cr_assert_eq(p->y, 1);

  map_node_free(&node, (FnFree) safe_free);
}

// ####################
// map_node_free
// ####################
Test(map_node_free, _1) {
  MapNode* node = map_node_new(point_new(0, 1)).ok;

  cr_assert_eq(node != NULL, true);
  cr_assert_eq(node->value != NULL, true);

  int e = map_node_free(&node, (FnFree) safe_free);
  cr_assert_eq(e, 0);
  cr_assert_eq(node, NULL);
}

// ####################
// map_new
// ####################
Test(map_new, _1) {
  Map* map = map_new().ok;

  cr_assert_eq(map != NULL, true);
  cr_assert_eq(map->root, NULL);

  map_free(&map, NULL);
}

// ####################
// map_clear
// ####################
Test(map_clear, empty) {
  Map* map = map_new().ok;
  int e = map_clear(map, NULL);

  cr_assert_eq(e, 0);
  cr_assert_eq(map != NULL, true);
  cr_assert_eq(map->root, NULL);

  map_free(&map, NULL);
}

Test(map_clear, single) {
  Map* map = map_new().ok;
  map->root = map_node_new(point_new(0, 0)).ok;

  int e = map_clear(map, (FnFree) safe_free);
  cr_assert_eq(e, 0);
  cr_assert_eq(map != NULL, true);
  cr_assert_eq(map->root, NULL);

  map_free(&map, (FnFree) safe_free);
}

// // TODO: check this, check a b c is freed
// Test(map_clear, multiple) {
//   Map* map = map_new().ok;
//   MapNode* a = map_node_new(point_new(0, 0)).ok;
//   MapNode* b = map_node_new(point_new(1, 1)).ok;
//   MapNode* c = map_node_new(point_new(2, 2)).ok;
// 
//   a->next = b;
//   b->previous = a;
//   b->next = c;
//   c->previous = b;
//   map->head = a;
//   map->tail = c;
// 
//   int e = map_clear(map, (FnFree) safe_free);
//   cr_assert_eq(e, 0);
//   cr_assert_eq(map != NULL, true);
//   cr_assert_eq(map->head, NULL);
//   cr_assert_eq(map->tail, NULL);
// 
//   map_free(&map, (FnFree) safe_free);
// }
