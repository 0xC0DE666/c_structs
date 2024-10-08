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

  (*node)->next = NULL;
  (*node)->previous = NULL;
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

int linked_list_size(LinkedList* list) {
  int sze = 0;
  Node* n = list->head;
  while(n) {
    ++sze;
    n = n->next;
  }
  return sze;
}

LinkedList* linked_list_new() {
  LinkedList* list = malloc(sizeof(LinkedList));

  if (list == NULL) {
    return NULL;
  }

  pthread_mutex_init(&list->lock, NULL);
  list->head = NULL;
  list->tail = NULL;
  
  return list;
}


int linked_list_clear(LinkedList* const list, FreeFn const free_value) {
  int e = pthread_mutex_lock(&list->lock);
  if (e) return e;

  // empty
  if (list->head == NULL && list->tail == NULL) {
    e = pthread_mutex_unlock(&list->lock);
    if (e) return e;

    return 0;
  }

  // sinlge node
  if (list->head == list->tail) {
    e = node_free(&list->head, free_value);
    if (e) return e;

    list->head = NULL;
    list->tail = NULL;

    e = pthread_mutex_unlock(&list->lock);
    if (e) return e;

    return 0;
  }

  // multiple nodes
  while(list->head) {
    Node* next = list->head->next;
    list->head->next ? list->head->next->previous = NULL : 0;
    list->head->previous ? list->head->previous->next = NULL : 0;

    e = node_free(&list->head, free_value);
    if (e) return e;

    list->head = next;
  }
  list->head = NULL;
  list->tail = NULL;

  e = pthread_mutex_unlock(&list->lock);
  if (e) return e;

  return 0;
}

int linked_list_free(LinkedList** const list, FreeFn const free_value) {
  int e = linked_list_clear(*list, free_value);
  if (e) return e;

  pthread_mutex_destroy(&(*list)->lock);
  free(*list);
  *list = NULL;

  return 0;
}

int linked_list_append(LinkedList* list, void* value) {
  int e = pthread_mutex_lock(&list->lock);
  if (e) return e;

  Node* node = node_new(value);
  if (list->head == NULL && list->tail == NULL) {
    list->head = node;
    list->tail = node;

    e = pthread_mutex_unlock(&list->lock);
    if (e) return e;
    
    return 0;
  }
  
  list->tail->next = node;
  node->previous = list->tail;
  list->tail = node;

  e = pthread_mutex_unlock(&list->lock);
  if (e) return e;

  return 0;
}

int linked_list_prepend(LinkedList* list, void* value) {
  int e = pthread_mutex_lock(&list->lock);
  if (e) return e;

  Node* node = node_new(value);
  if (list->head == NULL && list->tail == NULL) {
    list->head = node;
    list->tail = node;

    e = pthread_mutex_unlock(&list->lock);
    if (e) return e;
    
    return 0;
  }

  list->head->previous = node;
  node->next = list->head;
  list->head = node;

  e = pthread_mutex_unlock(&list->lock);
  if (e) return e;

  return 0;
}

int linked_list_insert_before(LinkedList* const list, Node* const node, void* const value) {
  int e = pthread_mutex_lock(&list->lock);
  if (e) return e;
  
  Node* new_node = node_new(value);
  // insert before head
  if (node == list->head) {
    list->head->previous = new_node;
    new_node->next = list->head;
    list->head = new_node;

    e = pthread_mutex_unlock(&list->lock);
    if (e) return e;

    return 0;
  }

  // insert before mid node
  new_node->next = node;
  new_node->previous = node->previous;
  node->previous->next = new_node;
  node->previous = new_node;

  e = pthread_mutex_unlock(&list->lock);
  if (e) return e;

  return 0;
}


  int linked_list_insert_after(LinkedList* const list, Node* const node, void* const value) {
  int e = pthread_mutex_lock(&list->lock);
  if (e) return e;
  
  Node* new_node = node_new(value);
  // insert after tail
  if (node == list->tail) {
    new_node->previous = list->tail;
    list->tail->next = new_node;
    list->tail = new_node;

    e = pthread_mutex_unlock(&list->lock);
    if (e) return e;

    return 0;
  }

  // insert after mid node
  new_node->previous = node;
  new_node->next = node->next;
  node->next->previous = new_node;
  node->next = new_node;

  e = pthread_mutex_unlock(&list->lock);
  if (e) return e;

  return 0;
}

Result linked_list_remove_head(LinkedList* const list) {
  int e = pthread_mutex_lock(&list->lock);
  if (e) return fail(e, "failed to lock");

  // empty
  if (list->head == NULL && list->tail == NULL) {
    e = pthread_mutex_unlock(&list->lock);
    if (e) return fail(e, "failed to unlock");

    return success(NULL);
  }

  // sinlge node
  if (list->head == list->tail) {
    Node* n = list->head;
    list->head = NULL;
    list->tail = NULL;

    e = pthread_mutex_unlock(&list->lock);
    if (e) return fail(e, "failed to unlock");

    return success(n);
  }

  // multiple nodes
  Node* n = list->head;
  list->head = list->head->next;
  list->head->previous = NULL;
  n->next = NULL;
  n->previous = NULL;

  e = pthread_mutex_unlock(&list->lock);
  if (e) return fail(e, "failed to unlock");

  return success(n);
}

Result linked_list_remove_tail(LinkedList* const list) {
  int e = pthread_mutex_lock(&list->lock);
  if (e) return fail(e, "failed to lock");

  // empty
  if (list->head == NULL && list->tail == NULL) {
    e = pthread_mutex_unlock(&list->lock);
    if (e) return fail(e, "failed to unlock");

    return success(NULL);
  }

  // sinlge node
  if (list->head == list->tail) {
    Node* n = list->head;
    list->head = NULL;
    list->tail = NULL;

    e = pthread_mutex_unlock(&list->lock);
    if (e) return fail(e, "failed to unlock");

    return success(n);
  }

  // multiple nodes
  Node* n = list->tail;
  list->tail = list->tail->previous;
  list->tail->next = NULL;
  n->next = NULL;
  n->previous = NULL;

  e = pthread_mutex_unlock(&list->lock);
  if (e) return fail(e, "failed to unlock");

  return success(n);
}
