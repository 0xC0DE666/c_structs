#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "c_structs.h"

int node_lock(Node* node) {
  int e = pthread_mutex_trylock(&node->lock);
  if (e) return e;

  if (node->next != NULL) {
    e = pthread_mutex_trylock(&node->next->lock);
    if (e) {
      pthread_mutex_unlock(&node->lock);
      return e;
    }
  }

  if (node->previous != NULL) {
    e = pthread_mutex_trylock(&node->previous->lock);
    if (e) {
      pthread_mutex_unlock(&node->lock);
      if (node->next) {
        pthread_mutex_unlock(&node->next->lock);
      }
      return e;
    }
  }

  return 0;
}

int node_unlock(Node* node) {
  int e = pthread_mutex_unlock(&node->lock);
  if (e) return e;

  if (node->next != NULL) {
    e = pthread_mutex_unlock(&node->next->lock);
    if (e) return e;
  }

  if (node->previous != NULL) {
    e = pthread_mutex_unlock(&node->previous->lock);
    if (e) return e;
  }

  return 0;
}

Node* node_new(void* const value) {
  Node* node = malloc(sizeof(Node));

  if (node == NULL) {
    return NULL;
  }

  pthread_mutex_init(&node->lock, NULL);
  node->value = value;
  node->next = NULL;
  node->previous = NULL;
  
  return node;
}

int node_free(Node** node, FreeFn const free_value) {
  int e = node_lock(*node);
  if (e) return e;

  free_value(&(*node)->value); 
  (*node)->value = NULL;

  e = node_unlock(*node);
  if (e) return e;

  e = pthread_mutex_destroy(&(*node)->lock);
  if (e) return e;

  free(*node);
  *node = NULL;

  return 0;
}

int node_set_value(Node* const node, void* const value) {
  int e = node_lock(node);
  if (e) return e;

  node->value = value;

  e = node_unlock(node);
  if (e) return e;

  return 0;
}

int node_set_next(Node* const node, Node* const other) {
  int e = node_lock(node);
  if (e) return e;

  node->next = other;

  e = node_unlock(node);
  if (e) return e;

  return 0;
}

int node_set_previous(Node* const node, Node* const other) {
  int e = node_lock(node);
  if (e) return e;

  node->previous = other;

  e = node_unlock(node);
  if (e) return e;

  return 0;
}

// LinkedList* linked_list_new() {
//   LinkedList* list = malloc(sizeof(LinkedList));
// 
//   if (list == NULL) {
//     return NULL;
//   }
//   
//   return list;
// }
// 
// 
// Int linked_list_free(LinkedList** const list, FreeFn const free_value) {
//   return 0;
// }
