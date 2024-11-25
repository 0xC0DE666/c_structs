#include <stdbool.h>
#include <pthread.h>

#include "../deps/libc_errors/c_errors.h"

#ifndef C_STRUCTS_H
#define C_STRUCTS_H

//####################
// COMMON
//####################
#define ERR_MSG_AT_CAPACITY "at capacity"
#define ERR_MSG_INVALID_INDEX "invalid index"
#define ERR_MSG_INVALID_POSITION "invalid position"

typedef void (*const FreeFn)(void** const);
typedef bool (*const PredicateFn)(void* const);
typedef char* (*const ToStringFn)(void* const);

//####################
// ARRAY
//####################
typedef struct Array {
  pthread_rwlock_t lock;
 unsigned int capacity;
 unsigned int size;
  void* elements[];
} Array;

bool array_index_valid(const Array* array, unsigned int index);
bool array_has_capacity(const Array* array);

Result array_new(uint capacity);
int array_clear(Array* const array, FreeFn const free_element);
int array_free(Array** const array, FreeFn const free_element);

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
int grid_clear(Grid* const grid, FreeFn const free_element);
int grid_free(Grid** const grid, FreeFn const free_element);

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
typedef struct Node {
  void* value;
  struct Node* next;
  struct Node* previous;
} Node;

Result node_new(void* const value);
int node_free(Node** const node, FreeFn const free_value);

typedef struct LinkedList {
  pthread_rwlock_t lock;
  struct Node* head;
  struct Node* tail;
} LinkedList;

int linked_list_size(LinkedList* const list);

Result linked_list_new();
int linked_list_clear(LinkedList* const list, FreeFn const free_value);
int linked_list_free(LinkedList** const list, FreeFn const free_value);

int linked_list_append(LinkedList* const list, void* const value);
int linked_list_prepend(LinkedList* const list, void* const value);

int linked_list_insert_before(LinkedList* const list, Node* const node, void* const value);
int linked_list_insert_after(LinkedList* const list, Node* const node, void* const value);

Result linked_list_remove_head(LinkedList* const list);
Result linked_list_remove_tail(LinkedList* const list);
Result linked_list_remove(LinkedList* const list, Node* node);

Result linked_list_find(LinkedList* const list, PredicateFn const predicate);

Result linked_list_to_string(LinkedList* const list, ToStringFn const to_string);

#endif
