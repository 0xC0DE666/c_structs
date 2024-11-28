#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "c_structs.h"

Result map_node_new(void* const value) {
  MapNode* node = malloc(sizeof(MapNode));

  if (node == NULL) {
    return result_std_error();
  }

  node->value = value;
  node->left_child = NULL;
  node->right_child = NULL;
  
  return result_ok(node);
}

int map_node_free(MapNode** node, FnFree const free_value) {
  free_value(&(*node)->value); 

  (*node)->value = NULL;
  (*node)->left_child = NULL;
  (*node)->right_child = NULL;
  free(*node);
  *node = NULL;

  return 0;
}

// int map_size(Map* map) {
//   int sze = 0;
//   MapNode* n = map->head;
//   while(n) {
//     ++sze;
//     n = n->next;
//   }
//   return sze;
// }
// 
// Result map_new() {
//   Map* map = malloc(sizeof(Map));
// 
//   if (map == NULL) {
//     return result_std_error();
//   }
// 
//   pthread_rwlock_init(&map->lock, NULL);
//   map->head = NULL;
//   map->tail = NULL;
//   
//   return result_ok(map);
// }
// 
// 
// int map_clear(Map* const map, FnFree const free_value) {
//   int e = pthread_rwlock_trywrlock(&map->lock);
//   if (e) return e;
// 
//   // empty
//   if (map->head == NULL && map->tail == NULL) {
//     e = pthread_rwlock_unlock(&map->lock);
//     if (e) return e;
// 
//     return 0;
//   }
// 
//   // sinlge node
//   if (map->head == map->tail) {
//     e = map_node_free(&map->head, free_value);
//     if (e) return e;
// 
//     map->head = NULL;
//     map->tail = NULL;
// 
//     e = pthread_rwlock_unlock(&map->lock);
//     if (e) return e;
// 
//     return 0;
//   }
// 
//   // multiple nodes
//   while(map->head) {
//     MapNode* next = map->head->next;
//     map->head->next ? map->head->next->previous = NULL : 0;
//     map->head->previous ? map->head->previous->next = NULL : 0;
// 
//     e = map_node_free(&map->head, free_value);
//     if (e) return e;
// 
//     map->head = next;
//   }
//   map->head = NULL;
//   map->tail = NULL;
// 
//   e = pthread_rwlock_unlock(&map->lock);
//   if (e) return e;
// 
//   return 0;
// }
// 
// int map_free(Map** const map, FnFree const free_value) {
//   int e = map_clear(*map, free_value);
//   if (e) return e;
// 
//   pthread_rwlock_destroy(&(*map)->lock);
//   free(*map);
//   *map = NULL;
// 
//   return 0;
// }
// 
// int map_insert(Map* const map, void* const value, FnComparator compare) {
//   int e = pthread_rwlock_trywrlock(&map->lock);
//   if (e) return e;
// 
//   MapNode* node = map_node_new(value).ok;
//   if (map->head == NULL && map->tail == NULL) {
//     map->head = node;
//     map->tail = node;
// 
//     e = pthread_rwlock_unlock(&map->lock);
//     if (e) return e;
//     
//     return 0;
//   }
//   
//   map->tail->next = node;
//   node->previous = map->tail;
//   map->tail = node;
// 
//   e = pthread_rwlock_unlock(&map->lock);
//   if (e) return e;
// 
//   return 0;
// }
// 
// Result map_remove(Map* const map, MapNode* node) {
//   int e = pthread_rwlock_trywrlock(&map->lock);
//   if (e) return result_std_error();
// 
//   // sinlge
//   if (map->head == map->tail) {
//     e = pthread_rwlock_unlock(&map->lock);
//     if (e) return result_std_error();
// 
//     return map_remove_head(map);
//   }
// 
//   // head
//   if (node == map->head) {
//     e = pthread_rwlock_unlock(&map->lock);
//     if (e) return result_std_error();
// 
//     return map_remove_head(map);
//   }
// 
//   // tail
//   if (node == map->tail) {
//     e = pthread_rwlock_unlock(&map->lock);
//     if (e) return result_std_error();
// 
//     return map_remove_tail(map);
//   }
// 
//   // mid
//   node->next->previous = node->previous;
//   node->previous->next = node->next;
//   node->next = NULL;
//   node->previous = NULL;
// 
//   e = pthread_rwlock_unlock(&map->lock);
//   if (e) return result_std_error();
// 
//   return result_ok(node);
// }
// 
// 
// Result map_get(Map* const map, MapNode* node) {
//   int e = pthread_rwlock_trywrlock(&map->lock);
//   if (e) return result_std_error();
// 
//   MapNode* n = map->head;
//   while (n) {
//     if (predicate(n)) {
//       e = pthread_rwlock_unlock(&map->lock);
//       if (e) return result_std_error();
// 
//       return result_ok(n);
//     }
//     n = n->next;
//   }
//   
//   e = pthread_rwlock_unlock(&map->lock);
//   if (e) return result_std_error();
// 
//   return result_ok(NULL);
// }
