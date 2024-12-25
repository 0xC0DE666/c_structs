#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../test/utils.h"
#include "c_structs.h"

// ####################
// TreeNode
// ####################

Result tree_node_new(void* const value) {
  TreeNode* node = malloc(sizeof(TreeNode));

  if (node == NULL) {
    return result_std_error();
  }

  node->value = value;
  node->parent = NULL;
  node->left_child = NULL;
  node->right_child = NULL;
  node->rank = 0;
  node->passed = false;
  
  return result_ok(node);
}

int tree_node_free(TreeNode** node, FnFree const free_value) {
  free_value(&(*node)->value); 

  TreeNode* parent = (*node)->parent;
  if (parent != NULL && *node == parent->left_child) {
    parent->left_child = NULL;
  } 

  if (parent != NULL && *node == parent->right_child) {
    parent->right_child = NULL;
  }

  (*node)->value = NULL;
  (*node)->parent = NULL;
  (*node)->left_child = NULL;
  (*node)->right_child = NULL;

  free(*node);
  *node = NULL;

  return 0;
}

bool tree_node_leaf(TreeNode* node) {
  return node->left_child == NULL && node->right_child == NULL;
}

// ####################
// Tree
// ####################

Result tree_new() {
  Tree* tree = malloc(sizeof(Tree));

  if (tree == NULL) {
    return result_std_error();
  }

  int e = pthread_rwlock_init(&tree->lock, NULL);
  if (e) {
    free(tree);
    return result_std_error();
  }
  tree->root = NULL;
  
  return result_ok(tree);
}

int tree_clear(Tree* const tree, FnFree const free_value) {
  int e = pthread_rwlock_trywrlock(&tree->lock);
  if (e) return e;

  TreeNode* node = tree->root;
  TreeNode* parent = NULL;
  while(node != NULL) {
    if (node->left_child != NULL) {
      node = node->left_child;
      continue;
    }

    if (node->right_child != NULL) {
      node = node->right_child;
      continue;
    }

    if (tree_node_leaf(node)) {
      parent = node->parent;
    }

    int e = tree_node_free(&node, free_value);
    if (e) return e;
    node = parent;
  }
  tree->root = NULL;

  e = pthread_rwlock_unlock(&tree->lock);
  if (e) return e;

  return 0;
}

int tree_free(Tree** const tree, FnFree const free_value) {
  int e = tree_clear(*tree, free_value);
  if (e) return e;

  pthread_rwlock_destroy(&(*tree)->lock);
  free(*tree);
  *tree = NULL;

  return 0;
}

// TODO: add func tree_set_passed();
int tree_height(Tree* tree) {
  int e = pthread_rwlock_trywrlock(&tree->lock);
  if (e) return e;

  int root_count = -1;
  int left_tree = 0;
  int right_tree = 0;
  char side[6] = "left";

  TreeNode* node = tree->root;
  while(node != NULL) {
    if (node == tree->root) {
      ++root_count;
      if (root_count == 1) {
        strcpy(side, "right");
      }
    }
    node->passed = true;

    if (node->left_child != NULL && node->left_child->passed == false) {
      strcmp(side, "left") == 0 ? ++left_tree : ++right_tree;
      node = node->left_child;
      continue;
    }

    if (node->right_child != NULL && node->right_child->passed == false) {
      strcmp(side, "left") == 0 ? ++left_tree : ++right_tree;
      node = node->right_child;
      continue;
    }

    if (node->passed || tree_node_leaf(node)) {
      node = node->parent;
    }
  }

  e = pthread_rwlock_unlock(&tree->lock);
  if (e) return e;

  return (left_tree > right_tree ? left_tree : right_tree) + 1;
}

int tree_insert(Tree* const tree, void* const value, FnComparator compare) {
  int e = pthread_rwlock_trywrlock(&tree->lock);
  if (e) return e;

  TreeNode* new_node = tree_node_new(value).ok;
  if (tree->root == NULL) {
    tree->root = new_node;

    e = pthread_rwlock_unlock(&tree->lock);
    if (e) return e;

    return SUC_CODE_GENERAL;
  }

  TreeNode* cur_node = tree->root;
  bool inserted = false;
  while (true) {
    if (compare(value, cur_node->value) < 0) {
      if (cur_node->left_child != NULL) {
        cur_node = cur_node->left_child;
        continue;
      }
      new_node->parent = cur_node;
      cur_node->left_child = new_node;
      inserted = true;
    } else {
      if (cur_node->right_child != NULL) {
        cur_node = cur_node->right_child;
        continue;
      }
      new_node->parent = cur_node;
      cur_node->right_child = new_node;
      inserted = true;
    }

    if (inserted) {
      e = pthread_rwlock_unlock(&tree->lock);
      if (e) return e;

      return SUC_CODE_GENERAL;
    }
  }

  return ERR_CODE_GENERAL;
}

//int tree_rank_nodes(Tree* const tree) {
//  int e = pthread_rwlock_trywrlock(&tree->lock);
//  if (e) return e;
//
//  TreeNode* node = tree->root;
//  while(node != NULL) {
//    if (node->left_child != NULL) {
//      node = node->left_child;
//      continue;
//    }
//
//    if (node->right_child != NULL) {
//      node = node->right_child;
//      continue;
//    }
//
//    if (node->left_child == NULL && node->right_child == NULL) {
//      parent = node->parent;
//    }
//
//    int e = tree_node_free(&node, free_value);
//    if (e) return e;
//    node = parent;
//  }
//  tree->root = NULL;
//
//  e = pthread_rwlock_unlock(&tree->lock);
//  if (e) return e;
//
//  return 0;
//}



// Result tree_remove(Tree* const tree, TreeNode* node) {
//   int e = pthread_rwlock_trywrlock(&tree->lock);
//   if (e) return result_std_error();
// 
//   // sinlge
//   if (tree->head == tree->tail) {
//     e = pthread_rwlock_unlock(&tree->lock);
//     if (e) return result_std_error();
// 
//     return tree_remove_head(tree);
//   }
// 
//   // head
//   if (node == tree->head) {
//     e = pthread_rwlock_unlock(&tree->lock);
//     if (e) return result_std_error();
// 
//     return tree_remove_head(tree);
//   }
// 
//   // tail
//   if (node == tree->tail) {
//     e = pthread_rwlock_unlock(&tree->lock);
//     if (e) return result_std_error();
// 
//     return tree_remove_tail(tree);
//   }
// 
//   // mid
//   node->next->previous = node->previous;
//   node->previous->next = node->next;
//   node->next = NULL;
//   node->previous = NULL;
// 
//   e = pthread_rwlock_unlock(&tree->lock);
//   if (e) return result_std_error();
// 
//   return result_ok(node);
// }
// 
// 
// Result tree_get(Tree* const tree, TreeNode* node) {
//   int e = pthread_rwlock_trywrlock(&tree->lock);
//   if (e) return result_std_error();
// 
//   TreeNode* n = tree->head;
//   while (n) {
//     if (predicate(n)) {
//       e = pthread_rwlock_unlock(&tree->lock);
//       if (e) return result_std_error();
// 
//       return result_ok(n);
//     }
//     n = n->next;
//   }
//   
//   e = pthread_rwlock_unlock(&tree->lock);
//   if (e) return result_std_error();
// 
//   return result_ok(NULL);
// }
