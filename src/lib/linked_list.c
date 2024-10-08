#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "c_structs.h"


Node* node_new(void* const value) {
  Node* node = malloc(sizeof(Node));

  if (node == NULL) {
    return NULL;
  }

  node->value = value;
  node->next = NULL;
  node->prev = NULL;
  
  return node;
}

int node_clear(Node* const node, FreeFn const free_value) {
  
  return 0;
}

int node_free(Node** const node, FreeFn const free_value) {
  
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
