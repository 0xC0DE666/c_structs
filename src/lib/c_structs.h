#include <stdbool.h>
#include <pthread.h>

#include "../deps/libc_errors/c_errors.h"

#ifndef C_STRUCTS_H
#define C_STRUCTS_H

//####################
// COMMON
//####################

typedef void (*const FnFree)(void** const);
typedef char* (*const ToStringFn)(void* const);
typedef bool (*const FnPredicate)(void* const);
typedef int (*const FnComparator)(void* const); // -1 0 1

//####################
// ARRAY
//####################

#define ERR_MSG_ARRAY_AT_CAPACITY "Array is at capacity."
#define ERR_MSG_ARRAY_INDEX_INVALID "Invalid array index."

typedef struct Array {
  pthread_rwlock_t lock;
  unsigned int capacity;
  unsigned int size;
  void* elements[];
} Array;

bool array_index_valid(const Array* array, unsigned int index);
bool array_has_capacity(const Array* array);

Result array_new(uint capacity);
int array_clear(Array* const array, FnFree const free_element);
int array_free(Array** const array, FnFree const free_element);

int array_append(Array* const array, void* const element);
int array_prepend(Array* const array, void* const element);
int array_set(Array* const array, unsigned int index, void* const element);

Result array_get(Array* const array, unsigned int index);
Result array_remove(Array* const array, unsigned int index);

typedef void (*ArrayEachFn)(void* const);
int array_for_each(Array* const array, ArrayEachFn const each);

typedef void* (*ArrayMapFn)(void* const);
Result array_map(Array* const array, ArrayMapFn const map);

Result array_to_string(Array* const array, ToStringFn const to_string);


//####################
// GRID
//####################

#define ERR_MSG_GRID_POSITION_INVALID "Invalid grid position."

typedef struct Position {
 unsigned int row;
 unsigned int column;
} Position;

Position position_new(unsigned int row, unsigned int column);
char* position_to_string(Position* position);

typedef struct Grid {
  pthread_rwlock_t lock;
  unsigned int rows;
  unsigned int columns;
  unsigned int capacity;
  unsigned int size;
  void* elements[];
} Grid;

bool grid_position_valid(Grid* const grid, Position* const position);
bool grid_has_capacity(Grid* const grid);

Result grid_new(unsigned int rows, unsigned int columns);
int grid_clear(Grid* const grid, FnFree const free_element);
int grid_free(Grid** const grid, FnFree const free_element);

int grid_set(Grid* const grid, Position* const position, void* const element);

Result grid_get(Grid* const grid, Position* const position);
Result grid_remove(Grid* const grid, Position* const position);

typedef void (*GridEachFn)(void* const);
int grid_for_each(Grid* const grid, GridEachFn const each);

typedef void* (*GridMapFn)(void* const);
Result grid_map(Grid* const grid, GridMapFn const map);

Result grid_to_string(Grid* const grid, ToStringFn const to_string);


//####################
// LINKED LIST
//####################
typedef struct ListNode {
  void* value;
  struct ListNode* next;
  struct ListNode* previous;
} ListNode;

Result node_new(void* const value);
int node_free(ListNode** const node, FnFree const free_value);

typedef struct List {
  pthread_rwlock_t lock;
  struct ListNode* head;
  struct ListNode* tail;
} List;

int list_size(List* const list);

Result list_new();
int list_clear(List* const list, FnFree const free_value);
int list_free(List** const list, FnFree const free_value);

int list_append(List* const list, void* const value);
int list_prepend(List* const list, void* const value);

int list_insert_before(List* const list, ListNode* const node, void* const value);
int list_insert_after(List* const list, ListNode* const node, void* const value);

Result list_remove_head(List* const list);
Result list_remove_tail(List* const list);
Result list_remove(List* const list, ListNode* node);

Result list_find(List* const list, FnPredicate const predicate);

Result list_to_string(List* const list, ToStringFn const to_string);

//####################
// Map
//####################

typedef struct MapNode {
  void* value;
  struct MapNode* left_child;
  struct MapNode* right_child;
} MapNode;

typedef struct Map {
  pthread_rwlock_t lock;
  struct MapNode* root;
} Map;

int map_size(Map* const map);

Result map_new();
int map_clear(Map* const map, FnFree const free_value);
int map_free(Map** const map, FnFree const free_value);

int map_insert(Map* const map, void* const value, FnComparator compare);

Result map_get(Map* const map, MapNode* node);
Result map_remove(Map* const map, MapNode* node);

#endif
