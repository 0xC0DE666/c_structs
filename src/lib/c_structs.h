#include <stdbool.h>
#include <pthread.h>

#ifndef C_STRUCTS_H
#define C_STRUCTS_H

//####################
// COMMON
//####################
typedef struct Error {
  int code;
  char* message;
} Error;


Error* error(int code, char* message);
void error_free(Error** error);

typedef struct Result {
  void* ok;
  Error* error;
} Result;

Result success(void* value);
Result fail(int code, char* message);

#define ERR_MALLOC_FAILED "malloc failed"

#define ERR_RWLOCK_INIT_FAILED "rwlock init failed"
#define ERR_RWLOCK_DESTROY_FAILED "rwlock destroy failed"
#define ERR_RDLOCK_FAILED "rdlock failed"
#define ERR_WRLOCK_FAILED "wrlock failed"
#define ERR_RWLOCK_UNLOCK_FAILED "rwlock unlock failed"

#define ERR_INDEX_OUT_OF_BOUNDS "index out of bounds"

typedef void (*FreeFn)(void** const);
typedef bool (*PredicateFn)(void* const);
typedef char* (*ToStringFn)(void* const);


//####################
// ARRAY
//####################
typedef struct Array {
  pthread_rwlock_t lock;
  int capacity;
  int size;
  void* elements[];
} Array;

bool array_index_valid(Array* const array, int index);
bool array_has_capacity(Array* const array);

Result array_new(int capacity);
int array_clear(Array* const array, FreeFn const free_element);
int array_free(Array** const array, FreeFn const free_element);

int array_append(Array* const array, void* const element);
int array_prepend(Array* const array, void* const element);
int array_set(Array* const array, int index, void* const element);

Result array_get(Array* const array, int index);
Result array_remove(Array* const array, int index);

typedef void (*ArrayEachFn)(void* const);
int array_for_each(Array* const array, ArrayEachFn const each);

typedef void* (*ArrayMapFn)(void* const);
Result array_map(Array* const array, ArrayMapFn const map);

Result array_to_string(Array* const array, ToStringFn const to_string);


//####################
// MATRIX
//####################
typedef struct Position {
  int row;
  int column;
} Position;

Position position_new(int row, int column);
char* position_to_string(Position* position);

typedef struct Matrix {
  pthread_rwlock_t lock;
  int rows;
  int columns;
  int capacity;
  int size;
  void* elements[];
} Matrix;

bool matrix_position_valid(Matrix* const matrix, Position* const position);
bool matrix_has_capacity(Matrix* const matrix);

Result matrix_new(int rows, int columns);
int matrix_clear(Matrix* const matrix, FreeFn const free_element);
int matrix_free(Matrix** const matrix, FreeFn const free_element);

int matrix_set(Matrix* const matrix, Position* const position, void* const element);

void* matrix_get(Matrix* const matrix, Position* const position);
void* matrix_remove(Matrix* const matrix, Position* const position);

typedef void (*MatrixEachFn)(void* const);
void matrix_for_each(Matrix* const matrix, MatrixEachFn const each);

typedef void* (*MatrixMapFn)(void* const);
Matrix* matrix_map(Matrix* const matrix, MatrixMapFn const map);

char* matrix_to_string(Matrix* const matrix, ToStringFn const to_string);


//####################
// LINKED LIST
//####################
typedef struct Node {
  void* value;
  struct Node* next;
  struct Node* previous;
} Node;

Node* node_new(void* const value);
int node_free(Node** const node, FreeFn const free_value);

typedef struct LinkedList {
  pthread_rwlock_t lock;
  struct Node* head;
  struct Node* tail;
} LinkedList;

int linked_list_size(LinkedList* const list);

LinkedList* linked_list_new();
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

#endif
