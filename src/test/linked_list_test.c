#include <stdbool.h>
//#include <stdio.h>
//#include <stdlib.h>

#include <string.h>

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "../lib/c_structs.h"
#include "utils.h"

// ####################
// linked_list_new
// ####################
// Test(linked_list_new, _1) {
//   Array* linked_list = linked_list_new(10);
// 
//   cr_assert_eq(linked_list->capacity, 10);
//   cr_assert_eq(linked_list->size, 0);
//   cr_assert_eq(linked_list->elements == NULL, false);
// 
//   for (int i = 0; i < linked_list->size; ++i) {
//     cr_assert_eq(linked_list->elements[i], NULL);
//   }
// 
//   linked_list_free(&linked_list, NULL);
// }
// 
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
