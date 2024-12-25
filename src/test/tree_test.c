#include <stdbool.h>
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
// tree_node_leaf
// ####################
Test(tree_node_leaf, _1) {
  TreeNode* node = tree_node_new(point_new(0, 1)).ok;
  cr_assert_eq(tree_node_leaf(node), true);

  node->left_child = tree_node_new(point_new(0, 0)).ok;
  cr_assert_eq(tree_node_leaf(node), false);

  int e = tree_node_free(&(node->left_child), (FnFree) safe_free);
  cr_assert_eq(e, 0);
  e = tree_node_free(&node, (FnFree) safe_free);
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

Test(tree_clear, multiple) {
  Tree* tree = point_tree();

  int e = tree_clear(tree, (FnFree) safe_free);
  cr_assert_eq(e, 0);
  cr_assert_eq(tree != NULL, true);
  cr_assert_eq(tree->root, NULL);

  tree_free(&tree, (FnFree) safe_free);
}

// ####################
// tree_free
// ####################
Test(tree_free, _1) {
  Tree* tree = point_tree();

  int e = tree_free(&tree, (FnFree) safe_free);
  cr_assert_eq(e, 0);
  cr_assert_eq(tree, NULL);
}

// ####################
// tree_insert
// ####################
Test(tree_insert, _1) {
  Tree* tree = tree_new().ok;

  // root
  int e = tree_insert(tree, point_new(1, 1), (FnComparator) point_compare);
  cr_assert_eq(e, 0);

  TreeNode* n = tree->root;
  cr_assert_eq(n->parent == NULL, true);

  Point* p = (Point*) n->value;
  cr_assert_eq(p != NULL, true);
  cr_assert_eq(p->x, 1);
  cr_assert_eq(p->y, 1);

  // left_child
  e = tree_insert(tree, point_new(0, 0), (FnComparator) point_compare);
  cr_assert_eq(e, 0);

  n = tree->root->left_child;
  cr_assert_eq(n->parent, tree->root);

  p = (Point*) n->value;
  cr_assert_eq(p != NULL, true);
  cr_assert_eq(p->x, 0);
  cr_assert_eq(p->y, 0);

  // 2nd left_child
  e = tree_insert(tree, point_new(-1, -1), (FnComparator) point_compare);
  cr_assert_eq(e, 0);

  n = tree->root->left_child->left_child;
  cr_assert_eq(n->parent, tree->root->left_child);

  p = (Point*) n->value;
  cr_assert_eq(p != NULL, true);
  cr_assert_eq(p->x, -1);
  cr_assert_eq(p->y, -1);

  // right_child
  e = tree_insert(tree, point_new(2, 2), (FnComparator) point_compare);
  cr_assert_eq(e, 0);

  n = tree->root->right_child;
  cr_assert_eq(n->parent, tree->root);

  p = (Point*) n->value;
  cr_assert_eq(p != NULL, true);
  cr_assert_eq(p->x, 2);
  cr_assert_eq(p->y, 2);

  // 2nd right_child
  e = tree_insert(tree, point_new(2, 2), (FnComparator) point_compare);
  cr_assert_eq(e, 0);

  n = tree->root->right_child->right_child;
  cr_assert_eq(n->parent, tree->root->right_child);

  p = (Point*) n->value;
  cr_assert_eq(p != NULL, true);
  cr_assert_eq(p->x, 2);
  cr_assert_eq(p->y, 2);

  tree_free(&tree, (FnFree) safe_free);
}
