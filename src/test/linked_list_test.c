#include <stdbool.h>
//#include <stdio.h>
//#include <stdlib.h>

#include <string.h>

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "../lib/c_structs.h"
#include "utils.h"

// ####################
// node_new
// ####################
Test(node_new, _1) {
  Node* node = node_new(point_new(0, 1));
  cr_assert_eq(node != NULL, true);
  cr_assert_eq(node->value != NULL, true);
  cr_assert_eq(node->next, NULL);
  cr_assert_eq(node->prev, NULL);

  Point* p = node->value;
  cr_assert_eq(p->x, 0);
  cr_assert_eq(p->y, 1);

  node_free(&node, (FreeFn) point_free);
}

// ####################
// node_free
// ####################
Test(node_free, _1) {
  Node* node = node_new(point_new(0, 1));
  cr_assert_eq(node != NULL, true);
  cr_assert_eq(node->value != NULL, true);

  int e = node_free(&node, (FreeFn) point_free);
  cr_assert_eq(e, 0);
  cr_assert_eq(node, NULL);
}

// ####################
// linked_list_new
// ####################
//Test(linked_list_new, _1) {
//  LinkedList* list = linked_list_new();
//
//  cr_assert_eq(list != NULL, true);
//  cr_assert_eq(list->head, NULL);
//  cr_assert_eq(list->tail, NULL);
//
//  linked_list_free(&list, NULL);
//}

// // ####################
// // linked_list_clear
// // ####################
// Test(linked_list_clear, _1) {
//   Array* linked_list = linked_list_new(5);
// 
//   for (int i = 0; i < linked_list->capacity; ++i) {
//     linked_list_append(linked_list, point_new(i, i + 1));
//     cr_assert_eq(linked_list->size, i + 1);
//     cr_assert_eq(linked_list_get(linked_list, i) != NULL, true);
//   }
//   cr_assert_eq(linked_list->size, linked_list->capacity);
// 
//   linked_list_clear(linked_list, (FreeFn) point_free);
//   cr_assert_eq(linked_list != NULL, true);
//   cr_assert_eq(linked_list->size, 0);
// 
//   for (int i = 0; i < linked_list->capacity; ++i) {
//     cr_assert_eq(linked_list->elements[i], NULL);
//   }
// 
//   linked_list_free(&linked_list, (FreeFn) point_free);
// }
// 
// // ####################
// // linked_list_free
// // ####################
// Test(linked_list_free, _1) {
//   Array* linked_list = linked_list_new(5);
// 
//   for (int i = 0; i < linked_list->capacity; ++i) {
//     linked_list_append(linked_list, point_new(i, i + 1));
//     cr_assert_eq(linked_list->size, i + 1);
//   }
//   cr_assert_eq(linked_list->size, linked_list->capacity);
// 
//   linked_list_free(&linked_list, (FreeFn) point_free);
//   cr_assert_eq(linked_list, NULL);
// }
