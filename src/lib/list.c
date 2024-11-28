#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "c_structs.h"

Result list_node_new(void* const value) {
  ListNode* node = malloc(sizeof(ListNode));

  if (node == NULL) {
    return result_std_error();
  }

  node->value = value;
  node->next = NULL;
  node->previous = NULL;
  
  return result_ok(node);
}

int list_node_free(ListNode** node, FnFree const free_value) {
  free_value(&(*node)->value); 

  (*node)->value = NULL;
  (*node)->next = NULL;
  (*node)->previous = NULL;
  free(*node);
  *node = NULL;

  return 0;
}

int list_size(List* list) {
  int sze = 0;
  ListNode* n = list->head;
  while(n) {
    ++sze;
    n = n->next;
  }
  return sze;
}

Result list_new() {
  List* list = malloc(sizeof(List));

  if (list == NULL) {
    return result_std_error();
  }

  int e = pthread_rwlock_init(&list->lock, NULL);
  if (e) {
    free(list);
    return result_std_error();
  }
  list->head = NULL;
  list->tail = NULL;
  
  return result_ok(list);
}


int list_clear(List* const list, FnFree const free_value) {
  int e = pthread_rwlock_trywrlock(&list->lock);
  if (e) return e;

  while(list->head) {
    ListNode* next = list->head->next;
    list->head->next ? list->head->next->previous = NULL : 0;
    list->head->previous ? list->head->previous->next = NULL : 0;

    e = list_node_free(&list->head, free_value);
    if (e) return e;

    list->head = next;
  }
  list->head = NULL;
  list->tail = NULL;

  e = pthread_rwlock_unlock(&list->lock);
  if (e) return e;

  return 0;
}

int list_free(List** const list, FnFree const free_value) {
  int e = list_clear(*list, free_value);
  if (e) return e;

  pthread_rwlock_destroy(&(*list)->lock);
  free(*list);
  *list = NULL;

  return 0;
}

int list_append(List* list, void* value) {
  int e = pthread_rwlock_trywrlock(&list->lock);
  if (e) return e;

  ListNode* node = list_node_new(value).ok;
  if (list->head == NULL && list->tail == NULL) {
    list->head = node;
    list->tail = node;

    e = pthread_rwlock_unlock(&list->lock);
    if (e) return e;
    
    return 0;
  }
  
  list->tail->next = node;
  node->previous = list->tail;
  list->tail = node;

  e = pthread_rwlock_unlock(&list->lock);
  if (e) return e;

  return 0;
}

int list_prepend(List* list, void* value) {
  int e = pthread_rwlock_trywrlock(&list->lock);
  if (e) return e;

  ListNode* node = list_node_new(value).ok;
  if (list->head == NULL && list->tail == NULL) {
    list->head = node;
    list->tail = node;

    e = pthread_rwlock_unlock(&list->lock);
    if (e) return e;
    
    return 0;
  }

  list->head->previous = node;
  node->next = list->head;
  list->head = node;

  e = pthread_rwlock_unlock(&list->lock);
  if (e) return e;

  return 0;
}

int list_insert_before(List* const list, ListNode* const node, void* const value) {
  int e = pthread_rwlock_trywrlock(&list->lock);
  if (e) return e;
  
  ListNode* new_node = list_node_new(value).ok;
  // insert before head
  if (node == list->head) {
    list->head->previous = new_node;
    new_node->next = list->head;
    list->head = new_node;

    e = pthread_rwlock_unlock(&list->lock);
    if (e) return e;

    return 0;
  }

  // insert before mid node
  new_node->next = node;
  new_node->previous = node->previous;
  node->previous->next = new_node;
  node->previous = new_node;

  e = pthread_rwlock_unlock(&list->lock);
  if (e) return e;

  return 0;
}


  int list_insert_after(List* const list, ListNode* const node, void* const value) {
  int e = pthread_rwlock_trywrlock(&list->lock);
  if (e) return e;
  
  ListNode* new_node = list_node_new(value).ok;
  // insert after tail
  if (node == list->tail) {
    new_node->previous = list->tail;
    list->tail->next = new_node;
    list->tail = new_node;

    e = pthread_rwlock_unlock(&list->lock);
    if (e) return e;

    return 0;
  }

  // insert after mid node
  new_node->previous = node;
  new_node->next = node->next;
  node->next->previous = new_node;
  node->next = new_node;

  e = pthread_rwlock_unlock(&list->lock);
  if (e) return e;

  return 0;
}

Result list_remove_head(List* const list) {
  int e = pthread_rwlock_trywrlock(&list->lock);
  if (e) return result_std_error();

  // empty
  if (list->head == NULL && list->tail == NULL) {
    e = pthread_rwlock_unlock(&list->lock);
    if (e) return result_std_error();

    return result_ok(NULL);
  }

  // sinlge node
  if (list->head == list->tail) {
    ListNode* n = list->head;
    list->head = NULL;
    list->tail = NULL;

    e = pthread_rwlock_unlock(&list->lock);
    if (e) return result_std_error();

    return result_ok(n);
  }

  // multiple mid
  ListNode* n = list->head;
  list->head = list->head->next;
  list->head->previous = NULL;
  n->next = NULL;
  n->previous = NULL;

  e = pthread_rwlock_unlock(&list->lock);
  if (e) return result_std_error();

  return result_ok(n);
}

Result list_remove_tail(List* const list) {
  int e = pthread_rwlock_trywrlock(&list->lock);
  if (e) return result_std_error();

  // empty
  if (list->head == NULL && list->tail == NULL) {
    e = pthread_rwlock_unlock(&list->lock);
    if (e) return result_std_error();

    return result_ok(NULL);
  }

  // sinlge node
  if (list->head == list->tail) {
    ListNode* n = list->head;
    list->head = NULL;
    list->tail = NULL;

    e = pthread_rwlock_unlock(&list->lock);
    if (e) return result_std_error();

    return result_ok(n);
  }

  // multiple mid
  ListNode* n = list->tail;
  list->tail = list->tail->previous;
  list->tail->next = NULL;
  n->next = NULL;
  n->previous = NULL;

  e = pthread_rwlock_unlock(&list->lock);
  if (e) return result_std_error();

  return result_ok(n);
}

Result list_remove(List* const list, ListNode* node) {
  int e = pthread_rwlock_trywrlock(&list->lock);
  if (e) return result_std_error();

  // single
  if (list->head == list->tail) {
    e = pthread_rwlock_unlock(&list->lock);
    if (e) return result_std_error();

    return list_remove_head(list);
  }

  // head
  if (node == list->head) {
    e = pthread_rwlock_unlock(&list->lock);
    if (e) return result_std_error();

    return list_remove_head(list);
  }

  // tail
  if (node == list->tail) {
    e = pthread_rwlock_unlock(&list->lock);
    if (e) return result_std_error();

    return list_remove_tail(list);
  }

  // mid
  node->next->previous = node->previous;
  node->previous->next = node->next;
  node->next = NULL;
  node->previous = NULL;

  e = pthread_rwlock_unlock(&list->lock);
  if (e) return result_std_error();

  return result_ok(node);
}


Result list_find(List* const list, FnPredicate const predicate) {
  int e = pthread_rwlock_trywrlock(&list->lock);
  if (e) return result_std_error();

  ListNode* n = list->head;
  while (n) {
    if (predicate(n)) {
      e = pthread_rwlock_unlock(&list->lock);
      if (e) return result_std_error();

      return result_ok(n);
    }
    n = n->next;
  }
  
  e = pthread_rwlock_unlock(&list->lock);
  if (e) return result_std_error();

  return result_ok(NULL);
}

Result list_to_string(List* const list, FnToString const to_string) {
  int e = pthread_rwlock_trywrlock(&list->lock);
  if (e) return result_std_error();

  int sze = list_size(list);
  if (sze == 0) {
    char* buffer = malloc(sizeof(char) * 3);
    sprintf(buffer, "{}");

    e = pthread_rwlock_unlock(&list->lock);
    if (e) {
      free(buffer);
      return result_std_error();
    };

    return result_ok(buffer);
  }

  char* values[sze] = {};
  int lengths[sze] = {};
  int sum_lengths = 0;

  ListNode* n = list->head;
  int i = 0;
  while(n != NULL) {
    void* value = n->value;
    values[i] = value != NULL ? to_string(value) : "NULL";
    lengths[i] = strlen(values[i]);
    sum_lengths += lengths[i];
    ++i;
    n = n->next;
  }

  int total_length = sum_lengths + (sze * 2) + 4;
  char* buffer = malloc(sizeof(char) * total_length);

  if (buffer == NULL) {
    e = pthread_rwlock_unlock(&list->lock);
    if (e) return result_std_error();

    return result_std_error();
  }

  sprintf(buffer, "{");
  for (int i = 0; i < sze; ++i) {
    strcat(buffer, values[i]);
    if (i < sze - 1) {
      strcat(buffer, ", ");
    }
  }
  strcat(buffer, "}\0");

  e = pthread_rwlock_unlock(&list->lock);
  if (e) {
    free(buffer);
    return result_std_error();
  }

  return result_ok(buffer);
}
