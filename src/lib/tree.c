#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "c_structs.h"

Result tree_node_new(void* const value) {
  TreeNode* node = malloc(sizeof(TreeNode));

  if (node == NULL) {
    return result_std_error();
  }

  node->value = value;
  node->left_child = NULL;
  node->right_child = NULL;
  
  return result_ok(node);
}

int tree_node_free(TreeNode** node, FnFree const free_value) {
  free_value(&(*node)->value); 

  (*node)->value = NULL;
  (*node)->left_child = NULL;
  (*node)->right_child = NULL;
  free(*node);
  *node = NULL;

  return 0;
}

// int tree_size(Tree* tree) {
//   int sze = 0;
//   TreeNode* n = tree->head;
//   while(n) {
//     ++sze;
//     n = n->next;
//   }
//   return sze;
// }

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

  // empty
  if (tree->root == NULL) {
    e = pthread_rwlock_unlock(&tree->lock);
    if (e) return e;

    return 0;
  }

  // sinlge node
  if (tree->root != NULL) {
    e = tree_node_free(&tree->root, free_value);
    if (e) return e;

    tree->root = NULL;

    e = pthread_rwlock_unlock(&tree->lock);
    if (e) return e;

    return 0;
  }

  // // multiple nodes
  // while(tree->head) {
  //   TreeNode* next = tree->head->next;
  //   tree->head->next ? tree->head->next->previous = NULL : 0;
  //   tree->head->previous ? tree->head->previous->next = NULL : 0;

  //   e = tree_node_free(&tree->head, free_value);
  //   if (e) return e;

  //   tree->head = next;
  // }
  // tree->head = NULL;
  // tree->tail = NULL;

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

// int tree_insert(Tree* const tree, void* const value, FnComparator compare) {
//   int e = pthread_rwlock_trywrlock(&tree->lock);
//   if (e) return e;
// 
//   TreeNode* node = tree_node_new(value).ok;
//   if (tree->head == NULL && tree->tail == NULL) {
//     tree->head = node;
//     tree->tail = node;
// 
//     e = pthread_rwlock_unlock(&tree->lock);
//     if (e) return e;
//     
//     return 0;
//   }
//   
//   tree->tail->next = node;
//   node->previous = tree->tail;
//   tree->tail = node;
// 
//   e = pthread_rwlock_unlock(&tree->lock);
//   if (e) return e;
// 
//   return 0;
// }
// 
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
