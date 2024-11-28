#include <stdbool.h>
//#include <stdio.h>
//#include <stdlib.h>

#include <string.h>

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "../lib/c_structs.h"
#include "utils.h"

// ####################
// tree_node_new
// ####################
Test(tree_node_new, _1) {
  TreeNode* node = tree_node_new(point_new(0, 1)).ok;

  cr_assert_eq(node != NULL, true);
  cr_assert_eq(node->value != NULL, true);
  cr_assert_eq(node->left_child, NULL);
  cr_assert_eq(node->right_child, NULL);

  Point* p = node->value;
  cr_assert_eq(p->x, 0);
  cr_assert_eq(p->y, 1);

  tree_node_free(&node, (FnFree) safe_free);
}

// ####################
// tree_node_free
// ####################
Test(tree_node_free, _1) {
  TreeNode* node = tree_node_new(point_new(0, 1)).ok;

  cr_assert_eq(node != NULL, true);
  cr_assert_eq(node->value != NULL, true);

  int e = tree_node_free(&node, (FnFree) safe_free);
  cr_assert_eq(e, 0);
  cr_assert_eq(node, NULL);
}

// ####################
// tree_new
// ####################
Test(tree_new, _1) {
  Tree* tree = tree_new().ok;

  cr_assert_eq(tree != NULL, true);
  cr_assert_eq(tree->root, NULL);

  tree_free(&tree, NULL);
}

// ####################
// tree_clear
// ####################
Test(tree_clear, empty) {
  Tree* tree = tree_new().ok;
  int e = tree_clear(tree, NULL);

  cr_assert_eq(e, 0);
  cr_assert_eq(tree != NULL, true);
  cr_assert_eq(tree->root, NULL);

  tree_free(&tree, NULL);
}

Test(tree_clear, single) {
  Tree* tree = tree_new().ok;
  tree->root = tree_node_new(point_new(0, 0)).ok;

  int e = tree_clear(tree, (FnFree) safe_free);
  cr_assert_eq(e, 0);
  cr_assert_eq(tree != NULL, true);
  cr_assert_eq(tree->root, NULL);

  tree_free(&tree, (FnFree) safe_free);
}

// // TODO: check this, check a b c is freed
// Test(tree_clear, multiple) {
//   Tree* tree = tree_new().ok;
//   TreeNode* a = tree_node_new(point_new(0, 0)).ok;
//   TreeNode* b = tree_node_new(point_new(1, 1)).ok;
//   TreeNode* c = tree_node_new(point_new(2, 2)).ok;
// 
//   a->next = b;
//   b->previous = a;
//   b->next = c;
//   c->previous = b;
//   tree->head = a;
//   tree->tail = c;
// 
//   int e = tree_clear(tree, (FnFree) safe_free);
//   cr_assert_eq(e, 0);
//   cr_assert_eq(tree != NULL, true);
//   cr_assert_eq(tree->head, NULL);
//   cr_assert_eq(tree->tail, NULL);
// 
//   tree_free(&tree, (FnFree) safe_free);
// }
