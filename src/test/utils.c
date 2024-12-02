#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

void* as_is(void* v) {
  return v;
}

void print_str(char* str) {
  printf("%s", str);
}

void safe_free(void** ptr) {
  if (ptr == NULL || *ptr == NULL) return;

  free(*ptr);
  *ptr = NULL;
}

void need_not_free(void** ptr) {
  if (*ptr == NULL) {}
}

Point* point_new(int x, int y) {
  Point* point = malloc(sizeof(Point));

  if (point == NULL) {
    return NULL;
  }
  point->x = x;
  point->y = y;

  return point;
}

char* point_to_string(Point* point) {
  if (point == NULL) return "";

  char* buffer = malloc(sizeof(char) * 14);
 
  sprintf(
    buffer,
    "(%d, %d)",
    point->x,
    point->y
  ); 
 
  return buffer;
}

void point_double(Point* point) {
  point->x = point->x * 2;
  point->y = point->y * 2;
}

Tree* generate_tree() {
  Tree* tree = tree_new().ok;
  tree->root = tree_node_new(point_new(1, 1)).ok;

  tree->root->left_child = tree_node_new(point_new(0, 0)).ok;
  tree->root->left_child->parent = tree->root;

  tree->root->right_child = tree_node_new(point_new(2, 2)).ok;
  tree->root->right_child->parent = tree->root;

  tree->root->left_child->right_child = tree_node_new(point_new(3, 3)).ok;
  tree->root->left_child->right_child->parent = tree->root->left_child;

  tree->root->right_child->left_child = tree_node_new(point_new(4, 4)).ok;
  tree->root->right_child->left_child->parent = tree->root->right_child;

  return tree;
}
